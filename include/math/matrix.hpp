#pragma once

#include <ostream>

template <typename T> class mat_t {
    private:
        T m_buffer[16];

    public:
        mat_t(T a00, T a01, T a02, T a03, 
            T a10, T a11, T a12, T a13, 
            T a20, T a21, T a22, T a23,
            T a30, T a31, T a32, T a33)
            : m_buffer{
                a00, a01, a02, a03,
                a10, a11, a12, a13,
                a20, a21, a22, a23,
                a30, a31, a32, a33
            } { }

        T& operator[](std::size_t index) {
            return m_buffer[index];
        }

        const T& operator[](std::size_t index) const {
            return m_buffer[index];
        }

        const T& get(std::size_t row, std::size_t col) const {
            return m_buffer[row * 4 + col];
        }

        mat_t<T> transponse() const {
            return mat_t<T>(
                m_buffer[0],m_buffer[4],m_buffer[8],m_buffer[12],
                m_buffer[1],m_buffer[5],m_buffer[9],m_buffer[13],
                m_buffer[2],m_buffer[6],m_buffer[10],m_buffer[14],
                m_buffer[3],m_buffer[7],m_buffer[11],m_buffer[15]
            );
        }
};

template<typename T> mat_t<T> operator*(const mat_t<T>& a, const mat_t<T>& b) {
    // faster than loops
    // we just multiply stuff by hand
    return mat_t<T>(
        a[0]*b[0]+a[1]*b[4]+a[2]*b[8]+a[3]*b[12],
        a[0]*b[1]+a[1]*b[5]+a[2]*b[9]+a[3]*b[13],
        a[0]*b[2]+a[1]*b[6]+a[2]*b[10]+a[3]*b[14],
        a[0]*b[3]+a[1]*b[7]+a[2]*b[11]+a[3]*b[15],
        a[4]*b[0]+a[5]*b[4]+a[6]*b[8]+a[7]*b[12],
        a[4]*b[1]+a[5]*b[5]+a[6]*b[9]+a[7]*b[13],
        a[4]*b[2]+a[5]*b[6]+a[6]*b[10]+a[7]*b[14],
        a[4]*b[3]+a[5]*b[7]+a[6]*b[11]+a[7]*b[15],
        a[8]*b[0]+a[9]*b[4]+a[10]*b[8]+a[11]*b[12],
        a[8]*b[1]+a[9]*b[5]+a[10]*b[9]+a[11]*b[13],
        a[8]*b[2]+a[9]*b[6]+a[10]*b[10]+a[11]*b[14],
        a[8]*b[3]+a[9]*b[7]+a[10]*b[11]+a[11]*b[15],
        a[12]*b[0]+a[13]*b[4]+a[14]*b[8]+a[15]*b[12],
        a[12]*b[1]+a[13]*b[5]+a[14]*b[9]+a[15]*b[13],
        a[12]*b[2]+a[13]*b[6]+a[14]*b[10]+a[15]*b[14],
        a[12]*b[3]+a[13]*b[7]+a[14]*b[11]+a[15]*b[15]
    );
}

template<typename T> std::ostream& operator<< (std::ostream& out, const mat_t<T>& a) {
    out << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << std::endl
        << a[4] << " " << a[5] << " " << a[6] << " " << a[7] << std::endl
        << a[8] << " " << a[9] << " " << a[10] << " " << a[11] << std::endl
        << a[12] << " " << a[13] << " " << a[14] << " " << a[15] << std::endl;

    return out;
}