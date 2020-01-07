#pragma once

template <typename T> class Vec2D;

template <class T> bool operator== (const Vec2D<T> & p1, const Vec2D<T> & p2) {
    return (p1[0] == p2[0] && p1[1] == p2[1]);
}

template <class T> Vec2D<T> operator+ (const Vec2D<T> & p1, const Vec2D<T> & p2) {
    return Vec2D<T> (p1[0] + p2[0], p1[1] + p2[1]);
}

template <class T> Vec2D<T> operator* (float factor, const Vec2D<T> & p) {
    return Vec2D<T> (p[0] * factor, p[1] * factor);
}

template<typename T>
class Vec2D {
public:
    inline Vec2D (void)	{
        p[0] = p[1] = T();
    }
    inline Vec2D (T p0, T p1) {
        p[0] = p0;
        p[1] = p1;
    };
    inline Vec2D (T* pp) {
        p[0] = pp[0];
        p[1] = pp[1];
    };
    // ---------
    // Operators
    // ---------
    inline T& operator[] (int Index) {
        return (p[Index]);
    };

    inline Vec2D& operator*= (T s) {
        p[0] *= s;
        p[1] *= s;
        return (*this);
    };
    inline const T& operator[] (int Index) const {
        return (p[Index]);
    };

    T p[2];
};

typedef Vec2D<float> Vec2Df;

