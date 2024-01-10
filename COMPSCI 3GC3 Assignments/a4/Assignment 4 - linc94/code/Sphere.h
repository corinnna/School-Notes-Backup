#ifndef SPHERE_H
#define SPHERE_H

#include <memory>

using namespace std;
class Material;

class HitResult {
public:
    HitResult() { m_isHit = false; };
    bool m_isHit;
    Vector3D m_hitPos;
    Vector3D m_hitNormal;
    shared_ptr<Material> m_hitMaterial;
    float m_t;
};


class Sphere {
    
public:
    Sphere() {}
    Sphere(Vector3D center, float r, shared_ptr<Material> m)
    {
        m_center = center;
        m_radius = r;
        m_pMaterial = m;
    }
    HitResult hit(Ray& r, float min_t, float max_t);

    public:
    Vector3D m_center;
    float m_radius;
    shared_ptr<Material> m_pMaterial;
};
// TODO 2

//test if ray hits this sphere within range min_t and max_t
HitResult Sphere::hit(Ray& ray, float min_t, float max_t)
{   
    // initialize hit_result
    HitResult hit_result;

    // using the formula/algorithm found in slides

    // o - c
    Vector3D oc = ray.origin() - m_center;

    // d * (o-c)
    float half_b = dot(oc, ray.direction());

    // d * d
    float a = ray.direction().length_squared();

    // (o - c) * (o - c) - R^2
    float c = oc.length_squared() - m_radius * m_radius;

    // calculate discriminant (b^2 - 4ac)
    float discriminant = half_b * half_b - a * c;

    // if the discriminant is <0 there are no solutions (i.e. no collisions/hits)
    // if the discriminant is =0 there is one solution
    // if the discriminant is >0 there are 2 solutions
    if (discriminant < 0.0) {
        // no hits found
        hit_result.m_isHit = false;
        return hit_result;
    }

    // calculate possible solutions
    float t1 = (-half_b - sqrt(discriminant)) / a;
    float t2 = (-half_b + sqrt(discriminant)) / a;

    // check that solutions are within our t range
    if ((t1 <= min_t || t1 >= max_t) && (t2 <= min_t || t2 >= max_t)) {
        // if they arent within our range there might as well be no hit
        hit_result.m_isHit = false;
        return hit_result;
    }

    // we found a hit!
    hit_result.m_isHit = true;

    // if t1 is within our range, it is our t (it will always be smaller than t2 bc it is minus)
    if (t1 >= min_t && t1 <= max_t) {
        hit_result.m_t = t1;
    }

    // otherwise t2 is our hit
    else if (t2 >= min_t || t2 <= max_t) {
        hit_result.m_t = t2;
    }

    // set remaining values
    hit_result.m_hitPos = ray.at(hit_result.m_t);
    hit_result.m_hitNormal = (hit_result.m_hitPos - m_center) / m_radius;
    hit_result.m_hitMaterial = m_pMaterial;
    
    return hit_result;
}

#endif
