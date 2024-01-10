#ifndef RAY_H
#define RAY_H

#include "Vector3D.h"


class Ray
{  
public:
    Vector3D m_origin;
    Vector3D m_direction;
    
    Ray(){}
    Ray(Vector3D& origin, Vector3D& direction)
    {
        m_origin = origin;
        m_direction = direction;
    }
    Vector3D origin()
    {
        return m_origin;
    }
    Vector3D direction()
    {
        return m_direction;
    }
    Vector3D at(float t)
    {
        return m_origin + t * m_direction;
    }
};

#endif
