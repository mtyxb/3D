/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include <algorithm>
#include <math.h>
#include <QDebug>
#include "mtmatrix.h"

#define MT_PI  3.1415926

static double mt_angle_to_radians(double angle)
{
    return MT_PI * angle / 180.0;
}

template <typename T, int R>
MTMatrix<T, R>::MTMatrix()
{
    identity();
}

template <typename T, int R>
QString MTMatrix<T, R>::to_string() const
{
    QString result;
    result.append("{");
    for (int i = 0; i < R; i++)
    {
        result.append(values[i].to_string());
        if (i + 1 < R)
        {
            result.append(",");
        }
    }
    result.append("}");
    return result;
}

template <typename T, int R>
const MTVector<T, R> MTMatrix<T, R>::row(int index) const
{
    return values[index];
}

template <typename T, int R>
MTVector<T, R>& MTMatrix<T, R>::row(int index)
{
    return values[index];
}

template <typename T, int R>
const MTVector<T, R> MTMatrix<T, R>::col(int index) const
{
    MTVector<T, R> result;
    for (int i = 0; i < R; i++)
    {
        result[i] = values[i][index];
    }
    return result;
}


template <typename T, int R>
const MTVector<T, R> MTMatrix<T, R>::operator[](int index) const
{
    return row(index);
}

template <typename T, int R>
MTVector<T, R>& MTMatrix<T, R>::operator[](int index)
{
    return row(index);
}

template <typename T, int R>
const MTVector<T, R> MTMatrix<T, R>::operator()(int index) const
{
    return col(index);
}



template <typename T, int R>
static  MTMatrix<T, R - 1> mt_low_dim_matrix(const MTMatrix<T, R>& m, int row, int col)
{
   constexpr int N = R - 1;
   MTMatrix<T, N> result;
   for (int i = 0; i < N; i++)
   {
       for (int j = 0; j < N; j++)
       {
            result[i][j] = m[i < row ? i : i + 1][j < col ? j : j + 1];
       }
   }
   return result;
}

template <typename T, int R>
static T mt_det_of_matrix(const MTMatrix<T, R>& m)
{
  if constexpr (1 == R)
  {
        return m[0][0];
  }
  else
  {
       T result(0);
       constexpr int N = R - 1;
       int j = 0;
       for (int i = 0; i < R; i++)
       {
           MTMatrix<T, N> o = mt_low_dim_matrix(m, i, j);
           result += m[i][j] * mt_det_of_matrix(o) * ((i + j) % 2 ? -1 : 1);
       }
       return result;
  }
}

template <typename T, int R>
T MTMatrix<T, R>::det() const
{
    return mt_det_of_matrix(*this);
}

template <typename T, int R>
MTMatrix<T, R>& MTMatrix<T, R>::multiply(const MTMatrix<T, R>& other)
{
    MTMatrix<T, R> result;
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < R; j++)
        {
            result[i][j] = values[i].dot(other(j));
        }
    }
    *this = result;
    return *this;
}

template <typename T, int R>
MTVector<T, R> MTMatrix<T, R>::multiply(const MTVector<T, R>& other) const
{
    MTVector<T, R> result;
    for (int i = 0; i < R; i++)
    {
       result[i] = values[i].dot(other);
    }
    return result;
}

template <typename T, int R>
MTMatrix<T, R>& MTMatrix<T, R>::identity()
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < R; j++)
        {
            values[i][j] =  (i == j ? T(1) : T(0));
        }
    }    return *this;
}



template <typename T, int R>
MTMatrix<T, R>& MTMatrix<T, R>::invert()
{

    return *this;
}

template <typename T, int R>
MTMatrix<T, R>& MTMatrix<T, R>::transpose()
{
    MTMatrix<T, R> result;
    for (int i = 0; i < R; i++)
    {
        result[i] = col(i);
    }
    *this = result;
    return *this;
}


////////////////////////////////////////////////////////////////////

template <typename T>
MTMatrix4x4T<T>& MTMatrix4x4T<T>::ortho(float left, float right, float bottom, float top, float near, float far)
{
    return *this;
}

template <typename T>
MTMatrix4x4T<T>& MTMatrix4x4T<T>::frustum(float left, float right, float bottom, float top, float near, float far)
{
    if (left == right || bottom == top || near == far)
    {
       return *this;
    }

    float width = right - left;
    float height = top - bottom;
    float clip = far - near;

    MTMatrix4x4T<T>& ref = *this;
    ref[0][0] = 2.0f * near / width;
    ref[0][1] = 0.0f;
    ref[0][2] = (left + right) / width;
    ref[0][3] = 0.0f;

    ref[1][0] = 0.0f;
    ref[1][1] = 2.0f * near / height;
    ref[1][2] = (top + bottom) / height;
    ref[1][3] = 0.0f;

    ref[2][0] = 0.0f;
    ref[2][1] = 0.0f;
    ref[2][2] = -(near + far) / clip;
    ref[2][3] = -2.0f * near * far / clip;

    ref[3][0] = 0.0f;
    ref[3][1] = 0.0f;
    ref[3][2] = -1.0f;
    ref[3][3] = 0.0f;

    return *this;
}

template <typename T>
MTMatrix4x4T<T>& MTMatrix4x4T<T>::perspective(float angle, float aspect, float near, float far)
{
    if (near == far || aspect == 0.0f)
    {
        return *this;
    }


    float radians = mt_angle_to_radians(angle / 2.0f);
    float sine = std::sin(radians);
    if (sine == 0.0f)
    {
        return *this;
    }

    float cotan = std::cos(radians) / sine;
    float clip = far - near;

    MTMatrix4x4T<T>& ref = *this;

    ref[0][0] = cotan / aspect;
    ref[0][1] = 0.0f;
    ref[0][2] = 0.0f;
    ref[0][3] = 0.0f;

    ref[1][0] = 0.0f;
    ref[1][1] = cotan;
    ref[1][2] = 0.0f;
    ref[1][3] = 0.0f;

    ref[2][0] = 0.0f;
    ref[2][1] = 0.0f;
    ref[2][2] = -(near + far) / clip;
    ref[2][3] = -(2.0f * near * far) / clip;

    ref[3][0] = 0.0f;
    ref[3][1] = 0.0f;
    ref[3][2] = -1.0f;
    ref[3][3] = 0.0f;

    return *this;
}

template <typename T>
MTMatrix4x4T<T>& MTMatrix4x4T<T>::lookat(const MTVector3& eye, const MTVector3& center, const MTVector3& up)
{
    MTVector3 forward = center - eye;
    if (forward.length() < 0.00001)
    {
        return *this;
    }

    forward.normalize();

    MTVector3 side = forward.cross(up).normal();
    MTVector3 upv  = side.cross(forward);

    enum {x, y, z};
    MTMatrix4x4T<T>& ref = *this;
    ref[0][0] = side[x];
    ref[0][1] = side[y];
    ref[0][2] = side[z];
    ref[0][3] = 0.0f;

    ref[1][0] = upv[x];
    ref[1][1] = upv[y];
    ref[1][2] = upv[z];
    ref[1][3] = 0.0f;

    ref[2][0] = -forward[x];
    ref[2][1] = -forward[y];
    ref[2][2] = -forward[z];
    ref[2][3] = 0.0f;

    ref[3][0] = 0.0f;
    ref[3][1] = 0.0f;
    ref[3][2] = 0.0f;
    ref[3][3] = 1.0f;

    translate(-eye);
    return *this;
}

template <typename T>
MTMatrix4x4T<T>& MTMatrix4x4T<T>::viewport(float left, float bottom, float width, float height, float near, float far)
{
    const float halfw = width / 2.0f;
    const float halfh = height / 2.0f;

    MTMatrix4x4 t;
    t[0][0] = halfw;
    t[0][1] = 0.0f;
    t[0][2] = 0.0f;
    t[0][3] = left + halfw;

    t[1][0] = 0.0f;
    t[1][1] = halfh;
    t[1][2] = 0.0f;
    t[1][3] = bottom + halfh;

    t[2][0] = 0.0f;
    t[2][1] = 0.0f;
    t[2][2] = (far - near) / 2.0f;
    t[2][3] = (near + far) / 2.0f;

    t[3][0] = 0.0f;
    t[3][1] = 0.0f;
    t[3][2] = 0.0f;
    t[3][3] = 1.0f;

    this->multiply(t);
    return *this;
}

template <typename T>
MTMatrix4x4T<T>&  MTMatrix4x4T<T>::scale(float x, float y, float z)
{
    MTMatrix4x4T<T>& ref = *this;
    ref[0][0] *= x;
    ref[1][0] *= x;
    ref[2][0] *= x;
    ref[3][0] *= x;
    ref[0][1] *= y;
    ref[1][1] *= y;
    ref[2][1] *= y;
    ref[3][1] *= y;
    ref[0][2] *= z;
    ref[1][2] *= z;
    ref[2][2] *= z;
    ref[3][2] *= z;
    return *this;
}

template <typename T>
MTMatrix4x4T<T>&  MTMatrix4x4T<T>::translate(float x, float y, float z)
{
    MTMatrix4x4T<T>& ref = *this;
    ref[0][3] += ref[0][0] * x + ref[0][1] * y + ref[0][2] * z;
    ref[1][3] += ref[1][0] * x + ref[1][1] * y + ref[1][2] * z;
    ref[2][3] += ref[2][0] * x + ref[2][1] * y + ref[2][2] * z;
    ref[3][3] += ref[3][0] * x + ref[3][1] * y + ref[3][2] * z;
    return *this;
}

template <typename T>
MTMatrix4x4T<T>&  MTMatrix4x4T<T>::rotate(float angle, float x, float y, float z)
{
    if (angle == 0.0f)
    {
        return *this;
    }
    float c, s;
    if (angle == 90.0f || angle == -270.0f)
    {
        s = 1.0f;
        c = 0.0f;
    }
    else if (angle == -90.0f || angle == 270.0f)
    {
        s = -1.0f;
        c = 0.0f;
    }
    else if (angle == 180.0f || angle == -180.0f)
    {
        s = 0.0f;
        c = -1.0f;
    }
    else
    {
        float a = mt_angle_to_radians(angle);
        c = std::cos(a);
        s = std::sin(a);
    }

    double len = double(x) * double(x) +
                 double(y) * double(y) +
                 double(z) * double(z);
    if (std::fabs(len - 1.0) > 0.00001 && (std::fabs(len) > 0.00001))
    {
        len = std::sqrt(len);
        x = float(double(x) / len);
        y = float(double(y) / len);
        z = float(double(z) / len);
    }

    float ic = 1.0f - c;
    MTMatrix4x4 rot;
    rot[0][0] = x * x * ic + c;
    rot[0][1] = x * y * ic - z * s;
    rot[0][2] = x * z * ic + y * s;
    rot[0][3] = 0.0f;

    rot[1][0] = y * x * ic + z * s;
    rot[1][1] = y * y * ic + c;
    rot[1][2] = y * z * ic - x * s;
    rot[1][3] = 0.0f;

    rot[2][0] = x * z * ic - y * s;
    rot[2][1] = y * z * ic + x * s;
    rot[2][2] = z * z * ic + c;
    rot[2][3] = 0.0f;

    rot[3][0] = 0.0f;
    rot[3][1] = 0.0f;
    rot[3][2] = 0.0f;
    rot[3][3] = 1.0f;

    this->multiply(rot);
    return *this;
}


template <typename T>
MTMatrix4x4T<T>&  MTMatrix4x4T<T>::translate(const MTVector3& offset)
{
    return translate(offset[0], offset[1], offset[2]);
}

template <typename T>
MTMatrix4x4T<T>&  MTMatrix4x4T<T>::scale(const MTVector3& factor)
{
    return scale(factor[0], factor[1], factor[2]);
}

template <typename T>
MTMatrix4x4T<T>&  MTMatrix4x4T<T>::rotate(float angle, const MTVector3& axis)
{
    return rotate(angle, axis[0], axis[1], axis[2]);
}

template class MTMatrix<float, 1>;
template class MTMatrix<float, 2>;
template class MTMatrix<float, 3>;
template class MTMatrix<float, 4>;
template class MTMatrix4x4T<float>;

