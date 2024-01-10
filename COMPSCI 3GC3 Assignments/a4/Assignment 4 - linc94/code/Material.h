#ifndef MATERIAL_H
#define MATERIAL_H

class HitResult;

class ReflectResult
{
public:
    Ray m_ray;
    Vector3D m_color;
};

class Material
{
public:
    Vector3D m_color;
    virtual ReflectResult reflect(Ray& ray, HitResult& hit) = 0;
};


class Diffuse : public Material
{
public:
    Diffuse(const Vector3D& color)
    {
        m_color = color;
    };
    
    // TODO 4
    virtual ReflectResult reflect(Ray& ray, HitResult& hit) override
    {
        // initialize resulting ray (with colour)
        ReflectResult res;

        // generate a unit vector that has a random direction (we use 1 and -1 to mimic a unit circle)
        Vector3D randPoint = Vector3D::random(-1, 1);

        // normalize the vector to get its direction
        Vector3D randDir = normalize(randPoint);

        // normalize the hit point's normal to get its direction
        Vector3D hitDir = normalize(hit.m_hitNormal);

        // origin of the resultant ray will be at the hit point
        res.m_ray.m_origin = hit.m_hitPos;

        // direction of the resultant ray is the addition of the hit point's normal direction and random direction
        // normalize it to get its direction
        res.m_ray.m_direction = normalize(randDir + hitDir);

        // colour set based on value passed during object insantiation
        res.m_color = m_color; 

        // return resultant ray
        return res;
    }
};


class Specular : public Material
{
public:
    Specular(const Vector3D& color)
    {
        m_color = color;
    }
    
    // TODO 5
    virtual ReflectResult reflect(Ray& ray, HitResult& hit) override
    {
        // initialize resulting ray (with colour)
        ReflectResult res;
        
        // origin of the resultant ray will be at the hit point
        res.m_ray.m_origin = hit.m_hitPos;

        // we use the original ray's direction to calculate the resultant ray's direction since it is mirrored
        // if we draw a horizontal line across the original ray's direction, the mirrored ray will be the ray reflected across the x-axis
        // the distance from the original ray to the x-axis can be calculated by doing: -dot(original ray direction, original hit point normal)
        // so if we subtract double this, we will get the mirrored ray
        // we multiply it by the normal to give the distance a direction
        // therfore we can do: mirrored ray = original ray direction - 2 * dot(original ray's direction, original hit point normal) * original hit point normal
        res.m_ray.m_direction = ray.direction() - 2 * dot(ray.direction(), normalize(hit.m_hitNormal)) * normalize(hit.m_hitNormal);
        
        // colour set based on value passed during object insantiation
        res.m_color = m_color;

        // return resultant ray
        return res;
    }
};
#endif
