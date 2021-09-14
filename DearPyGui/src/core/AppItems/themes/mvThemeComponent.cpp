#include "mvThemeComponent.h"
#include "mvApp.h"
#include <array>
#include "mvItemRegistry.h"
#include "mvLog.h"
#include "mvPythonExceptions.h"
#include "mvAppItemCommons.h"

namespace Marvel {

    void mvThemeComponent::InsertParser(std::map<std::string, mvPythonParser>* parsers)
    {
        std::vector<mvPythonDataElement> args;

        AddCommonArgs(args,(CommonParserArgs)(
            MV_PARSER_ARG_ID |
            MV_PARSER_ARG_PARENT |
            MV_PARSER_ARG_BEFORE
            ));

        args.push_back({ mvPyDataType::Integer, "item_type", mvArgType::POSITIONAL_ARG, "0" });
        args.push_back({ mvPyDataType::Bool, "enabled_state", mvArgType::KEYWORD_ARG, "True" });

        mvPythonParserSetup setup;
        setup.about = "Adds a theme component.";
        setup.category = { "Themes", "Containers" };
        setup.returnType = mvPyDataType::UUID;
        setup.createContextManager = true;

        mvPythonParser parser = FinalizeParser(setup, args);
        parsers->insert({ s_command, parser });

    }

    mvThemeComponent::mvThemeComponent(mvUUID uuid)
        :
        mvAppItem(uuid)
    {
    }

    void mvThemeComponent::draw(ImDrawList* drawlist, float x, float y)
    {

        for (auto& childset : _children)
        {
            for (auto& child : childset)
                child->draw(drawlist, x, y);
        }

    }

    void mvThemeComponent::customAction(void* data)
    {
        for (auto& childset : _children)
        {
            for (auto& child : childset)
                child->customAction(data);
        }
        
    }

    void mvThemeComponent::handleSpecificPositionalArgs(PyObject* dict)
    {
        if (!VerifyPositionalArguments(mvApp::GetApp()->getParsers()[s_command], dict))
            return;

        for (int i = 0; i < PyTuple_Size(dict); i++)
        {
            PyObject* item = PyTuple_GetItem(dict, i);
            switch (i)
            {
            case 0:
            {
                _specificType = ToInt(item);
                constexpr_for<1, (int)mvAppItemType::ItemTypeCount, 1>(
                    [&](auto i) {
                        using item_type = typename mvItemTypeMap<i>::type;
                        if (i == _specificType)
                            _specificComponentPtr = &item_type::s_class_theme_component;
                    });
                break;
            }
            default:
                break;
            }
        }
    }

    void mvThemeComponent::handleSpecificKeywordArgs(PyObject* dict)
    {
        if (dict == nullptr)
            return;

        if (PyObject* item = PyDict_GetItemString(dict, "enabled_state")) _specificEnabled = ToBool(item);
    }
}
