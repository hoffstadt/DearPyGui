/* Float object implementation */

/* XXX There should be overflow checks here, but it's hard to check
   for any kind of float exception without losing portability. */

#include "Python.h"
#include "pycore_dtoa.h"

#include <ctype.h>
#include <float.h>

/*[clinic input]
class float "PyObject *" "&PyFloat_Type"
[clinic start generated code]*/
/*[clinic end generated code: output=da39a3ee5e6b4b0d input=dd0003f68f144284]*/

#include "clinic/floatobject.c.h"

/* Special free list
   free_list is a singly-linked list of available PyFloatObjects, linked
   via abuse of their ob_type members.
*/

#ifndef PyFloat_MAXFREELIST
#define PyFloat_MAXFREELIST    100
#endif
static int numfree = 0;
static PyFloatObject *free_list = NULL;

double
PyFloat_GetMax(void)
{
    return DBL_MAX;
}

double
PyFloat_GetMin(void)
{
    return DBL_MIN;
}

static PyTypeObject FloatInfoType;

PyDoc_STRVAR(floatinfo__doc__,
"sys.float_info\n\
\n\
A named tuple holding information about the float type. It contains low level\n\
information about the precision and internal representation. Please study\n\
your system's :file:`float.h` for more information.");

static PyStructSequence_Field floatinfo_fields[] = {
    {"max",             "DBL_MAX -- maximum representable finite float"},
    {"max_exp",         "DBL_MAX_EXP -- maximum int e such that radix**(e-1) "
                    "is representable"},
    {"max_10_exp",      "DBL_MAX_10_EXP -- maximum int e such that 10**e "
                    "is representable"},
    {"min",             "DBL_MIN -- Minimum positive normalized float"},
    {"min_exp",         "DBL_MIN_EXP -- minimum int e such that radix**(e-1) "
                    "is a normalized float"},
    {"min_10_exp",      "DBL_MIN_10_EXP -- minimum int e such that 10**e is "
                    "a normalized"},
    {"dig",             "DBL_DIG -- digits"},
    {"mant_dig",        "DBL_MANT_DIG -- mantissa digits"},
    {"epsilon",         "DBL_EPSILON -- Difference between 1 and the next "
                    "representable float"},
    {"radix",           "FLT_RADIX -- radix of exponent"},
    {"rounds",          "FLT_ROUNDS -- rounding mode"},
    {0}
};

static PyStructSequence_Desc floatinfo_desc = {
    "sys.float_info",           /* name */
    floatinfo__doc__,           /* doc */
    floatinfo_fields,           /* fields */
    11
};

PyObject *
PyFloat_GetInfo(void)
{
    PyObject* floatinfo;
    int pos = 0;

    floatinfo = PyStructSequence_New(&FloatInfoType);
    if (floatinfo == NULL) {
        return NULL;
    }

#define SetIntFlag(flag) \
    PyStructSequence_SET_ITEM(floatinfo, pos++, PyLong_FromLong(flag))
#define SetDblFlag(flag) \
    PyStructSequence_SET_ITEM(floatinfo, pos++, PyFloat_FromDouble(flag))

    SetDblFlag(DBL_MAX);
    SetIntFlag(DBL_MAX_EXP);
    SetIntFlag(DBL_MAX_10_EXP);
    SetDblFlag(DBL_MIN);
    SetIntFlag(DBL_MIN_EXP);
    SetIntFlag(DBL_MIN_10_EXP);
    SetIntFlag(DBL_DIG);
    SetIntFlag(DBL_MANT_DIG);
    SetDblFlag(DBL_EPSILON);
    SetIntFlag(FLT_RADIX);
    SetIntFlag(FLT_ROUNDS);
#undef SetIntFlag
#undef SetDblFlag

    if (PyErr_Occurred()) {
        Py_CLEAR(floatinfo);
        return NULL;
    }
    return floatinfo;
}

PyObject *
PyFloat_FromDouble(double fval)
{
    PyFloatObject *op = free_list;
    if (op != NULL) {
        free_list = (PyFloatObject *) Py_TYPE(op);
        numfree--;
    } else {
        op = (PyFloatObject*) PyObject_MALLOC(sizeof(PyFloatObject));
        if (!op)
            return PyErr_NoMemory();
    }
    /* Inline PyObject_New */
    (void)PyObject_INIT(op, &PyFloat_Type);
    op->ob_fval = fval;
    return (PyObject *) op;
}

static PyObject *
float_from_string_inner(const char *s, Py_ssize_t len, void *obj)
{
    double x;
    const char *end;
    const char *last = s + len;
    /* strip space */
    while (s < last && Py_ISSPACE(*s)) {
        s++;
    }

    while (s < last - 1 && Py_ISSPACE(last[-1])) {
        last--;
    }

    /* We don't care about overflow or underflow.  If the platform
     * supports them, infinities and signed zeroes (on underflow) are
     * fine. */
    x = PyOS_string_to_double(s, (char **)&end, NULL);
    if (end != last) {
        PyErr_Format(PyExc_ValueError,
                     "could not convert string to float: "
                     "%R", obj);
        return NULL;
    }
    else if (x == -1.0 && PyErr_Occurred()) {
        return NULL;
    }
    else {
        return PyFloat_FromDouble(x);
    }
}

PyObject *
PyFloat_FromString(PyObject *v)
{
    const char *s;
    PyObject *s_buffer = NULL;
    Py_ssize_t len;
    Py_buffer view = {NULL, NULL};
    PyObject *result = NULL;

    if (PyUnicode_Check(v)) {
        s_buffer = _PyUnicode_TransformDecimalAndSpaceToASCII(v);
        if (s_buffer == NULL)
            return NULL;
        assert(PyUnicode_IS_ASCII(s_buffer));
        /* Simply get a pointer to existing ASCII characters. */
        s = PyUnicode_AsUTF8AndSize(s_buffer, &len);
        assert(s != NULL);
    }
    else if (PyBytes_Check(v)) {
        s = PyBytes_AS_STRING(v);
        len = PyBytes_GET_SIZE(v);
    }
    else if (PyByteArray_Check(v)) {
        s = PyByteArray_AS_STRING(v);
        len = PyByteArray_GET_SIZE(v);
    }
    else if (PyObject_GetBuffer(v, &view, PyBUF_SIMPLE) == 0) {
        s = (const char *)view.buf;
        len = view.len;
        /* Copy to NUL-terminated buffer. */
        s_buffer = PyBytes_FromStringAndSize(s, len);
        if (s_buffer == NULL) {
            PyBuffer_Release(&view);
            return NULL;
        }
        s = PyBytes_AS_STRING(s_buffer);
    }
    else {
        PyErr_Format(PyExc_TypeError,
            "float() argument must be a string or a number, not '%.200s'",
            Py_TYPE(v)->tp_name);
        return NULL;
    }
    result = _Py_string_to_number_with_underscores(s, len, "float", v, v,
                                                   float_from_string_inner);
    PyBuffer_Release(&view);
    Py_XDECREF(s_buffer);
    return result;
}

static void
float_dealloc(PyFloatObject *op)
{
    if (PyFloat_CheckExact(op)) {
        if (numfree >= PyFloat_MAXFREELIST)  {
            PyObject_FREE(op);
            return;
        }
        numfree++;
        Py_SET_TYPE(op, (PyTypeObject *)free_list);
        free_list = op;
    }
    else
        Py_TYPE(op)->tp_free((PyObject *)op);
}

double
PyFloat_AsDouble(PyObject *op)
{
    PyNumberMethods *nb;
    PyObject *res;
    double val;

    if (op == NULL) {
        PyErr_BadArgument();
        return -1;
    }

    if (PyFloat_Check(op)) {
        return PyFloat_AS_DOUBLE(op);
    }

    nb = Py_TYPE(op)->tp_as_number;
    if (nb == NULL || nb->nb_float == NULL) {
        if (nb && nb->nb_index) {
            PyObject *res = PyNumber_Index(op);
            if (!res) {
                return -1;
            }
            double val = PyLong_AsDouble(res);
            Py_DECREF(res);
            return val;
        }
        PyErr_Format(PyExc_TypeError, "must be real number, not %.50s",
                     Py_TYPE(op)->tp_name);
        return -1;
    }

    res = (*nb->nb_float) (op);
    if (res == NULL) {
        return -1;
    }
    if (!PyFloat_CheckExact(res)) {
        if (!PyFloat_Check(res)) {
            PyErr_Format(PyExc_TypeError,
                         "%.50s.__float__ returned non-float (type %.50s)",
                         Py_TYPE(op)->tp_name, Py_TYPE(res)->tp_name);
            Py_DECREF(res);
            return -1;
        }
        if (PyErr_WarnFormat(PyExc_DeprecationWarning, 1,
                "%.50s.__float__ returned non-float (type %.50s).  "
                "The ability to return an instance of a strict subclass of float "
                "is deprecated, and may be removed in a future version of Python.",
                Py_TYPE(op)->tp_name, Py_TYPE(res)->tp_name)) {
            Py_DECREF(res);
            return -1;
        }
    }

    val = PyFloat_AS_DOUBLE(res);
    Py_DECREF(res);
    return val;
}

/* Macro and helper that convert PyObject obj to a C double and store
   the value in dbl.  If conversion to double raises an exception, obj is
   set to NULL, and the function invoking this macro returns NULL.  If
   obj is not of float or int type, Py_NotImplemented is incref'ed,
   stored in obj, and returned from the function invoking this macro.
*/
#define CONVERT_TO_DOUBLE(obj, dbl)                     \
    if (PyFloat_Check(obj))                             \
        dbl = PyFloat_AS_DOUBLE(obj);                   \
    else if (convert_to_double(&(obj), &(dbl)) < 0)     \
        return obj;

/* Methods */

static int
convert_to_double(PyObject **v, double *dbl)
{
    PyObject *obj = *v;

    if (PyLong_Check(obj)) {
        *dbl = PyLong_AsDouble(obj);
        if (*dbl == -1.0 && PyErr_Occurred()) {
            *v = NULL;
            return -1;
        }
    }
    else {
        Py_INCREF(Py_NotImplemented);
        *v = Py_NotImplemented;
        return -1;
    }
    return 0;
}

static PyObject *
float_repr(PyFloatObject *v)
{
    PyObject *result;
    char *buf;

    buf = PyOS_double_to_string(PyFloat_AS_DOUBLE(v),
                                'r', 0,
                                Py_DTSF_ADD_DOT_0,
                                NULL);
    if (!buf)
        return PyErr_NoMemory();
    result = _PyUnicode_FromASCII(buf, strlen(buf));
    PyMem_Free(buf);
    return result;
}

/* Comparison is pretty much a nightmare.  When comparing float to float,
 * we do it as straightforwardly (and long-windedly) as conceivable, so
 * that, e.g., Python x == y delivers the same result as the platform
 * C x == y when x and/or y is a NaN.
 * When mixing float with an integer type, there's no good *uniform* approach.
 * Converting the double to an integer obviously doesn't work, since we
 * may lose info from fractional bits.  Converting the integer to a double
 * also has two failure modes:  (1) an int may trigger overflow (too
 * large to fit in the dynamic range of a C double); (2) even a C long may have
 * more bits than fit in a C double (e.g., on a 64-bit box long may have
 * 63 bits of precision, but a C double probably has only 53), and then
 * we can falsely claim equality when low-order integer bits are lost by
 * coercion to double.  So this part is painful too.
 */

static PyObject*
float_richcompare(PyObject *v, PyObject *w, int op)
{
    double i, j;
    int r = 0;

    assert(PyFloat_Check(v));
    i = PyFloat_AS_DOUBLE(v);

    /* Switch on the type of w.  Set i and j to doubles to be compared,
     * and op to the richcomp to use.
     */
    if (PyFloat_Check(w))
        j = PyFloat_AS_DOUBLE(w);

    else if (!Py_IS_FINITE(i)) {
        if (PyLong_Check(w))
            /* If i is an infinity, its magnitude exceeds any
             * finite integer, so it doesn't matter which int we
             * compare i with.  If i is a NaN, similarly.
             */
            j = 0.0;
        else
            goto Unimplemented;
    }

    else if (PyLong_Check(w)) {
        int vsign = i == 0.0 ? 0 : i < 0.0 ? -1 : 1;
        int wsign = _PyLong_Sign(w);
        size_t nbits;
        int exponent;

        if (vsign != wsign) {
            /* Magnitudes are irrelevant -- the signs alone
             * determine the outcome.
             */
            i = (double)vsign;
            j = (double)wsign;
            goto Compare;
        }
        /* The signs are the same. */
        /* Convert w to a double if it fits.  In particular, 0 fits. */
        nbits = _PyLong_NumBits(w);
        if (nbits == (size_t)-1 && PyErr_Occurred()) {
            /* This long is so large that size_t isn't big enough
             * to hold the # of bits.  Replace with little doubles
             * that give the same outcome -- w is so large that
             * its magnitude must exceed the magnitude of any
             * finite float.
             */
            PyErr_Clear();
            i = (double)vsign;
            assert(wsign != 0);
            j = wsign * 2.0;
            goto Compare;
        }
        if (nbits <= 48) {
            j = PyLong_AsDouble(w);
            /* It's impossible that <= 48 bits overflowed. */
            assert(j != -1.0 || ! PyErr_Occurred());
            goto Compare;
        }
        assert(wsign != 0); /* else nbits was 0 */
        assert(vsign != 0); /* if vsign were 0, then since wsign is
                             * not 0, we would have taken the
                             * vsign != wsign branch at the start */
        /* We want to work with non-negative numbers. */
        if (vsign < 0) {
            /* "Multiply both sides" by -1; this also swaps the
             * comparator.
             */
            i = -i;
            op = _Py_SwappedOp[op];
        }
        assert(i > 0.0);
        (void) frexp(i, &exponent);
        /* exponent is the # of bits in v before the radix point;
         * we know that nbits (the # of bits in w) > 48 at this point
         */
        if (exponent < 0 || (size_t)exponent < nbits) {
            i = 1.0;
            j = 2.0;
            goto Compare;
        }
        if ((size_t)exponent > nbits) {
            i = 2.0;
            j = 1.0;
            goto Compare;
        }
        /* v and w have the same number of bits before the radix
         * point.  Construct two ints that have the same comparison
         * outcome.
         */
        {
            double fracpart;
            double intpart;
            PyObject *result = NULL;
            PyObject *vv = NULL;
            PyObject *ww = w;

            if (wsign < 0) {
                ww = PyNumber_Negative(w);
                if (ww == NULL)
                    goto Error;
            }
            else
                Py_INCREF(ww);

            fracpart = modf(i, &intpart);
            vv = PyLong_FromDouble(intpart);
            if (vv == NULL)
                goto Error;

            if (fracpart != 0.0) {
                /* Shift left, and or a 1 bit into vv
                 * to represent the lost fraction.
                 */
                PyObject *temp;

                temp = _PyLong_Lshift(ww, 1);
                if (temp == NULL)
                    goto Error;
                Py_DECREF(ww);
                ww = temp;

                temp = _PyLong_Lshift(vv, 1);
                if (temp == NULL)
                    goto Error;
                Py_DECREF(vv);
                vv = temp;

                temp = PyNumber_Or(vv, _PyLong_One);
                if (temp == NULL)
                    goto Error;
                Py_DECREF(vv);
                vv = temp;
            }

            r = PyObject_RichCompareBool(vv, ww, op);
            if (r < 0)
                goto Error;
            result = PyBool_FromLong(r);
         Error:
            Py_XDECREF(vv);
            Py_XDECREF(ww);
            return result;
        }
    } /* else if (PyLong_Check(w)) */

    else        /* w isn't float or int */
        goto Unimplemented;

 Compare:
    switch (op) {
    case Py_EQ:
        r = i == j;
        break;
    case Py_NE:
        r = i != j;
        break;
    case Py_LE:
        r = i <= j;
        break;
    case Py_GE:
        r = i >= j;
        break;
    case Py_LT:
        r = i < j;
        break;
    case Py_GT:
        r = i > j;
        break;
    }
    return PyBool_FromLong(r);

 Unimplemented:
    Py_RETURN_NOTIMPLEMENTED;
}

static Py_hash_t
float_hash(PyFloatObject *v)
{
    return _Py_HashDouble(v->ob_fval);
}

static PyObject *
float_add(PyObject *v, PyObject *w)
{
    double a,b;
    CONVERT_TO_DOUBLE(v, a);
    CONVERT_TO_DOUBLE(w, b);
    a = a + b;
    return PyFloat_FromDouble(a);
}

static PyObject *
float_sub(PyObject *v, PyObject *w)
{
    double a,b;
    CONVERT_TO_DOUBLE(v, a);
    CONVERT_TO_DOUBLE(w, b);
    a = a - b;
    return PyFloat_FromDouble(a);
}

static PyObject *
float_mul(PyObject *v, PyObject *w)
{
    double a,b;
    CONVERT_TO_DOUBLE(v, a);
    CONVERT_TO_DOUBLE(w, b);
    a = a * b;
    return PyFloat_FromDouble(a);
}

static PyObject *
float_div(PyObject *v, PyObject *w)
{
    double a,b;
    CONVERT_TO_DOUBLE(v, a);
    CONVERT_TO_DOUBLE(w, b);
    if (b == 0.0) {
        PyErr_SetString(PyExc_ZeroDivisionError,
                        "float division by zero");
        return NULL;
    }
    a = a / b;
    return PyFloat_FromDouble(a);
}

static PyObject *
float_rem(PyObject *v, PyObject *w)
{
    double vx, wx;
    double mod;
    CONVERT_TO_DOUBLE(v, vx);
    CONVERT_TO_DOUBLE(w, wx);
    if (wx == 0.0) {
        PyErr_SetString(PyExc_ZeroDivisionError,
                        "float modulo");
        return NULL;
    }
    mod = fmod(vx, wx);
    if (mod) {
        /* ensure the remainder has the same sign as the denominator */
        if ((wx < 0) != (mod < 0)) {
            mod += wx;
        }
    }
    else {
        /* the remainder is zero, and in the presence of signed zeroes
           fmod returns different results across platforms; ensure
           it has the same sign as the denominator. */
        mod = copysign(0.0, wx);
    }
    return PyFloat_FromDouble(mod);
}

static void
_float_div_mod(double vx, double wx, double *floordiv, double *mod)
{
    double div;
    *mod = fmod(vx, wx);
    /* fmod is typically exact, so vx-mod is *mathematically* an
       exact multiple of wx.  But this is fp arithmetic, and fp
       vx - mod is an approximation; the result is that div may
       not be an exact integral value after the division, although
       it will always be very close to one.
    */
    div = (vx - *mod) / wx;
    if (*mod) {
        /* ensure the remainder has the same sign as the denominator */
        if ((wx < 0) != (*mod < 0)) {
            *mod += wx;
            div -= 1.0;
        }
    }
    else {
        /* the remainder is zero, and in the presence of signed zeroes
           fmod returns different results across platforms; ensure
           it has the same sign as the denominator. */
        *mod = copysign(0.0, wx);
    }
    /* snap quotient to nearest integral value */
    if (div) {
        *floordiv = floor(div);
        if (div - *floordiv > 0.5) {
            *floordiv += 1.0;
        }
    }
    else {
        /* div is zero - get the same sign as the true quotient */
        *floordiv = copysign(0.0, vx / wx); /* zero w/ sign of vx/wx */
    }
}

static PyObject *
float_divmod(PyObject *v, PyObject *w)
{
    double vx, wx;
    double mod, floordiv;
    CONVERT_TO_DOUBLE(v, vx);
    CONVERT_TO_DOUBLE(w, wx);
    if (wx == 0.0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "float divmod()");
        return NULL;
    }
    _float_div_mod(vx, wx, &floordiv, &mod);
    return Py_BuildValue("(dd)", floordiv, mod);
}

static PyObject *
float_floor_div(PyObject *v, PyObject *w)
{
    double vx, wx;
    double mod, floordiv;
    CONVERT_TO_DOUBLE(v, vx);
    CONVERT_TO_DOUBLE(w, wx);
    if (wx == 0.0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "float floor division by zero");
        return NULL;
    }
    _float_div_mod(vx, wx, &floordiv, &mod);
    return PyFloat_FromDouble(floordiv);
}

/* determine whether x is an odd integer or not;  assumes that
   x is not an infinity or nan. */
#define DOUBLE_IS_ODD_INTEGER(x) (fmod(fabs(x), 2.0) == 1.0)

static PyObject *
float_pow(PyObject *v, PyObject *w, PyObject *z)
{
    double iv, iw, ix;
    int negate_result = 0;

    if ((PyObject *)z != Py_None) {
        PyErr_SetString(PyExc_TypeError, "pow() 3rd argument not "
            "allowed unless all arguments are integers");
        return NULL;
    }

    CONVERT_TO_DOUBLE(v, iv);
    CONVERT_TO_DOUBLE(w, iw);

    /* Sort out special cases here instead of relying on pow() */
    if (iw == 0) {              /* v**0 is 1, even 0**0 */
        return PyFloat_FromDouble(1.0);
    }
    if (Py_IS_NAN(iv)) {        /* nan**w = nan, unless w == 0 */
        return PyFloat_FromDouble(iv);
    }
    if (Py_IS_NAN(iw)) {        /* v**nan = nan, unless v == 1; 1**nan = 1 */
        return PyFloat_FromDouble(iv == 1.0 ? 1.0 : iw);
    }
    if (Py_IS_INFINITY(iw)) {
        /* v**inf is: 0.0 if abs(v) < 1; 1.0 if abs(v) == 1; inf if
         *     abs(v) > 1 (including case where v infinite)
         *
         * v**-inf is: inf if abs(v) < 1; 1.0 if abs(v) == 1; 0.0 if
         *     abs(v) > 1 (including case where v infinite)
         */
        iv = fabs(iv);
        if (iv == 1.0)
            return PyFloat_FromDouble(1.0);
        else if ((iw > 0.0) == (iv > 1.0))
            return PyFloat_FromDouble(fabs(iw)); /* return inf */
        else
            return PyFloat_FromDouble(0.0);
    }
    if (Py_IS_INFINITY(iv)) {
        /* (+-inf)**w is: inf for w positive, 0 for w negative; in
         *     both cases, we need to add the appropriate sign if w is
         *     an odd integer.
         */
        int iw_is_odd = DOUBLE_IS_ODD_INTEGER(iw);
        if (iw > 0.0)
            return PyFloat_FromDouble(iw_is_odd ? iv : fabs(iv));
        else
            return PyFloat_FromDouble(iw_is_odd ?
                                      copysign(0.0, iv) : 0.0);
    }
    if (iv == 0.0) {  /* 0**w is: 0 for w positive, 1 for w zero
                         (already dealt with above), and an error
                         if w is negative. */
        int iw_is_odd = DOUBLE_IS_ODD_INTEGER(iw);
        if (iw < 0.0) {
            PyErr_SetString(PyExc_ZeroDivisionError,
                            "0.0 cannot be raised to a "
                            "negative power");
            return NULL;
        }
        /* use correct sign if iw is odd */
        return PyFloat_FromDouble(iw_is_odd ? iv : 0.0);
    }

    if (iv < 0.0) {
        /* Whether this is an error is a mess, and bumps into libm
         * bugs so we have to figure it out ourselves.
         */
        if (iw != floor(iw)) {
            /* Negative numbers raised to fractional powers
             * become complex.
             */
            return PyComplex_Type.tp_as_number->nb_power(v, w, z);
        }
        /* iw is an exact integer, albeit perhaps a very large
         * one.  Replace iv by its absolute value and remember
         * to negate the pow result if iw is odd.
         */
        iv = -iv;
        negate_result = DOUBLE_IS_ODD_INTEGER(iw);
    }

    if (iv == 1.0) { /* 1**w is 1, even 1**inf and 1**nan */
        /* (-1) ** large_integer also ends up here.  Here's an
         * extract from the comments for the previous
         * implementation explaining why this special case is
         * necessary:
         *
         * -1 raised to an exact integer should never be exceptional.
         * Alas, some libms (chiefly glibc as of early 2003) return
         * NaN and set EDOM on pow(-1, large_int) if the int doesn't
         * happen to be representable in a *C* integer.  That's a
         * bug.
         */
        return PyFloat_FromDouble(negate_result ? -1.0 : 1.0);
    }

    /* Now iv and iw are finite, iw is nonzero, and iv is
     * positive and not equal to 1.0.  We finally allow
     * the platform pow to step in and do the rest.
     */
    errno = 0;
    ix = pow(iv, iw);
    Py_ADJUST_ERANGE1(ix);
    if (negate_result)
        ix = -ix;

    if (errno != 0) {
        /* We don't expect any errno value other than ERANGE, but
         * the range of libm bugs appears unbounded.
         */
        PyErr_SetFromErrno(errno == ERANGE ? PyExc_OverflowError :
                             PyExc_ValueError);
        return NULL;
    }
    return PyFloat_FromDouble(ix);
}

#undef DOUBLE_IS_ODD_INTEGER

static PyObject *
float_neg(PyFloatObject *v)
{
    return PyFloat_FromDouble(-v->ob_fval);
}

static PyObject *
float_abs(PyFloatObject *v)
{
    return PyFloat_FromDouble(fabs(v->ob_fval));
}

static int
float_bool(PyFloatObject *v)
{
    return v->ob_fval != 0.0;
}

/*[clinic input]
float.is_integer

Return True if the float is an integer.
[clinic start generated code]*/

static PyObject *
float_is_integer_impl(PyObject *self)
/*[clinic end generated code: output=7112acf95a4d31ea input=311810d3f777e10d]*/
{
    double x = PyFloat_AsDouble(self);
    PyObject *o;

    if (x == -1.0 && PyErr_Occurred())
        return NULL;
    if (!Py_IS_FINITE(x))
        Py_RETURN_FALSE;
    errno = 0;
    o = (floor(x) == x) ? Py_True : Py_False;
    if (errno != 0) {
        PyErr_SetFromErrno(errno == ERANGE ? PyExc_OverflowError :
                             PyExc_ValueError);
        return NULL;
    }
    Py_INCREF(o);
    return o;
}

/*[clinic input]
float.__trunc__

Return the Integral closest to x between 0 and x.
[clinic start generated code]*/

static PyObject *
float___trunc___impl(PyObject *self)
/*[clinic end generated code: output=dd3e289dd4c6b538 input=591b9ba0d650fdff]*/
{
    return PyLong_FromDouble(PyFloat_AS_DOUBLE(self));
}

/*[clinic input]
float.__floor__

Return the floor as an Integral.
[clinic start generated code]*/

static PyObject *
float___floor___impl(PyObject *self)
/*[clinic end generated code: output=e0551dbaea8c01d1 input=77bb13eb12e268df]*/
{
    double x = PyFloat_AS_DOUBLE(self);
    return PyLong_FromDouble(floor(x));
}

/*[clinic input]
float.__ceil__

Return the ceiling as an Integral.
[clinic start generated code]*/

static PyObject *
float___ceil___impl(PyObject *self)
/*[clinic end generated code: output=a2fd8858f73736f9 input=79e41ae94aa0a516]*/
{
    double x = PyFloat_AS_DOUBLE(self);
    return PyLong_FromDouble(ceil(x));
}

/* double_round: rounds a finite double to the closest multiple of
   10**-ndigits; here ndigits is within reasonable bounds (typically, -308 <=
   ndigits <= 323).  Returns a Python float, or sets a Python error and
   returns NULL on failure (OverflowError and memory errors are possible). */

#ifndef PY_NO_SHORT_FLOAT_REPR
/* version of double_round that uses the correctly-rounded string<->double
   conversions from Python/dtoa.c */

static PyObject *
double_round(double x, int ndigits) {

    double rounded;
    Py_ssize_t buflen, mybuflen=100;
    char *buf, *buf_end, shortbuf[100], *mybuf=shortbuf;
    int decpt, sign;
    PyObject *result = NULL;
    _Py_SET_53BIT_PRECISION_HEADER;

    /* round to a decimal string */
    _Py_SET_53BIT_PRECISION_START;
    buf = _Py_dg_dtoa(x, 3, ndigits, &decpt, &sign, &buf_end);
    _Py_SET_53BIT_PRECISION_END;
    if (buf == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    /* Get new buffer if shortbuf is too small.  Space needed <= buf_end -
    buf + 8: (1 extra for '0', 1 for sign, 5 for exp, 1 for '\0').  */
    buflen = buf_end - buf;
    if (buflen + 8 > mybuflen) {
        mybuflen = buflen+8;
        mybuf = (char *)PyMem_Malloc(mybuflen);
        if (mybuf == NULL) {
            PyErr_NoMemory();
            goto exit;
        }
    }
    /* copy buf to mybuf, adding exponent, sign and leading 0 */
    PyOS_snprintf(mybuf, mybuflen, "%s0%se%d", (sign ? "-" : ""),
                  buf, decpt - (int)buflen);

    /* and convert the resulting string back to a double */
    errno = 0;
    _Py_SET_53BIT_PRECISION_START;
    rounded = _Py_dg_strtod(mybuf, NULL);
    _Py_SET_53BIT_PRECISION_END;
    if (errno == ERANGE && fabs(rounded) >= 1.)
        PyErr_SetString(PyExc_OverflowError,
                        "rounded value too large to represent");
    else
        result = PyFloat_FromDouble(rounded);

    /* done computing value;  now clean up */
    if (mybuf != shortbuf)
        PyMem_Free(mybuf);
  exit:
    _Py_dg_freedtoa(buf);
    return result;
}

#else /* PY_NO_SHORT_FLOAT_REPR */

/* fallback version, to be used when correctly rounded binary<->decimal
   conversions aren't available */

static PyObject *
double_round(double x, int ndigits) {
    double pow1, pow2, y, z;
    if (ndigits >= 0) {
        if (ndigits > 22) {
            /* pow1 and pow2 are each safe from overflow, but
               pow1*pow2 ~= pow(10.0, ndigits) might overflow */
            pow1 = pow(10.0, (double)(ndigits-22));
            pow2 = 1e22;
        }
        else {
            pow1 = pow(10.0, (double)ndigits);
            pow2 = 1.0;
        }
        y = (x*pow1)*pow2;
        /* if y overflows, then rounded value is exactly x */
        if (!Py_IS_FINITE(y))
            return PyFloat_FromDouble(x);
    }
    else {
        pow1 = pow(10.0, (double)-ndigits);
        pow2 = 1.0; /* unused; silences a gcc compiler warning */
        y = x / pow1;
    }

    z = round(y);
    if (fabs(y-z) == 0.5)
        /* halfway between two integers; use round-half-even */
        z = 2.0*round(y/2.0);

    if (ndigits >= 0)
        z = (z / pow2) / pow1;
    else
        z *= pow1;

    /* if computation resulted in overflow, raise OverflowError */
    if (!Py_IS_FINITE(z)) {
        PyErr_SetString(PyExc_OverflowError,
                        "overflow occurred during round");
        return NULL;
    }

    return PyFloat_FromDouble(z);
}

#endif /* PY_NO_SHORT_FLOAT_REPR */

/* round a Python float v to the closest multiple of 10**-ndigits */

/*[clinic input]
float.__round__

    ndigits as o_ndigits: object = None
    /

Return the Integral closest to x, rounding half toward even.

When an argument is passed, work like built-in round(x, ndigits).
[clinic start generated code]*/

static PyObject *
float___round___impl(PyObject *self, PyObject *o_ndigits)
/*[clinic end generated code: output=374c36aaa0f13980 input=fc0fe25924fbc9ed]*/
{
    double x, rounded;
    Py_ssize_t ndigits;

    x = PyFloat_AsDouble(self);
    if (o_ndigits == Py_None) {
        /* single-argument round or with None ndigits:
         * round to nearest integer */
        rounded = round(x);
        if (fabs(x-rounded) == 0.5)
            /* halfway case: round to even */
            rounded = 2.0*round(x/2.0);
        return PyLong_FromDouble(rounded);
    }

    /* interpret second argument as a Py_ssize_t; clips on overflow */
    ndigits = PyNumber_AsSsize_t(o_ndigits, NULL);
    if (ndigits == -1 && PyErr_Occurred())
        return NULL;

    /* nans and infinities round to themselves */
    if (!Py_IS_FINITE(x))
        return PyFloat_FromDouble(x);

    /* Deal with extreme values for ndigits. For ndigits > NDIGITS_MAX, x
       always rounds to itself.  For ndigits < NDIGITS_MIN, x always
       rounds to +-0.0.  Here 0.30103 is an upper bound for log10(2). */
#define NDIGITS_MAX ((int)((DBL_MANT_DIG-DBL_MIN_EXP) * 0.30103))
#define NDIGITS_MIN (-(int)((DBL_MAX_EXP + 1) * 0.30103))
    if (ndigits > NDIGITS_MAX)
        /* return x */
        return PyFloat_FromDouble(x);
    else if (ndigits < NDIGITS_MIN)
        /* return 0.0, but with sign of x */
        return PyFloat_FromDouble(0.0*x);
    else
        /* finite x, and ndigits is not unreasonably large */
        return double_round(x, (int)ndigits);
#undef NDIGITS_MAX
#undef NDIGITS_MIN
}

static PyObject *
float_float(PyObject *v)
{
    if (PyFloat_CheckExact(v))
        Py_INCREF(v);
    else
        v = PyFloat_FromDouble(((PyFloatObject *)v)->ob_fval);
    return v;
}

/*[clinic input]
float.conjugate

Return self, the complex conjugate of any float.
[clinic start generated code]*/

static PyObject *
float_conjugate_impl(PyObject *self)
/*[clinic end generated code: output=8ca292c2479194af input=82ba6f37a9ff91dd]*/
{
    return float_float(self);
}

/* turn ASCII hex characters into integer values and vice versa */

static char
char_from_hex(int x)
{
    assert(0 <= x && x < 16);
    return Py_hexdigits[x];
}

static int
hex_from_char(char c) {
    int x;
    switch(c) {
    case '0':
        x = 0;
        break;
    case '1':
        x = 1;
        break;
    case '2':
        x = 2;
        break;
    case '3':
        x = 3;
        break;
    case '4':
        x = 4;
        break;
    case '5':
        x = 5;
        break;
    case '6':
        x = 6;
        break;
    case '7':
        x = 7;
        break;
    case '8':
        x = 8;
        break;
    case '9':
        x = 9;
        break;
    case 'a':
    case 'A':
        x = 10;
        break;
    case 'b':
    case 'B':
        x = 11;
        break;
    case 'c':
    case 'C':
        x = 12;
        break;
    case 'd':
    case 'D':
        x = 13;
        break;
    case 'e':
    case 'E':
        x = 14;
        break;
    case 'f':
    case 'F':
        x = 15;
        break;
    default:
        x = -1;
        break;
    }
    return x;
}

/* convert a float to a hexadecimal string */

/* TOHEX_NBITS is DBL_MANT_DIG rounded up to the next integer
   of the form 4k+1. */
#define TOHEX_NBITS DBL_MANT_DIG + 3 - (DBL_MANT_DIG+2)%4

/*[clinic input]
float.hex

Return a hexadecimal representation of a floating-point number.

>>> (-0.1).hex()
'-0x1.999999999999ap-4'
>>> 3.14159.hex()
'0x1.921f9f01b866ep+1'
[clinic start generated code]*/

static PyObject *
float_hex_impl(PyObject *self)
/*[clinic end generated code: output=0ebc9836e4d302d4 input=bec1271a33d47e67]*/
{
    double x, m;
    int e, shift, i, si, esign;
    /* Space for 1+(TOHEX_NBITS-1)/4 digits, a decimal point, and the
       trailing NUL byte. */
    char s[(TOHEX_NBITS-1)/4+3];

    CONVERT_TO_DOUBLE(self, x);

    if (Py_IS_NAN(x) || Py_IS_INFINITY(x))
        return float_repr((PyFloatObject *)self);

    if (x == 0.0) {
        if (copysign(1.0, x) == -1.0)
            return PyUnicode_FromString("-0x0.0p+0");
        else
            return PyUnicode_FromString("0x0.0p+0");
    }

    m = frexp(fabs(x), &e);
    shift = 1 - Py_MAX(DBL_MIN_EXP - e, 0);
    m = ldexp(m, shift);
    e -= shift;

    si = 0;
    s[si] = char_from_hex((int)m);
    si++;
    m -= (int)m;
    s[si] = '.';
    si++;
    for (i=0; i < (TOHEX_NBITS-1)/4; i++) {
        m *= 16.0;
        s[si] = char_from_hex((int)m);
        si++;
        m -= (int)m;
    }
    s[si] = '\0';

    if (e < 0) {
        esign = (int)'-';
        e = -e;
    }
    else
        esign = (int)'+';

    if (x < 0.0)
        return PyUnicode_FromFormat("-0x%sp%c%d", s, esign, e);
    else
        return PyUnicode_FromFormat("0x%sp%c%d", s, esign, e);
}

/* Convert a hexadecimal string to a float. */

/*[clinic input]
@classmethod
float.fromhex

    string: object
    /

Create a floating-point number from a hexadecimal string.

>>> float.fromhex('0x1.ffffp10')
2047.984375
>>> float.fromhex('-0x1p-1074')
-5e-324
[clinic start generated code]*/

static PyObject *
float_fromhex(PyTypeObject *type, PyObject *string)
/*[clinic end generated code: output=46c0274d22b78e82 input=0407bebd354bca89]*/
{
    PyObject *result;
    double x;
    long exp, top_exp, lsb, key_digit;
    const char *s, *coeff_start, *s_store, *coeff_end, *exp_start, *s_end;
    int half_eps, digit, round_up, negate=0;
    Py_ssize_t length, ndigits, fdigits, i;

    /*
     * For the sake of simplicity and correctness, we impose an artificial
     * limit on ndigits, the total number of hex digits in the coefficient
     * The limit is chosen to ensure that, writing exp for the exponent,
     *
     *   (1) if exp > LONG_MAX/2 then the value of the hex string is
     *   guaranteed to overflow (provided it's nonzero)
     *
     *   (2) if exp < LONG_MIN/2 then the value of the hex string is
     *   guaranteed to underflow to 0.
     *
     *   (3) if LONG_MIN/2 <= exp <= LONG_MAX/2 then there's no danger of
     *   overflow in the calculation of exp and top_exp below.
     *
     * More specifically, ndigits is assumed to satisfy the following
     * inequalities:
     *
     *   4*ndigits <= DBL_MIN_EXP - DBL_MANT_DIG - LONG_MIN/2
     *   4*ndigits <= LONG_MAX/2 + 1 - DBL_MAX_EXP
     *
     * If either of these inequalities is not satisfied, a ValueError is
     * raised.  Otherwise, write x for the value of the hex string, and
     * assume x is nonzero.  Then
     *
     *   2**(exp-4*ndigits) <= |x| < 2**(exp+4*ndigits).
     *
     * Now if exp > LONG_MAX/2 then:
     *
     *   exp - 4*ndigits >= LONG_MAX/2 + 1 - (LONG_MAX/2 + 1 - DBL_MAX_EXP)
     *                    = DBL_MAX_EXP
     *
     * so |x| >= 2**DBL_MAX_EXP, which is too large to be stored in C
     * double, so overflows.  If exp < LONG_MIN/2, then
     *
     *   exp + 4*ndigits <= LONG_MIN/2 - 1 + (
     *                      DBL_MIN_EXP - DBL_MANT_DIG - LONG_MIN/2)
     *                    = DBL_MIN_EXP - DBL_MANT_DIG - 1
     *
     * and so |x| < 2**(DBL_MIN_EXP-DBL_MANT_DIG-1), hence underflows to 0
     * when converted to a C double.
     *
     * It's easy to show that if LONG_MIN/2 <= exp <= LONG_MAX/2 then both
     * exp+4*ndigits and exp-4*ndigits are within the range of a long.
     */

    s = PyUnicode_AsUTF8AndSize(string, &length);
    if (s == NULL)
        return NULL;
    s_end = s + length;

    /********************
     * Parse the string *
     ********************/

    /* leading whitespace */
    while (Py_ISSPACE(*s))
        s++;

    /* infinities and nans */
    x = _Py_parse_inf_or_nan(s, (char **)&coeff_end);
    if (coeff_end != s) {
        s = coeff_end;
        goto finished;
    }

    /* optional sign */
    if (*s == '-') {
        s++;
        negate = 1;
    }
    else if (*s == '+')
        s++;

    /* [0x] */
    s_store = s;
    if (*s == '0') {
        s++;
        if (*s == 'x' || *s == 'X')
            s++;
        else
            s = s_store;
    }

    /* coefficient: <integer> [. <fraction>] */
    coeff_start = s;
    while (hex_from_char(*s) >= 0)
        s++;
    s_store = s;
    if (*s == '.') {
        s++;
        while (hex_from_char(*s) >= 0)
            s++;
        coeff_end = s-1;
    }
    else
        coeff_end = s;

    /* ndigits = total # of hex digits; fdigits = # after point */
    ndigits = coeff_end - coeff_start;
    fdigits = coeff_end - s_store;
    if (ndigits == 0)
        goto parse_error;
    if (ndigits > Py_MIN(DBL_MIN_EXP - DBL_MANT_DIG - LONG_MIN/2,
                         LONG_MAX/2 + 1 - DBL_MAX_EXP)/4)
        goto insane_length_error;

    /* [p <exponent>] */
    if (*s == 'p' || *s == 'P') {
        s++;
        exp_start = s;
        if (*s == '-' || *s == '+')
            s++;
        if (!('0' <= *s && *s <= '9'))
            goto parse_error;
        s++;
        while ('0' <= *s && *s <= '9')
            s++;
        exp = strtol(exp_start, NULL, 10);
    }
    else
        exp = 0;

/* for 0 <= j < ndigits, HEX_DIGIT(j) gives the jth most significant digit */
#define HEX_DIGIT(j) hex_from_char(*((j) < fdigits ?            \
                     coeff_end-(j) :                                    \
                     coeff_end-1-(j)))

    /*******************************************
     * Compute rounded value of the hex string *
     *******************************************/

    /* Discard leading zeros, and catch extreme overflow and underflow */
    while (ndigits > 0 && HEX_DIGIT(ndigits-1) == 0)
        ndigits--;
    if (ndigits == 0 || exp < LONG_MIN/2) {
        x = 0.0;
        goto finished;
    }
    if (exp > LONG_MAX/2)
        goto overflow_error;

    /* Adjust exponent for fractional part. */
    exp = exp - 4*((long)fdigits);

    /* top_exp = 1 more than exponent of most sig. bit of coefficient */
    top_exp = exp + 4*((long)ndigits - 1);
    for (digit = HEX_DIGIT(ndigits-1); digit != 0; digit /= 2)
        top_exp++;

    /* catch almost all nonextreme cases of overflow and underflow here */
    if (top_exp < DBL_MIN_EXP - DBL_MANT_DIG) {
        x = 0.0;
        goto finished;
    }
    if (top_exp > DBL_MAX_EXP)
        goto overflow_error;

    /* lsb = exponent of least significant bit of the *rounded* value.
       This is top_exp - DBL_MANT_DIG unless result is subnormal. */
    lsb = Py_MAX(top_exp, (long)DBL_MIN_EXP) - DBL_MANT_DIG;

    x = 0.0;
    if (exp >= lsb) {
        /* no rounding required */
        for (i = ndigits-1; i >= 0; i--)
            x = 16.0*x + HEX_DIGIT(i);
        x = ldexp(x, (int)(exp));
        goto finished;
    }
    /* rounding required.  key_digit is the index of the hex digit
       containing the first bit to be rounded away. */
    half_eps = 1 << (int)((lsb - exp - 1) % 4);
    key_digit = (lsb - exp - 1) / 4;
    for (i = ndigits-1; i > key_digit; i--)
        x = 16.0*x + HEX_DIGIT(i);
    digit = HEX_DIGIT(key_digit);
    x = 16.0*x + (double)(digit & (16-2*half_eps));

    /* round-half-even: round up if bit lsb-1 is 1 and at least one of
       bits lsb, lsb-2, lsb-3, lsb-4, ... is 1. */
    if ((digit & half_eps) != 0) {
        round_up = 0;
        if ((digit & (3*half_eps-1)) != 0 || (half_eps == 8 &&
                key_digit+1 < ndigits && (HEX_DIGIT(key_digit+1) & 1) != 0))
            round_up = 1;
        else
            for (i = key_digit-1; i >= 0; i--)
                if (HEX_DIGIT(i) != 0) {
                    round_up = 1;
                    break;
                }
        if (round_up) {
            x += 2*half_eps;
            if (top_exp == DBL_MAX_EXP &&
                x == ldexp((double)(2*half_eps), DBL_MANT_DIG))
                /* overflow corner case: pre-rounded value <
                   2**DBL_MAX_EXP; rounded=2**DBL_MAX_EXP. */
                goto overflow_error;
        }
    }
    x = ldexp(x, (int)(exp+4*key_digit));

  finished:
    /* optional trailing whitespace leading to the end of the string */
    while (Py_ISSPACE(*s))
        s++;
    if (s != s_end)
        goto parse_error;
    result = PyFloat_FromDouble(negate ? -x : x);
    if (type != &PyFloat_Type && result != NULL) {
        Py_SETREF(result, PyObject_CallOneArg((PyObject *)type, result));
    }
    return result;

  overflow_error:
    PyErr_SetString(PyExc_OverflowError,
                    "hexadecimal value too large to represent as a float");
    return NULL;

  parse_error:
    PyErr_SetString(PyExc_ValueError,
                    "invalid hexadecimal floating-point string");
    return NULL;

  insane_length_error:
    PyErr_SetString(PyExc_ValueError,
                    "hexadecimal string too long to convert");
    return NULL;
}

/*[clinic input]
float.as_integer_ratio

Return integer ratio.

Return a pair of integers, whose ratio is exactly equal to the original float
and with a positive denominator.

Raise OverflowError on infinities and a ValueError on NaNs.

>>> (10.0).as_integer_ratio()
(10, 1)
>>> (0.0).as_integer_ratio()
(0, 1)
>>> (-.25).as_integer_ratio()
(-1, 4)
[clinic start generated code]*/

static PyObject *
float_as_integer_ratio_impl(PyObject *self)
/*[clinic end generated code: output=65f25f0d8d30a712 input=e21d08b4630c2e44]*/
{
    double self_double;
    double float_part;
    int exponent;
    int i;

    PyObject *py_exponent = NULL;
    PyObject *numerator = NULL;
    PyObject *denominator = NULL;
    PyObject *result_pair = NULL;
    PyNumberMethods *long_methods = PyLong_Type.tp_as_number;

    CONVERT_TO_DOUBLE(self, self_double);

    if (Py_IS_INFINITY(self_double)) {
        PyErr_SetString(PyExc_OverflowError,
                        "cannot convert Infinity to integer ratio");
        return NULL;
    }
    if (Py_IS_NAN(self_double)) {
        PyErr_SetString(PyExc_ValueError,
                        "cannot convert NaN to integer ratio");
        return NULL;
    }

    float_part = frexp(self_double, &exponent);        /* self_double == float_part * 2**exponent exactly */

    for (i=0; i<300 && float_part != floor(float_part) ; i++) {
        float_part *= 2.0;
        exponent--;
    }
    /* self == float_part * 2**exponent exactly and float_part is integral.
       If FLT_RADIX != 2, the 300 steps may leave a tiny fractional part
       to be truncated by PyLong_FromDouble(). */

    numerator = PyLong_FromDouble(float_part);
    if (numerator == NULL)
        goto error;
    denominator = PyLong_FromLong(1);
    if (denominator == NULL)
        goto error;
    py_exponent = PyLong_FromLong(Py_ABS(exponent));
    if (py_exponent == NULL)
        goto error;

    /* fold in 2**exponent */
    if (exponent > 0) {
        Py_SETREF(numerator,
                  long_methods->nb_lshift(numerator, py_exponent));
        if (numerator == NULL)
            goto error;
    }
    else {
        Py_SETREF(denominator,
                  long_methods->nb_lshift(denominator, py_exponent));
        if (denominator == NULL)
            goto error;
    }

    result_pair = PyTuple_Pack(2, numerator, denominator);

error:
    Py_XDECREF(py_exponent);
    Py_XDECREF(denominator);
    Py_XDECREF(numerator);
    return result_pair;
}

static PyObject *
float_subtype_new(PyTypeObject *type, PyObject *x);

/*[clinic input]
@classmethod
float.__new__ as float_new
    x: object(c_default="_PyLong_Zero") = 0
    /

Convert a string or number to a floating point number, if possible.
[clinic start generated code]*/

static PyObject *
float_new_impl(PyTypeObject *type, PyObject *x)
/*[clinic end generated code: output=ccf1e8dc460ba6ba input=540ee77c204ff87a]*/
{
    if (type != &PyFloat_Type)
        return float_subtype_new(type, x); /* Wimp out */
    /* If it's a string, but not a string subclass, use
       PyFloat_FromString. */
    if (PyUnicode_CheckExact(x))
        return PyFloat_FromString(x);
    return PyNumber_Float(x);
}

/* Wimpy, slow approach to tp_new calls for subtypes of float:
   first create a regular float from whatever arguments we got,
   then allocate a subtype instance and initialize its ob_fval
   from the regular float.  The regular float is then thrown away.
*/
static PyObject *
float_subtype_new(PyTypeObject *type, PyObject *x)
{
    PyObject *tmp, *newobj;

    assert(PyType_IsSubtype(type, &PyFloat_Type));
    tmp = float_new_impl(&PyFloat_Type, x);
    if (tmp == NULL)
        return NULL;
    assert(PyFloat_Check(tmp));
    newobj = type->tp_alloc(type, 0);
    if (newobj == NULL) {
        Py_DECREF(tmp);
        return NULL;
    }
    ((PyFloatObject *)newobj)->ob_fval = ((PyFloatObject *)tmp)->ob_fval;
    Py_DECREF(tmp);
    return newobj;
}

/*[clinic input]
float.__getnewargs__
[clinic start generated code]*/

static PyObject *
float___getnewargs___impl(PyObject *self)
/*[clinic end generated code: output=873258c9d206b088 input=002279d1d77891e6]*/
{
    return Py_BuildValue("(d)", ((PyFloatObject *)self)->ob_fval);
}

/* this is for the benefit of the pack/unpack routines below */

typedef enum {
    unknown_format, ieee_big_endian_format, ieee_little_endian_format
} float_format_type;

static float_format_type double_format, float_format;
static float_format_type detected_double_format, detected_float_format;

/*[clinic input]
@classmethod
float.__getformat__

    typestr: str
        Must be 'double' or 'float'.
    /

You probably don't want to use this function.

It exists mainly to be used in Python's test suite.

This function returns whichever of 'unknown', 'IEEE, big-endian' or 'IEEE,
little-endian' best describes the format of floating point numbers used by the
C type named by typestr.
[clinic start generated code]*/

static PyObject *
float___getformat___impl(PyTypeObject *type, const char *typestr)
/*[clinic end generated code: output=2bfb987228cc9628 input=d5a52600f835ad67]*/
{
    float_format_type r;

    if (strcmp(typestr, "double") == 0) {
        r = double_format;
    }
    else if (strcmp(typestr, "float") == 0) {
        r = float_format;
    }
    else {
        PyErr_SetString(PyExc_ValueError,
                        "__getformat__() argument 1 must be "
                        "'double' or 'float'");
        return NULL;
    }

    switch (r) {
    case unknown_format:
        return PyUnicode_FromString("unknown");
    case ieee_little_endian_format:
        return PyUnicode_FromString("IEEE, little-endian");
    case ieee_big_endian_format:
        return PyUnicode_FromString("IEEE, big-endian");
    default:
        PyErr_SetString(PyExc_RuntimeError,
                        "insane float_format or double_format");
        return NULL;
    }
}

/*[clinic input]
@classmethod
float.__setformat__

    typestr: str
        Must be 'double' or 'float'.
    fmt: str
        Must be one of 'unknown', 'IEEE, big-endian' or 'IEEE, little-endian',
        and in addition can only be one of the latter two if it appears to
        match the underlying C reality.
    /

You probably don't want to use this function.

It exists mainly to be used in Python's test suite.

Override the automatic determination of C-level floating point type.
This affects how floats are converted to and from binary strings.
[clinic start generated code]*/

static PyObject *
float___setformat___impl(PyTypeObject *type, const char *typestr,
                         const char *fmt)
/*[clinic end generated code: output=06864de1fb5f1f04 input=c0e9e04dd87f9988]*/
{
    float_format_type f;
    float_format_type detected;
    float_format_type *p;

    if (strcmp(typestr, "double") == 0) {
        p = &double_format;
        detected = detected_double_format;
    }
    else if (strcmp(typestr, "float") == 0) {
        p = &float_format;
        detected = detected_float_format;
    }
    else {
        PyErr_SetString(PyExc_ValueError,
                        "__setformat__() argument 1 must "
                        "be 'double' or 'float'");
        return NULL;
    }

    if (strcmp(fmt, "unknown") == 0) {
        f = unknown_format;
    }
    else if (strcmp(fmt, "IEEE, little-endian") == 0) {
        f = ieee_little_endian_format;
    }
    else if (strcmp(fmt, "IEEE, big-endian") == 0) {
        f = ieee_big_endian_format;
    }
    else {
        PyErr_SetString(PyExc_ValueError,
                        "__setformat__() argument 2 must be "
                        "'unknown', 'IEEE, little-endian' or "
                        "'IEEE, big-endian'");
        return NULL;

    }

    if (f != unknown_format && f != detected) {
        PyErr_Format(PyExc_ValueError,
                     "can only set %s format to 'unknown' or the "
                     "detected platform value", typestr);
        return NULL;
    }

    *p = f;
    Py_RETURN_NONE;
}

static PyObject *
float_getreal(PyObject *v, void *closure)
{
    return float_float(v);
}

static PyObject *
float_getimag(PyObject *v, void *closure)
{
    return PyFloat_FromDouble(0.0);
}

/*[clinic input]
float.__format__

  format_spec: unicode
  /

Formats the float according to format_spec.
[clinic start generated code]*/

static PyObject *
float___format___impl(PyObject *self, PyObject *format_spec)
/*[clinic end generated code: output=b260e52a47eade56 input=2ece1052211fd0e6]*/
{
    _PyUnicodeWriter writer;
    int ret;

    _PyUnicodeWriter_Init(&writer);
    ret = _PyFloat_FormatAdvancedWriter(
        &writer,
        self,
        format_spec, 0, PyUnicode_GET_LENGTH(format_spec));
    if (ret == -1) {
        _PyUnicodeWriter_Dealloc(&writer);
        return NULL;
    }
    return _PyUnicodeWriter_Finish(&writer);
}

static PyMethodDef float_methods[] = {
    FLOAT_CONJUGATE_METHODDEF
    FLOAT___TRUNC___METHODDEF
    FLOAT___FLOOR___METHODDEF
    FLOAT___CEIL___METHODDEF
    FLOAT___ROUND___METHODDEF
    FLOAT_AS_INTEGER_RATIO_METHODDEF
    FLOAT_FROMHEX_METHODDEF
    FLOAT_HEX_METHODDEF
    FLOAT_IS_INTEGER_METHODDEF
    FLOAT___GETNEWARGS___METHODDEF
    FLOAT___GETFORMAT___METHODDEF
    FLOAT___SETFORMAT___METHODDEF
    FLOAT___FORMAT___METHODDEF
    {NULL,              NULL}           /* sentinel */
};

static PyGetSetDef float_getset[] = {
    {"real",
     float_getreal, (setter)NULL,
     "the real part of a complex number",
     NULL},
    {"imag",
     float_getimag, (setter)NULL,
     "the imaginary part of a complex number",
     NULL},
    {NULL}  /* Sentinel */
};


static PyNumberMethods float_as_number = {
    float_add,          /* nb_add */
    float_sub,          /* nb_subtract */
    float_mul,          /* nb_multiply */
    float_rem,          /* nb_remainder */
    float_divmod,       /* nb_divmod */
    float_pow,          /* nb_power */
    (unaryfunc)float_neg, /* nb_negative */
    float_float,        /* nb_positive */
    (unaryfunc)float_abs, /* nb_absolute */
    (inquiry)float_bool, /* nb_bool */
    0,                  /* nb_invert */
    0,                  /* nb_lshift */
    0,                  /* nb_rshift */
    0,                  /* nb_and */
    0,                  /* nb_xor */
    0,                  /* nb_or */
    float___trunc___impl, /* nb_int */
    0,                  /* nb_reserved */
    float_float,        /* nb_float */
    0,                  /* nb_inplace_add */
    0,                  /* nb_inplace_subtract */
    0,                  /* nb_inplace_multiply */
    0,                  /* nb_inplace_remainder */
    0,                  /* nb_inplace_power */
    0,                  /* nb_inplace_lshift */
    0,                  /* nb_inplace_rshift */
    0,                  /* nb_inplace_and */
    0,                  /* nb_inplace_xor */
    0,                  /* nb_inplace_or */
    float_floor_div,    /* nb_floor_divide */
    float_div,          /* nb_true_divide */
    0,                  /* nb_inplace_floor_divide */
    0,                  /* nb_inplace_true_divide */
};

PyTypeObject PyFloat_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "float",
    sizeof(PyFloatObject),
    0,
    (destructor)float_dealloc,                  /* tp_dealloc */
    0,                                          /* tp_vectorcall_offset */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_as_async */
    (reprfunc)float_repr,                       /* tp_repr */
    &float_as_number,                           /* tp_as_number */
    0,                                          /* tp_as_sequence */
    0,                                          /* tp_as_mapping */
    (hashfunc)float_hash,                       /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    PyObject_GenericGetAttr,                    /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    float_new__doc__,                           /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    float_richcompare,                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    float_methods,                              /* tp_methods */
    0,                                          /* tp_members */
    float_getset,                               /* tp_getset */
    0,                                          /* tp_base */
    0,                                          /* tp_dict */
    0,                                          /* tp_descr_get */
    0,                                          /* tp_descr_set */
    0,                                          /* tp_dictoffset */
    0,                                          /* tp_init */
    0,                                          /* tp_alloc */
    float_new,                                  /* tp_new */
};

int
_PyFloat_Init(void)
{
    /* We attempt to determine if this machine is using IEEE
       floating point formats by peering at the bits of some
       carefully chosen values.  If it looks like we are on an
       IEEE platform, the float packing/unpacking routines can
       just copy bits, if not they resort to arithmetic & shifts
       and masks.  The shifts & masks approach works on all finite
       values, but what happens to infinities, NaNs and signed
       zeroes on packing is an accident, and attempting to unpack
       a NaN or an infinity will raise an exception.

       Note that if we're on some whacked-out platform which uses
       IEEE formats but isn't strictly little-endian or big-
       endian, we will fall back to the portable shifts & masks
       method. */

#if SIZEOF_DOUBLE == 8
    {
        double x = 9006104071832581.0;
        if (memcmp(&x, "\x43\x3f\xff\x01\x02\x03\x04\x05", 8) == 0)
            detected_double_format = ieee_big_endian_format;
        else if (memcmp(&x, "\x05\x04\x03\x02\x01\xff\x3f\x43", 8) == 0)
            detected_double_format = ieee_little_endian_format;
        else
            detected_double_format = unknown_format;
    }
#else
    detected_double_format = unknown_format;
#endif

#if SIZEOF_FLOAT == 4
    {
        float y = 16711938.0;
        if (memcmp(&y, "\x4b\x7f\x01\x02", 4) == 0)
            detected_float_format = ieee_big_endian_format;
        else if (memcmp(&y, "\x02\x01\x7f\x4b", 4) == 0)
            detected_float_format = ieee_little_endian_format;
        else
            detected_float_format = unknown_format;
    }
#else
    detected_float_format = unknown_format;
#endif

    double_format = detected_double_format;
    float_format = detected_float_format;

    /* Init float info */
    if (FloatInfoType.tp_name == NULL) {
        if (PyStructSequence_InitType2(&FloatInfoType, &floatinfo_desc) < 0) {
            return 0;
        }
    }
    return 1;
}

void
_PyFloat_ClearFreeList(void)
{
    PyFloatObject *f = free_list, *next;
    for (; f; f = next) {
        next = (PyFloatObject*) Py_TYPE(f);
        PyObject_FREE(f);
    }
    free_list = NULL;
    numfree = 0;
}

void
_PyFloat_Fini(void)
{
    _PyFloat_ClearFreeList();
}

/* Print summary info about the state of the optimized allocator */
void
_PyFloat_DebugMallocStats(FILE *out)
{
    _PyDebugAllocatorStats(out,
                           "free PyFloatObject",
                           numfree, sizeof(PyFloatObject));
}


/*----------------------------------------------------------------------------
 * _PyFloat_{Pack,Unpack}{2,4,8}.  See floatobject.h.
 * To match the NPY_HALF_ROUND_TIES_TO_EVEN behavior in:
 * https://github.com/numpy/numpy/blob/master/numpy/core/src/npymath/halffloat.c
 * We use:
 *       bits = (unsigned short)f;    Note the truncation
 *       if ((f - bits > 0.5) || (f - bits == 0.5 && bits % 2)) {
 *           bits++;
 *       }
 */

int
_PyFloat_Pack2(double x, unsigned char *p, int le)
{
    unsigned char sign;
    int e;
    double f;
    unsigned short bits;
    int incr = 1;

    if (x == 0.0) {
        sign = (copysign(1.0, x) == -1.0);
        e = 0;
        bits = 0;
    }
    else if (Py_IS_INFINITY(x)) {
        sign = (x < 0.0);
        e = 0x1f;
        bits = 0;
    }
    else if (Py_IS_NAN(x)) {
        /* There are 2046 distinct half-precision NaNs (1022 signaling and
           1024 quiet), but there are only two quiet NaNs that don't arise by
           quieting a signaling NaN; we get those by setting the topmost bit
           of the fraction field and clearing all other fraction bits. We
           choose the one with the appropriate sign. */
        sign = (copysign(1.0, x) == -1.0);
        e = 0x1f;
        bits = 512;
    }
    else {
        sign = (x < 0.0);
        if (sign) {
            x = -x;
        }

        f = frexp(x, &e);
        if (f < 0.5 || f >= 1.0) {
            PyErr_SetString(PyExc_SystemError,
                            "frexp() result out of range");
            return -1;
        }

        /* Normalize f to be in the range [1.0, 2.0) */
        f *= 2.0;
        e--;

        if (e >= 16) {
            goto Overflow;
        }
        else if (e < -25) {
            /* |x| < 2**-25. Underflow to zero. */
            f = 0.0;
            e = 0;
        }
        else if (e < -14) {
            /* |x| < 2**-14. Gradual underflow */
            f = ldexp(f, 14 + e);
            e = 0;
        }
        else /* if (!(e == 0 && f == 0.0)) */ {
            e += 15;
            f -= 1.0; /* Get rid of leading 1 */
        }

        f *= 1024.0; /* 2**10 */
        /* Round to even */
        bits = (unsigned short)f; /* Note the truncation */
        assert(bits < 1024);
        assert(e < 31);
        if ((f - bits > 0.5) || ((f - bits == 0.5) && (bits % 2 == 1))) {
            ++bits;
            if (bits == 1024) {
                /* The carry propagated out of a string of 10 1 bits. */
                bits = 0;
                ++e;
                if (e == 31)
                    goto Overflow;
            }
        }
    }

    bits |= (e << 10) | (sign << 15);

    /* Write out result. */
    if (le) {
        p += 1;
        incr = -1;
    }

    /* First byte */
    *p = (unsigned char)((bits >> 8) & 0xFF);
    p += incr;

    /* Second byte */
    *p = (unsigned char)(bits & 0xFF);

    return 0;

  Overflow:
    PyErr_SetString(PyExc_OverflowError,
                    "float too large to pack with e format");
    return -1;
}

int
_PyFloat_Pack4(double x, unsigned char *p, int le)
{
    if (float_format == unknown_format) {
        unsigned char sign;
        int e;
        double f;
        unsigned int fbits;
        int incr = 1;

        if (le) {
            p += 3;
            incr = -1;
        }

        if (x < 0) {
            sign = 1;
            x = -x;
        }
        else
            sign = 0;

        f = frexp(x, &e);

        /* Normalize f to be in the range [1.0, 2.0) */
        if (0.5 <= f && f < 1.0) {
            f *= 2.0;
            e--;
        }
        else if (f == 0.0)
            e = 0;
        else {
            PyErr_SetString(PyExc_SystemError,
                            "frexp() result out of range");
            return -1;
        }

        if (e >= 128)
            goto Overflow;
        else if (e < -126) {
            /* Gradual underflow */
            f = ldexp(f, 126 + e);
            e = 0;
        }
        else if (!(e == 0 && f == 0.0)) {
            e += 127;
            f -= 1.0; /* Get rid of leading 1 */
        }

        f *= 8388608.0; /* 2**23 */
        fbits = (unsigned int)(f + 0.5); /* Round */
        assert(fbits <= 8388608);
        if (fbits >> 23) {
            /* The carry propagated out of a string of 23 1 bits. */
            fbits = 0;
            ++e;
            if (e >= 255)
                goto Overflow;
        }

        /* First byte */
        *p = (sign << 7) | (e >> 1);
        p += incr;

        /* Second byte */
        *p = (char) (((e & 1) << 7) | (fbits >> 16));
        p += incr;

        /* Third byte */
        *p = (fbits >> 8) & 0xFF;
        p += incr;

        /* Fourth byte */
        *p = fbits & 0xFF;

        /* Done */
        return 0;

    }
    else {
        float y = (float)x;
        int i, incr = 1;

        if (Py_IS_INFINITY(y) && !Py_IS_INFINITY(x))
            goto Overflow;

        unsigned char s[sizeof(float)];
        memcpy(s, &y, sizeof(float));

        if ((float_format == ieee_little_endian_format && !le)
            || (float_format == ieee_big_endian_format && le)) {
            p += 3;
            incr = -1;
        }

        for (i = 0; i < 4; i++) {
            *p = s[i];
            p += incr;
        }
        return 0;
    }
  Overflow:
    PyErr_SetString(PyExc_OverflowError,
                    "float too large to pack with f format");
    return -1;
}

int
_PyFloat_Pack8(double x, unsigned char *p, int le)
{
    if (double_format == unknown_format) {
        unsigned char sign;
        int e;
        double f;
        unsigned int fhi, flo;
        int incr = 1;

        if (le) {
            p += 7;
            incr = -1;
        }

        if (x < 0) {
            sign = 1;
            x = -x;
        }
        else
            sign = 0;

        f = frexp(x, &e);

        /* Normalize f to be in the range [1.0, 2.0) */
        if (0.5 <= f && f < 1.0) {
            f *= 2.0;
            e--;
        }
        else if (f == 0.0)
            e = 0;
        else {
            PyErr_SetString(PyExc_SystemError,
                            "frexp() result out of range");
            return -1;
        }

        if (e >= 1024)
            goto Overflow;
        else if (e < -1022) {
            /* Gradual underflow */
            f = ldexp(f, 1022 + e);
            e = 0;
        }
        else if (!(e == 0 && f == 0.0)) {
            e += 1023;
            f -= 1.0; /* Get rid of leading 1 */
        }

        /* fhi receives the high 28 bits; flo the low 24 bits (== 52 bits) */
        f *= 268435456.0; /* 2**28 */
        fhi = (unsigned int)f; /* Truncate */
        assert(fhi < 268435456);

        f -= (double)fhi;
        f *= 16777216.0; /* 2**24 */
        flo = (unsigned int)(f + 0.5); /* Round */
        assert(flo <= 16777216);
        if (flo >> 24) {
            /* The carry propagated out of a string of 24 1 bits. */
            flo = 0;
            ++fhi;
            if (fhi >> 28) {
                /* And it also propagated out of the next 28 bits. */
                fhi = 0;
                ++e;
                if (e >= 2047)
                    goto Overflow;
            }
        }

        /* First byte */
        *p = (sign << 7) | (e >> 4);
        p += incr;

        /* Second byte */
        *p = (unsigned char) (((e & 0xF) << 4) | (fhi >> 24));
        p += incr;

        /* Third byte */
        *p = (fhi >> 16) & 0xFF;
        p += incr;

        /* Fourth byte */
        *p = (fhi >> 8) & 0xFF;
        p += incr;

        /* Fifth byte */
        *p = fhi & 0xFF;
        p += incr;

        /* Sixth byte */
        *p = (flo >> 16) & 0xFF;
        p += incr;

        /* Seventh byte */
        *p = (flo >> 8) & 0xFF;
        p += incr;

        /* Eighth byte */
        *p = flo & 0xFF;
        /* p += incr; */

        /* Done */
        return 0;

      Overflow:
        PyErr_SetString(PyExc_OverflowError,
                        "float too large to pack with d format");
        return -1;
    }
    else {
        const unsigned char *s = (unsigned char*)&x;
        int i, incr = 1;

        if ((double_format == ieee_little_endian_format && !le)
            || (double_format == ieee_big_endian_format && le)) {
            p += 7;
            incr = -1;
        }

        for (i = 0; i < 8; i++) {
            *p = *s++;
            p += incr;
        }
        return 0;
    }
}

double
_PyFloat_Unpack2(const unsigned char *p, int le)
{
    unsigned char sign;
    int e;
    unsigned int f;
    double x;
    int incr = 1;

    if (le) {
        p += 1;
        incr = -1;
    }

    /* First byte */
    sign = (*p >> 7) & 1;
    e = (*p & 0x7C) >> 2;
    f = (*p & 0x03) << 8;
    p += incr;

    /* Second byte */
    f |= *p;

    if (e == 0x1f) {
#ifdef PY_NO_SHORT_FLOAT_REPR
        if (f == 0) {
            /* Infinity */
            return sign ? -Py_HUGE_VAL : Py_HUGE_VAL;
        }
        else {
            /* NaN */
#ifdef Py_NAN
            return sign ? -Py_NAN : Py_NAN;
#else
            PyErr_SetString(
                PyExc_ValueError,
                "can't unpack IEEE 754 NaN "
                "on platform that does not support NaNs");
            return -1;
#endif  /* #ifdef Py_NAN */
        }
#else
        if (f == 0) {
            /* Infinity */
            return _Py_dg_infinity(sign);
        }
        else {
            /* NaN */
            return _Py_dg_stdnan(sign);
        }
#endif  /* #ifdef PY_NO_SHORT_FLOAT_REPR */
    }

    x = (double)f / 1024.0;

    if (e == 0) {
        e = -14;
    }
    else {
        x += 1.0;
        e -= 15;
    }
    x = ldexp(x, e);

    if (sign)
        x = -x;

    return x;
}

double
_PyFloat_Unpack4(const unsigned char *p, int le)
{
    if (float_format == unknown_format) {
        unsigned char sign;
        int e;
        unsigned int f;
        double x;
        int incr = 1;

        if (le) {
            p += 3;
            incr = -1;
        }

        /* First byte */
        sign = (*p >> 7) & 1;
        e = (*p & 0x7F) << 1;
        p += incr;

        /* Second byte */
        e |= (*p >> 7) & 1;
        f = (*p & 0x7F) << 16;
        p += incr;

        if (e == 255) {
            PyErr_SetString(
                PyExc_ValueError,
                "can't unpack IEEE 754 special value "
                "on non-IEEE platform");
            return -1;
        }

        /* Third byte */
        f |= *p << 8;
        p += incr;

        /* Fourth byte */
        f |= *p;

        x = (double)f / 8388608.0;

        /* XXX This sadly ignores Inf/NaN issues */
        if (e == 0)
            e = -126;
        else {
            x += 1.0;
            e -= 127;
        }
        x = ldexp(x, e);

        if (sign)
            x = -x;

        return x;
    }
    else {
        float x;

        if ((float_format == ieee_little_endian_format && !le)
            || (float_format == ieee_big_endian_format && le)) {
            char buf[4];
            char *d = &buf[3];
            int i;

            for (i = 0; i < 4; i++) {
                *d-- = *p++;
            }
            memcpy(&x, buf, 4);
        }
        else {
            memcpy(&x, p, 4);
        }

        return x;
    }
}

double
_PyFloat_Unpack8(const unsigned char *p, int le)
{
    if (double_format == unknown_format) {
        unsigned char sign;
        int e;
        unsigned int fhi, flo;
        double x;
        int incr = 1;

        if (le) {
            p += 7;
            incr = -1;
        }

        /* First byte */
        sign = (*p >> 7) & 1;
        e = (*p & 0x7F) << 4;

        p += incr;

        /* Second byte */
        e |= (*p >> 4) & 0xF;
        fhi = (*p & 0xF) << 24;
        p += incr;

        if (e == 2047) {
            PyErr_SetString(
                PyExc_ValueError,
                "can't unpack IEEE 754 special value "
                "on non-IEEE platform");
            return -1.0;
        }

        /* Third byte */
        fhi |= *p << 16;
        p += incr;

        /* Fourth byte */
        fhi |= *p  << 8;
        p += incr;

        /* Fifth byte */
        fhi |= *p;
        p += incr;

        /* Sixth byte */
        flo = *p << 16;
        p += incr;

        /* Seventh byte */
        flo |= *p << 8;
        p += incr;

        /* Eighth byte */
        flo |= *p;

        x = (double)fhi + (double)flo / 16777216.0; /* 2**24 */
        x /= 268435456.0; /* 2**28 */

        if (e == 0)
            e = -1022;
        else {
            x += 1.0;
            e -= 1023;
        }
        x = ldexp(x, e);

        if (sign)
            x = -x;

        return x;
    }
    else {
        double x;

        if ((double_format == ieee_little_endian_format && !le)
            || (double_format == ieee_big_endian_format && le)) {
            char buf[8];
            char *d = &buf[7];
            int i;

            for (i = 0; i < 8; i++) {
                *d-- = *p++;
            }
            memcpy(&x, buf, 8);
        }
        else {
            memcpy(&x, p, 8);
        }

        return x;
    }
}
