## Programming Assignment 5

This assignment focuses on three features:

    Adding directional and spot lights
    Adding HDR rendering and texture support
    Implementing several BRDF models

Directional and spot lights

Directional lights are defined by the following XML elements (the fields should be self-explanatory):

<DirectionalLight id="1">
            <Direction>1 -0.8 -1</Direction>
            <Radiance>200 200 200</Radiance>
</DirectionalLight>

Spotlights, on the other hand, are defined by the following:

<SpotLight id="1">
            <Position>-0.93 1 0.9</Position>
            <Direction>1 -1 -1</Direction>
            <Intensity>600 600 600</Intensity>
            <CoverageAngle>10</CoverageAngle>
            <FalloffAngle>8</FalloffAngle>
</SpotLight>

Note that a spot light is just like a point light. It is defined by its intensity and therefore to compute radiance at a particular distance its intensity should be attenuated using the inverse square law (same as a point light). The difference is that a spot light has an emission direction indicated by its Direction element. The light emits with full power around this direction until FalloffAngle degrees defined symetrically around this direction. Then between FalloffAngle and CoverageAngle the light's power drops in a nonlinear fashion. The following psuedocode can be used for this effect:

real cosP = -(l.dot(dir)); // negate cos as l is toward the light source
real fallOffScale;

if (cosP < cosTotalCoverage)
{
    fallOffScale = 0;
}
else if (cosP >= cosFalloffStart)
{
    fallOffScale = 1;
}
else
{
    real d = (cosP - cosTotalCoverage) / (cosFalloffStart - cosTotalCoverage);
    fallOffScale = d * d * d * d;
}

HDR Support

High dynamic range (HDR) images represent the light distribution in a real-world in linear scene units as opposed to display units typically defined as 24-bits per color channel. Therefore, HDR images have no range and bit-depth restrictions. In practice, the floating point data type is commonly used to represent HDR data. HDR images are sometimes normalized to be in [0, 1] range but this is not a requirement. They can contain very small values (e.g. 1e-6) as well as very large values (e.g. 1e6).

In our ray tracers, we will add HDR support in two places: 1) The final image can be rendered in HDR format and 2) Input textures can be in HDR format. A very commonly used HDR format is the ".exr" format and read/write support for it is widely available.

When saving your image in HDR format you should not apply any clamping and/or gamma correction. But because HDR images cannot be directly displayed on standard monitors, a necessary operation in the HDR pipeline is tone mapping. Tone mapping maps the values from an arbitrary input range (float) to a display referred range ([0, 255], 24-bits per pixel). There are many tone mapping operators available in the literature. For this assignment we will use a simple yet powerful operator known as the photographic tone mapping operator (reference attached). The original author provides a "C" source code (https://www.cs.utah.edu/~reinhard/cdrom/source.html) for this operator so you do not have to implement it yourself (however, if you only implement its "global" version it is fairly simple to do so).

Now imagine that you computed luminance (Y) from an RGB HDR image for tone mapping. Also assume the Y' = f(Y) represents your tone mapping function.  The final RGB colors after tone mapping are computed as:

R' = Y' * (R / Y)^s

G' = Y' * (G / Y)^s

B' = Y' * (B / Y)^s

where "s" represents saturation. These R'G'B' values must be clamped to [0, 1], gamma corrected, and then scaled to [0, 255] range before saving them in the PNG file.

Using HDR images as texture is essentially identical to using regular images as texture. No extra elements are introduced for HDR textures.
BRDF Support

Bidirectional reflectance distribution functions model the interaction of the light with objects at the surface level. In this assignment you are expected to implement several simple models along with a physically based surface reflection model (Torrance-Sparrow). To help you in this part of this assignment, I've prepared a summary document that explains these models as succinctly as possible. I've also included several papers in case you want to dig deeper in this subject.

Good luck.
