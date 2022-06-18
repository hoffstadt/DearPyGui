.. highlight:: c

.. _floatobjects:

Floating Point Objects
----------------------

.. index:: object: floating point


.. c:type:: PyFloatObject

   This subtype of :c:type:`PyObject` represents a Python floating point object.


.. c:var:: PyTypeObject PyFloat_Type

   This instance of :c:type:`PyTypeObject` represents the Python floating point
   type.  This is the same object as :class:`float` in the Python layer.


.. c:function:: int PyFloat_Check(PyObject *p)

   Return true if its argument is a :c:type:`PyFloatObject` or a subtype of
   :c:type:`PyFloatObject`.  This function always succeeds.


.. c:function:: int PyFloat_CheckExact(PyObject *p)

   Return true if its argument is a :c:type:`PyFloatObject`, but not a subtype of
   :c:type:`PyFloatObject`.  This function always succeeds.


.. c:function:: PyObject* PyFloat_FromString(PyObject *str)

   Create a :c:type:`PyFloatObject` object based on the string value in *str*, or
   ``NULL`` on failure.


.. c:function:: PyObject* PyFloat_FromDouble(double v)

   Create a :c:type:`PyFloatObject` object from *v*, or ``NULL`` on failure.


.. c:function:: double PyFloat_AsDouble(PyObject *pyfloat)

   Return a C :c:type:`double` representation of the contents of *pyfloat*.  If
   *pyfloat* is not a Python floating point object but has a :meth:`__float__`
   method, this method will first be called to convert *pyfloat* into a float.
   If ``__float__()`` is not defined then it falls back to :meth:`__index__`.
   This method returns ``-1.0`` upon failure, so one should call
   :c:func:`PyErr_Occurred` to check for errors.

   .. versionchanged:: 3.8
      Use :meth:`__index__` if available.


.. c:function:: double PyFloat_AS_DOUBLE(PyObject *pyfloat)

   Return a C :c:type:`double` representation of the contents of *pyfloat*, but
   without error checking.


.. c:function:: PyObject* PyFloat_GetInfo(void)

   Return a structseq instance which contains information about the
   precision, minimum and maximum values of a float. It's a thin wrapper
   around the header file :file:`float.h`.


.. c:function:: double PyFloat_GetMax()

   Return the maximum representable finite float *DBL_MAX* as C :c:type:`double`.


.. c:function:: double PyFloat_GetMin()

   Return the minimum normalized positive float *DBL_MIN* as C :c:type:`double`.
