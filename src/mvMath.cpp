#include "mvMath.h"

mvVec2::operator ImVec2()
{
    return ImVec2{ x, y };
}

mvVec2::operator ImPlotPoint()
{
    return ImPlotPoint{ (double)x, (double)y };
}

mvVec4::operator ImPlotPoint()
{
    return ImPlotPoint{ (double)x, (double)y };
}

mvVec4::operator mvVec2()
{
    return mvVec2{ x, y };
}

mvVec4::operator mvVec3()
{
    return mvVec3{ x, y, z };
}

mvVec4::operator ImVec4()
{
    if (x < 0 || y < 0 || z < 0 || w < 0)
        return ImVec4(0, 0, 0, -1);
    return ImVec4{ x, y , z, w};
}

f32&
mvVec2::operator[](i32 index)
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    default: return y;
    }
}

f32&
mvVec3::operator[](i32 index)
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    default: return z;
    }
}

f32&
mvVec4::operator[](i32 index)
{
    switch (index)
    {
    case 0: return x;
    case 1: return y;
    case 2: return z;
    case 3: return w;
    default: return w;
    }
}

mvVec4&
mvMat4::operator[](i32 index)
{
    switch (index)
    {
    case 0: return cols[0];
    case 1: return cols[1];
    case 2: return cols[2];
    case 3: return cols[3];
    default: return cols[3];
    }
}

mvVec2
operator+(mvVec2 left, mvVec2 right)
{
    mvVec2 result = left;

    result.x += right.x;
    result.y += right.y;

    return result;
}

mvVec2
operator-(mvVec2 left, mvVec2 right)
{
    mvVec2 result = left;

    result.x -= right.x;
    result.y -= right.y;

    return result;
}

mvVec2
operator*(mvVec2 left, mvVec2 right)
{
    mvVec2 result = left;

    result.x *= right.x;
    result.y *= right.y;

    return result;
}

mvVec2
operator*(mvVec2 left, f32 right)
{
    mvVec2 result = left;

    result[0] *= right;
    result[1] *= right;

    return result;
}

mvVec3 
operator+(mvVec3 left, mvVec3 right)
{
    mvVec3 result = left;

    result.x += right.x;
    result.y += right.y;
    result.z += right.z;

    return result;
}

mvVec3 
operator-(mvVec3 left, mvVec3 right)
{
    mvVec3 result = left;

    result.x -= right.x;
    result.y -= right.y;
    result.z -= right.z;

    return result;
}

mvVec3 
operator*(mvVec3 left, mvVec3 right)
{
    mvVec3 result = left;

    result.x *= right.x;
    result.y *= right.y;
    result.z *= right.z;

    return result;
}

mvVec3
operator*(mvVec3 left, f32 right)
{
    mvVec3 result = left;

    result[0] *= right;
    result[1] *= right;
    result[2] *= right;

    return result;
}

mvVec4 
operator+(mvVec4 left, mvVec4 right)
{
    mvVec4 result = left;

    result.x += right.x;
    result.y += right.y;
    result.z += right.z;
    result.w += right.w;

    return result;
}

mvVec4 
operator-(mvVec4 left, mvVec4 right)
{
    mvVec4 result = left;

    result.x -= right.x;
    result.y -= right.y;
    result.z -= right.z;
    result.w -= right.w;

    return result;
}

mvVec4 
operator*(mvVec4 left, mvVec4 right)
{
    mvVec4 result = left;

    result.x = left.x * right.x;
    result.y = left.y * right.y;
    result.z = left.z * right.z;
    result.w = left.w * right.w;

    return result;
}

mvVec4 
operator*(mvMat4 left, mvVec4 right)
{
    mvVec4 Mov0 = { right[0], right[0], right[0], right[0] };
    mvVec4 Mov1 = { right[1], right[1], right[1], right[1] };
    mvVec4 Mul0 = left[0] * Mov0;
    mvVec4 Mul1 = left[1] * Mov1;
    mvVec4 Add0 = Mul0 + Mul1;
    mvVec4 Mov2 = { right[2], right[2], right[2], right[2] };
    mvVec4 Mov3 = { right[3], right[3], right[3], right[3] };
    mvVec4 Mul2 = left[2] * Mov2;
    mvVec4 Mul3 = left[3] * Mov3;
    mvVec4 Add1 = Mul2 + Mul3;
    mvVec4 Add2 = Add0 + Add1;
    return Add2;
}

mvVec4 
operator*(mvVec4 left, f32 right)
{
    mvVec4 result = left;

    result[0] *= right;
    result[1] *= right;
    result[2] *= right;
    result[3] *= right;

    return result;
}

mvMat4 
operator*(mvMat4 left, mvMat4 right)
{
    mvVec4 SrcA0 = left[0];
    mvVec4 SrcA1 = left[1];
    mvVec4 SrcA2 = left[2];
    mvVec4 SrcA3 = left[3];

    mvVec4 SrcB0 = right[0];
    mvVec4 SrcB1 = right[1];
    mvVec4 SrcB2 = right[2];
    mvVec4 SrcB3 = right[3];

    mvMat4 result{};

    result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
    result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
    result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
    result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];

    return result;
}

mvMat4 
operator+(mvMat4 left, mvMat4 right)
{

    mvMat4 result{};

    result[0] = left[0] + right[0];
    result[1] = left[1] + right[1];
    result[2] = left[2] + right[2];
    result[3] = left[3] + right[3];

    return result;
}

mvMat4 
operator-(mvMat4 left, mvMat4 right)
{

    mvMat4 result{};

    result[0] = left[0] - right[0];
    result[1] = left[1] - right[1];
    result[2] = left[2] - right[2];
    result[3] = left[3] - right[3];

    return result;
}

mvMat4
operator*(mvMat4 left, f32 right)
{
    mvMat4 result = left;

    for (u32 i = 0; i < 4; i++)
        for (u32 j = 0; j < 4; j++)
            result[i][j] *= right;

    return result;
}

mvMat4 
mvIdentityMat4()
{
    mvMat4 result{};

    result[0][0] = 1.0f;
    result[1][1] = 1.0f;
    result[2][2] = 1.0f;
    result[3][3] = 1.0f;

    return result;
}

mvMat4 
mvTranslate(mvMat4 m, mvVec3 v)
{
    mvMat4 result = m;

    result.cols[3] =
        m[0] * v.x +
        m[1] * v.y +
        m[2] * v.z +
        m[3];

    return result;
}

mvMat4 
mvRotate(mvMat4 m, f32 angle, mvVec3 v)
{
    const f32 a = angle;
    const f32 c = cos(a);
    const f32 s = sin(a);

    mvVec3 axis = mvNormalize(v);
    mvVec3 temp = axis * (1.0f - c);

    mvMat4 rotate{};

    rotate[0][0] = c + temp[0] * axis[0];
    rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    rotate[1][1] = c + temp[1] * axis[1];
    rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    rotate[2][2] = c + temp[2] * axis[2];

    mvMat4 result{};
    result[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
    result[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
    result[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
    result[3] = m[3];
    
    return result;
}

mvMat4 
mvScale(mvMat4 m, mvVec3 v)
{
    mvMat4 result{};
    result[0] = m[0] * v[0];
    result[1] = m[1] * v[1];
    result[2] = m[2] * v[2];
    result[3] = m[3];
    return result;
}

mvVec3 
mvNormalize(mvVec3 v)
{
    f32 length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    mvVec3 result{};
    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;
    return result;
}

mvVec3 
mvCross(mvVec3 v1, mvVec3 v2)
{
    mvVec3 result{};
    result.x = v1.y * v2.z - v2.y * v1.z;
    result.y = v1.z * v2.x - v2.z * v1.x;
    result.z = v1.x * v2.y - v2.x * v1.y;
    return result;
}

f32
mvDot(mvVec3 v1, mvVec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

mvMat4
mvLookAtRH(mvVec3 eye, mvVec3 center, mvVec3 up)
{
    mvVec3 zaxis = mvNormalize(center - eye);
    mvVec3 xaxis = mvNormalize(mvCross(zaxis, up));
    mvVec3 yaxis = mvCross(xaxis, zaxis);

    mvMat4 viewMatrix = mvIdentityMat4();

    // row 0
    viewMatrix[0][0] = xaxis.x;
    viewMatrix[1][0] = xaxis.y;
    viewMatrix[2][0] = xaxis.z;
    viewMatrix[3][0] = -mvDot(xaxis, eye);

    // row 1
    viewMatrix[0][1] = yaxis.x;
    viewMatrix[1][1] = yaxis.y;
    viewMatrix[2][1] = yaxis.z;
    viewMatrix[3][1] = -mvDot(yaxis, eye);

    // row 2
    viewMatrix[0][2] = -zaxis.x;
    viewMatrix[1][2] = -zaxis.y;
    viewMatrix[2][2] = -zaxis.z;
    viewMatrix[3][2] = mvDot(zaxis, eye);

    return viewMatrix;
}

mvMat4
mvOrthoRH(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
{
    mvMat4 result = mvIdentityMat4();
    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = -2.0f / (zFar - zNear);
    result[3][0] = -(right + left) / (right - left);
    result[3][1] = -(top + bottom) / (top - bottom);
    result[3][2] = -(zFar + zNear) / (zFar - zNear);
    return result;
}

mvMat4
mvPerspectiveRH(f32 fovy, f32 aspect, f32 zNear, f32 zFar)
{
    const f32 tanHalfFovy = tan(fovy / 2.0f);

    mvMat4 result{};
    result[0][0] = 1.0f / (aspect * tanHalfFovy);
    result[1][1] = 1.0f / (tanHalfFovy);
    result[2][2] = -(zFar + zNear) / (zFar - zNear);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
    return result;
}

mvMat4
mvInvert(mvMat4 m)
{
    float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
    float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
    float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
    
    float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
    float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
    float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
    
    float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
    float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
    float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
    
    float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
    float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
    float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
    
    float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
    float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
    float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
    
    float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
    float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
    float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

    mvVec4 Fac0 = {Coef00, Coef00, Coef02, Coef03};
    mvVec4 Fac1 = {Coef04, Coef04, Coef06, Coef07};
    mvVec4 Fac2 = {Coef08, Coef08, Coef10, Coef11};
    mvVec4 Fac3 = {Coef12, Coef12, Coef14, Coef15};
    mvVec4 Fac4 = {Coef16, Coef16, Coef18, Coef19};
    mvVec4 Fac5 = {Coef20, Coef20, Coef22, Coef23};

    mvVec4 Vec0 = {m[1][0], m[0][0], m[0][0], m[0][0]};
    mvVec4 Vec1 = {m[1][1], m[0][1], m[0][1], m[0][1]};
    mvVec4 Vec2 = {m[1][2], m[0][2], m[0][2], m[0][2]};
    mvVec4 Vec3 = {m[1][3], m[0][3], m[0][3], m[0][3]};

    mvVec4 Inv0 = {Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2};
    mvVec4 Inv1 = {Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4};
    mvVec4 Inv2 = {Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5};
    mvVec4 Inv3 = {Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5};

    mvVec4 SignA = {+1.0f, -1.0f, +1.0f, -1.0f};
    mvVec4 SignB = {-1.0f, +1.0f, -1.0f, +1.0f};

    mvMat4 Inverse = mvConstructMat4(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

    mvVec4 Row0 = { Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0] };

    mvVec4 Dot0 = (m[0] * Row0);
    float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

    float OneOverDeterminant = 1.0f / Dot1;

    return Inverse * OneOverDeterminant;
}

mvMat4
mvConstructMat4(mvVec4 c0, mvVec4 c1, mvVec4 c2, mvVec4 c3)
{
    mvMat4 result{};
    result[0] = c0;
    result[1] = c1;
    result[2] = c2;
    result[3] = c3;
    return result;
}

mvMat4 
mvCreateMatrix(
    f32 m00, f32 m01, f32 m02, f32 m03,
    f32 m10, f32 m11, f32 m12, f32 m13,
    f32 m20, f32 m21, f32 m22, f32 m23,
    f32 m30, f32 m31, f32 m32, f32 m33
    )
{
    mvMat4 m{};

    // column 0
    m[0][0] = m00;
    m[0][1] = m10;
    m[0][2] = m20;
    m[0][3] = m30;

    // column 1
    m[1][0] = m01;
    m[1][1] = m11;
    m[1][2] = m21;
    m[1][3] = m31;

    // column 2
    m[2][0] = m02;
    m[2][1] = m12;
    m[2][2] = m22;
    m[2][3] = m32;

    // column 3
    m[3][0] = m03;
    m[3][1] = m13;
    m[3][2] = m23;
    m[3][3] = m33;

    return m;
}

mvMat4
mvFPSViewRH(mvVec3 eye, float pitch, float yaw)
{

    // I assume the values are already converted to radians.
    f32 cosPitch = cos(pitch);
    f32 sinPitch = sin(pitch);
    f32 cosYaw = cos(yaw);
    f32 sinYaw = sin(yaw);

    mvVec3 xaxis = { cosYaw, 0, -sinYaw };
    mvVec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
    mvVec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

    mvMat4 viewMatrix = mvConstructMat4(
        mvVec4{ xaxis.x, yaxis.x, zaxis.x, 0 },
        mvVec4{ xaxis.y, yaxis.y, zaxis.y, 0 },
        mvVec4{ xaxis.z, yaxis.z, zaxis.z, 0 },
        mvVec4{ -mvDot(xaxis, eye), -mvDot(yaxis, eye), -mvDot(zaxis, eye), 1 }
    );

    return viewMatrix;
}