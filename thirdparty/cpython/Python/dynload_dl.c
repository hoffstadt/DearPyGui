
/* Support for dynamic loading of extension modules */

#include "dl.h"

#include "Python.h"
#include "importdl.h"


extern char *Py_GetProgramName(void);

const char *_PyImport_DynLoadFiletab[] = {".o", NULL};


dl_funcptr _PyImport_FindSharedFuncptr(const char *prefix,
                                       const char *shortname,
                                       const char *pathname, FILE *fp)
{
    char funcname[258];

    PyOS_snprintf(funcname, sizeof(funcname), "%.20s_%.200s", prefix, shortname);
    return dl_loadmod(Py_GetProgramName(), pathname, funcname);
}
