## Programming Assignment 6

In this assignment you are expected to add path tracing (also known as global illumination) support to your ray tracers. Additionally you should implement object lights and environment maps.

Object lights and environment maps

We introduce two new XML elements: LightMesh and LightSphere. These have all the properties that regular objects have. Additionally they contain a "Radiance" element, which specifies the radiance emitted from them in all directions. Because a single radiance value is specified these lights would appear equally bright to an observer from all viewing directions.

When rendering with these lights, you should sample a point on the light surface. You should then divide the radiance of this light with the probability of sampling this direction (you must convert a surface based pdf to a solid angle based pdf). This is very similar to the AreaLight element that we supported before except that these lights are actual objects and hence visible.

As for the environments maps, you can consider them to be textures mapped to an infinitely far away sphere and providing illumination for the scene. The XML element name is: SphericalDirectionalLight. When sampling these lights, you can first generate a random direction, w_i. You can then convert the x, y, z components of w_i to \theta and \phi coordinates. Finally, you can make texture lookup by converting these coordinates to u, v values. Do not forget to divide the radiance by (1 / 2pi) as this is the probability of generating a random direction vector.

Path tracing

In the path tracing part, we send indirect illumination rays for every primary ray that hits an intersection point. For each sample, only one ray is sent. These rays are recursively traced into the scene and again new rays are sent (until MaxRecursionDepth is reached) when they intersect other objects. Whenever we send a ray to sample indirect illumination we have two options: send this ray in a completely random manner or send it by prioritizing certain directions. In either case, we must normalize the returning radiance by the pdf (probability density function) of the distribution that we use to sample the directions.

In pure path tracing, we do not try to send rays to light sources. Some rays hit the light sources and some don't. This generally produces very noisy images even at high sample rates. An alternative technique is known as "NextEventEstimation". In this case, at every intersection point we send two illumination rays: one to the light source and one in a random direction. If we have multiple light sources we can send a ray to each light source separately. The important thing here is to make sure that indirect illumination rays do not also hit the light sources. Otherwise one or more lights will have been counted twice, introducing bias. To prevent this from happening, we return zero radiance if an indirect ray hits a light source.

A second technique in patch tracing is known as "RussianRoulette", in which randomness is used to terminate rays instead of using a hard threshold such as maximum recursion depth. At every intersection point, we check if we should spawn a new ray. A sensible approach is to use the "potential" of a ray to make a contribution. Initially all rays have a potential of (1, 1, 1). When they bounce from a surface their potential decreases due to absorption by the surface. For example after bouncing off a surface with a reflectance of (0.2, 0.4, 0.6), its potential will be reduced by these factors. After multiple reflections the ray's potential may drop to almost negligible levels. We can use the maximum of the RGB channel's potential to decide if we should terminate this path.

More precisely, we generate a uniform random number "q" in range [0, 1]. If q > maxPotential we terminate the path. This means that the path continues with a probability of "maxPotential". If the above check fails we send a new ray and multiply the returned radiance by "1 / maxPotential". In a statistical sense, this balances out the energy lost due to killed rays. Russian Roulette technique does not reduce variance, so do not expect less noisy images. But it provides a unbiased way of terminating rays.

Finally, an optimized sampling strategy known as "ImportanceSampling" can be used to reduce variance. In this technique, instead of sending rays in a completely random manner, we prioritize certain directions while still being random. In this assignment importance sampling means sending indirect illumination rays with a probability that is proportional to the cosine of the angle with respect to the surface normal. As always we must normalize the returned radiance with this pdf to prevent bias.

Inputs

I've prepared several types of input files for you to test your path tracers. You can download them at: https://drive.google.com/open?id=1ePmZWYml5gTht_7KsAJKV_eZyBLIZJ7H

directLighting: Various input files demonstrating object lights

pathTracing: A simple Cornellbox scene for testing different path tracing parameters

sponza: A high polygon model of the Sponza Palace (in Dubrovnik) to show the difference between direct lighting and global illumination (my path tracer took 21 hours to render this scene)

veach-ajar: A good test case for global illumination algorithms as almost none of the objects here receive direct lighting. All the illumination is due to indirect lighting. Noise is still present at 10000 samples per pixel and my path tracer took 3 days (71 hours to be precise) to render this beast.
