#include "plane.h"
#include "hit.h"
#include "ray.h"
#include <cfloat>
#include <limits>

// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
Hit Plane::Intersection(const Ray& ray, int part) const
{
    double t = 0.0, denom = 0.0;
    Hit intersection = {0,0,0};

    t = dot(x1 - ray.endpoint, normal);
    denom = dot(ray.direction, normal);
    if (denom == 0) {
        return {0,0,0};
    }
    t /= denom;

    if (t > small_t) {
        intersection = {this, t, part};
    }

    return intersection;
}

vec3 Plane::Normal(const vec3& point, int part) const
{
    return normal;
}

// There is not a good answer for the bounding box of an infinite object.
// The safe thing to do is to return a box that contains everything.
Box Plane::Bounding_Box(int part) const
{
    Box b;
    b.hi.fill(std::numeric_limits<double>::max());
    b.lo=-b.hi;
    return b;
}

