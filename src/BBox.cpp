#include "BBox.h"

bool BBox::hit(const Ray& r, float tmin, float tmax) const
{
	for(int i = 0; i < 3; i++)
	{
		float a = 1.0f / r.direction()[i];

		float ti_min = (_min[i] - r.origin()[i]) * a;
		float ti_max = (_max[i] - r.origin()[i]) * a;

		if(a < 0.0f)
		{
			std::swap(ti_min, ti_max);
		}

		tmin = ti_min > tmin ? ti_min : tmin;
		tmax = ti_max < tmax ? ti_max : tmax;

		if(tmin >= tmax)
		{
                	return false;
                }

        }

	return true;
}

BBox surrounding_box(const BBox& b0, const BBox& b1) 
{ 
        Vec3 small(std::min(b0.min().x(), b1.min().x()),  
                        std::min(b0.min().y(), b1.min().y()), 
                        std::min(b0.min().z(), b1.min().z())); 
 
        Vec3 big(std::max(b0.max().x(), b1.max().x()), 
                        std::max(b0.max().y(), b1.max().y()), 
                        std::max(b0.max().z(), b1.max().z())); 
 
        return BBox(small, big); 
}
