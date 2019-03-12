# Programming Assignment 2

In this assignment your task is to add the following features to your ray tracer:

   - A chosen acceleration structure (can be one of grid, octree, kdtree, or BVH: one of the latter two is strongly encourated)

   - Multisampling

   - Various effects related to multisampling, known as distribution ray tracing (depth of field, soft shadows, glossy reflections). I've attached a paper that introduced the idea of distribution ray tracing/

Please investigate the XML files to observe the new tags that you need to process.

Note that we are now starting to process objects in PLY format. PLY is a commonly used format for rendering meshes. See the following resource to learn more about PLY and how to parse it: http://paulbourke.net/dataformats/ply/

You will see that the mesh element now has a shading attribute, whose value can be "smooth", which means smooth shading. In smooth shading, the idea is to interpolate the surface normal across a primitive instead of using the same normal for all points on the primitive. This is done as follows:

1) For each vertex, keep a list of triangles that share this vertex.

2) Compute the normal of the vertex as the average surface normal of the triangles in this list. This means that the surface normals  at three vertices of a triangle can be different.

3) When a ray hits a point on a triangle, compute the surface normal of this point as the barycentric average of the three normals at the vertices.

Finally a correction. In the class, I remember writing on the board the following formula for computing the "t" value of the ray that passes through the image sample "p" and the center of the lens "o":

t = (-focusDistance) / (p - o).z

However, this formula is not correct if the camera is not looking towards the negative-z direction. The correct formula should be:

t = (-focusDistance) / w.dot(p - o);

where "w" is a unit vector along the negative gaze direction.

Good luck.
