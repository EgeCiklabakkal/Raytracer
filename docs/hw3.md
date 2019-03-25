# Programming Assignment 3

In this assignment your task is to add transformations and instancing support to your ray tracers. As a related concept, we will also add motion blur as a remaining topic from distribution ray tracing. The XML format is extended to support these features.

Transformations

 First of all, a list of transformations will be defined in the file. They will look like as follows:

<Transformations>
        <Translation id="1">0 -6 0</Translation>
        <Scaling id="1">3 0.1 0.1</Scaling>
        <Rotation id="1">45 0 1 0</Rotation>
</Transformations>

When objects are defined later in the file, they will make reference to these transformations. An example is given below:

<Mesh id="7">
        <Material>6</Material>
        <Faces plyFile="ply/cube.ply" />
        <Transformations>t7 r1 t8 s3 t5</Transformations>
</Mesh>

This means apply t7 first (translation with id 7), followed by r1, t8, s3, and t5. Do not try to be smart about this, just apply the transformations in the given order. Remember that normals are transformed by replacing the upper 3x3 of the transformations matrix with its inverse transpose. Also remember that vectors remain the same under translation (you can do this by using zero as the fourth component).

Instancing

Instancing is used to render multiple copies of the same object without introducing memory overhead. The geometry has a single copy, but the rays intersected with an instance are transformed with the inverse transformation matrix of the instance. The following XML elements are used to define an instance:

<Mesh id="1">
        <Material>1</Material>
        <Transformations>r1 t2</Transformations>
        <Faces plyFile="ply/dragon_remeshed.ply" />
        <MotionBlur>10 10 10</MotionBlur>
</Mesh>

<MeshInstance id="1" baseMeshId="1" resetTransform="true">
        <Material>3</Material>
        <Transformations>r1 t3</Transformations>
</MeshInstance>

Here the attribute "baseMeshId" gives the id of the mesh that contains the original geometry data. "resetTransform" indicates whether the base mesh's transforms should be retained (and therefore the instance's transforms are applied on top of it) or transformations should start scratch from the instance.

Motion Blur

Motion blur is used to simulate the objects that were in motion during the image capture process. The <MotionBlur> tag shown above defines the velocity vector of the object. This means that the object is between its original world position and the position computed by adding this vector to it. To support this, each ray must be augmented with a random time parameter in range [0, 1].

Vertex Offset

You will see that some meshes have "vertexOffset" attribute. This simply means that the indices defined for the object are relative, the real index is computed by adding this offset to the relative indices. This is used to simplify defining indices for some types of meshes.

Good luck. As always ask in the forum if you need help.
