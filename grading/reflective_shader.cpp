#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
    
    if (recursion_depth >= world.recursion_depth_limit) {
        color = this->shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
        return color * (1 - reflectivity);  // 1 - reflectivity to decrease brightness at end
    }
    
    vec3 color_o = this->shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);   // obj surface color
    vec3 r = ((dot(normal, -ray.direction) * normal) * 2 + ray.direction);  // reflective vector to obj
   
    Ray objRay(intersection_point, r);  // ray from intersection to obj
    vec3 color_r = world.Cast_Ray(objRay, recursion_depth + 1); // cast ray for reflection color

    color = color_o + (this->reflectivity * (color_r - color_o));
    return color;
}
