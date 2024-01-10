#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3D.h"
#include "Ray.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Camera
{
public:
    Camera(Vector3D eye, Vector3D target, Vector3D up, float fov, float aspect_ratio)
    {
        float theta =  fov * M_PI / 180.0;
        float h = tan(theta/2);
        m_ndc_height = 2.0 * h;
        m_ndc_width = aspect_ratio * m_ndc_height;
        
        m_w = normalize(eye - target);
        m_u = normalize(cross(up, m_w));
        m_v = cross(m_w, m_u);
        m_eye = eye;
    }
    
    Ray generate_ray(float col, float row)
    {
        Vector3D direction = (col - 0.5)*m_ndc_width * m_u + (row-0.5) * m_ndc_height * m_v - m_w;
        return Ray(m_eye, direction);
    }

private:
    Vector3D m_eye;
    float m_ndc_width, m_ndc_height;
    Vector3D m_u, m_v, m_w;
};

#endif
