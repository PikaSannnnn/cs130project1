#include <limits>
#include "box.h"

// Return whether the ray intersects this box.
bool Box::Intersection(const Ray& ray) const
{
    
    TODO;
    return true;
}

// Compute the smallest box that contains both *this and bb.
Box Box::Union(const Box& bb) const
{
    Box box;
    
    for (int i = 0; i < 3; i++) {
        // getting lowest components
        if (bb.lo[i] < this->lo[i]) {
            box.lo[i] = bb.lo[i];
        }
        else {
            box.lo[i] = this->lo[i];
        } 

        // getting highest components
        if (bb.hi[i] > this->hi[i]) {
            box.hi[i] = bb.hi[i];
        }
        else {
            box.hi[i] = this->hi[i];
        }
    }

    return box;
}

// Enlarge this box (if necessary) so that pt also lies inside it.
void Box::Include_Point(const vec3& pt)
{
    if (pt[0] < lo[0] && pt[1] < lo[1] && pt[2] < lo[2]) {  // if pt is less than the low point, enlarge lower end
        lo = pt;    // enlarge by assigning new pt as the new low
    }
    else if (pt[0] > lo[0] && pt[1] > lo[1] && pt[2] > lo[2]) { // if pt is greater than high point, enlarge upper end
        hi = pt;    // enlarge by assigning new pt as the new high
    }
    // else point already exists in box
}

// Create a box to which points can be correctly added using Include_Point.
void Box::Make_Empty()
{
    lo.fill(std::numeric_limits<double>::infinity());
    hi=-lo;
}

// Create a box that contains everything.
void Box::Make_Full()
{
    hi.fill(std::numeric_limits<double>::infinity());
    lo=-hi;
}
