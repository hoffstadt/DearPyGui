#pragma once
#include <map>
#include <vector>
#include "mvPythonParser.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

namespace Marvel {

    template <typename T>
    struct mvModule
    {

        static PyMethodDef* GetMethods()
        {
            return T::GetSubMethods();
        }

        static const std::vector<std::pair<std::string, long>>& GetModuleConstants()
        {
            return T::GetSubModuleConstants();
        }

        static const std::map<std::string, mvPythonParser>& GetModuleParsers()
        {
            return T::GetSubModuleParsers();
        }
    };

}