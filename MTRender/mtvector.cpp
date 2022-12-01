/****************************************
*  WebSite:  https://github.com/mtyxb/
*  Email:       mtyxb@qq.com
****************************************/

#include <algorithm>
#include <math.h>
#include "mtvector.h"

template <typename T, int N>
MTVector<T, N>::MTVector()
{
   std::fill_n(reinterpret_cast<T*>(values), N, T(0));
}

template <typename T, int N>
MTVector<T, N>::MTVector(const std::initializer_list<T>& list)
{
    int n = std::min((int)list.size(), N);
    std::copy_n(list.begin(),
                n,
                reinterpret_cast<T*>(values));
    if (n < N)
    {
     std::fill_n(values + n, N - n, T(0));
    }
}

template <typename T, int N>
QString MTVector<T, N>::to_string() const
{
    QString result;
    result.append("{");
    for (int i = 0; i < N; i++)
    {
       result.append(QString("%1").arg(values[i]));
       if (i + 1 < N)
       {
           result.append(",");
       }
   }
    result.append("}");
    return result;
}

template <typename T, int N>
const T* MTVector<T, N>::data() const
{
    return reinterpret_cast<const T*>(values);
}

template <typename T, int N>
T* MTVector<T, N>::data()
{
    return reinterpret_cast<T*>(values);
}

template <typename T, int N>
T MTVector<T, N>::operator[](const int index) const
{
    return values[index];
}

template <typename T, int N>
T& MTVector<T, N>::operator[](const int index)
{
    return values[index];
}

template <typename T, int N>
T MTVector<T, N>::length() const
{
    T sum = T(0);
    for (int i = 0; i < N; i++)
    {
        sum += values[i] * values[i];
    }
    return std::sqrt(sum);
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::normalize()
{
    T len = length();
    if (T(0) == len)
    {
        return *this;
    }
    for (int i = 0; i < N; i++)
    {
        values[i] /= len;
    }
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::divide_by_w()
{
    if constexpr (4 == N)
    {
        if (1.0f != values[3] && 0.0f != values[3])
        {
            for (int i = 0; i < 3; i++)
            {
                values[i] /= values[3];
            }
            values[3] = 1.f;
        }
    }
    return *this;
}

template <typename T, int N>
MTVector<T, N> MTVector<T, N>::normal() const
{
    MTVector<T, N> result = *this;
    return result.normalize();
}


template <typename T, int N>
T MTVector<T, N>::dot(const MTVector<T, N>& other) const
{
    T result = T(0);
    for (int i = 0; i < N; i++)
    {
        result += values[i] * other[i];
    }
    return result;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator+=(const T& other)
{
    std::transform(values, values + N,
                          values,
                          [other](const T& a) {
            return a + other;
    });
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator-=(const T& other)
{
    std::transform(values, values + N,
                          values,
                          [other](const T& a) {
            return a - other;
    });
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator*=(const T& other)
{
    std::transform(values, values + N,
                          values,
                          [other](const T& a) {
            return a * other;
    });
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator/=(const T& other)
{
    std::transform(values, values + N,
                          values,
                          [other](const T& a) {
            if (T(0) == other)
            {
                return std::numeric_limits<T>::max();
            }
            return a / other;
    });
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator+=(const MTVector<T, N>& other)
{
     std::transform(values, values + N,
                    other.data(),
                    values,
                    [](const T& a, const T& b) {
                        return a + b;
                    });
     return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator-=(const MTVector<T, N>& other)
{
    std::transform(values, values + N,
                   other.data(),
                   values,
                   [](const T& a, const T& b) {
                       return a - b;
                   });
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator*=(const MTVector<T, N>& other)
{
    std::transform(values, values + N,
                   other.data(),
                   values,
                   [](const T& a, const T& b) {
                       return a * b;
                   });
    return *this;
}

template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::operator/=(const MTVector<T, N>& other)
{
    std::transform(values, values + N,
                   other.data(),
                   values,
                   [](const T& a, const T& b) {
                      if (T(0) == b)
                      {
                          return std::numeric_limits<T>::max();
                      }
                      return a / b;
                   });
    return *this;
}


template <typename T, int N>
MTVector<T, N>& MTVector<T, N>::simplify()
{
    if constexpr (std::is_same<T, float>::value || std::is_same<T, double>::value)
    {
        constexpr double DELTA = 10000.;
        for (int i = 0; i < N; i++)
        {
            if (std::fabs((double)values[i]) * DELTA < 1.0)
            {
                values[i] = T(0);
            }
        }
    }
    return *this;
}

////////////////////////////////////////////////////

template <typename T>
MTVector3T<T>::MTVector3T()
    : MTVector<T, 3>()
{
}

template <typename T>
MTVector3T<T>::MTVector3T(const MTVector<T, 3>& other)
{
    MTVector3T<T>& ref = *this;
    ref[0] = other[0];
    ref[1] = other[1];
    ref[2] = other[2];
}

template <typename T>
MTVector3T<T>::MTVector3T(T x, T y, T z)
    : MTVector3T<T>{x, y, z}
{
}

template <typename T>
MTVector3T<T>::MTVector3T(const std::initializer_list<T>& list)
    : MTVector<T, 3> (list)
{
}

template<typename T>
MTVector3T<T> MTVector3T<T>::cross(const MTVector3T& other) const
{
    const T* values = this->data();
    enum {x, y, z};
    return MTVector3T<T>{values[y] * other[z] - values[z] * other[y],
                      values[z] * other[x] - values[x] * other[z],
                      values[x] * other[y] - values[y] * other[x]};
}


/////////////////////////////////////////////////

template class MTVector<float, 1>;
template class MTVector<float, 2>;
template class MTVector<float, 3>;
template class MTVector<float, 4>;
template class MTVector3T<float>;
