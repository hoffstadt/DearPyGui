#pragma once

#include "mvItemRegistry.h"

namespace Marvel {

    class mvLoadingIndicator : public mvAppItem
    {

    public:

        static void InsertParser(std::map<std::string, mvPythonParser>* parsers);

        MV_APPLY_WIDGET_REGISTRATION(mvAppItemType::mvLoadingIndicator, add_loading_indicator)
        MV_NO_COMMANDS
        MV_DEFAULT_PARENTS
        MV_DEFAULT_CHILDREN

    public:

        explicit mvLoadingIndicator(mvUUID uuid);

        void draw(ImDrawList* drawlist, float x, float y) override;
        void handleSpecificKeywordArgs(PyObject* dict) override;
        void getSpecificConfiguration(PyObject* dict) override;
        void applySpecificTemplate(mvAppItem* item) override;

    private:

        int     _style = 0;
        int     _circleCount = 8;
        float   _radius = 3.0f;
        float   _speed = 1.0f;
        float   _thickness = 1.0f;
        mvColor _mainColor = mvColor(51, 51, 55, 255);
        mvColor _optionalColor = mvColor(29, 151, 236, 103);

    };

}
