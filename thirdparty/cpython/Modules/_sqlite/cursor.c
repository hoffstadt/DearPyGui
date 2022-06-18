/* cursor.c - the cursor type
 *
 * Copyright (C) 2004-2010 Gerhard Häring <gh@ghaering.de>
 *
 * This file is part of pysqlite.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "cursor.h"
#include "module.h"
#include "util.h"

PyObject* pysqlite_cursor_iternext(pysqlite_Cursor* self);

static const char errmsg_fetch_across_rollback[] = "Cursor needed to be reset because of commit/rollback and can no longer be fetched from.";

static int pysqlite_cursor_init(pysqlite_Cursor* self, PyObject* args, PyObject* kwargs)
{
    pysqlite_Connection* connection;

    if (!PyArg_ParseTuple(args, "O!", &pysqlite_ConnectionType, &connection))
    {
        return -1;
    }

    Py_INCREF(connection);
    Py_XSETREF(self->connection, connection);
    Py_CLEAR(self->statement);
    Py_CLEAR(self->next_row);
    Py_CLEAR(self->row_cast_map);

    Py_INCREF(Py_None);
    Py_XSETREF(self->description, Py_None);

    Py_INCREF(Py_None);
    Py_XSETREF(self->lastrowid, Py_None);

    self->arraysize = 1;
    self->closed = 0;
    self->reset = 0;

    self->rowcount = -1L;

    Py_INCREF(Py_None);
    Py_XSETREF(self->row_factory, Py_None);

    if (!pysqlite_check_thread(self->connection)) {
        return -1;
    }

    if (!pysqlite_connection_register_cursor(connection, (PyObject*)self)) {
        return -1;
    }

    self->initialized = 1;

    return 0;
}

static void pysqlite_cursor_dealloc(pysqlite_Cursor* self)
{
    /* Reset the statement if the user has not closed the cursor */
    if (self->statement) {
        pysqlite_statement_reset(self->statement);
        Py_DECREF(self->statement);
    }

    Py_XDECREF(self->connection);
    Py_XDECREF(self->row_cast_map);
    Py_XDECREF(self->description);
    Py_XDECREF(self->lastrowid);
    Py_XDECREF(self->row_factory);
    Py_XDECREF(self->next_row);

    if (self->in_weakreflist != NULL) {
        PyObject_ClearWeakRefs((PyObject*)self);
    }

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *
_pysqlite_get_converter(const char *keystr, Py_ssize_t keylen)
{
    PyObject *key;
    PyObject *upcase_key;
    PyObject *retval;
    _Py_IDENTIFIER(upper);

    key = PyUnicode_FromStringAndSize(keystr, keylen);
    if (!key) {
        return NULL;
    }
    upcase_key = _PyObject_CallMethodIdNoArgs(key, &PyId_upper);
    Py_DECREF(key);
    if (!upcase_key) {
        return NULL;
    }

    retval = PyDict_GetItemWithError(_pysqlite_converters, upcase_key);
    Py_DECREF(upcase_key);

    return retval;
}

static int
pysqlite_build_row_cast_map(pysqlite_Cursor* self)
{
    int i;
    const char* pos;
    const char* colname;
    const char* decltype;
    PyObject* converter;

    if (!self->connection->detect_types) {
        return 0;
    }

    Py_XSETREF(self->row_cast_map, PyList_New(0));
    if (!self->row_cast_map) {
        return -1;
    }

    for (i = 0; i < sqlite3_column_count(self->statement->st); i++) {
        converter = NULL;

        if (self->connection->detect_types & PARSE_COLNAMES) {
            colname = sqlite3_column_name(self->statement->st, i);
            if (colname) {
                const char *type_start = NULL;
                for (pos = colname; *pos != 0; pos++) {
                    if (*pos == '[') {
                        type_start = pos + 1;
                    }
                    else if (*pos == ']' && type_start != NULL) {
                        converter = _pysqlite_get_converter(type_start, pos - type_start);
                        if (!converter && PyErr_Occurred()) {
                            Py_CLEAR(self->row_cast_map);
                            return -1;
                        }
                        break;
                    }
                }
            }
        }

        if (!converter && self->connection->detect_types & PARSE_DECLTYPES) {
            decltype = sqlite3_column_decltype(self->statement->st, i);
            if (decltype) {
                for (pos = decltype;;pos++) {
                    /* Converter names are split at '(' and blanks.
                     * This allows 'INTEGER NOT NULL' to be treated as 'INTEGER' and
                     * 'NUMBER(10)' to be treated as 'NUMBER', for example.
                     * In other words, it will work as people expect it to work.*/
                    if (*pos == ' ' || *pos == '(' || *pos == 0) {
                        converter = _pysqlite_get_converter(decltype, pos - decltype);
                        if (!converter && PyErr_Occurred()) {
                            Py_CLEAR(self->row_cast_map);
                            return -1;
                        }
                        break;
                    }
                }
            }
        }

        if (!converter) {
            converter = Py_None;
        }

        if (PyList_Append(self->row_cast_map, converter) != 0) {
            Py_CLEAR(self->row_cast_map);
            return -1;
        }
    }

    return 0;
}

static PyObject *
_pysqlite_build_column_name(pysqlite_Cursor *self, const char *colname)
{
    const char* pos;
    Py_ssize_t len;

    if (!colname) {
        Py_RETURN_NONE;
    }

    if (self->connection->detect_types & PARSE_COLNAMES) {
        for (pos = colname; *pos; pos++) {
            if (*pos == '[') {
                if ((pos != colname) && (*(pos-1) == ' ')) {
                    pos--;
                }
                break;
            }
        }
        len = pos - colname;
    }
    else {
        len = strlen(colname);
    }
    return PyUnicode_FromStringAndSize(colname, len);
}

/*
 * Returns a row from the currently active SQLite statement
 *
 * Precondidition:
 * - sqlite3_step() has been called before and it returned SQLITE_ROW.
 */
static PyObject *
_pysqlite_fetch_one_row(pysqlite_Cursor* self)
{
    int i, numcols;
    PyObject* row;
    PyObject* item = NULL;
    int coltype;
    PyObject* converter;
    PyObject* converted;
    Py_ssize_t nbytes;
    const char* val_str;
    char buf[200];
    const char* colname;
    PyObject* error_msg;

    if (self->reset) {
        PyErr_SetString(pysqlite_InterfaceError, errmsg_fetch_across_rollback);
        return NULL;
    }

    Py_BEGIN_ALLOW_THREADS
    numcols = sqlite3_data_count(self->statement->st);
    Py_END_ALLOW_THREADS

    row = PyTuple_New(numcols);
    if (!row)
        return NULL;

    for (i = 0; i < numcols; i++) {
        if (self->connection->detect_types
                && self->row_cast_map != NULL
                && i < PyList_GET_SIZE(self->row_cast_map))
        {
            converter = PyList_GET_ITEM(self->row_cast_map, i);
        }
        else {
            converter = Py_None;
        }

        if (converter != Py_None) {
            nbytes = sqlite3_column_bytes(self->statement->st, i);
            val_str = (const char*)sqlite3_column_blob(self->statement->st, i);
            if (!val_str) {
                Py_INCREF(Py_None);
                converted = Py_None;
            } else {
                item = PyBytes_FromStringAndSize(val_str, nbytes);
                if (!item)
                    goto error;
                converted = PyObject_CallOneArg(converter, item);
                Py_DECREF(item);
            }
        } else {
            Py_BEGIN_ALLOW_THREADS
            coltype = sqlite3_column_type(self->statement->st, i);
            Py_END_ALLOW_THREADS
            if (coltype == SQLITE_NULL) {
                Py_INCREF(Py_None);
                converted = Py_None;
            } else if (coltype == SQLITE_INTEGER) {
                converted = PyLong_FromLongLong(sqlite3_column_int64(self->statement->st, i));
            } else if (coltype == SQLITE_FLOAT) {
                converted = PyFloat_FromDouble(sqlite3_column_double(self->statement->st, i));
            } else if (coltype == SQLITE_TEXT) {
                val_str = (const char*)sqlite3_column_text(self->statement->st, i);
                nbytes = sqlite3_column_bytes(self->statement->st, i);
                if (self->connection->text_factory == (PyObject*)&PyUnicode_Type) {
                    converted = PyUnicode_FromStringAndSize(val_str, nbytes);
                    if (!converted && PyErr_ExceptionMatches(PyExc_UnicodeDecodeError)) {
                        PyErr_Clear();
                        colname = sqlite3_column_name(self->statement->st, i);
                        if (!colname) {
                            colname = "<unknown column name>";
                        }
                        PyOS_snprintf(buf, sizeof(buf) - 1, "Could not decode to UTF-8 column '%s' with text '%s'",
                                     colname , val_str);
                        error_msg = PyUnicode_Decode(buf, strlen(buf), "ascii", "replace");
                        if (!error_msg) {
                            PyErr_SetString(pysqlite_OperationalError, "Could not decode to UTF-8");
                        } else {
                            PyErr_SetObject(pysqlite_OperationalError, error_msg);
                            Py_DECREF(error_msg);
                        }
                    }
                } else if (self->connection->text_factory == (PyObject*)&PyBytes_Type) {
                    converted = PyBytes_FromStringAndSize(val_str, nbytes);
                } else if (self->connection->text_factory == (PyObject*)&PyByteArray_Type) {
                    converted = PyByteArray_FromStringAndSize(val_str, nbytes);
                } else {
                    converted = PyObject_CallFunction(self->connection->text_factory, "y#", val_str, nbytes);
                }
            } else {
                /* coltype == SQLITE_BLOB */
                nbytes = sqlite3_column_bytes(self->statement->st, i);
                converted = PyBytes_FromStringAndSize(
                    sqlite3_column_blob(self->statement->st, i), nbytes);
            }
        }

        if (!converted) {
            goto error;
        }
        PyTuple_SetItem(row, i, converted);
    }

    if (PyErr_Occurred())
        goto error;

    return row;

error:
    Py_DECREF(row);
    return NULL;
}

/*
 * Checks if a cursor object is usable.
 *
 * 0 => error; 1 => ok
 */
static int check_cursor(pysqlite_Cursor* cur)
{
    if (!cur->initialized) {
        PyErr_SetString(pysqlite_ProgrammingError, "Base Cursor.__init__ not called.");
        return 0;
    }

    if (cur->closed) {
        PyErr_SetString(pysqlite_ProgrammingError, "Cannot operate on a closed cursor.");
        return 0;
    }

    if (cur->locked) {
        PyErr_SetString(pysqlite_ProgrammingError, "Recursive use of cursors not allowed.");
        return 0;
    }

    return pysqlite_check_thread(cur->connection) && pysqlite_check_connection(cur->connection);
}

static PyObject *
_pysqlite_query_execute(pysqlite_Cursor* self, int multiple, PyObject* args)
{
    PyObject* operation;
    PyObject* parameters_list = NULL;
    PyObject* parameters_iter = NULL;
    PyObject* parameters = NULL;
    int i;
    int rc;
    PyObject* func_args;
    PyObject* result;
    int numcols;
    PyObject* descriptor;
    PyObject* column_name;
    PyObject* second_argument = NULL;
    sqlite_int64 lastrowid;

    if (!check_cursor(self)) {
        goto error;
    }

    self->locked = 1;
    self->reset = 0;

    Py_CLEAR(self->next_row);

    if (multiple) {
        /* executemany() */
        if (!PyArg_ParseTuple(args, "UO", &operation, &second_argument)) {
            goto error;
        }

        if (PyIter_Check(second_argument)) {
            /* iterator */
            Py_INCREF(second_argument);
            parameters_iter = second_argument;
        } else {
            /* sequence */
            parameters_iter = PyObject_GetIter(second_argument);
            if (!parameters_iter) {
                goto error;
            }
        }
    } else {
        /* execute() */
        if (!PyArg_ParseTuple(args, "U|O", &operation, &second_argument)) {
            goto error;
        }

        parameters_list = PyList_New(0);
        if (!parameters_list) {
            goto error;
        }

        if (second_argument == NULL) {
            second_argument = PyTuple_New(0);
            if (!second_argument) {
                goto error;
            }
        } else {
            Py_INCREF(second_argument);
        }
        if (PyList_Append(parameters_list, second_argument) != 0) {
            Py_DECREF(second_argument);
            goto error;
        }
        Py_DECREF(second_argument);

        parameters_iter = PyObject_GetIter(parameters_list);
        if (!parameters_iter) {
            goto error;
        }
    }

    if (self->statement != NULL) {
        /* There is an active statement */
        pysqlite_statement_reset(self->statement);
    }

    /* reset description and rowcount */
    Py_INCREF(Py_None);
    Py_SETREF(self->description, Py_None);
    self->rowcount = 0L;

    func_args = PyTuple_New(1);
    if (!func_args) {
        goto error;
    }
    Py_INCREF(operation);
    if (PyTuple_SetItem(func_args, 0, operation) != 0) {
        goto error;
    }

    if (self->statement) {
        (void)pysqlite_statement_reset(self->statement);
    }

    Py_XSETREF(self->statement,
              (pysqlite_Statement *)pysqlite_cache_get(self->connection->statement_cache, func_args));
    Py_DECREF(func_args);

    if (!self->statement) {
        goto error;
    }

    if (self->statement->in_use) {
        Py_SETREF(self->statement,
                  PyObject_New(pysqlite_Statement, &pysqlite_StatementType));
        if (!self->statement) {
            goto error;
        }
        rc = pysqlite_statement_create(self->statement, self->connection, operation);
        if (rc != SQLITE_OK) {
            Py_CLEAR(self->statement);
            goto error;
        }
    }

    pysqlite_statement_reset(self->statement);
    pysqlite_statement_mark_dirty(self->statement);

    /* We start a transaction implicitly before a DML statement.
       SELECT is the only exception. See #9924. */
    if (self->connection->begin_statement && self->statement->is_dml) {
        if (sqlite3_get_autocommit(self->connection->db)) {
            result = _pysqlite_connection_begin(self->connection);
            if (!result) {
                goto error;
            }
            Py_DECREF(result);
        }
    }

    while (1) {
        parameters = PyIter_Next(parameters_iter);
        if (!parameters) {
            break;
        }

        pysqlite_statement_mark_dirty(self->statement);

        pysqlite_statement_bind_parameters(self->statement, parameters);
        if (PyErr_Occurred()) {
            goto error;
        }

        rc = pysqlite_step(self->statement->st, self->connection);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            if (PyErr_Occurred()) {
                /* there was an error that occurred in a user-defined callback */
                if (_pysqlite_enable_callback_tracebacks) {
                    PyErr_Print();
                } else {
                    PyErr_Clear();
                }
            }
            (void)pysqlite_statement_reset(self->statement);
            _pysqlite_seterror(self->connection->db, NULL);
            goto error;
        }

        if (pysqlite_build_row_cast_map(self) != 0) {
            _PyErr_FormatFromCause(pysqlite_OperationalError, "Error while building row_cast_map");
            goto error;
        }

        assert(rc == SQLITE_ROW || rc == SQLITE_DONE);
        Py_BEGIN_ALLOW_THREADS
        numcols = sqlite3_column_count(self->statement->st);
        Py_END_ALLOW_THREADS
        if (self->description == Py_None && numcols > 0) {
            Py_SETREF(self->description, PyTuple_New(numcols));
            if (!self->description) {
                goto error;
            }
            for (i = 0; i < numcols; i++) {
                descriptor = PyTuple_New(7);
                if (!descriptor) {
                    goto error;
                }
                column_name = _pysqlite_build_column_name(self,
                                sqlite3_column_name(self->statement->st, i));
                if (!column_name) {
                    Py_DECREF(descriptor);
                    goto error;
                }
                PyTuple_SetItem(descriptor, 0, column_name);
                Py_INCREF(Py_None); PyTuple_SetItem(descriptor, 1, Py_None);
                Py_INCREF(Py_None); PyTuple_SetItem(descriptor, 2, Py_None);
                Py_INCREF(Py_None); PyTuple_SetItem(descriptor, 3, Py_None);
                Py_INCREF(Py_None); PyTuple_SetItem(descriptor, 4, Py_None);
                Py_INCREF(Py_None); PyTuple_SetItem(descriptor, 5, Py_None);
                Py_INCREF(Py_None); PyTuple_SetItem(descriptor, 6, Py_None);
                PyTuple_SetItem(self->description, i, descriptor);
            }
        }

        if (self->statement->is_dml) {
            self->rowcount += (long)sqlite3_changes(self->connection->db);
        } else {
            self->rowcount= -1L;
        }

        if (!multiple) {
            Py_BEGIN_ALLOW_THREADS
            lastrowid = sqlite3_last_insert_rowid(self->connection->db);
            Py_END_ALLOW_THREADS
            Py_SETREF(self->lastrowid, PyLong_FromLongLong(lastrowid));
            if (self->lastrowid == NULL) {
                goto error;
            }
        }

        if (rc == SQLITE_ROW) {
            if (multiple) {
                PyErr_SetString(pysqlite_ProgrammingError, "executemany() can only execute DML statements.");
                goto error;
            }

            self->next_row = _pysqlite_fetch_one_row(self);
            if (self->next_row == NULL)
                goto error;
        } else if (rc == SQLITE_DONE && !multiple) {
            pysqlite_statement_reset(self->statement);
            Py_CLEAR(self->statement);
        }

        if (multiple) {
            pysqlite_statement_reset(self->statement);
        }
        Py_XDECREF(parameters);
    }

error:
    Py_XDECREF(parameters);
    Py_XDECREF(parameters_iter);
    Py_XDECREF(parameters_list);

    self->locked = 0;

    if (PyErr_Occurred()) {
        self->rowcount = -1L;
        return NULL;
    } else {
        Py_INCREF(self);
        return (PyObject*)self;
    }
}

PyObject* pysqlite_cursor_execute(pysqlite_Cursor* self, PyObject* args)
{
    return _pysqlite_query_execute(self, 0, args);
}

PyObject* pysqlite_cursor_executemany(pysqlite_Cursor* self, PyObject* args)
{
    return _pysqlite_query_execute(self, 1, args);
}

static PyObject *
pysqlite_cursor_executescript(pysqlite_Cursor* self, PyObject* args)
{
    PyObject* script_obj;
    const char* script_cstr;
    sqlite3_stmt* statement;
    int rc;
    PyObject* result;

    if (!PyArg_ParseTuple(args, "O", &script_obj)) {
        return NULL;
    }

    if (!check_cursor(self)) {
        return NULL;
    }

    self->reset = 0;

    if (PyUnicode_Check(script_obj)) {
        script_cstr = PyUnicode_AsUTF8(script_obj);
        if (!script_cstr) {
            return NULL;
        }
    } else {
        PyErr_SetString(PyExc_ValueError, "script argument must be unicode.");
        return NULL;
    }

    /* commit first */
    result = pysqlite_connection_commit(self->connection, NULL);
    if (!result) {
        goto error;
    }
    Py_DECREF(result);

    while (1) {
        Py_BEGIN_ALLOW_THREADS
        rc = sqlite3_prepare_v2(self->connection->db,
                                script_cstr,
                                -1,
                                &statement,
                                &script_cstr);
        Py_END_ALLOW_THREADS
        if (rc != SQLITE_OK) {
            _pysqlite_seterror(self->connection->db, NULL);
            goto error;
        }

        /* execute statement, and ignore results of SELECT statements */
        rc = SQLITE_ROW;
        while (rc == SQLITE_ROW) {
            rc = pysqlite_step(statement, self->connection);
            if (PyErr_Occurred()) {
                (void)sqlite3_finalize(statement);
                goto error;
            }
        }

        if (rc != SQLITE_DONE) {
            (void)sqlite3_finalize(statement);
            _pysqlite_seterror(self->connection->db, NULL);
            goto error;
        }

        rc = sqlite3_finalize(statement);
        if (rc != SQLITE_OK) {
            _pysqlite_seterror(self->connection->db, NULL);
            goto error;
        }

        if (*script_cstr == (char)0) {
            break;
        }
    }

error:
    if (PyErr_Occurred()) {
        return NULL;
    } else {
        Py_INCREF(self);
        return (PyObject*)self;
    }
}

PyObject* pysqlite_cursor_iternext(pysqlite_Cursor *self)
{
    PyObject* next_row_tuple;
    PyObject* next_row;
    int rc;

    if (!check_cursor(self)) {
        return NULL;
    }

    if (self->reset) {
        PyErr_SetString(pysqlite_InterfaceError, errmsg_fetch_across_rollback);
        return NULL;
    }

    if (!self->next_row) {
         if (self->statement) {
            (void)pysqlite_statement_reset(self->statement);
            Py_CLEAR(self->statement);
        }
        return NULL;
    }

    next_row_tuple = self->next_row;
    assert(next_row_tuple != NULL);
    self->next_row = NULL;

    if (self->row_factory != Py_None) {
        next_row = PyObject_CallFunction(self->row_factory, "OO", self, next_row_tuple);
        if (next_row == NULL) {
            self->next_row = next_row_tuple;
            return NULL;
        }
        Py_DECREF(next_row_tuple);
    } else {
        next_row = next_row_tuple;
    }

    if (self->statement) {
        rc = pysqlite_step(self->statement->st, self->connection);
        if (PyErr_Occurred()) {
            (void)pysqlite_statement_reset(self->statement);
            Py_DECREF(next_row);
            return NULL;
        }
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            (void)pysqlite_statement_reset(self->statement);
            Py_DECREF(next_row);
            _pysqlite_seterror(self->connection->db, NULL);
            return NULL;
        }

        if (rc == SQLITE_ROW) {
            self->next_row = _pysqlite_fetch_one_row(self);
            if (self->next_row == NULL) {
                (void)pysqlite_statement_reset(self->statement);
                return NULL;
            }
        }
    }

    return next_row;
}

PyObject* pysqlite_cursor_fetchone(pysqlite_Cursor* self, PyObject* args)
{
    PyObject* row;

    row = pysqlite_cursor_iternext(self);
    if (!row && !PyErr_Occurred()) {
        Py_RETURN_NONE;
    }

    return row;
}

PyObject* pysqlite_cursor_fetchmany(pysqlite_Cursor* self, PyObject* args, PyObject* kwargs)
{
    static char *kwlist[] = {"size", NULL};

    PyObject* row;
    PyObject* list;
    int maxrows = self->arraysize;
    int counter = 0;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|i:fetchmany", kwlist, &maxrows)) {
        return NULL;
    }

    list = PyList_New(0);
    if (!list) {
        return NULL;
    }

    while ((row = pysqlite_cursor_iternext(self))) {
        if (PyList_Append(list, row) < 0) {
            Py_DECREF(row);
            break;
        }
        Py_DECREF(row);

        if (++counter == maxrows) {
            break;
        }
    }

    if (PyErr_Occurred()) {
        Py_DECREF(list);
        return NULL;
    } else {
        return list;
    }
}

PyObject* pysqlite_cursor_fetchall(pysqlite_Cursor* self, PyObject* args)
{
    PyObject* row;
    PyObject* list;

    list = PyList_New(0);
    if (!list) {
        return NULL;
    }

    while ((row = pysqlite_cursor_iternext(self))) {
        if (PyList_Append(list, row) < 0) {
            Py_DECREF(row);
            break;
        }
        Py_DECREF(row);
    }

    if (PyErr_Occurred()) {
        Py_DECREF(list);
        return NULL;
    } else {
        return list;
    }
}

PyObject* pysqlite_noop(pysqlite_Connection* self, PyObject* args)
{
    /* don't care, return None */
    Py_RETURN_NONE;
}

PyObject* pysqlite_cursor_close(pysqlite_Cursor* self, PyObject* args)
{
    if (!self->connection) {
        PyErr_SetString(pysqlite_ProgrammingError,
                        "Base Cursor.__init__ not called.");
        return NULL;
    }
    if (!pysqlite_check_thread(self->connection) || !pysqlite_check_connection(self->connection)) {
        return NULL;
    }

    if (self->statement) {
        (void)pysqlite_statement_reset(self->statement);
        Py_CLEAR(self->statement);
    }

    self->closed = 1;

    Py_RETURN_NONE;
}

static PyMethodDef cursor_methods[] = {
    {"execute", (PyCFunction)pysqlite_cursor_execute, METH_VARARGS,
        PyDoc_STR("Executes an SQL statement.")},
    {"executemany", (PyCFunction)pysqlite_cursor_executemany, METH_VARARGS,
        PyDoc_STR("Repeatedly executes an SQL statement.")},
    {"executescript", (PyCFunction)pysqlite_cursor_executescript, METH_VARARGS,
        PyDoc_STR("Executes multiple SQL statements at once.")},
    {"fetchone", (PyCFunction)pysqlite_cursor_fetchone, METH_NOARGS,
        PyDoc_STR("Fetches one row from the resultset.")},
    {"fetchmany", (PyCFunction)(void(*)(void))pysqlite_cursor_fetchmany, METH_VARARGS|METH_KEYWORDS,
        PyDoc_STR("Fetches several rows from the resultset.")},
    {"fetchall", (PyCFunction)pysqlite_cursor_fetchall, METH_NOARGS,
        PyDoc_STR("Fetches all rows from the resultset.")},
    {"close", (PyCFunction)pysqlite_cursor_close, METH_NOARGS,
        PyDoc_STR("Closes the cursor.")},
    {"setinputsizes", (PyCFunction)pysqlite_noop, METH_VARARGS,
        PyDoc_STR("Required by DB-API. Does nothing in sqlite3.")},
    {"setoutputsize", (PyCFunction)pysqlite_noop, METH_VARARGS,
        PyDoc_STR("Required by DB-API. Does nothing in sqlite3.")},
    {NULL, NULL}
};

static struct PyMemberDef cursor_members[] =
{
    {"connection", T_OBJECT, offsetof(pysqlite_Cursor, connection), READONLY},
    {"description", T_OBJECT, offsetof(pysqlite_Cursor, description), READONLY},
    {"arraysize", T_INT, offsetof(pysqlite_Cursor, arraysize), 0},
    {"lastrowid", T_OBJECT, offsetof(pysqlite_Cursor, lastrowid), READONLY},
    {"rowcount", T_LONG, offsetof(pysqlite_Cursor, rowcount), READONLY},
    {"row_factory", T_OBJECT, offsetof(pysqlite_Cursor, row_factory), 0},
    {NULL}
};

static const char cursor_doc[] =
PyDoc_STR("SQLite database cursor class.");

PyTypeObject pysqlite_CursorType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        MODULE_NAME ".Cursor",                          /* tp_name */
        sizeof(pysqlite_Cursor),                        /* tp_basicsize */
        0,                                              /* tp_itemsize */
        (destructor)pysqlite_cursor_dealloc,            /* tp_dealloc */
        0,                                              /* tp_vectorcall_offset */
        0,                                              /* tp_getattr */
        0,                                              /* tp_setattr */
        0,                                              /* tp_as_async */
        0,                                              /* tp_repr */
        0,                                              /* tp_as_number */
        0,                                              /* tp_as_sequence */
        0,                                              /* tp_as_mapping */
        0,                                              /* tp_hash */
        0,                                              /* tp_call */
        0,                                              /* tp_str */
        0,                                              /* tp_getattro */
        0,                                              /* tp_setattro */
        0,                                              /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE, /* tp_flags */
        cursor_doc,                                     /* tp_doc */
        0,                                              /* tp_traverse */
        0,                                              /* tp_clear */
        0,                                              /* tp_richcompare */
        offsetof(pysqlite_Cursor, in_weakreflist),      /* tp_weaklistoffset */
        PyObject_SelfIter,                              /* tp_iter */
        (iternextfunc)pysqlite_cursor_iternext,         /* tp_iternext */
        cursor_methods,                                 /* tp_methods */
        cursor_members,                                 /* tp_members */
        0,                                              /* tp_getset */
        0,                                              /* tp_base */
        0,                                              /* tp_dict */
        0,                                              /* tp_descr_get */
        0,                                              /* tp_descr_set */
        0,                                              /* tp_dictoffset */
        (initproc)pysqlite_cursor_init,                 /* tp_init */
        0,                                              /* tp_alloc */
        0,                                              /* tp_new */
        0                                               /* tp_free */
};

extern int pysqlite_cursor_setup_types(void)
{
    pysqlite_CursorType.tp_new = PyType_GenericNew;
    return PyType_Ready(&pysqlite_CursorType);
}
