
/* Support for dynamic loading of extension modules */

#include "Python.h"
#include "importdl.h"

#include <errno.h>      /*  for global errno      */
#include <string.h>     /*  for strerror()        */
#include <stdlib.h>     /*  for malloc(), free()  */
#include <sys/ldr.h>


#ifdef AIX_GENUINE_CPLUSPLUS
#include <load.h>
#define aix_load loadAndInit
#else
#define aix_load load
#endif


extern char *Py_GetProgramName(void);

typedef struct Module {
    struct Module *next;
    void          *entry;
} Module, *ModulePtr;

const char *_PyImport_DynLoadFiletab[] = {".so", NULL};

static int
aix_getoldmodules(void **modlistptr)
{
    ModulePtr       modptr, prevmodptr;
    struct ld_info  *ldiptr;
    char            *ldibuf;
    int             errflag, bufsize = 1024;
    unsigned int    offset;
    char *progname = Py_GetProgramName();

    /*
    -- Get the list of loaded modules into ld_info structures.
    */
    if ((ldibuf = malloc(bufsize)) == NULL) {
        PyErr_SetString(PyExc_ImportError, strerror(errno));
        return -1;
    }
    while ((errflag = loadquery(L_GETINFO, ldibuf, bufsize)) == -1
           && errno == ENOMEM) {
        free(ldibuf);
        bufsize += 1024;
        if ((ldibuf = malloc(bufsize)) == NULL) {
            PyErr_SetString(PyExc_ImportError, strerror(errno));
            return -1;
        }
    }
    if (errflag == -1) {
        PyErr_SetString(PyExc_ImportError, strerror(errno));
        return -1;
    }
    /*
    -- Make the modules list from the ld_info structures.
    */
    ldiptr = (struct ld_info *)ldibuf;
    prevmodptr = NULL;
    do {
        if (strstr(progname, ldiptr->ldinfo_filename) == NULL &&
            strstr(ldiptr->ldinfo_filename, "python") == NULL) {
            /*
            -- Extract only the modules belonging to the main
            -- executable + those containing "python" as a
            -- substring (like the "python[version]" binary or
            -- "libpython[version].a" in case it's a shared lib).
            */
            offset = (unsigned int)ldiptr->ldinfo_next;
            ldiptr = (struct ld_info *)((char*)ldiptr + offset);
            continue;
        }
        if ((modptr = (ModulePtr)malloc(sizeof(Module))) == NULL) {
            PyErr_SetString(PyExc_ImportError, strerror(errno));
            while (*modlistptr) {
                modptr = (ModulePtr)*modlistptr;
                *modlistptr = (void *)modptr->next;
                free(modptr);
            }
            return -1;
        }
        modptr->entry = ldiptr->ldinfo_dataorg;
        modptr->next  = NULL;
        if (prevmodptr == NULL)
            *modlistptr = (void *)modptr;
        else
            prevmodptr->next = modptr;
        prevmodptr = modptr;
        offset = (unsigned int)ldiptr->ldinfo_next;
        ldiptr = (struct ld_info *)((char*)ldiptr + offset);
    } while (offset);
    free(ldibuf);
    return 0;
}


static void
aix_loaderror(const char *pathname)
{

    char *message[1024], errbuf[1024];
    PyObject *pathname_ob = NULL;
    PyObject *errbuf_ob = NULL;
    int i,j;

    struct errtab {
        int errNo;
        char *errstr;
    } load_errtab[] = {
        {L_ERROR_TOOMANY,               "too many errors, rest skipped."},
        {L_ERROR_NOLIB,                 "can't load library:"},
        {L_ERROR_UNDEF,                 "can't find symbol in library:"},
        {L_ERROR_RLDBAD,
         "RLD index out of range or bad relocation type:"},
        {L_ERROR_FORMAT,                "not a valid, executable xcoff file:"},
        {L_ERROR_MEMBER,
         "file not an archive or does not contain requested member:"},
        {L_ERROR_TYPE,                  "symbol table mismatch:"},
        {L_ERROR_ALIGN,                 "text alignment in file is wrong."},
        {L_ERROR_SYSTEM,                "System error:"},
        {L_ERROR_ERRNO,                 NULL}
    };

#define ERRBUF_APPEND(s) strncat(errbuf, s, sizeof(errbuf)-strlen(errbuf)-1)

    PyOS_snprintf(errbuf, sizeof(errbuf), "from module %.200s ", pathname);

    if (!loadquery(L_GETMESSAGES, &message[0], sizeof(message))) {
        ERRBUF_APPEND(strerror(errno));
        ERRBUF_APPEND("\n");
    }
    for(i = 0; message[i] && *message[i]; i++) {
        int nerr = atoi(message[i]);
        for (j=0; j < Py_ARRAY_LENGTH(load_errtab); j++) {
            if (nerr == load_errtab[j].errNo && load_errtab[j].errstr)
            ERRBUF_APPEND(load_errtab[j].errstr);
        }
        while (Py_ISDIGIT(*message[i])) message[i]++ ;
        ERRBUF_APPEND(message[i]);
        ERRBUF_APPEND("\n");
    }
    /* Subtract 1 from the length to trim off trailing newline */
    errbuf_ob = PyUnicode_DecodeLocaleAndSize(errbuf, strlen(errbuf)-1, "surrogateescape");
    if (errbuf_ob == NULL)
        return;
    pathname_ob = PyUnicode_DecodeFSDefault(pathname);
    if (pathname_ob == NULL) {
        Py_DECREF(errbuf_ob);
        return;
    }
    PyErr_SetImportError(errbuf_ob, NULL, pathname_ob);
    Py_DECREF(pathname_ob);
    Py_DECREF(errbuf_ob);
    return;
}


dl_funcptr _PyImport_FindSharedFuncptr(const char *prefix,
                                       const char *shortname,
                                       const char *pathname, FILE *fp)
{
    dl_funcptr p;

    /*
    -- Invoke load() with L_NOAUTODEFER leaving the imported symbols
    -- of the shared module unresolved. Thus we have to resolve them
    -- explicitly with loadbind. The new module is loaded, then we
    -- resolve its symbols using the list of already loaded modules
    -- (only those that belong to the python executable). Get these
    -- with loadquery(L_GETINFO).
    */

    static void *staticmodlistptr = NULL;

    if (!staticmodlistptr)
        if (aix_getoldmodules(&staticmodlistptr) == -1)
            return NULL;
    p = (dl_funcptr) aix_load((char *)pathname, L_NOAUTODEFER, 0);
    if (p == NULL) {
        aix_loaderror(pathname);
        return NULL;
    }

    return p;
}
