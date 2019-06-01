#include "simpleCDF.h"

SimpleCDF::SimpleCDF(const std::vector<Shape*>& meshTriangles, const glm::mat4& trans)
{
	n = meshTriangles.size();
        float areaAcc = 0.0f;

        for(unsigned int i = 0; i < n; i++)
        {
                MeshTriangle *triangle_ptr = dynamic_cast<MeshTriangle*>(meshTriangles[i]);
                if(!triangle_ptr)
                {
                        throw std::runtime_error("Unable to cast downcast Shape* to MeshTriangle*");
                }

                areaAcc += triangle_ptr->area(trans);
                area_data.push_back(areaAcc);
                ptr_data.push_back(triangle_ptr);
        }

        // Divide by whole area, obtain cdf
        this->area = areaAcc;
        for(unsigned int i = 0; i < n; i++) { area_data[i] /= areaAcc; }
}

MeshTriangle* SimpleCDF::query(float key) const
{
	for(unsigned int i = 0; i < n; i++)
	{
		if(key <= area_data[i])
		{
			return ptr_data[i];
		}
	}

	return ptr_data[n];
}
