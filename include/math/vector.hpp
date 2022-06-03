#pragma once

#include <cstddef>
#include <cmath>

#include "matrix.hpp"

template <typename T> class vec_t {
    private:
        T m_buffer[4];

    public:
        T * get_buffer() const {
            return m_buffer;
        }
        
        // subscript operators
        T& operator[](std::size_t index) {
            return m_buffer[index];
        }

        const T& operator[](std::size_t index) const {
            return m_buffer[index];
        }

        void set(T x, T y, T z, T w) {
            m_buffer[0] = x;
            m_buffer[1] = y;
            m_buffer[2] = z;
            m_buffer[3] = w;
        }

        void set(T x, T y, T z) {
            m_buffer[0] = x;
            m_buffer[1] = y;
            m_buffer[2] = z;
        }

        vec_t(T x, T y, T z, T w) : m_buffer{x, y, z, w} { }
        vec_t(T x, T y, T z) : m_buffer{x, y, z} { }
        vec_t(T value) : m_buffer{value, value, value} { }

        // math functions
        T length() const {
            return sqrt(dot(*this));
        }

        T dot(const vec_t& v) const {
            return m_buffer[0] * v[0] + m_buffer[1] * v[1] + m_buffer[2] * v[2];
        }

        void normalize() {
            const T len = length();
            m_buffer[0] = m_buffer[0] / len;
            m_buffer[1] = m_buffer[1] / len;
            m_buffer[2] = m_buffer[2] / len;
            m_buffer[3] = m_buffer[3] / len;
        }

        void perspective_divide() {
            m_buffer[0] = m_buffer[0] / m_buffer[3];
            m_buffer[1] = m_buffer[1] / m_buffer[3];
            m_buffer[2] = m_buffer[2] / m_buffer[3];
        }
};

template <typename T> vec_t<T> operator*(const mat_t<T>& t, const vec_t<T>& v) {
    return vec_t<T>(
        t[0]*v[0]+t[1]*v[1]+t[2]*v[2]+t[3]*v[3],
        t[4]*v[0]+t[5]*v[1]+t[6]*v[2]+t[7]*v[3],
        t[8]*v[0]+t[9]*v[1]+t[10]*v[2]+t[11]*v[3],
        t[12]*v[0]+t[13]*v[1]+t[14]*v[2]+t[15]*v[3]
    );
}

template<typename T> vec_t<T> cross(const vec_t<T>& a, const vec_t<T>& b) {
    return vec_t<T>(
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    );
}

template<typename T> vec_t<T> operator+(const vec_t<T>& a, const vec_t<T>& b) {
    return vec_t<T>(a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]);
}

template<typename T> vec_t<T> operator-(const vec_t<T>& a, const vec_t<T>& b) {
    return vec_t<T>(a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]);
}

template <typename T> std::ostream& operator<<(std::ostream& out, const vec_t<T>& v) {
    out << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << std::endl;
    return out;
}