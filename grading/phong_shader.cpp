#include "light.h"
#include "object.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = world.ambient_color * world.ambient_intensity * color_ambient; // L * R, R = color_ambient, L = color intensity = ambient_color * ambient_intensity
    
    for (auto light_n : world.lights) { 
        vec3 L = light_n->position - intersection_point;    // light vector from point to light
        Ray lray(intersection_point, L);
        if (world.enable_shadows) {
            if (!(world.Closest_Intersection(lray).dist >= L.magnitude())) {    // if the hit is detected within where the light is visible
                continue;
            }
        }

        vec3 normL = L.normalized();
        color += light_n->Emitted_Light(L) * color_diffuse * std::max(dot(normal, normL), 0.0);

        vec3 reflection = ((dot(normal, normL) * 2) * normal - L.normalized()).normalized(); 
        // vec3 reflection = -1.0 * L.normalized();
        color += light_n->Emitted_Light(L) * color_specular * std::pow(std::max(dot(ray.direction * -1.0, reflection), 0.0), specular_power);
    }
    // TODO; //determine the color
    return color;
}
