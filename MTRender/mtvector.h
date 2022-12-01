/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#ifndef MTVECTOR_H
#define MTVECTOR_H

#include <math.h>
#include <initializer_list>
#include <QString>


template <typename T, int N>
class MTVector
{
public:
    using value_type = T;
    static constexpr int COUNT = N;

    MTVector();
    MTVector(const std::initializer_list<T>& list);

    QString to_string() const;

    const T* data() const;
    T* data();

    T operator[](const int index) const;
    T& operator[](const int index);

    T length() const;
    MTVector<T, N> normal() const;
    MTVector<T, N>& normalize();
    MTVector<T, N>& divide_by_w();
    T dot(const MTVector<T, N>& other) const;

    MTVector<T, N>& operator+=(const T& other);
    MTVector<T, N>& operator-=(const T& other);
    MTVector<T, N>& operator*=(const T& other);
    MTVector<T, N>& operator/=(const T& other);

    MTVector<T, N>& operator+=(const MTVector<T, N>& other);
    MTVector<T, N>& operator-=(const MTVector<T, N>& other);
    MTVector<T, N>& operator*=(const MTVector<T, N>& other);
    MTVector<T, N>& operator/=(const MTVector<T, N>& other);

    MTVector<T, N>& simplify();

private:
    T  values[N];
};

template <typename T, int N>
MTVector<T, N> operator+(const MTVector<T, N>& a, const MTVector<T, N>& b)
{
    MTVector<T, N> result = a;
    result += b;
    return result;
}

template <typename T, int N>
MTVector<T, N> operator-(const MTVector<T, N>& a, const MTVector<T, N>& b)
{
    MTVector<T, N> result = a;
    result -= b;
    return result;
}

template <typename T, int N>
MTVector<T, N> operator*(const MTVector<T, N>& a, const MTVector<T, N>& b)
{
    MTVector<T, N> result = a;
    result *= b;
    return result;
}

template <typename T, int N>
MTVector<T, N> operator*(const MTVector<T, N>& a, const T& b)
{
    MTVector<T, N> result = a;
    std::transform(a.data(), a.data() + N, a.data(),
                   [&b](const T& v){
        return v * b;
    });
    return result;
}

template <typename T, int N>
MTVector<T, N> operator/(const MTVector<T, N>& a, const MTVector<T, N>& b)
{
    MTVector<T, N> result = a;
    result /= b;
    return result;
}


template <typename T, int N>
const MTVector<T, N> operator-(const MTVector<T, N>& v)
{
    MTVector<T, N> result;
     std::transform(v.data(), v.data() + N,
                    result.data(),
                    [](const T& a) {
                        return -a;
                    });
     return result;
}


template <typename T, int N>
MTVector<T, N> mt_clamp_vector(const MTVector<T, N>& a, const MTVector<T, N>& b, float t)
{
    MTVector<T, N> tmp = b - a;
    tmp *= t;
    tmp += a;
    return tmp;
}

template <typename T>
class MTVector3T : public MTVector<T, 3>
{
public:
     MTVector3T();
     MTVector3T(const MTVector<T, 3>& other);
     MTVector3T(T x, T y, T z);
     MTVector3T(const std::initializer_list<T>& list);
     MTVector3T cross(const MTVector3T& other) const;
};



using MTVector1 = MTVector<float, 1>;
using MTVector2 = MTVector<float, 2>;
using MTVector3 = MTVector3T<float>;
using MTVector4 = MTVector<float, 4>;

template <typename T>
static bool mt_is_equal(const T& a, const T& b)
{
    constexpr double DELTA = 10000.;
    double x = std::fabs((double)a - (double)b) * DELTA;
    double y = std::min(std::fabs((double)a), std::fabs((double)b));
    return x <= y || x < 1.0;
}

template <typename T>
static bool mt_is_zero(T a)
{
    return mt_is_equal(a, T(0));
}



#endif // MTVECTOR_H
