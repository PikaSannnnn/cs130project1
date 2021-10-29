#include "mesh.h"
#include "plane.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Hit intersection = {0,0,0};
    if (part < 0) { // need to check every individual triangle in mesh
        double closest = std::numeric_limits<double>::max();
        intersection.dist = closest;    // set current distance to "infinite" or no intersection
        // for (auto triangle : triangles) {
        int numtri = triangles.size();
        for (int i = 0; i < numtri; i++) {
            if (Intersect_Triangle(ray, i, closest)) {  // if triangle intersects
                if (closest < intersection.dist) {
                    intersection.part = i;
                    intersection.object = this;
                    intersection.dist = closest;
                }
            }
        }
    }
    else {  // given not -1, return this triangle instead
        if (Intersect_Triangle(ray, part, intersection.dist)) {
            intersection.part = part;
            intersection.object = this;
        }
    }

    return intersection;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);

    // How to get the triangle?
    ivec3 triangle = triangles[part];   // get triangle w/ vertex indices

    // A = 0, B = 1, C = 2, D = 3, ...
    // triangle stores vertex's index
    vec3 a = vertices[triangle[0]];
    vec3 b = vertices[triangle[1]];
    vec3 c = vertices[triangle[2]];

    vec3 n = cross(b - a, c - a);

    return vec3(n.normalized());
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    ivec3 triangle = triangles[tri];
    vec3 a = vertices[triangle[0]];
    vec3 b = vertices[triangle[1]];
    vec3 c = vertices[triangle[2]];

    vec3 p = ray.endpoint + ray.direction * dist;
    Plane tri_plane(a, Normal(a, tri));
    Hit intersection = tri_plane.Intersection(ray, tri);

    if (intersection.object == nullptr) {
        return false;
    }

    // Area(subtriangle) / Area(abc)
    double gamma = cross(b - a, p - a).magnitude() /
                    cross(b - a, c - a).magnitude();
    double alpha = cross(b - p, c - p).magnitude() /
                    cross(b - a, c - a).magnitude();
    double beta = cross(p - a, b - a).magnitude() /
                    cross(b - a, c - a).magnitude();                

    if (gamma > -weight_tol && alpha > -weight_tol && beta > -weight_tol) {
        if (gamma < (1 + weight_tol) && alpha < (1 + weight_tol) && beta < (1 + weight_tol)) {
            dist = intersection.dist;
            return true;
        }
    }

    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
