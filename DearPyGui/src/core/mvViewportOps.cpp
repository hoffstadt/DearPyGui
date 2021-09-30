#include "mvViewportOps.h"
#include "mvViewport.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "mvPythonExceptions.h"
#include "mvPyObject.h"
#include "mvPythonTranslator.h"
#include "mvCallbackRegistry.h"

namespace Marvel {

    void 
    InsertParser_mvViewport(std::map<std::string, mvPythonParser>* parsers)
    {
        {
            std::vector<mvPythonDataElement> args;
            args.reserve(16);
            args.push_back({ mvPyDataType::String, "title", mvArgType::KEYWORD_ARG, "'Dear PyGui'" });
            args.push_back({ mvPyDataType::String, "small_icon", mvArgType::KEYWORD_ARG, "''" });
            args.push_back({ mvPyDataType::String, "large_icon", mvArgType::KEYWORD_ARG, "''" });
            args.push_back({ mvPyDataType::Integer, "width", mvArgType::KEYWORD_ARG, "1280" });
            args.push_back({ mvPyDataType::Integer, "height", mvArgType::KEYWORD_ARG, "800" });
            args.push_back({ mvPyDataType::Integer, "x_pos", mvArgType::KEYWORD_ARG, "100" });
            args.push_back({ mvPyDataType::Integer, "y_pos", mvArgType::KEYWORD_ARG, "100" });
            args.push_back({ mvPyDataType::Integer, "min_width", mvArgType::KEYWORD_ARG, "250" });
            args.push_back({ mvPyDataType::Integer, "max_width", mvArgType::KEYWORD_ARG, "10000" });
            args.push_back({ mvPyDataType::Integer, "min_height", mvArgType::KEYWORD_ARG, "250" });
            args.push_back({ mvPyDataType::Integer, "max_height", mvArgType::KEYWORD_ARG, "10000" });
            args.push_back({ mvPyDataType::Bool, "resizable", mvArgType::KEYWORD_ARG, "True" });
            args.push_back({ mvPyDataType::Bool, "vsync", mvArgType::KEYWORD_ARG, "True" });
            args.push_back({ mvPyDataType::Bool, "always_on_top", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "decorated", mvArgType::KEYWORD_ARG, "True" });
            args.push_back({ mvPyDataType::FloatList, "clear_color", mvArgType::KEYWORD_ARG, "(0, 0, 0, 255)" });

            mvPythonParserSetup setup;
            setup.about = "Creates a viewport.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "create_viewport", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::Bool, "minimized", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::Bool, "maximized", mvArgType::KEYWORD_ARG, "False" });
            args.push_back({ mvPyDataType::UUID, "viewport", mvArgType::DEPRECATED_REMOVE_KEYWORD_ARG });

            mvPythonParserSetup setup;
            setup.about = "Shows the main viewport.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "show_viewport", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "item" });

            mvPythonParserSetup setup;
            setup.about = "Configures a viewport.";
            setup.category = { "General" };
            setup.unspecifiedKwargs = true;
            setup.internal = true;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "configure_viewport", parser });
        }

        {
            std::vector<mvPythonDataElement> args;
            args.push_back({ mvPyDataType::UUID, "item" });

            mvPythonParserSetup setup;
            setup.about = "Returns a viewport's configuration.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Dict;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "get_viewport_configuration", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Checks if a viewport has been created and shown.";
            setup.category = { "General" };
            setup.returnType = mvPyDataType::Bool;

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "is_viewport_ok", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Maximizes the viewport.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "maximize_viewport", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Minimizes a viewport.";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "minimize_viewport", parser });
        }

        {
            std::vector<mvPythonDataElement> args;

            mvPythonParserSetup setup;
            setup.about = "Toggle viewport fullscreen mode..";
            setup.category = { "General" };

            mvPythonParser parser = FinalizeParser(setup, args);
            parsers->insert({ "toggle_viewport_fullscreen", parser });
        }

    }

    mv_python_function
    get_viewport_configuration(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        PyObject* pdict = PyDict_New();

        mvViewport* viewport = GContext->viewport;
        if (viewport)
        {
            PyDict_SetItemString(pdict, "clear_color", mvPyObject(ToPyColor(viewport->clearColor)));
            PyDict_SetItemString(pdict, "small_icon", mvPyObject(ToPyString(viewport->small_icon)));
            PyDict_SetItemString(pdict, "large_icon", mvPyObject(ToPyString(viewport->large_icon)));
            PyDict_SetItemString(pdict, "x_pos", mvPyObject(ToPyInt(viewport->xpos)));
            PyDict_SetItemString(pdict, "y_pos", mvPyObject(ToPyInt(viewport->ypos)));
            PyDict_SetItemString(pdict, "width", mvPyObject(ToPyInt(viewport->actualWidth)));
            PyDict_SetItemString(pdict, "height", mvPyObject(ToPyInt(viewport->actualHeight)));
            PyDict_SetItemString(pdict, "client_width", mvPyObject(ToPyInt(viewport->clientWidth)));
            PyDict_SetItemString(pdict, "client_height", mvPyObject(ToPyInt(viewport->clientHeight)));
            PyDict_SetItemString(pdict, "resizable", mvPyObject(ToPyBool(viewport->resizable)));
            PyDict_SetItemString(pdict, "vsync", mvPyObject(ToPyBool(viewport->vsync)));
            PyDict_SetItemString(pdict, "min_width", mvPyObject(ToPyInt(viewport->minwidth)));
            PyDict_SetItemString(pdict, "max_width", mvPyObject(ToPyInt(viewport->maxwidth)));
            PyDict_SetItemString(pdict, "min_height", mvPyObject(ToPyInt(viewport->minheight)));
            PyDict_SetItemString(pdict, "max_height", mvPyObject(ToPyInt(viewport->maxheight)));
            PyDict_SetItemString(pdict, "always_on_top", mvPyObject(ToPyBool(viewport->alwaysOnTop)));
            PyDict_SetItemString(pdict, "decorated", mvPyObject(ToPyBool(viewport->decorated)));
            PyDict_SetItemString(pdict, "title", mvPyObject(ToPyString(viewport->title)));
        }
        else
            mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

        return pdict;
    }

    mv_python_function
    is_viewport_ok(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);

        mvViewport* viewport = GContext->viewport;
        if (viewport)
        {
            if (viewport->shown)
                return ToPyBool(true);
        }

        return ToPyBool(false);
    }

    mv_python_function
    create_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        const char* title = "Dear PyGui";
        const char* small_icon = "";
        const char* large_icon = "";
        int width = 1280;
        int height = 800;
        int x_pos = 100;
        int y_pos = 100;
        int min_width = 250;
        int max_width = 10000;
        int min_height = 250;
        int max_height = 10000;

        int resizable = true;
        int vsync = true;
        int always_on_top = false;
        int decorated = true;

        PyObject* color = PyList_New(4);
        PyList_SetItem(color, 0, PyFloat_FromDouble(0.0));
        PyList_SetItem(color, 1, PyFloat_FromDouble(0.0));
        PyList_SetItem(color, 2, PyFloat_FromDouble(0.0));
        PyList_SetItem(color, 3, PyFloat_FromDouble(1.0));


        if (!Parse((GetParsers())["create_viewport"], args, kwargs, __FUNCTION__,
            &title, &small_icon, &large_icon, &width, &height, &x_pos, &y_pos, &min_width, &max_width, &min_height, &max_height,
            &resizable, &vsync, &always_on_top, &decorated, &color
        ))
            return GetPyNone();

        mvViewport* viewport = mvCreateViewport(width, height);
        if (PyObject* item = PyDict_GetItemString(kwargs, "clear_color")) viewport->clearColor = ToColor(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "small_icon")) viewport->small_icon = ToString(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "large_icon")) viewport->large_icon = ToString(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "x_pos")) { viewport->posDirty = true;  viewport->xpos = ToInt(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "y_pos")) { viewport->posDirty = true;  viewport->ypos = ToInt(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "width")) { viewport->sizeDirty = true;  viewport->actualWidth = ToInt(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "height")) { viewport->sizeDirty = true;  viewport->actualHeight = ToInt(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "resizable")) { viewport->modesDirty = true;  viewport->resizable = ToBool(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "vsync")) viewport->vsync = ToBool(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "min_width")) viewport->minwidth = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "max_width")) viewport->maxwidth = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "min_height")) viewport->minheight = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "max_height")) viewport->maxheight = ToInt(item);
        if (PyObject* item = PyDict_GetItemString(kwargs, "always_on_top")) { viewport->modesDirty = true; viewport->alwaysOnTop = ToBool(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "decorated")) { viewport->modesDirty = true; viewport->decorated = ToBool(item); }
        if (PyObject* item = PyDict_GetItemString(kwargs, "title")) { viewport->titleDirty = true; viewport->title = ToString(item); }

        if (viewport->sizeDirty)
        {
            if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
            mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
                CreateEventArgument("actual_width", viewport->actualWidth),
                CreateEventArgument("actual_height", viewport->actualHeight),
                CreateEventArgument("client_width", viewport->actualWidth),
                CreateEventArgument("client_height", viewport->actualHeight)
                });
        }

        GContext->viewport = viewport;

        return GetPyNone();
    }

    mv_python_function
    show_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        int minimized = false;
        int maximized = false;

        if (!Parse((GetParsers())["show_viewport"], args, kwargs, __FUNCTION__,
            &minimized, &maximized))
            return GetPyNone();

        mvViewport* viewport = GContext->viewport;
        if (viewport)
        {
            mvShowViewport(minimized, maximized);
            viewport->shown = true;
        }
        else
            mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

        return GetPyNone();
    }

    mv_python_function
    configure_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
    {

        mvViewport* viewport = GContext->viewport;
        if (viewport)
        {
            if (PyObject* item = PyDict_GetItemString(kwargs, "clear_color")) viewport->clearColor = ToColor(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "small_icon")) viewport->small_icon = ToString(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "large_icon")) viewport->large_icon = ToString(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "x_pos")) { viewport->posDirty = true;  viewport->xpos = ToInt(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "y_pos")) { viewport->posDirty = true;  viewport->ypos = ToInt(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "width")) { viewport->sizeDirty = true;  viewport->actualWidth = ToInt(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "height")) { viewport->sizeDirty = true;  viewport->actualHeight = ToInt(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "resizable")) { viewport->modesDirty = true;  viewport->resizable = ToBool(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "vsync")) viewport->vsync = ToBool(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "min_width")) viewport->minwidth = ToInt(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "max_width")) viewport->maxwidth = ToInt(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "min_height")) viewport->minheight = ToInt(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "max_height")) viewport->maxheight = ToInt(item);
            if (PyObject* item = PyDict_GetItemString(kwargs, "always_on_top")) { viewport->modesDirty = true; viewport->alwaysOnTop = ToBool(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "decorated")) { viewport->modesDirty = true; viewport->decorated = ToBool(item); }
            if (PyObject* item = PyDict_GetItemString(kwargs, "title")) { viewport->titleDirty = true; viewport->title = ToString(item); }

            if (viewport->sizeDirty)
            {
                if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
                mvEventBus::Publish(mvEVT_CATEGORY_VIEWPORT, mvEVT_VIEWPORT_RESIZE, {
                    CreateEventArgument("actual_width", viewport->actualWidth),
                    CreateEventArgument("actual_height", viewport->actualHeight),
                    CreateEventArgument("client_width", viewport->actualWidth),
                    CreateEventArgument("client_height", viewport->actualHeight)
                    });
            }
        }
        else
            mvThrowPythonError(mvErrorCode::mvNone, "No viewport created");

        return GetPyNone();
    }

    mv_python_function
    maximize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        GContext->callbackRegistry->submit([=]()
            {
                mvMaximizeViewport();
            });

        return GetPyNone();
    }

    mv_python_function
    minimize_viewport(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        GContext->callbackRegistry->submit([=]()
            {
                mvMinimizeViewport();
            });

        return GetPyNone();
    }

    mv_python_function
    toggle_viewport_fullscreen(PyObject* self, PyObject* args, PyObject* kwargs)
    {
        if (!GContext->manualMutexControl) std::lock_guard<std::mutex> lk(GContext->mutex);
        GContext->callbackRegistry->submit([=]()
            {
                mvToggleFullScreen();
            });

        return GetPyNone();
    }
}
