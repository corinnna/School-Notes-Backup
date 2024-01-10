#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

float clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float random_float(/*[0,1)*/)
{
    return rand() / (RAND_MAX + 1.0);
}

float random_float(float min, float max)
{
    return min + (max-min)*random_float();
}

int random_int(int min, int max)
{
    return static_cast<int>(random_float(min, max+1));
}

class Vector3D
{
public:
    Vector3D()
    {
        m_x= 0;
        m_y = 0;
        m_z = 0;
    }
    Vector3D(float x, float y, float z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    float x() const
    {
        return m_x;
    }
    float y() const
    {
        return m_y;
    }
    float z() const
    {
        return m_z;
    }
    
    Vector3D operator-() const
    {
        return Vector3D(-m_x, -m_y, -m_z);
    }
    
    Vector3D& operator+=(const Vector3D &v)
    {
        m_x += v.m_x;
        m_y += v.m_y;
        m_z += v.m_z;
        return *this;
    }
    
    Vector3D& operator*=(const float t)
    {
        m_x *= t;
        m_y *= t;
        m_z *= t;
        return *this;
    }
    
    Vector3D& operator/=(const float t)
    {
        return *this *= 1/t;
    }

    float length() const
    {
        return sqrt(length_squared());
    }

    float length_squared() const
    {
        return m_x*m_x + m_y*m_y + m_z*m_z;
    }

    static Vector3D random() {
        return Vector3D(random_float(), random_float(), random_float());
    }

    static Vector3D random(float min, float max) {
        return Vector3D(random_float(min,max), random_float(min,max), random_float(min,max));
    }
    
public:
    float m_x;
    float m_y;
    float m_z;
};

Vector3D operator+(const Vector3D &u, const Vector3D &v)
{
    return Vector3D(u.m_x + v.m_x, u.m_y + v.m_y, u.m_z + v.m_z);
}

Vector3D operator-(const Vector3D &u, const Vector3D &v)
{
    return Vector3D(u.m_x - v.m_x, u.m_y - v.m_y, u.m_z - v.m_z);
}

Vector3D operator*(const Vector3D &u, const Vector3D &v)
{
    return Vector3D(u.m_x * v.m_x, u.m_y * v.m_y, u.m_z * v.m_z);
}

Vector3D operator*(float t, const Vector3D &v)
{
    return Vector3D(t*v.m_x, t*v.m_y, t*v.m_z);
}

Vector3D operator*(const Vector3D &v, float t)
{
    return t * v;
}

Vector3D operator/(Vector3D v, float t)
{
    return (1/t) * v;
}

float dot(const Vector3D &u, const Vector3D &v)
{
    return u.m_x * v.m_x + u.m_y * v.m_y + u.m_z * v.m_z;
}

Vector3D cross(const Vector3D &u, const Vector3D &v)
{
    return Vector3D(u.m_y * v.m_z- u.m_z * v.m_y,
                u.m_z * v.m_x - u.m_x * v.m_z,
                u.m_x * v.m_y - u.m_y * v.m_x);
}

Vector3D normalize(Vector3D v)
{
    return v / v.length();
}

#endif
