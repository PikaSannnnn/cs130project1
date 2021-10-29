#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    Hit intersection = {0,0,0};
    double t1 = 0.0, t2 = 0.0;

    vec3 u = ray.direction;
    vec3 e = ray.endpoint;

    double a, b, c;

    a = u.magnitude_squared();
    // b = dot((u * 2), (e - this->center));
    b = 2 * dot(u, (e - this->center));
    c = dot((e - this->center), (e - this->center)) - std::pow(this->radius, 2);

    double determin = std::pow(b, 2) - (4 * a * c);
    if (determin < 0) {
        return {0,0,0};
    }

    t1 = (-1 * b) + std::sqrt(determin);
    t2 = (-1 * b) - std::sqrt(determin);

    t1 /= (2 * a);
    t2 /= (2 * a);

    if (t1 > small_t && t2 > t1) {  // t is distance, t cannot be negative!
        intersection = {this, t1, part};
    }
    else if (t2 > small_t && t1 > t2) {
        intersection = {this, t2, part};
    }

    return intersection;
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal = point - center;
    normal /= radius;
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}

