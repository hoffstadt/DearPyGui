#pragma once

#include <imgui.h>
#include <implot.h>
#include <cmath>
#include "mvTypes.h"

struct mvVec2
{
	union { f32 x, r, u; };
	union { f32 y, g, v; };

	float& operator[](i32 index);
    operator ImVec2();
    operator ImPlotPoint();
};

struct mvVec3
{
	union { f32 x, r; };
	union { f32 y, g; };
	union { f32 z, b; };

	float& operator[](i32 index);
};

struct mvVec4
{
	union { f32 x, r; };
	union { f32 y, g; };
	union { f32 z, b; };
	union { f32 w, a; };

	float& operator[](i32 index);
    operator ImVec4();
    operator mvVec2();
    operator mvVec3();
    operator ImPlotPoint();

	mvVec2 xy()
	{
		return mvVec2{ x, y };
	}

	mvVec3 xyz()
	{
		return mvVec3{ x, y, z };
	}
};

struct mvMat4
{
	mvVec4 cols[4];

	mvVec4& operator[](i32 index);
};

mvMat4 mvIdentityMat4();
mvMat4 mvConstructMat4(mvVec4 c0, mvVec4 c1, mvVec4 c2, mvVec4 c3);

struct mvTransforms
{
	mvMat4 model               = mvIdentityMat4();
	mvMat4 modelView           = mvIdentityMat4();
	mvMat4 modelViewProjection = mvIdentityMat4();
};

mvVec2 operator+(mvVec2 left, mvVec2 right);
mvVec2 operator-(mvVec2 left, mvVec2 right);
mvVec2 operator*(mvVec2 left, mvVec2 right);
mvVec2 operator*(mvVec2 left, f32 right);

mvVec3 operator+(mvVec3 left, mvVec3 right);
mvVec3 operator-(mvVec3 left, mvVec3 right);
mvVec3 operator*(mvVec3 left, mvVec3 right);
mvVec3 operator*(mvVec3 left, f32 right);

mvVec4 operator+(mvVec4 left, mvVec4 right);
mvVec4 operator-(mvVec4 left, mvVec4 right);
mvVec4 operator*(mvVec4 left, mvVec4 right);
mvVec4 operator*(mvVec4 left, f32 right);
mvVec4 operator*(mvMat4 left, mvVec4 right);

mvMat4 operator*(mvMat4 left, mvMat4 right);
mvMat4 operator+(mvMat4 left, mvMat4 right);
mvMat4 operator-(mvMat4 left, mvMat4 right);
mvMat4 operator*(mvMat4 left, f32 right);

mvVec3 mvNormalize(mvVec3 v);
mvVec3 mvCross    (mvVec3 v1, mvVec3 v2);
f32    mvDot      (mvVec3 v1, mvVec3 v2);

mvMat4 mvLookAtRH(mvVec3 eye, mvVec3 center, mvVec3 up); // world space eye, center, up
mvMat4 mvFPSViewRH(mvVec3 eye, float pitch, float yaw);   // world space eye, center, up
mvMat4 mvTranslate(mvMat4 m, mvVec3 v);
mvMat4 mvRotate(mvMat4 m, f32 angle, mvVec3 v);
mvMat4 mvScale(mvMat4 m, mvVec3 v);
mvMat4 mvOrthoRH(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
mvMat4 mvPerspectiveRH(f32 fovy, f32 aspect, f32 zNear, f32 zFar);
mvMat4 mvCreateMatrix(
	f32 m00, f32 m01, f32 m02, f32 m03,
	f32 m10, f32 m11, f32 m12, f32 m13,
	f32 m20, f32 m21, f32 m22, f32 m23,
	f32 m30, f32 m31, f32 m32, f32 m33
);
