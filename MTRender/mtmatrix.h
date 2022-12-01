/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#ifndef MTMATRIX_H
#define MTMATRIX_H

#include "mtvector.h"
#include <QString>


template <typename T, int R>
class MTMatrix
{
public:
    using value_type = T;
    static constexpr int COUNT = R;

    MTMatrix();
    QString to_string() const;

    const MTVector<T, R> row(int index) const;
    MTVector<T, R>& row(int index);
    const MTVector<T, R> col(int index) const;
    const MTVector<T, R> operator[](int index) const;
    MTVector<T, R>& operator[](int index);
    const MTVector<T, R> operator()(int index) const;

    T  det() const;
    MTMatrix<T, R>& multiply(const MTMatrix<T, R>& other);
    MTVector<T, R> multiply(const MTVector<T, R>& other) const;

    MTMatrix<T, R>& identity();
    MTMatrix<T, R>& invert();
    MTMatrix<T, R>& transpose();


private:
  MTVector<T, R>  values[R];
};

template <typename T, int N>
MTMatrix<T, N> operator*(const MTMatrix<T, N>& a, const MTMatrix<T, N>& b)
{
    MTMatrix<T, N> result = a;
    return result.multiply(b);
}

template <typename T, int N>
MTVector<T, N> operator*(const MTMatrix<T, N>& a, const MTVector<T, N>& b)
{
    return a.multiply(b);
}

template <typename T>
class MTMatrix4x4T : public MTMatrix<T, 4>
{
public:
    MTMatrix4x4T& ortho(float left, float right, float bottom, float top, float near, float far);
    MTMatrix4x4T& frustum(float left, float right, float bottom, float top, float near, float far);
    MTMatrix4x4T& perspective(float angle, float aspect, float near, float far);
    MTMatrix4x4T& lookat(const MTVector3& eye, const MTVector3& center, const MTVector3& up);
    MTMatrix4x4T& viewport(float left, float bottom, float width, float height, float near, float far);

    MTMatrix4x4T&  scale(float x, float y, float z);
    MTMatrix4x4T&  translate(float x, float y, float z);
    MTMatrix4x4T&  rotate(float angle, float x, float y, float z);

    MTMatrix4x4T&  translate(const MTVector3& offset);
    MTMatrix4x4T&  scale(const MTVector3& factor);
    MTMatrix4x4T&  rotate(float angle, const MTVector3& axis);
};

template <typename T>
MTVector3T<T> operator*(const MTMatrix4x4T<T>& a, const MTVector3T<T>& b)
{
    MTVector<T, 4> result = a.multiply(MTVector<T, 4>{b[0], b[1], b[2], 1.f});
    return MTVector3T<T>{result[0], result[1], result[2]};
}

using MTMatrix1x1 = MTMatrix<float, 1>;
using MTMatrix2x2 = MTMatrix<float, 2>;
using MTMatrix3x3 = MTMatrix<float, 3>;
using MTMatrix4x4 = MTMatrix4x4T<float>;

#endif // MTMATRIX_H
