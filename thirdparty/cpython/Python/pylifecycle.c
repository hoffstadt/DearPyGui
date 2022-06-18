/* Python interpreter top-level routines, including init/exit */

#include "Python.h"

#include "Python-ast.h"
#undef Yield   /* undefine macro conflicting with <winbase.h> */

#include "pycore_ceval.h"         // _PyEval_FiniGIL()
#include "pycore_context.h"       // _PyContext_Init()
#include "pycore_fileutils.h"     // _Py_ResetForceASCII()
#include "pycore_import.h"        // _PyImport_Cleanup()
#include "pycore_initconfig.h"    // _PyStatus_OK()
#include "pycore_object.h"        // _PyDebug_PrintTotalRefs()
#include "pycore_pathconfig.h"    // _PyConfig_WritePathConfig()
#include "pycore_pyerrors.h"      // _PyErr_Occurred()
#include "pycore_pylifecycle.h"   // _PyErr_Print()
#include "pycore_pystate.h"       // _PyThreadState_GET()
#include "pycore_sysmodule.h"     // _PySys_ClearAuditHooks()
#include "pycore_traceback.h"     // _Py_DumpTracebackThreads()

#include "grammar.h"              // PyGrammar_RemoveAccelerators()
#include <locale.h>               // setlocale()

#ifdef HAVE_SIGNAL_H
#  include <signal.h>             // SIG_IGN
#endif

#ifdef HAVE_LANGINFO_H
#  include <langinfo.h>           // nl_langinfo(CODESET)
#endif

#ifdef MS_WINDOWS
#  undef BYTE
#  include "windows.h"

   extern PyTypeObject PyWindowsConsoleIO_Type;
#  define PyWindowsConsoleIO_Check(op) \
       (PyObject_TypeCheck((op), &PyWindowsConsoleIO_Type))
#endif


_Py_IDENTIFIER(flush);
_Py_IDENTIFIER(name);
_Py_IDENTIFIER(stdin);
_Py_IDENTIFIER(stdout);
_Py_IDENTIFIER(stderr);
_Py_IDENTIFIER(threading);

#ifdef __cplusplus
extern "C" {
#endif

extern grammar _PyParser_Grammar; /* From graminit.c */

/* Forward declarations */
static PyStatus add_main_module(PyInterpreterState *interp);
static PyStatus init_import_site(void);
static PyStatus init_set_builtins_open(void);
static PyStatus init_sys_streams(PyThreadState *tstate);
static void call_py_exitfuncs(PyThreadState *tstate);
static void wait_for_thread_shutdown(PyThreadState *tstate);
static void call_ll_exitfuncs(_PyRuntimeState *runtime);

int _Py_UnhandledKeyboardInterrupt = 0;
_PyRuntimeState _PyRuntime = _PyRuntimeState_INIT;
static int runtime_initialized = 0;

PyStatus
_PyRuntime_Initialize(void)
{
    /* XXX We only initialize once in the process, which aligns with
       the static initialization of the former globals now found in
       _PyRuntime.  However, _PyRuntime *should* be initialized with
       every Py_Initialize() call, but doing so breaks the runtime.
       This is because the runtime state is not properly finalized
       currently. */
    if (runtime_initialized) {
        return _PyStatus_OK();
    }
    runtime_initialized = 1;

    return _PyRuntimeState_Init(&_PyRuntime);
}

void
_PyRuntime_Finalize(void)
{
    _PyRuntimeState_Fini(&_PyRuntime);
    runtime_initialized = 0;
}

int
_Py_IsFinalizing(void)
{
    return _PyRuntimeState_GetFinalizing(&_PyRuntime) != NULL;
}

/* Hack to force loading of object files */
int (*_PyOS_mystrnicmp_hack)(const char *, const char *, Py_ssize_t) = \
    PyOS_mystrnicmp; /* Python/pystrcmp.o */

/* PyModule_GetWarningsModule is no longer necessary as of 2.6
since _warnings is builtin.  This API should not be used. */
PyObject *
PyModule_GetWarningsModule(void)
{
    return PyImport_ImportModule("warnings");
}


/* APIs to access the initialization flags
 *
 * Can be called prior to Py_Initialize.
 */

int
_Py_IsCoreInitialized(void)
{
    return _PyRuntime.core_initialized;
}

int
Py_IsInitialized(void)
{
    return _PyRuntime.initialized;
}


/* Global initializations.  Can be undone by Py_FinalizeEx().  Don't
   call this twice without an intervening Py_FinalizeEx() call.  When
   initializations fail, a fatal error is issued and the function does
   not return.  On return, the first thread and interpreter state have
   been created.

   Locking: you must hold the interpreter lock while calling this.
   (If the lock has not yet been initialized, that's equivalent to
   having the lock, but you cannot use multiple threads.)

*/

static PyStatus
init_importlib(PyThreadState *tstate, PyObject *sysmod)
{
    PyObject *importlib;
    PyObject *impmod;
    PyObject *value;
    PyInterpreterState *interp = tstate->interp;
    int verbose = _PyInterpreterState_GetConfig(interp)->verbose;

    /* Import _importlib through its frozen version, _frozen_importlib. */
    if (PyImport_ImportFrozenModule("_frozen_importlib") <= 0) {
        return _PyStatus_ERR("can't import _frozen_importlib");
    }
    else if (verbose) {
        PySys_FormatStderr("import _frozen_importlib # frozen\n");
    }
    importlib = PyImport_AddModule("_frozen_importlib");
    if (importlib == NULL) {
        return _PyStatus_ERR("couldn't get _frozen_importlib from sys.modules");
    }
    interp->importlib = importlib;
    Py_INCREF(interp->importlib);

    interp->import_func = PyDict_GetItemString(interp->builtins, "__import__");
    if (interp->import_func == NULL)
        return _PyStatus_ERR("__import__ not found");
    Py_INCREF(interp->import_func);

    /* Import the _imp module */
    impmod = PyInit__imp();
    if (impmod == NULL) {
        return _PyStatus_ERR("can't import _imp");
    }
    else if (verbose) {
        PySys_FormatStderr("import _imp # builtin\n");
    }
    if (_PyImport_SetModuleString("_imp", impmod) < 0) {
        return _PyStatus_ERR("can't save _imp to sys.modules");
    }

    /* Install importlib as the implementation of import */
    value = PyObject_CallMethod(importlib, "_install", "OO", sysmod, impmod);
    if (value == NULL) {
        _PyErr_Print(tstate);
        return _PyStatus_ERR("importlib install failed");
    }
    Py_DECREF(value);
    Py_DECREF(impmod);

    return _PyStatus_OK();
}

static PyStatus
init_importlib_external(PyThreadState *tstate)
{
    PyObject *value;
    value = PyObject_CallMethod(tstate->interp->importlib,
                                "_install_external_importers", "");
    if (value == NULL) {
        _PyErr_Print(tstate);
        return _PyStatus_ERR("external importer setup failed");
    }
    Py_DECREF(value);
    return _PyImportZip_Init(tstate);
}

/* Helper functions to better handle the legacy C locale
 *
 * The legacy C locale assumes ASCII as the default text encoding, which
 * causes problems not only for the CPython runtime, but also other
 * components like GNU readline.
 *
 * Accordingly, when the CLI detects it, it attempts to coerce it to a
 * more capable UTF-8 based alternative as follows:
 *
 *     if (_Py_LegacyLocaleDetected()) {
 *         _Py_CoerceLegacyLocale();
 *     }
 *
 * See the documentation of the PYTHONCOERCECLOCALE setting for more details.
 *
 * Locale coercion also impacts the default error handler for the standard
 * streams: while the usual default is "strict", the default for the legacy
 * C locale and for any of the coercion target locales is "surrogateescape".
 */

int
_Py_LegacyLocaleDetected(int warn)
{
#ifndef MS_WINDOWS
    if (!warn) {
        const char *locale_override = getenv("LC_ALL");
        if (locale_override != NULL && *locale_override != '\0') {
            /* Don't coerce C locale if the LC_ALL environment variable
               is set */
            return 0;
        }
    }

    /* On non-Windows systems, the C locale is considered a legacy locale */
    /* XXX (ncoghlan): some platforms (notably Mac OS X) don't appear to treat
     *                 the POSIX locale as a simple alias for the C locale, so
     *                 we may also want to check for that explicitly.
     */
    const char *ctype_loc = setlocale(LC_CTYPE, NULL);
    return ctype_loc != NULL && strcmp(ctype_loc, "C") == 0;
#else
    /* Windows uses code pages instead of locales, so no locale is legacy */
    return 0;
#endif
}

#ifndef MS_WINDOWS
static const char *_C_LOCALE_WARNING =
    "Python runtime initialized with LC_CTYPE=C (a locale with default ASCII "
    "encoding), which may cause Unicode compatibility problems. Using C.UTF-8, "
    "C.utf8, or UTF-8 (if available) as alternative Unicode-compatible "
    "locales is recommended.\n";

static void
emit_stderr_warning_for_legacy_locale(_PyRuntimeState *runtime)
{
    const PyPreConfig *preconfig = &runtime->preconfig;
    if (preconfig->coerce_c_locale_warn && _Py_LegacyLocaleDetected(1)) {
        PySys_FormatStderr("%s", _C_LOCALE_WARNING);
    }
}
#endif   /* !defined(MS_WINDOWS) */

typedef struct _CandidateLocale {
    const char *locale_name; /* The locale to try as a coercion target */
} _LocaleCoercionTarget;

static _LocaleCoercionTarget _TARGET_LOCALES[] = {
    {"C.UTF-8"},
    {"C.utf8"},
    {"UTF-8"},
    {NULL}
};


int
_Py_IsLocaleCoercionTarget(const char *ctype_loc)
{
    const _LocaleCoercionTarget *target = NULL;
    for (target = _TARGET_LOCALES; target->locale_name; target++) {
        if (strcmp(ctype_loc, target->locale_name) == 0) {
            return 1;
        }
    }
    return 0;
}


#ifdef PY_COERCE_C_LOCALE
static const char C_LOCALE_COERCION_WARNING[] =
    "Python detected LC_CTYPE=C: LC_CTYPE coerced to %.20s (set another locale "
    "or PYTHONCOERCECLOCALE=0 to disable this locale coercion behavior).\n";

static int
_coerce_default_locale_settings(int warn, const _LocaleCoercionTarget *target)
{
    const char *newloc = target->locale_name;

    /* Reset locale back to currently configured defaults */
    _Py_SetLocaleFromEnv(LC_ALL);

    /* Set the relevant locale environment variable */
    if (setenv("LC_CTYPE", newloc, 1)) {
        fprintf(stderr,
                "Error setting LC_CTYPE, skipping C locale coercion\n");
        return 0;
    }
    if (warn) {
        fprintf(stderr, C_LOCALE_COERCION_WARNING, newloc);
    }

    /* Reconfigure with the overridden environment variables */
    _Py_SetLocaleFromEnv(LC_ALL);
    return 1;
}
#endif

int
_Py_CoerceLegacyLocale(int warn)
{
    int coerced = 0;
#ifdef PY_COERCE_C_LOCALE
    char *oldloc = NULL;

    oldloc = _PyMem_RawStrdup(setlocale(LC_CTYPE, NULL));
    if (oldloc == NULL) {
        return coerced;
    }

    const char *locale_override = getenv("LC_ALL");
    if (locale_override == NULL || *locale_override == '\0') {
        /* LC_ALL is also not set (or is set to an empty string) */
        const _LocaleCoercionTarget *target = NULL;
        for (target = _TARGET_LOCALES; target->locale_name; target++) {
            const char *new_locale = setlocale(LC_CTYPE,
                                               target->locale_name);
            if (new_locale != NULL) {
#if !defined(_Py_FORCE_UTF8_LOCALE) && defined(HAVE_LANGINFO_H) && defined(CODESET)
                /* Also ensure that nl_langinfo works in this locale */
                char *codeset = nl_langinfo(CODESET);
                if (!codeset || *codeset == '\0') {
                    /* CODESET is not set or empty, so skip coercion */
                    new_locale = NULL;
                    _Py_SetLocaleFromEnv(LC_CTYPE);
                    continue;
                }
#endif
                /* Successfully configured locale, so make it the default */
                coerced = _coerce_default_locale_settings(warn, target);
                goto done;
            }
        }
    }
    /* No C locale warning here, as Py_Initialize will emit one later */

    setlocale(LC_CTYPE, oldloc);

done:
    PyMem_RawFree(oldloc);
#endif
    return coerced;
}

/* _Py_SetLocaleFromEnv() is a wrapper around setlocale(category, "") to
 * isolate the idiosyncrasies of different libc implementations. It reads the
 * appropriate environment variable and uses its value to select the locale for
 * 'category'. */
char *
_Py_SetLocaleFromEnv(int category)
{
    char *res;
#ifdef __ANDROID__
    const char *locale;
    const char **pvar;
#ifdef PY_COERCE_C_LOCALE
    const char *coerce_c_locale;
#endif
    const char *utf8_locale = "C.UTF-8";
    const char *env_var_set[] = {
        "LC_ALL",
        "LC_CTYPE",
        "LANG",
        NULL,
    };

    /* Android setlocale(category, "") doesn't check the environment variables
     * and incorrectly sets the "C" locale at API 24 and older APIs. We only
     * check the environment variables listed in env_var_set. */
    for (pvar=env_var_set; *pvar; pvar++) {
        locale = getenv(*pvar);
        if (locale != NULL && *locale != '\0') {
            if (strcmp(locale, utf8_locale) == 0 ||
                    strcmp(locale, "en_US.UTF-8") == 0) {
                return setlocale(category, utf8_locale);
            }
            return setlocale(category, "C");
        }
    }

    /* Android uses UTF-8, so explicitly set the locale to C.UTF-8 if none of
     * LC_ALL, LC_CTYPE, or LANG is set to a non-empty string.
     * Quote from POSIX section "8.2 Internationalization Variables":
     * "4. If the LANG environment variable is not set or is set to the empty
     * string, the implementation-defined default locale shall be used." */

#ifdef PY_COERCE_C_LOCALE
    coerce_c_locale = getenv("PYTHONCOERCECLOCALE");
    if (coerce_c_locale == NULL || strcmp(coerce_c_locale, "0") != 0) {
        /* Some other ported code may check the environment variables (e.g. in
         * extension modules), so we make sure that they match the locale
         * configuration */
        if (setenv("LC_CTYPE", utf8_locale, 1)) {
            fprintf(stderr, "Warning: failed setting the LC_CTYPE "
                            "environment variable to %s\n", utf8_locale);
        }
    }
#endif
    res = setlocale(category, utf8_locale);
#else /* !defined(__ANDROID__) */
    res = setlocale(category, "");
#endif
    _Py_ResetForceASCII();
    return res;
}


/* Global initializations.  Can be undone by Py_Finalize().  Don't
   call this twice without an intervening Py_Finalize() call.

   Every call to Py_InitializeFromConfig, Py_Initialize or Py_InitializeEx
   must have a corresponding call to Py_Finalize.

   Locking: you must hold the interpreter lock while calling these APIs.
   (If the lock has not yet been initialized, that's equivalent to
   having the lock, but you cannot use multiple threads.)

*/

static PyStatus
pyinit_core_reconfigure(_PyRuntimeState *runtime,
                        PyThreadState **tstate_p,
                        const PyConfig *config)
{
    PyStatus status;
    PyThreadState *tstate = _PyThreadState_GET();
    if (!tstate) {
        return _PyStatus_ERR("failed to read thread state");
    }
    *tstate_p = tstate;

    PyInterpreterState *interp = tstate->interp;
    if (interp == NULL) {
        return _PyStatus_ERR("can't make main interpreter");
    }

    status = _PyConfig_Write(config, runtime);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    status = _PyInterpreterState_SetConfig(interp, config);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    config = _PyInterpreterState_GetConfig(interp);

    if (config->_install_importlib) {
        status = _PyConfig_WritePathConfig(config);
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }
    return _PyStatus_OK();
}


static PyStatus
pycore_init_runtime(_PyRuntimeState *runtime,
                    const PyConfig *config)
{
    if (runtime->initialized) {
        return _PyStatus_ERR("main interpreter already initialized");
    }

    PyStatus status = _PyConfig_Write(config, runtime);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    /* Py_Finalize leaves _Py_Finalizing set in order to help daemon
     * threads behave a little more gracefully at interpreter shutdown.
     * We clobber it here so the new interpreter can start with a clean
     * slate.
     *
     * However, this may still lead to misbehaviour if there are daemon
     * threads still hanging around from a previous Py_Initialize/Finalize
     * pair :(
     */
    _PyRuntimeState_SetFinalizing(runtime, NULL);

    status = _Py_HashRandomization_Init(config);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    status = _PyInterpreterState_Enable(runtime);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    return _PyStatus_OK();
}


static PyStatus
init_interp_create_gil(PyThreadState *tstate)
{
    PyStatus status;

    /* finalize_interp_delete() comment explains why _PyEval_FiniGIL() is
       only called here. */
    _PyEval_FiniGIL(tstate);

    /* Auto-thread-state API */
    status = _PyGILState_Init(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    /* Create the GIL and take it */
    status = _PyEval_InitGIL(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    return _PyStatus_OK();
}


static PyStatus
pycore_create_interpreter(_PyRuntimeState *runtime,
                          const PyConfig *config,
                          PyThreadState **tstate_p)
{
    PyInterpreterState *interp = PyInterpreterState_New();
    if (interp == NULL) {
        return _PyStatus_ERR("can't make main interpreter");
    }

    PyStatus status = _PyInterpreterState_SetConfig(interp, config);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    PyThreadState *tstate = PyThreadState_New(interp);
    if (tstate == NULL) {
        return _PyStatus_ERR("can't make first thread");
    }
    (void) PyThreadState_Swap(tstate);

    status = init_interp_create_gil(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    *tstate_p = tstate;
    return _PyStatus_OK();
}


static PyStatus
pycore_init_types(PyThreadState *tstate)
{
    PyStatus status;
    int is_main_interp = _Py_IsMainInterpreter(tstate);

    status = _PyGC_Init(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    if (is_main_interp) {
        status = _PyTypes_Init();
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }


    if (!_PyLong_Init(tstate)) {
        return _PyStatus_ERR("can't init longs");
    }

    if (is_main_interp) {
        status = _PyUnicode_Init();
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }

    status = _PyExc_Init();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    if (is_main_interp) {
        if (!_PyFloat_Init()) {
            return _PyStatus_ERR("can't init float");
        }

        if (_PyStructSequence_Init() < 0) {
            return _PyStatus_ERR("can't initialize structseq");
        }
    }

    status = _PyErr_Init();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    if (is_main_interp) {
        if (!_PyContext_Init()) {
            return _PyStatus_ERR("can't init context");
        }
    }

    return _PyStatus_OK();
}


static PyStatus
pycore_init_builtins(PyThreadState *tstate)
{
    assert(!_PyErr_Occurred(tstate));

    PyObject *bimod = _PyBuiltin_Init(tstate);
    if (bimod == NULL) {
        goto error;
    }

    PyInterpreterState *interp = tstate->interp;
    if (_PyImport_FixupBuiltin(bimod, "builtins", interp->modules) < 0) {
        goto error;
    }

    PyObject *builtins_dict = PyModule_GetDict(bimod);
    if (builtins_dict == NULL) {
        goto error;
    }
    Py_INCREF(builtins_dict);
    interp->builtins = builtins_dict;

    PyStatus status = _PyBuiltins_AddExceptions(bimod);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    interp->builtins_copy = PyDict_Copy(interp->builtins);
    if (interp->builtins_copy == NULL) {
        goto error;
    }
    Py_DECREF(bimod);

    assert(!_PyErr_Occurred(tstate));

    return _PyStatus_OK();

error:
    Py_XDECREF(bimod);
    return _PyStatus_ERR("can't initialize builtins module");
}


static PyStatus
pycore_init_import_warnings(PyThreadState *tstate, PyObject *sysmod)
{
    assert(!_PyErr_Occurred(tstate));

    PyStatus status = _PyImportHooks_Init(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    const PyConfig *config = _PyInterpreterState_GetConfig(tstate->interp);
    if (_Py_IsMainInterpreter(tstate)) {
        /* Initialize _warnings. */
        status = _PyWarnings_InitState(tstate);
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }

        if (config->_install_importlib) {
            status = _PyConfig_WritePathConfig(config);
            if (_PyStatus_EXCEPTION(status)) {
                return status;
            }
        }
    }

    /* This call sets up builtin and frozen import support */
    if (config->_install_importlib) {
        status = init_importlib(tstate, sysmod);
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }

    assert(!_PyErr_Occurred(tstate));

    return _PyStatus_OK();
}


static PyStatus
pycore_interp_init(PyThreadState *tstate)
{
    PyStatus status;
    PyObject *sysmod = NULL;

    status = pycore_init_types(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        goto done;
    }

    status = _PySys_Create(tstate, &sysmod);
    if (_PyStatus_EXCEPTION(status)) {
        goto done;
    }

    status = pycore_init_builtins(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        goto done;
    }

    status = pycore_init_import_warnings(tstate, sysmod);

done:
    /* sys.modules['sys'] contains a strong reference to the module */
    Py_XDECREF(sysmod);
    return status;
}


static PyStatus
pyinit_config(_PyRuntimeState *runtime,
              PyThreadState **tstate_p,
              const PyConfig *config)
{
    PyStatus status = pycore_init_runtime(runtime, config);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    PyThreadState *tstate;
    status = pycore_create_interpreter(runtime, config, &tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    *tstate_p = tstate;

    status = pycore_interp_init(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    /* Only when we get here is the runtime core fully initialized */
    runtime->core_initialized = 1;
    return _PyStatus_OK();
}


PyStatus
_Py_PreInitializeFromPyArgv(const PyPreConfig *src_config, const _PyArgv *args)
{
    PyStatus status;

    if (src_config == NULL) {
        return _PyStatus_ERR("preinitialization config is NULL");
    }

    status = _PyRuntime_Initialize();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    _PyRuntimeState *runtime = &_PyRuntime;

    if (runtime->preinitialized) {
        /* If it's already configured: ignored the new configuration */
        return _PyStatus_OK();
    }

    /* Note: preinitialized remains 1 on error, it is only set to 0
       at exit on success. */
    runtime->preinitializing = 1;

    PyPreConfig config;

    status = _PyPreConfig_InitFromPreConfig(&config, src_config);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    status = _PyPreConfig_Read(&config, args);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    status = _PyPreConfig_Write(&config);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    runtime->preinitializing = 0;
    runtime->preinitialized = 1;
    return _PyStatus_OK();
}


PyStatus
Py_PreInitializeFromBytesArgs(const PyPreConfig *src_config, Py_ssize_t argc, char **argv)
{
    _PyArgv args = {.use_bytes_argv = 1, .argc = argc, .bytes_argv = argv};
    return _Py_PreInitializeFromPyArgv(src_config, &args);
}


PyStatus
Py_PreInitializeFromArgs(const PyPreConfig *src_config, Py_ssize_t argc, wchar_t **argv)
{
    _PyArgv args = {.use_bytes_argv = 0, .argc = argc, .wchar_argv = argv};
    return _Py_PreInitializeFromPyArgv(src_config, &args);
}


PyStatus
Py_PreInitialize(const PyPreConfig *src_config)
{
    return _Py_PreInitializeFromPyArgv(src_config, NULL);
}


PyStatus
_Py_PreInitializeFromConfig(const PyConfig *config,
                            const _PyArgv *args)
{
    assert(config != NULL);

    PyStatus status = _PyRuntime_Initialize();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    _PyRuntimeState *runtime = &_PyRuntime;

    if (runtime->preinitialized) {
        /* Already initialized: do nothing */
        return _PyStatus_OK();
    }

    PyPreConfig preconfig;

    _PyPreConfig_InitFromConfig(&preconfig, config);

    if (!config->parse_argv) {
        return Py_PreInitialize(&preconfig);
    }
    else if (args == NULL) {
        _PyArgv config_args = {
            .use_bytes_argv = 0,
            .argc = config->argv.length,
            .wchar_argv = config->argv.items};
        return _Py_PreInitializeFromPyArgv(&preconfig, &config_args);
    }
    else {
        return _Py_PreInitializeFromPyArgv(&preconfig, args);
    }
}


/* Begin interpreter initialization
 *
 * On return, the first thread and interpreter state have been created,
 * but the compiler, signal handling, multithreading and
 * multiple interpreter support, and codec infrastructure are not yet
 * available.
 *
 * The import system will support builtin and frozen modules only.
 * The only supported io is writing to sys.stderr
 *
 * If any operation invoked by this function fails, a fatal error is
 * issued and the function does not return.
 *
 * Any code invoked from this function should *not* assume it has access
 * to the Python C API (unless the API is explicitly listed as being
 * safe to call without calling Py_Initialize first)
 */
static PyStatus
pyinit_core(_PyRuntimeState *runtime,
            const PyConfig *src_config,
            PyThreadState **tstate_p)
{
    PyStatus status;

    status = _Py_PreInitializeFromConfig(src_config, NULL);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    PyConfig config;
    _PyConfig_InitCompatConfig(&config);

    status = _PyConfig_Copy(&config, src_config);
    if (_PyStatus_EXCEPTION(status)) {
        goto done;
    }

    status = PyConfig_Read(&config);
    if (_PyStatus_EXCEPTION(status)) {
        goto done;
    }

    if (!runtime->core_initialized) {
        status = pyinit_config(runtime, tstate_p, &config);
    }
    else {
        status = pyinit_core_reconfigure(runtime, tstate_p, &config);
    }
    if (_PyStatus_EXCEPTION(status)) {
        goto done;
    }

done:
    PyConfig_Clear(&config);
    return status;
}


/* Py_Initialize() has already been called: update the main interpreter
   configuration. Example of bpo-34008: Py_Main() called after
   Py_Initialize(). */
static PyStatus
_Py_ReconfigureMainInterpreter(PyThreadState *tstate)
{
    const PyConfig *config = _PyInterpreterState_GetConfig(tstate->interp);

    PyObject *argv = _PyWideStringList_AsList(&config->argv);
    if (argv == NULL) {
        return _PyStatus_NO_MEMORY(); \
    }

    int res = PyDict_SetItemString(tstate->interp->sysdict, "argv", argv);
    Py_DECREF(argv);
    if (res < 0) {
        return _PyStatus_ERR("fail to set sys.argv");
    }
    return _PyStatus_OK();
}


static PyStatus
init_interp_main(PyThreadState *tstate)
{
    assert(!_PyErr_Occurred(tstate));

    PyStatus status;
    int is_main_interp = _Py_IsMainInterpreter(tstate);
    PyInterpreterState *interp = tstate->interp;
    const PyConfig *config = _PyInterpreterState_GetConfig(interp);

    if (!config->_install_importlib) {
        /* Special mode for freeze_importlib: run with no import system
         *
         * This means anything which needs support from extension modules
         * or pure Python code in the standard library won't work.
         */
        if (is_main_interp) {
            interp->runtime->initialized = 1;
        }
        return _PyStatus_OK();
    }

    if (is_main_interp) {
        if (_PyTime_Init() < 0) {
            return _PyStatus_ERR("can't initialize time");
        }
    }

    if (_PySys_InitMain(tstate) < 0) {
        return _PyStatus_ERR("can't finish initializing sys");
    }

    status = init_importlib_external(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    if (is_main_interp) {
        /* initialize the faulthandler module */
        status = _PyFaulthandler_Init(config->faulthandler);
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }

    status = _PyUnicode_InitEncodings(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    if (is_main_interp) {
        if (_PySignal_Init(config->install_signal_handlers) < 0) {
            return _PyStatus_ERR("can't initialize signals");
        }

        if (_PyTraceMalloc_Init(config->tracemalloc) < 0) {
            return _PyStatus_ERR("can't initialize tracemalloc");
        }
    }

    status = init_sys_streams(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    status = init_set_builtins_open();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    status = add_main_module(interp);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }

    if (is_main_interp) {
        /* Initialize warnings. */
        PyObject *warnoptions = PySys_GetObject("warnoptions");
        if (warnoptions != NULL && PyList_Size(warnoptions) > 0)
        {
            PyObject *warnings_module = PyImport_ImportModule("warnings");
            if (warnings_module == NULL) {
                fprintf(stderr, "'import warnings' failed; traceback:\n");
                _PyErr_Print(tstate);
            }
            Py_XDECREF(warnings_module);
        }

        interp->runtime->initialized = 1;
    }

    if (config->site_import) {
        status = init_import_site();
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }

    if (is_main_interp) {
#ifndef MS_WINDOWS
        emit_stderr_warning_for_legacy_locale(interp->runtime);
#endif
    }

    assert(!_PyErr_Occurred(tstate));

    return _PyStatus_OK();
}


/* Update interpreter state based on supplied configuration settings
 *
 * After calling this function, most of the restrictions on the interpreter
 * are lifted. The only remaining incomplete settings are those related
 * to the main module (sys.argv[0], __main__ metadata)
 *
 * Calling this when the interpreter is not initializing, is already
 * initialized or without a valid current thread state is a fatal error.
 * Other errors should be reported as normal Python exceptions with a
 * non-zero return code.
 */
static PyStatus
pyinit_main(PyThreadState *tstate)
{
    PyInterpreterState *interp = tstate->interp;
    if (!interp->runtime->core_initialized) {
        return _PyStatus_ERR("runtime core not initialized");
    }

    if (interp->runtime->initialized) {
        return _Py_ReconfigureMainInterpreter(tstate);
    }

    PyStatus status = init_interp_main(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    return _PyStatus_OK();
}


PyStatus
_Py_InitializeMain(void)
{
    PyStatus status = _PyRuntime_Initialize();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    _PyRuntimeState *runtime = &_PyRuntime;
    PyThreadState *tstate = _PyRuntimeState_GetThreadState(runtime);
    return pyinit_main(tstate);
}


PyStatus
Py_InitializeFromConfig(const PyConfig *config)
{
    if (config == NULL) {
        return _PyStatus_ERR("initialization config is NULL");
    }

    PyStatus status;

    status = _PyRuntime_Initialize();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    _PyRuntimeState *runtime = &_PyRuntime;

    PyThreadState *tstate = NULL;
    status = pyinit_core(runtime, config, &tstate);
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    config = _PyInterpreterState_GetConfig(tstate->interp);

    if (config->_init_main) {
        status = pyinit_main(tstate);
        if (_PyStatus_EXCEPTION(status)) {
            return status;
        }
    }

    return _PyStatus_OK();
}


void
Py_InitializeEx(int install_sigs)
{
    PyStatus status;

    status = _PyRuntime_Initialize();
    if (_PyStatus_EXCEPTION(status)) {
        Py_ExitStatusException(status);
    }
    _PyRuntimeState *runtime = &_PyRuntime;

    if (runtime->initialized) {
        /* bpo-33932: Calling Py_Initialize() twice does nothing. */
        return;
    }

    PyConfig config;
    _PyConfig_InitCompatConfig(&config);

    config.install_signal_handlers = install_sigs;

    status = Py_InitializeFromConfig(&config);
    if (_PyStatus_EXCEPTION(status)) {
        Py_ExitStatusException(status);
    }
}

void
Py_Initialize(void)
{
    Py_InitializeEx(1);
}


/* Flush stdout and stderr */

static int
file_is_closed(PyObject *fobj)
{
    int r;
    PyObject *tmp = PyObject_GetAttrString(fobj, "closed");
    if (tmp == NULL) {
        PyErr_Clear();
        return 0;
    }
    r = PyObject_IsTrue(tmp);
    Py_DECREF(tmp);
    if (r < 0)
        PyErr_Clear();
    return r > 0;
}

static int
flush_std_files(void)
{
    PyObject *fout = _PySys_GetObjectId(&PyId_stdout);
    PyObject *ferr = _PySys_GetObjectId(&PyId_stderr);
    PyObject *tmp;
    int status = 0;

    if (fout != NULL && fout != Py_None && !file_is_closed(fout)) {
        tmp = _PyObject_CallMethodIdNoArgs(fout, &PyId_flush);
        if (tmp == NULL) {
            PyErr_WriteUnraisable(fout);
            status = -1;
        }
        else
            Py_DECREF(tmp);
    }

    if (ferr != NULL && ferr != Py_None && !file_is_closed(ferr)) {
        tmp = _PyObject_CallMethodIdNoArgs(ferr, &PyId_flush);
        if (tmp == NULL) {
            PyErr_Clear();
            status = -1;
        }
        else
            Py_DECREF(tmp);
    }

    return status;
}

/* Undo the effect of Py_Initialize().

   Beware: if multiple interpreter and/or thread states exist, these
   are not wiped out; only the current thread and interpreter state
   are deleted.  But since everything else is deleted, those other
   interpreter and thread states should no longer be used.

   (XXX We should do better, e.g. wipe out all interpreters and
   threads.)

   Locking: as above.

*/


static void
finalize_interp_types(PyThreadState *tstate, int is_main_interp)
{
    if (is_main_interp) {
        /* Sundry finalizers */
        _PyAST_Fini();
        _PyFrame_Fini();
        _PyTuple_Fini();
        _PyList_Fini();
        _PySet_Fini();
        _PyBytes_Fini();
    }

    _PyLong_Fini(tstate);

    if (is_main_interp) {
        _PyFloat_Fini();
        _PyDict_Fini();
        _PySlice_Fini();
    }

    _PyWarnings_Fini(tstate->interp);

    if (is_main_interp) {
        _Py_HashRandomization_Fini();
        _PyArg_Fini();
        _PyAsyncGen_Fini();
        _PyContext_Fini();
    }

    /* Cleanup Unicode implementation */
    _PyUnicode_Fini(tstate);

    if (is_main_interp) {
        _Py_ClearFileSystemEncoding();
    }
}


static void
finalize_interp_clear(PyThreadState *tstate)
{
    int is_main_interp = _Py_IsMainInterpreter(tstate);

    /* Clear interpreter state and all thread states */
    PyInterpreterState_Clear(tstate->interp);

    /* Trigger a GC collection on subinterpreters*/
    if (!is_main_interp) {
        _PyGC_CollectNoFail();
    }

    /* Clear all loghooks */
    /* Both _PySys_Audit function and users still need PyObject, such as tuple.
       Call _PySys_ClearAuditHooks when PyObject available. */
    if (is_main_interp) {
        _PySys_ClearAuditHooks(tstate);
    }

    finalize_interp_types(tstate, is_main_interp);

    if (is_main_interp) {
        /* XXX Still allocated:
           - various static ad-hoc pointers to interned strings
           - int and float free list blocks
           - whatever various modules and libraries allocate
        */

        PyGrammar_RemoveAccelerators(&_PyParser_Grammar);

        _PyExc_Fini();
    }

    _PyGC_Fini(tstate);
}


static void
finalize_interp_delete(PyThreadState *tstate)
{
    if (_Py_IsMainInterpreter(tstate)) {
        /* Cleanup auto-thread-state */
        _PyGILState_Fini(tstate);
    }

    /* We can't call _PyEval_FiniGIL() here because destroying the GIL lock can
       fail when it is being awaited by another running daemon thread (see
       bpo-9901). Instead pycore_create_interpreter() destroys the previously
       created GIL, which ensures that Py_Initialize / Py_FinalizeEx can be
       called multiple times. */

    PyInterpreterState_Delete(tstate->interp);
}


int
Py_FinalizeEx(void)
{
    int status = 0;

    _PyRuntimeState *runtime = &_PyRuntime;
    if (!runtime->initialized) {
        return status;
    }

    /* Get current thread state and interpreter pointer */
    PyThreadState *tstate = _PyRuntimeState_GetThreadState(runtime);

    // Wrap up existing "threading"-module-created, non-daemon threads.
    wait_for_thread_shutdown(tstate);

    // Make any remaining pending calls.
    _Py_FinishPendingCalls(tstate);

    /* The interpreter is still entirely intact at this point, and the
     * exit funcs may be relying on that.  In particular, if some thread
     * or exit func is still waiting to do an import, the import machinery
     * expects Py_IsInitialized() to return true.  So don't say the
     * runtime is uninitialized until after the exit funcs have run.
     * Note that Threading.py uses an exit func to do a join on all the
     * threads created thru it, so this also protects pending imports in
     * the threads created via Threading.
     */

    call_py_exitfuncs(tstate);

    /* Copy the core config, PyInterpreterState_Delete() free
       the core config memory */
#ifdef Py_REF_DEBUG
    int show_ref_count = tstate->interp->config.show_ref_count;
#endif
#ifdef Py_TRACE_REFS
    int dump_refs = tstate->interp->config.dump_refs;
#endif
#ifdef WITH_PYMALLOC
    int malloc_stats = tstate->interp->config.malloc_stats;
#endif

    /* Remaining daemon threads will automatically exit
       when they attempt to take the GIL (ex: PyEval_RestoreThread()). */
    _PyRuntimeState_SetFinalizing(runtime, tstate);
    runtime->initialized = 0;
    runtime->core_initialized = 0;

    /* Destroy the state of all threads of the interpreter, except of the
       current thread. In practice, only daemon threads should still be alive,
       except if wait_for_thread_shutdown() has been cancelled by CTRL+C.
       Clear frames of other threads to call objects destructors. Destructors
       will be called in the current Python thread. Since
       _PyRuntimeState_SetFinalizing() has been called, no other Python thread
       can take the GIL at this point: if they try, they will exit
       immediately. */
    _PyThreadState_DeleteExcept(runtime, tstate);

    /* Flush sys.stdout and sys.stderr */
    if (flush_std_files() < 0) {
        status = -1;
    }

    /* Disable signal handling */
    PyOS_FiniInterrupts();

    /* Collect garbage.  This may call finalizers; it's nice to call these
     * before all modules are destroyed.
     * XXX If a __del__ or weakref callback is triggered here, and tries to
     * XXX import a module, bad things can happen, because Python no
     * XXX longer believes it's initialized.
     * XXX     Fatal Python error: Interpreter not initialized (version mismatch?)
     * XXX is easy to provoke that way.  I've also seen, e.g.,
     * XXX     Exception exceptions.ImportError: 'No module named sha'
     * XXX         in <function callback at 0x008F5718> ignored
     * XXX but I'm unclear on exactly how that one happens.  In any case,
     * XXX I haven't seen a real-life report of either of these.
     */
    _PyGC_CollectIfEnabled();

    /* Destroy all modules */
    _PyImport_Cleanup(tstate);

    /* Print debug stats if any */
    _PyEval_Fini();

    /* Flush sys.stdout and sys.stderr (again, in case more was printed) */
    if (flush_std_files() < 0) {
        status = -1;
    }

    /* Collect final garbage.  This disposes of cycles created by
     * class definitions, for example.
     * XXX This is disabled because it caused too many problems.  If
     * XXX a __del__ or weakref callback triggers here, Python code has
     * XXX a hard time running, because even the sys module has been
     * XXX cleared out (sys.stdout is gone, sys.excepthook is gone, etc).
     * XXX One symptom is a sequence of information-free messages
     * XXX coming from threads (if a __del__ or callback is invoked,
     * XXX other threads can execute too, and any exception they encounter
     * XXX triggers a comedy of errors as subsystem after subsystem
     * XXX fails to find what it *expects* to find in sys to help report
     * XXX the exception and consequent unexpected failures).  I've also
     * XXX seen segfaults then, after adding print statements to the
     * XXX Python code getting called.
     */
#if 0
    _PyGC_CollectIfEnabled();
#endif

    /* Disable tracemalloc after all Python objects have been destroyed,
       so it is possible to use tracemalloc in objects destructor. */
    _PyTraceMalloc_Fini();

    /* Destroy the database used by _PyImport_{Fixup,Find}Extension */
    _PyImport_Fini();

    /* Cleanup typeobject.c's internal caches. */
    _PyType_Fini();

    /* unload faulthandler module */
    _PyFaulthandler_Fini();

    /* dump hash stats */
    _PyHash_Fini();

#ifdef Py_REF_DEBUG
    if (show_ref_count) {
        _PyDebug_PrintTotalRefs();
    }
#endif

#ifdef Py_TRACE_REFS
    /* Display all objects still alive -- this can invoke arbitrary
     * __repr__ overrides, so requires a mostly-intact interpreter.
     * Alas, a lot of stuff may still be alive now that will be cleaned
     * up later.
     */
    if (dump_refs) {
        _Py_PrintReferences(stderr);
    }
#endif /* Py_TRACE_REFS */

    finalize_interp_clear(tstate);
    finalize_interp_delete(tstate);

#ifdef Py_TRACE_REFS
    /* Display addresses (& refcnts) of all objects still alive.
     * An address can be used to find the repr of the object, printed
     * above by _Py_PrintReferences.
     */
    if (dump_refs) {
        _Py_PrintReferenceAddresses(stderr);
    }
#endif /* Py_TRACE_REFS */
#ifdef WITH_PYMALLOC
    if (malloc_stats) {
        _PyObject_DebugMallocStats(stderr);
    }
#endif

    call_ll_exitfuncs(runtime);

    _PyRuntime_Finalize();
    return status;
}

void
Py_Finalize(void)
{
    Py_FinalizeEx();
}


/* Create and initialize a new interpreter and thread, and return the
   new thread.  This requires that Py_Initialize() has been called
   first.

   Unsuccessful initialization yields a NULL pointer.  Note that *no*
   exception information is available even in this case -- the
   exception information is held in the thread, and there is no
   thread.

   Locking: as above.

*/

static PyStatus
new_interpreter(PyThreadState **tstate_p, int isolated_subinterpreter)
{
    PyStatus status;

    status = _PyRuntime_Initialize();
    if (_PyStatus_EXCEPTION(status)) {
        return status;
    }
    _PyRuntimeState *runtime = &_PyRuntime;

    if (!runtime->initialized) {
        return _PyStatus_ERR("Py_Initialize must be called first");
    }

    /* Issue #10915, #15751: The GIL API doesn't work with multiple
       interpreters: disable PyGILState_Check(). */
    runtime->gilstate.check_enabled = 0;

    PyInterpreterState *interp = PyInterpreterState_New();
    if (interp == NULL) {
        *tstate_p = NULL;
        return _PyStatus_OK();
    }

    PyThreadState *tstate = PyThreadState_New(interp);
    if (tstate == NULL) {
        PyInterpreterState_Delete(interp);
        *tstate_p = NULL;
        return _PyStatus_OK();
    }

    PyThreadState *save_tstate = PyThreadState_Swap(tstate);

    /* Copy the current interpreter config into the new interpreter */
    const PyConfig *config;
    if (save_tstate != NULL) {
        config = _PyInterpreterState_GetConfig(save_tstate->interp);
    }
    else
    {
        /* No current thread state, copy from the main interpreter */
        PyInterpreterState *main_interp = PyInterpreterState_Main();
        config = _PyInterpreterState_GetConfig(main_interp);
    }

    status = _PyInterpreterState_SetConfig(interp, config);
    if (_PyStatus_EXCEPTION(status)) {
        goto error;
    }
    interp->config._isolated_interpreter = isolated_subinterpreter;

    status = init_interp_create_gil(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        goto error;
    }

    status = pycore_interp_init(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        goto error;
    }

    status = init_interp_main(tstate);
    if (_PyStatus_EXCEPTION(status)) {
        goto error;
    }

    *tstate_p = tstate;
    return _PyStatus_OK();

error:
    *tstate_p = NULL;

    /* Oops, it didn't work.  Undo it all. */
    PyErr_PrintEx(0);
    PyThreadState_Clear(tstate);
    PyThreadState_Delete(tstate);
    PyInterpreterState_Delete(interp);
    PyThreadState_Swap(save_tstate);

    return status;
}

PyThreadState *
_Py_NewInterpreter(int isolated_subinterpreter)
{
    PyThreadState *tstate = NULL;
    PyStatus status = new_interpreter(&tstate, isolated_subinterpreter);
    if (_PyStatus_EXCEPTION(status)) {
        Py_ExitStatusException(status);
    }
    return tstate;

}

PyThreadState *
Py_NewInterpreter(void)
{
    return _Py_NewInterpreter(0);
}

/* Delete an interpreter and its last thread.  This requires that the
   given thread state is current, that the thread has no remaining
   frames, and that it is its interpreter's only remaining thread.
   It is a fatal error to violate these constraints.

   (Py_FinalizeEx() doesn't have these constraints -- it zaps
   everything, regardless.)

   Locking: as above.

*/

void
Py_EndInterpreter(PyThreadState *tstate)
{
    PyInterpreterState *interp = tstate->interp;

    if (tstate != _PyThreadState_GET()) {
        Py_FatalError("thread is not current");
    }
    if (tstate->frame != NULL) {
        Py_FatalError("thread still has a frame");
    }
    interp->finalizing = 1;

    // Wrap up existing "threading"-module-created, non-daemon threads.
    wait_for_thread_shutdown(tstate);

    call_py_exitfuncs(tstate);

    if (tstate != interp->tstate_head || tstate->next != NULL) {
        Py_FatalError("not the last thread");
    }

    _PyImport_Cleanup(tstate);
    finalize_interp_clear(tstate);
    finalize_interp_delete(tstate);
}

/* Add the __main__ module */

static PyStatus
add_main_module(PyInterpreterState *interp)
{
    PyObject *m, *d, *loader, *ann_dict;
    m = PyImport_AddModule("__main__");
    if (m == NULL)
        return _PyStatus_ERR("can't create __main__ module");

    d = PyModule_GetDict(m);
    ann_dict = PyDict_New();
    if ((ann_dict == NULL) ||
        (PyDict_SetItemString(d, "__annotations__", ann_dict) < 0)) {
        return _PyStatus_ERR("Failed to initialize __main__.__annotations__");
    }
    Py_DECREF(ann_dict);

    if (PyDict_GetItemString(d, "__builtins__") == NULL) {
        PyObject *bimod = PyImport_ImportModule("builtins");
        if (bimod == NULL) {
            return _PyStatus_ERR("Failed to retrieve builtins module");
        }
        if (PyDict_SetItemString(d, "__builtins__", bimod) < 0) {
            return _PyStatus_ERR("Failed to initialize __main__.__builtins__");
        }
        Py_DECREF(bimod);
    }

    /* Main is a little special - imp.is_builtin("__main__") will return
     * False, but BuiltinImporter is still the most appropriate initial
     * setting for its __loader__ attribute. A more suitable value will
     * be set if __main__ gets further initialized later in the startup
     * process.
     */
    loader = PyDict_GetItemString(d, "__loader__");
    if (loader == NULL || loader == Py_None) {
        PyObject *loader = PyObject_GetAttrString(interp->importlib,
                                                  "BuiltinImporter");
        if (loader == NULL) {
            return _PyStatus_ERR("Failed to retrieve BuiltinImporter");
        }
        if (PyDict_SetItemString(d, "__loader__", loader) < 0) {
            return _PyStatus_ERR("Failed to initialize __main__.__loader__");
        }
        Py_DECREF(loader);
    }
    return _PyStatus_OK();
}

/* Import the site module (not into __main__ though) */

static PyStatus
init_import_site(void)
{
    PyObject *m;
    m = PyImport_ImportModule("site");
    if (m == NULL) {
        return _PyStatus_ERR("Failed to import the site module");
    }
    Py_DECREF(m);
    return _PyStatus_OK();
}

/* Check if a file descriptor is valid or not.
   Return 0 if the file descriptor is invalid, return non-zero otherwise. */
static int
is_valid_fd(int fd)
{
/* dup() is faster than fstat(): fstat() can require input/output operations,
   whereas dup() doesn't. There is a low risk of EMFILE/ENFILE at Python
   startup. Problem: dup() doesn't check if the file descriptor is valid on
   some platforms.

   bpo-30225: On macOS Tiger, when stdout is redirected to a pipe and the other
   side of the pipe is closed, dup(1) succeed, whereas fstat(1, &st) fails with
   EBADF. FreeBSD has similar issue (bpo-32849).

   Only use dup() on platforms where dup() is enough to detect invalid FD in
   corner cases: on Linux and Windows (bpo-32849). */
#if defined(__linux__) || defined(MS_WINDOWS)
    if (fd < 0) {
        return 0;
    }
    int fd2;

    _Py_BEGIN_SUPPRESS_IPH
    fd2 = dup(fd);
    if (fd2 >= 0) {
        close(fd2);
    }
    _Py_END_SUPPRESS_IPH

    return (fd2 >= 0);
#else
    struct stat st;
    return (fstat(fd, &st) == 0);
#endif
}

/* returns Py_None if the fd is not valid */
static PyObject*
create_stdio(const PyConfig *config, PyObject* io,
    int fd, int write_mode, const char* name,
    const wchar_t* encoding, const wchar_t* errors)
{
    PyObject *buf = NULL, *stream = NULL, *text = NULL, *raw = NULL, *res;
    const char* mode;
    const char* newline;
    PyObject *line_buffering, *write_through;
    int buffering, isatty;
    _Py_IDENTIFIER(open);
    _Py_IDENTIFIER(isatty);
    _Py_IDENTIFIER(TextIOWrapper);
    _Py_IDENTIFIER(mode);
    const int buffered_stdio = config->buffered_stdio;

    if (!is_valid_fd(fd))
        Py_RETURN_NONE;

    /* stdin is always opened in buffered mode, first because it shouldn't
       make a difference in common use cases, second because TextIOWrapper
       depends on the presence of a read1() method which only exists on
       buffered streams.
    */
    if (!buffered_stdio && write_mode)
        buffering = 0;
    else
        buffering = -1;
    if (write_mode)
        mode = "wb";
    else
        mode = "rb";
    buf = _PyObject_CallMethodId(io, &PyId_open, "isiOOOO",
                                 fd, mode, buffering,
                                 Py_None, Py_None, /* encoding, errors */
                                 Py_None, Py_False); /* newline, closefd */
    if (buf == NULL)
        goto error;

    if (buffering) {
        _Py_IDENTIFIER(raw);
        raw = _PyObject_GetAttrId(buf, &PyId_raw);
        if (raw == NULL)
            goto error;
    }
    else {
        raw = buf;
        Py_INCREF(raw);
    }

#ifdef MS_WINDOWS
    /* Windows console IO is always UTF-8 encoded */
    if (PyWindowsConsoleIO_Check(raw))
        encoding = L"utf-8";
#endif

    text = PyUnicode_FromString(name);
    if (text == NULL || _PyObject_SetAttrId(raw, &PyId_name, text) < 0)
        goto error;
    res = _PyObject_CallMethodIdNoArgs(raw, &PyId_isatty);
    if (res == NULL)
        goto error;
    isatty = PyObject_IsTrue(res);
    Py_DECREF(res);
    if (isatty == -1)
        goto error;
    if (!buffered_stdio)
        write_through = Py_True;
    else
        write_through = Py_False;
    if (buffered_stdio && (isatty || fd == fileno(stderr)))
        line_buffering = Py_True;
    else
        line_buffering = Py_False;

    Py_CLEAR(raw);
    Py_CLEAR(text);

#ifdef MS_WINDOWS
    /* sys.stdin: enable universal newline mode, translate "\r\n" and "\r"
       newlines to "\n".
       sys.stdout and sys.stderr: translate "\n" to "\r\n". */
    newline = NULL;
#else
    /* sys.stdin: split lines at "\n".
       sys.stdout and sys.stderr: don't translate newlines (use "\n"). */
    newline = "\n";
#endif

    PyObject *encoding_str = PyUnicode_FromWideChar(encoding, -1);
    if (encoding_str == NULL) {
        Py_CLEAR(buf);
        goto error;
    }

    PyObject *errors_str = PyUnicode_FromWideChar(errors, -1);
    if (errors_str == NULL) {
        Py_CLEAR(buf);
        Py_CLEAR(encoding_str);
        goto error;
    }

    stream = _PyObject_CallMethodId(io, &PyId_TextIOWrapper, "OOOsOO",
                                    buf, encoding_str, errors_str,
                                    newline, line_buffering, write_through);
    Py_CLEAR(buf);
    Py_CLEAR(encoding_str);
    Py_CLEAR(errors_str);
    if (stream == NULL)
        goto error;

    if (write_mode)
        mode = "w";
    else
        mode = "r";
    text = PyUnicode_FromString(mode);
    if (!text || _PyObject_SetAttrId(stream, &PyId_mode, text) < 0)
        goto error;
    Py_CLEAR(text);
    return stream;

error:
    Py_XDECREF(buf);
    Py_XDECREF(stream);
    Py_XDECREF(text);
    Py_XDECREF(raw);

    if (PyErr_ExceptionMatches(PyExc_OSError) && !is_valid_fd(fd)) {
        /* Issue #24891: the file descriptor was closed after the first
           is_valid_fd() check was called. Ignore the OSError and set the
           stream to None. */
        PyErr_Clear();
        Py_RETURN_NONE;
    }
    return NULL;
}

/* Set builtins.open to io.OpenWrapper */
static PyStatus
init_set_builtins_open(void)
{
    PyObject *iomod = NULL, *wrapper;
    PyObject *bimod = NULL;
    PyStatus res = _PyStatus_OK();

    if (!(iomod = PyImport_ImportModule("io"))) {
        goto error;
    }

    if (!(bimod = PyImport_ImportModule("builtins"))) {
        goto error;
    }

    if (!(wrapper = PyObject_GetAttrString(iomod, "OpenWrapper"))) {
        goto error;
    }

    /* Set builtins.open */
    if (PyObject_SetAttrString(bimod, "open", wrapper) == -1) {
        Py_DECREF(wrapper);
        goto error;
    }
    Py_DECREF(wrapper);
    goto done;

error:
    res = _PyStatus_ERR("can't initialize io.open");

done:
    Py_XDECREF(bimod);
    Py_XDECREF(iomod);
    return res;
}


/* Initialize sys.stdin, stdout, stderr and builtins.open */
static PyStatus
init_sys_streams(PyThreadState *tstate)
{
    PyObject *iomod = NULL;
    PyObject *m;
    PyObject *std = NULL;
    int fd;
    PyObject * encoding_attr;
    PyStatus res = _PyStatus_OK();
    const PyConfig *config = _PyInterpreterState_GetConfig(tstate->interp);

    /* Check that stdin is not a directory
       Using shell redirection, you can redirect stdin to a directory,
       crashing the Python interpreter. Catch this common mistake here
       and output a useful error message. Note that under MS Windows,
       the shell already prevents that. */
#ifndef MS_WINDOWS
    struct _Py_stat_struct sb;
    if (_Py_fstat_noraise(fileno(stdin), &sb) == 0 &&
        S_ISDIR(sb.st_mode)) {
        return _PyStatus_ERR("<stdin> is a directory, cannot continue");
    }
#endif

    /* Hack to avoid a nasty recursion issue when Python is invoked
       in verbose mode: pre-import the Latin-1 and UTF-8 codecs */
    if ((m = PyImport_ImportModule("encodings.utf_8")) == NULL) {
        goto error;
    }
    Py_DECREF(m);

    if (!(m = PyImport_ImportModule("encodings.latin_1"))) {
        goto error;
    }
    Py_DECREF(m);

    if (!(iomod = PyImport_ImportModule("io"))) {
        goto error;
    }

    /* Set sys.stdin */
    fd = fileno(stdin);
    /* Under some conditions stdin, stdout and stderr may not be connected
     * and fileno() may point to an invalid file descriptor. For example
     * GUI apps don't have valid standard streams by default.
     */
    std = create_stdio(config, iomod, fd, 0, "<stdin>",
                       config->stdio_encoding,
                       config->stdio_errors);
    if (std == NULL)
        goto error;
    PySys_SetObject("__stdin__", std);
    _PySys_SetObjectId(&PyId_stdin, std);
    Py_DECREF(std);

    /* Set sys.stdout */
    fd = fileno(stdout);
    std = create_stdio(config, iomod, fd, 1, "<stdout>",
                       config->stdio_encoding,
                       config->stdio_errors);
    if (std == NULL)
        goto error;
    PySys_SetObject("__stdout__", std);
    _PySys_SetObjectId(&PyId_stdout, std);
    Py_DECREF(std);

#if 1 /* Disable this if you have trouble debugging bootstrap stuff */
    /* Set sys.stderr, replaces the preliminary stderr */
    fd = fileno(stderr);
    std = create_stdio(config, iomod, fd, 1, "<stderr>",
                       config->stdio_encoding,
                       L"backslashreplace");
    if (std == NULL)
        goto error;

    /* Same as hack above, pre-import stderr's codec to avoid recursion
       when import.c tries to write to stderr in verbose mode. */
    encoding_attr = PyObject_GetAttrString(std, "encoding");
    if (encoding_attr != NULL) {
        const char *std_encoding = PyUnicode_AsUTF8(encoding_attr);
        if (std_encoding != NULL) {
            PyObject *codec_info = _PyCodec_Lookup(std_encoding);
            Py_XDECREF(codec_info);
        }
        Py_DECREF(encoding_attr);
    }
    _PyErr_Clear(tstate);  /* Not a fatal error if codec isn't available */

    if (PySys_SetObject("__stderr__", std) < 0) {
        Py_DECREF(std);
        goto error;
    }
    if (_PySys_SetObjectId(&PyId_stderr, std) < 0) {
        Py_DECREF(std);
        goto error;
    }
    Py_DECREF(std);
#endif

    goto done;

error:
    res = _PyStatus_ERR("can't initialize sys standard streams");

done:
    _Py_ClearStandardStreamEncoding();
    Py_XDECREF(iomod);
    return res;
}


static void
_Py_FatalError_DumpTracebacks(int fd, PyInterpreterState *interp,
                              PyThreadState *tstate)
{
    fputc('\n', stderr);
    fflush(stderr);

    /* display the current Python stack */
    _Py_DumpTracebackThreads(fd, interp, tstate);
}

/* Print the current exception (if an exception is set) with its traceback,
   or display the current Python stack.

   Don't call PyErr_PrintEx() and the except hook, because Py_FatalError() is
   called on catastrophic cases.

   Return 1 if the traceback was displayed, 0 otherwise. */

static int
_Py_FatalError_PrintExc(PyThreadState *tstate)
{
    PyObject *ferr, *res;
    PyObject *exception, *v, *tb;
    int has_tb;

    _PyErr_Fetch(tstate, &exception, &v, &tb);
    if (exception == NULL) {
        /* No current exception */
        return 0;
    }

    ferr = _PySys_GetObjectId(&PyId_stderr);
    if (ferr == NULL || ferr == Py_None) {
        /* sys.stderr is not set yet or set to None,
           no need to try to display the exception */
        return 0;
    }

    _PyErr_NormalizeException(tstate, &exception, &v, &tb);
    if (tb == NULL) {
        tb = Py_None;
        Py_INCREF(tb);
    }
    PyException_SetTraceback(v, tb);
    if (exception == NULL) {
        /* PyErr_NormalizeException() failed */
        return 0;
    }

    has_tb = (tb != Py_None);
    PyErr_Display(exception, v, tb);
    Py_XDECREF(exception);
    Py_XDECREF(v);
    Py_XDECREF(tb);

    /* sys.stderr may be buffered: call sys.stderr.flush() */
    res = _PyObject_CallMethodIdNoArgs(ferr, &PyId_flush);
    if (res == NULL) {
        _PyErr_Clear(tstate);
    }
    else {
        Py_DECREF(res);
    }

    return has_tb;
}

/* Print fatal error message and abort */

#ifdef MS_WINDOWS
static void
fatal_output_debug(const char *msg)
{
    /* buffer of 256 bytes allocated on the stack */
    WCHAR buffer[256 / sizeof(WCHAR)];
    size_t buflen = Py_ARRAY_LENGTH(buffer) - 1;
    size_t msglen;

    OutputDebugStringW(L"Fatal Python error: ");

    msglen = strlen(msg);
    while (msglen) {
        size_t i;

        if (buflen > msglen) {
            buflen = msglen;
        }

        /* Convert the message to wchar_t. This uses a simple one-to-one
           conversion, assuming that the this error message actually uses
           ASCII only. If this ceases to be true, we will have to convert. */
        for (i=0; i < buflen; ++i) {
            buffer[i] = msg[i];
        }
        buffer[i] = L'\0';
        OutputDebugStringW(buffer);

        msg += buflen;
        msglen -= buflen;
    }
    OutputDebugStringW(L"\n");
}
#endif


static void
fatal_error_dump_runtime(FILE *stream, _PyRuntimeState *runtime)
{
    fprintf(stream, "Python runtime state: ");
    PyThreadState *finalizing = _PyRuntimeState_GetFinalizing(runtime);
    if (finalizing) {
        fprintf(stream, "finalizing (tstate=%p)", finalizing);
    }
    else if (runtime->initialized) {
        fprintf(stream, "initialized");
    }
    else if (runtime->core_initialized) {
        fprintf(stream, "core initialized");
    }
    else if (runtime->preinitialized) {
        fprintf(stream, "preinitialized");
    }
    else if (runtime->preinitializing) {
        fprintf(stream, "preinitializing");
    }
    else {
        fprintf(stream, "unknown");
    }
    fprintf(stream, "\n");
    fflush(stream);
}


static inline void _Py_NO_RETURN
fatal_error_exit(int status)
{
    if (status < 0) {
#if defined(MS_WINDOWS) && defined(_DEBUG)
        DebugBreak();
#endif
        abort();
    }
    else {
        exit(status);
    }
}


static void _Py_NO_RETURN
fatal_error(FILE *stream, int header, const char *prefix, const char *msg,
            int status)
{
    const int fd = fileno(stream);
    static int reentrant = 0;

    if (reentrant) {
        /* Py_FatalError() caused a second fatal error.
           Example: flush_std_files() raises a recursion error. */
        fatal_error_exit(status);
    }
    reentrant = 1;

    if (header) {
        fprintf(stream, "Fatal Python error: ");
        if (prefix) {
            fputs(prefix, stream);
            fputs(": ", stream);
        }
        if (msg) {
            fputs(msg, stream);
        }
        else {
            fprintf(stream, "<message not set>");
        }
        fputs("\n", stream);
        fflush(stream);
    }

    _PyRuntimeState *runtime = &_PyRuntime;
    fatal_error_dump_runtime(stream, runtime);

    PyThreadState *tstate = _PyRuntimeState_GetThreadState(runtime);
    PyInterpreterState *interp = NULL;
    if (tstate != NULL) {
        interp = tstate->interp;
    }

    /* Check if the current thread has a Python thread state
       and holds the GIL.

       tss_tstate is NULL if Py_FatalError() is called from a C thread which
       has no Python thread state.

       tss_tstate != tstate if the current Python thread does not hold the GIL.
       */
    PyThreadState *tss_tstate = PyGILState_GetThisThreadState();
    int has_tstate_and_gil = (tss_tstate != NULL && tss_tstate == tstate);
    if (has_tstate_and_gil) {
        /* If an exception is set, print the exception with its traceback */
        if (!_Py_FatalError_PrintExc(tss_tstate)) {
            /* No exception is set, or an exception is set without traceback */
            _Py_FatalError_DumpTracebacks(fd, interp, tss_tstate);
        }
    }
    else {
        _Py_FatalError_DumpTracebacks(fd, interp, tss_tstate);
    }

    /* The main purpose of faulthandler is to display the traceback.
       This function already did its best to display a traceback.
       Disable faulthandler to prevent writing a second traceback
       on abort(). */
    _PyFaulthandler_Fini();

    /* Check if the current Python thread hold the GIL */
    if (has_tstate_and_gil) {
        /* Flush sys.stdout and sys.stderr */
        flush_std_files();
    }

#ifdef MS_WINDOWS
    fatal_output_debug(msg);
#endif /* MS_WINDOWS */

    fatal_error_exit(status);
}


#undef Py_FatalError

void _Py_NO_RETURN
Py_FatalError(const char *msg)
{
    fatal_error(stderr, 1, NULL, msg, -1);
}


void _Py_NO_RETURN
_Py_FatalErrorFunc(const char *func, const char *msg)
{
    fatal_error(stderr, 1, func, msg, -1);
}


void _Py_NO_RETURN
_Py_FatalErrorFormat(const char *func, const char *format, ...)
{
    static int reentrant = 0;
    if (reentrant) {
        /* _Py_FatalErrorFormat() caused a second fatal error */
        fatal_error_exit(-1);
    }
    reentrant = 1;

    FILE *stream = stderr;
    fprintf(stream, "Fatal Python error: ");
    if (func) {
        fputs(func, stream);
        fputs(": ", stream);
    }
    fflush(stream);

    va_list vargs;
#ifdef HAVE_STDARG_PROTOTYPES
    va_start(vargs, format);
#else
    va_start(vargs);
#endif
    vfprintf(stream, format, vargs);
    va_end(vargs);

    fputs("\n", stream);
    fflush(stream);

    fatal_error(stream, 0, NULL, NULL, -1);
}


void _Py_NO_RETURN
Py_ExitStatusException(PyStatus status)
{
    if (_PyStatus_IS_EXIT(status)) {
        exit(status.exitcode);
    }
    else if (_PyStatus_IS_ERROR(status)) {
        fatal_error(stderr, 1, status.func, status.err_msg, 1);
    }
    else {
        Py_FatalError("Py_ExitStatusException() must not be called on success");
    }
}

/* Clean up and exit */

/* For the atexit module. */
void _Py_PyAtExit(void (*func)(PyObject *), PyObject *module)
{
    PyInterpreterState *is = _PyInterpreterState_GET();

    /* Guard against API misuse (see bpo-17852) */
    assert(is->pyexitfunc == NULL || is->pyexitfunc == func);

    is->pyexitfunc = func;
    is->pyexitmodule = module;
}

static void
call_py_exitfuncs(PyThreadState *tstate)
{
    PyInterpreterState *interp = tstate->interp;
    if (interp->pyexitfunc == NULL)
        return;

    (*interp->pyexitfunc)(interp->pyexitmodule);
    _PyErr_Clear(tstate);
}

/* Wait until threading._shutdown completes, provided
   the threading module was imported in the first place.
   The shutdown routine will wait until all non-daemon
   "threading" threads have completed. */
static void
wait_for_thread_shutdown(PyThreadState *tstate)
{
    _Py_IDENTIFIER(_shutdown);
    PyObject *result;
    PyObject *threading = _PyImport_GetModuleId(&PyId_threading);
    if (threading == NULL) {
        if (_PyErr_Occurred(tstate)) {
            PyErr_WriteUnraisable(NULL);
        }
        /* else: threading not imported */
        return;
    }
    result = _PyObject_CallMethodIdNoArgs(threading, &PyId__shutdown);
    if (result == NULL) {
        PyErr_WriteUnraisable(threading);
    }
    else {
        Py_DECREF(result);
    }
    Py_DECREF(threading);
}

#define NEXITFUNCS 32
int Py_AtExit(void (*func)(void))
{
    if (_PyRuntime.nexitfuncs >= NEXITFUNCS)
        return -1;
    _PyRuntime.exitfuncs[_PyRuntime.nexitfuncs++] = func;
    return 0;
}

static void
call_ll_exitfuncs(_PyRuntimeState *runtime)
{
    while (runtime->nexitfuncs > 0) {
        /* pop last function from the list */
        runtime->nexitfuncs--;
        void (*exitfunc)(void) = runtime->exitfuncs[runtime->nexitfuncs];
        runtime->exitfuncs[runtime->nexitfuncs] = NULL;

        exitfunc();
    }

    fflush(stdout);
    fflush(stderr);
}

void _Py_NO_RETURN
Py_Exit(int sts)
{
    if (Py_FinalizeEx() < 0) {
        sts = 120;
    }

    exit(sts);
}


/* Restore signals that the interpreter has called SIG_IGN on to SIG_DFL.
 *
 * All of the code in this function must only use async-signal-safe functions,
 * listed at `man 7 signal` or
 * http://www.opengroup.org/onlinepubs/009695399/functions/xsh_chap02_04.html.
 *
 * If this function is updated, update also _posix_spawn() of subprocess.py.
 */
void
_Py_RestoreSignals(void)
{
#ifdef SIGPIPE
    PyOS_setsig(SIGPIPE, SIG_DFL);
#endif
#ifdef SIGXFZ
    PyOS_setsig(SIGXFZ, SIG_DFL);
#endif
#ifdef SIGXFSZ
    PyOS_setsig(SIGXFSZ, SIG_DFL);
#endif
}


/*
 * The file descriptor fd is considered ``interactive'' if either
 *   a) isatty(fd) is TRUE, or
 *   b) the -i flag was given, and the filename associated with
 *      the descriptor is NULL or "<stdin>" or "???".
 */
int
Py_FdIsInteractive(FILE *fp, const char *filename)
{
    if (isatty((int)fileno(fp)))
        return 1;
    if (!Py_InteractiveFlag)
        return 0;
    return (filename == NULL) ||
           (strcmp(filename, "<stdin>") == 0) ||
           (strcmp(filename, "???") == 0);
}


/* Wrappers around sigaction() or signal(). */

PyOS_sighandler_t
PyOS_getsig(int sig)
{
#ifdef HAVE_SIGACTION
    struct sigaction context;
    if (sigaction(sig, NULL, &context) == -1)
        return SIG_ERR;
    return context.sa_handler;
#else
    PyOS_sighandler_t handler;
/* Special signal handling for the secure CRT in Visual Studio 2005 */
#if defined(_MSC_VER) && _MSC_VER >= 1400
    switch (sig) {
    /* Only these signals are valid */
    case SIGINT:
    case SIGILL:
    case SIGFPE:
    case SIGSEGV:
    case SIGTERM:
    case SIGBREAK:
    case SIGABRT:
        break;
    /* Don't call signal() with other values or it will assert */
    default:
        return SIG_ERR;
    }
#endif /* _MSC_VER && _MSC_VER >= 1400 */
    handler = signal(sig, SIG_IGN);
    if (handler != SIG_ERR)
        signal(sig, handler);
    return handler;
#endif
}

/*
 * All of the code in this function must only use async-signal-safe functions,
 * listed at `man 7 signal` or
 * http://www.opengroup.org/onlinepubs/009695399/functions/xsh_chap02_04.html.
 */
PyOS_sighandler_t
PyOS_setsig(int sig, PyOS_sighandler_t handler)
{
#ifdef HAVE_SIGACTION
    /* Some code in Modules/signalmodule.c depends on sigaction() being
     * used here if HAVE_SIGACTION is defined.  Fix that if this code
     * changes to invalidate that assumption.
     */
    struct sigaction context, ocontext;
    context.sa_handler = handler;
    sigemptyset(&context.sa_mask);
    context.sa_flags = 0;
    if (sigaction(sig, &context, &ocontext) == -1)
        return SIG_ERR;
    return ocontext.sa_handler;
#else
    PyOS_sighandler_t oldhandler;
    oldhandler = signal(sig, handler);
#ifdef HAVE_SIGINTERRUPT
    siginterrupt(sig, 1);
#endif
    return oldhandler;
#endif
}

#ifdef __cplusplus
}
#endif
