
/* Support for dynamic loading of extension modules */

#include "Python.h"

#ifdef HAVE_DIRECT_H
#include <direct.h>
#endif
#include <ctype.h>

#include "importdl.h"
#include "patchlevel.h"
#include <windows.h>

#ifdef _DEBUG
#define PYD_DEBUG_SUFFIX "_d"
#else
#define PYD_DEBUG_SUFFIX ""
#endif

#ifdef PYD_PLATFORM_TAG
#define PYD_TAGGED_SUFFIX PYD_DEBUG_SUFFIX ".cp" Py_STRINGIFY(PY_MAJOR_VERSION) Py_STRINGIFY(PY_MINOR_VERSION) "-" PYD_PLATFORM_TAG ".pyd"
#else
#define PYD_TAGGED_SUFFIX PYD_DEBUG_SUFFIX ".cp" Py_STRINGIFY(PY_MAJOR_VERSION) Py_STRINGIFY(PY_MINOR_VERSION) ".pyd"
#endif

#define PYD_UNTAGGED_SUFFIX PYD_DEBUG_SUFFIX ".pyd"

const char *_PyImport_DynLoadFiletab[] = {
    PYD_TAGGED_SUFFIX,
    PYD_UNTAGGED_SUFFIX,
    NULL
};

/* Function to return the name of the "python" DLL that the supplied module
   directly imports.  Looks through the list of imported modules and
   returns the first entry that starts with "python" (case sensitive) and
   is followed by nothing but numbers until the separator (period).

   Returns a pointer to the import name, or NULL if no matching name was
   located.

   This function parses through the PE header for the module as loaded in
   memory by the system loader.  The PE header is accessed as documented by
   Microsoft in the MSDN PE and COFF specification (2/99), and handles
   both PE32 and PE32+.  It only worries about the direct import table and
   not the delay load import table since it's unlikely an extension is
   going to be delay loading Python (after all, it's already loaded).

   If any magic values are not found (e.g., the PE header or optional
   header magic), then this function simply returns NULL. */

#define DWORD_AT(mem) (*(DWORD *)(mem))
#define WORD_AT(mem)  (*(WORD *)(mem))

static char *GetPythonImport (HINSTANCE hModule)
{
    unsigned char *dllbase, *import_data, *import_name;
    DWORD pe_offset, opt_offset;
    WORD opt_magic;
    int num_dict_off, import_off;

    /* Safety check input */
    if (hModule == NULL) {
        return NULL;
    }

    /* Module instance is also the base load address.  First portion of
       memory is the MS-DOS loader, which holds the offset to the PE
       header (from the load base) at 0x3C */
    dllbase = (unsigned char *)hModule;
    pe_offset = DWORD_AT(dllbase + 0x3C);

    /* The PE signature must be "PE\0\0" */
    if (memcmp(dllbase+pe_offset,"PE\0\0",4)) {
        return NULL;
    }

    /* Following the PE signature is the standard COFF header (20
       bytes) and then the optional header.  The optional header starts
       with a magic value of 0x10B for PE32 or 0x20B for PE32+ (PE32+
       uses 64-bits for some fields).  It might also be 0x107 for a ROM
       image, but we don't process that here.

       The optional header ends with a data dictionary that directly
       points to certain types of data, among them the import entries
       (in the second table entry). Based on the header type, we
       determine offsets for the data dictionary count and the entry
       within the dictionary pointing to the imports. */

    opt_offset = pe_offset + 4 + 20;
    opt_magic = WORD_AT(dllbase+opt_offset);
    if (opt_magic == 0x10B) {
        /* PE32 */
        num_dict_off = 92;
        import_off   = 104;
    } else if (opt_magic == 0x20B) {
        /* PE32+ */
        num_dict_off = 108;
        import_off   = 120;
    } else {
        /* Unsupported */
        return NULL;
    }

    /* Now if an import table exists, offset to it and walk the list of
       imports.  The import table is an array (ending when an entry has
       empty values) of structures (20 bytes each), which contains (at
       offset 12) a relative address (to the module base) at which a
       string constant holding the import name is located. */

    if (DWORD_AT(dllbase + opt_offset + num_dict_off) >= 2) {
        /* We have at least 2 tables - the import table is the second
           one.  But still it may be that the table size is zero */
        if (0 == DWORD_AT(dllbase + opt_offset + import_off + sizeof(DWORD)))
            return NULL;
        import_data = dllbase + DWORD_AT(dllbase +
                                         opt_offset +
                                         import_off);
        while (DWORD_AT(import_data)) {
            import_name = dllbase + DWORD_AT(import_data+12);
            if (strlen(import_name) >= 6 &&
                !strncmp(import_name,"python",6)) {
                char *pch;

#ifndef _DEBUG
                /* In a release version, don't claim that python3.dll is
                   a Python DLL. */
                if (strcmp(import_name, "python3.dll") == 0) {
                    import_data += 20;
                    continue;
                }
#endif

                /* Ensure python prefix is followed only
                   by numbers to the end of the basename */
                pch = import_name + 6;
#ifdef _DEBUG
                while (*pch && pch[0] != '_' && pch[1] != 'd' && pch[2] != '.') {
#else
                while (*pch && *pch != '.') {
#endif
                    if (*pch >= '0' && *pch <= '9') {
                        pch++;
                    } else {
                        pch = NULL;
                        break;
                    }
                }

                if (pch) {
                    /* Found it - return the name */
                    return import_name;
                }
            }
            import_data += 20;
        }
    }

    return NULL;
}

dl_funcptr _PyImport_FindSharedFuncptrWindows(const char *prefix,
                                              const char *shortname,
                                              PyObject *pathname, FILE *fp)
{
    dl_funcptr p;
    char funcname[258], *import_python;
    const wchar_t *wpathname;

    _Py_CheckPython3();

_Py_COMP_DIAG_PUSH
_Py_COMP_DIAG_IGNORE_DEPR_DECLS
    wpathname = _PyUnicode_AsUnicode(pathname);
_Py_COMP_DIAG_POP
    if (wpathname == NULL)
        return NULL;

    PyOS_snprintf(funcname, sizeof(funcname), "%.20s_%.200s", prefix, shortname);

    {
        HINSTANCE hDLL = NULL;
        unsigned int old_mode;

        /* Don't display a message box when Python can't load a DLL */
        old_mode = SetErrorMode(SEM_FAILCRITICALERRORS);

        /* bpo-36085: We use LoadLibraryEx with restricted search paths
           to avoid DLL preloading attacks and enable use of the
           AddDllDirectory function. We add SEARCH_DLL_LOAD_DIR to
           ensure DLLs adjacent to the PYD are preferred. */
        Py_BEGIN_ALLOW_THREADS
        hDLL = LoadLibraryExW(wpathname, NULL,
                              LOAD_LIBRARY_SEARCH_DEFAULT_DIRS |
                              LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR);
        Py_END_ALLOW_THREADS

        /* restore old error mode settings */
        SetErrorMode(old_mode);

        if (hDLL==NULL){
            PyObject *message;
            unsigned int errorCode;

            /* Get an error string from Win32 error code */
            wchar_t theInfo[256]; /* Pointer to error text
                                  from system */
            int theLength; /* Length of error text */

            errorCode = GetLastError();

            theLength = FormatMessageW(
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS, /* flags */
                NULL, /* message source */
                errorCode, /* the message (error) ID */
                MAKELANGID(LANG_NEUTRAL,
                           SUBLANG_DEFAULT),
                           /* Default language */
                theInfo, /* the buffer */
                sizeof(theInfo) / sizeof(wchar_t), /* size in wchars */
                NULL); /* no additional format args. */

            /* Problem: could not get the error message.
               This should not happen if called correctly. */
            if (theLength == 0) {
                message = PyUnicode_FromFormat(
                    "DLL load failed with error code %u while importing %s",
                    errorCode, shortname);
            } else {
                /* For some reason a \r\n
                   is appended to the text */
                if (theLength >= 2 &&
                    theInfo[theLength-2] == '\r' &&
                    theInfo[theLength-1] == '\n') {
                    theLength -= 2;
                    theInfo[theLength] = '\0';
                }
                message = PyUnicode_FromFormat(
                    "DLL load failed while importing %s: ", shortname);

                PyUnicode_AppendAndDel(&message,
                    PyUnicode_FromWideChar(
                        theInfo,
                        theLength));
            }
            if (message != NULL) {
                PyObject *shortname_obj = PyUnicode_FromString(shortname);
                PyErr_SetImportError(message, shortname_obj, pathname);
                Py_XDECREF(shortname_obj);
                Py_DECREF(message);
            }
            return NULL;
        } else {
            char buffer[256];

            PyOS_snprintf(buffer, sizeof(buffer),
#ifdef _DEBUG
                          "python%d%d_d.dll",
#else
                          "python%d%d.dll",
#endif
                          PY_MAJOR_VERSION,PY_MINOR_VERSION);
            import_python = GetPythonImport(hDLL);

            if (import_python &&
                _stricmp(buffer,import_python)) {
                PyErr_Format(PyExc_ImportError,
                             "Module use of %.150s conflicts "
                             "with this version of Python.",
                             import_python);
                Py_BEGIN_ALLOW_THREADS
                FreeLibrary(hDLL);
                Py_END_ALLOW_THREADS
                return NULL;
            }
        }
        Py_BEGIN_ALLOW_THREADS
        p = GetProcAddress(hDLL, funcname);
        Py_END_ALLOW_THREADS
    }

    return p;
}
