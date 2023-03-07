#pragma once

#include "mvItemRegistry.h"

class mvViewportDrawlist : public mvAppItem
{

public:

    explicit mvViewportDrawlist(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    bool _front = true;

};

class mvDrawlist : public mvAppItem
{

public:

    explicit mvDrawlist(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    float  _startx = 0.0f;
    float  _starty = 0.0f;

};

class mvDrawNode : public mvAppItem
{

public:

    explicit mvDrawNode(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvDrawNode();

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;
};

class mvDrawLayer : public mvAppItem
{

public:

    explicit mvDrawLayer(mvUUID uuid) : mvAppItem(uuid) {}
    ~mvDrawLayer();

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

};

class mvDrawArrow : public mvAppItem
{

public:

    explicit mvDrawArrow(mvUUID uuid);

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    void updatePoints();

private:

    mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvColor _color;
    float   _thickness = 1.0f;
    float   _size = 4;
    mvVec4  _points[3];

};

class mvDrawBezierCubic : public mvAppItem
{

public:

    explicit mvDrawBezierCubic(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:

    mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p4 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvColor _color;
    float   _thickness = 0.0f;
    int     _segments = 0;

};

class mvDrawBezierQuadratic : public mvAppItem
{

public:

    explicit mvDrawBezierQuadratic(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:

    mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvColor _color;
    float   _thickness = 0.0f;
    int     _segments = 0;

};

class mvDrawCircle : public mvAppItem
{

public:

    explicit mvDrawCircle(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:

    mvVec4  _center = { 0.0f, 0.0f, 0.0f, 1.0f };
    float   _radius = 1.0f;
    int     _segments = 0;
    mvColor _color;
    mvColor _fill;
    float   _thickness = 1.0f;

};

class mvDrawEllipse : public mvAppItem
{

public:

    explicit mvDrawEllipse(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:

    mvVec4  _pmin = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _pmax = { 1.0f, 1.0f, 0.0f, 1.0f };
    int     _segments = 0;
    mvColor _color;
    mvColor _fill;
    float   _thickness = 1.0f;
    std::vector<mvVec4> _points;
    bool _dirty = true;

};

class mvDrawImage : public mvAppItem
{

public:

    explicit mvDrawImage(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:


    mvUUID      _textureUUID = 0;
    mvVec4      _pmax = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4      _pmin = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec2      _uv_min = { 0.0f, 0.0f };
    mvVec2      _uv_max = { 1.0f, 1.0f };
    mvColor     _color = mvImGuiCol_Text;

    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> _texture = nullptr;
    bool _internalTexture = false; // create a local texture if necessary

};

class mvDrawImageQuad : public mvAppItem
{

public:

    explicit mvDrawImageQuad(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:


    mvUUID      _textureUUID = 0;
    mvVec4      _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4      _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4      _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4      _p4 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec2      _uv1 = { 0.0f, 0.0f };
    mvVec2      _uv2 = { 1.0f, 0.0f };
    mvVec2      _uv3 = { 1.0f, 1.0f };
    mvVec2      _uv4 = { 0.0f, 1.0f };
    mvColor     _color = mvColor(255, 255, 255, 255);

    // pointer to existing item or internal
    std::shared_ptr<mvAppItem> _texture = nullptr;
    bool _internalTexture = false; // create a local texture if necessary

};

class mvDrawLine : public mvAppItem
{

public:

    explicit mvDrawLine(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvColor _color = { 1.0f, 1.0f, 1.0f, 1.0f };
    float   _thickness = 1.0f;

};

class mvDrawPolygon : public mvAppItem
{

public:

    explicit mvDrawPolygon(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:

    std::vector<mvVec4> _points;
    mvColor             _color;
    mvColor             _fill;
    float               _thickness = 1.0f;

};

class mvDrawPolyline : public mvAppItem
{

public:

    explicit mvDrawPolyline(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;


private:

    std::vector<mvVec4> _points;
    mvColor             _color;
    bool                _closed = false;
    float               _thickness = 1.0f;

};

class mvDrawQuad : public mvAppItem
{

public:

    explicit mvDrawQuad(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p4 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvColor _color;
    mvColor _fill;
    float   _thickness = 1.0f;

};

class mvDrawRect : public mvAppItem
{

public:

    explicit mvDrawRect(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    mvVec4  _pmax = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _pmin = { 1.0f, 0.0f, 0.0f, 1.0f };
    float   _rounding = 0.0f;
    mvColor _color;
    mvColor _color_upper_left;
    mvColor _color_upper_right;
    mvColor _color_bottom_left;
    mvColor _color_bottom_right;
    mvColor _fill;
    float   _thickness = 1.0f;
    bool    _multicolor = false;
};

class mvDrawText : public mvAppItem
{

public:

    explicit mvDrawText(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    mvVec4      _pos = { 0.0f, 0.0f, 0.0f, 1.0f };
    std::string _text;
    mvColor     _color;
    float       _size = 1.0f;
};

class mvDrawTriangle : public mvAppItem
{

public:

    explicit mvDrawTriangle(mvUUID uuid) : mvAppItem(uuid) {}

    void draw(ImDrawList* drawlist, float x, float y) override;
    void handleSpecificRequiredArgs(PyObject* args) override;
    void handleSpecificKeywordArgs(PyObject* dict) override;
    void getSpecificConfiguration(PyObject* dict) override;

private:

    mvVec4  _p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvVec4  _p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
    mvColor _color;
    mvColor _fill;
    float   _thickness = 1.0f;

};