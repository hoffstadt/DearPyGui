#include "mvClickedHandler.h"
#include "mvLog.h"
#include "mvItemRegistry.h"
#include "mvPythonExceptions.h"
#include "mvUtilities.h"

namespace Marvel {

    void mvClickedHandler::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_SHOW |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_CALLBACK)
        );

        args.push_back({ mvPyDataType::Integer, "button", mvArgType::POSITIONAL_ARG, "-1", "Submits callback for all mouse buttons" });

        mvPythonParserSetup setup;
        setup.about = "Adds a clicked handler.";
        setup.category = { "Widgets", "Events" };
        setup.returnType = mvPyDataType::UUID;

        mvPythonParser parser = FinalizeParser(setup, args);

        parsers->insert({ s_command, parser });
    }

    mvClickedHandler::mvClickedHandler(mvUUID uuid)
        :
        mvAppItem(uuid)
    {

    }

    void mvClickedHandler::customAction(void* data)
    {

        mvAppItemState* state = static_cast<mvAppItemState*>(data);
        if(_button == -1 || _button == 0)
            if (state->leftclicked)
            {
                mvSubmitCallback([=]()
                {
                    mvPyObject pArgs(PyTuple_New(2));
                    PyTuple_SetItem(pArgs, 0, ToPyInt(0));
                    PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
                    if (_alias.empty())
                        mvRunCallback(getCallback(false), _uuid, pArgs, _user_data);
                    else
                        mvRunCallback(getCallback(false), _alias, pArgs, _user_data);
                    });
            }

        if (_button == -1 || _button == 1)
            if (state->rightclicked)
            {
                mvSubmitCallback([=]()
                {
                    mvPyObject pArgs(PyTuple_New(2));
                    PyTuple_SetItem(pArgs, 0, ToPyInt(1));
                    PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
                    if (_alias.empty())
                        mvRunCallback(getCallback(false), _uuid, pArgs, _user_data);
                    else
                        mvRunCallback(getCallback(false), _alias, pArgs, _user_data);
                    });
            }

        if (_button == -1 || _button == 2)
            if (state->middleclicked)
            {
                mvSubmitCallback([=]()
                    {
                        mvPyObject pArgs(PyTuple_New(2));
                        PyTuple_SetItem(pArgs, 0, ToPyInt(2));
                        PyTuple_SetItem(pArgs, 1, ToPyUUID(state->parent)); // steals data, so don't deref
                        if (_alias.empty())
                            mvRunCallback(getCallback(false), _uuid, pArgs, _user_data);
                        else
                            mvRunCallback(getCallback(false), _alias, pArgs, _user_data);
                    });
            }

    }

    void mvClickedHandler::handleSpecificRequiredArgs(PyObject* dict)
    {
        if (!VerifyRequiredArguments(GetParsers()[s_command], dict))
            return;

        for (int i = 0; i < PyTuple_Size(dict); i++)
        {
            PyObject* item = PyTuple_GetItem(dict, i);
            switch (i)
            {
            
            case 0:
                _button = ToInt(item);
                break;

            default:
                break;
            }
        }
    }

    void mvClickedHandler::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "button")) _button = ToInt(item);
    }

    void mvClickedHandler::getSpecificConfiguration(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        PyDict_SetItemString(dict, "button", ToPyInt(_button));
    }

    void mvClickedHandler::applySpecificTemplate(mvAppItem* item)
    {
        auto titem = static_cast<mvClickedHandler*>(item);
        _button = titem->_button;
    }
}
