## Programming Assignment 4

In this assignment, your goal is to add texture mapping (including bump mapping) support to your ray tracers. Texture mapping is a diverse subject and has many modes of operation. You will be implementing a subset of these modes for sake of simplicity.

Textures will be defined using the following element in the XML file:

<Textures>
        <Texture id="1">
            <ImageName>textures/earth.jpg</ImageName>
            <Interpolation>bilinear</Interpolation>
            <DecalMode>replace_kd</DecalMode>
            <Normalizer>255</Normalizer>
        </Texture>
</Textures>

For ImageName possible values are either the name of a image file for standard texture mapping, perlin for Perlin noise, and checkerboard for a checkerboard pattern. Interpolation can be either nearest or bilinear. calMode can be replace_kd for replacing the diffuse reflectance coefficient, blend_kd for recomputing the diffuse reflectance as an equally weighted average of the original diffuse reflectance and the texture color, and finally replace_all for entirely disabling shading and instead using the texture color. Normalizer is used to normalize the texture value before being used (in the above example all texture values will be divided by 255 before being used as diffuse reflectance coefficients).

For perlin noise and checkerboard textures, the interpolation mode does not make sense as there is no sampling. Perlin noise itself has several parameters as exemplified below:

<Texture id="1" bumpmap="true">
            <ImageName>perlin</ImageName>
            <ScalingFactor>1</ScalingFactor>
            <Appearance>patch</Appearance>
            <DecalMode>replace_kd</DecalMode>
            <Normalizer>1</Normalizer>
</Texture>

The ScalingFactor is used to scale the 3D point at which the noise value is computed. Appearance can be either patch or vein. Remember that the perlin noise value is originally in range [-1,1]. patch means normalizing this value to [0, 1] range using n' = (n + 1) / 2. vein means taking the absolute value n' = abs(n). The DecalMode and Normalizer parameters have the same meaning as in image textures.

Note that in the above example, the texture has a bumpmap attribute. If this attribute exists and set to true, it means this texture should be used for bump mapping. This means that the texture is applied as usual but additionally the "shading normal" is computed by perturbing the "geometry normal" as we learned in the class (two PDF documents explaining this process will also be provided). Another possible attribute is bumpmapMultiplier which serves as a scaling factor for the bump value. The larger this multiplier is the larger the perturbation of the normal will be, but too large values may give unrealistic results.

Note that bump mapping uses a height map as determined by the noise function or an image texture. But because image textures can have R, G, B components, you can convert these to a single grayscale value using L = (R + G+ B)/3. You can perform bump mapping using these L values.

Checkerboard textures are easy to generate. You can simply compute the value at each point using the following pseudocode:

bool x = (int) ((pos.x + offset) * scale) % 2;
bool y = (int) ((pos.y + offset) * scale) % 2;
bool z = (int) ((pos.z + offset) * scale) % 2;

bool xorXY = x != y;

if (xorXY != z)
{   
    return black;
}
else
{
    return white;
}

The extra parameters of checkerboard textures are offset, scale, blackcolor, and whitecolor. An example is given below:

<Texture id="6">
            <ImageName>checkerboard</ImageName>
            <BlackColor>0.4 0.4 0.4</BlackColor>
            <WhiteColor>10 10 10</WhiteColor>
            <ScalingFactor>0.1</ScalingFactor>
            <Offset>79</Offset>
            <DecalMode>blend_kd</DecalMode>
            <Normalizer>1</Normalizer>
</Texture>

You may also note that some scene files have the following extra attributes. These attributes define the suggested epsilon values for shadow rays and intersection tests. You may use these or choose other epsilons that work well for you.

<ShadowRayEpsilon>1e-1</ShadowRayEpsilon>
<IntersectionTestEpsilon>1e-2</IntersectionTestEpsilon>

Finally, I have shared a relatively complex scene file called "veach-ajar" by converting the pbrt version of this file to our XML format. This necessitated addition of a new transformation element called Composite. This element defines the 16 values of a composite transformation matrix in row major order. Please see "scene.xml" under the veach-ajar folder for details.

Good luck.
