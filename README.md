# Projects 5 & 6: Lights, Camera & Action!

All project handouts can be found [here](https://cs1230.graphics/projects).

## Design
### Lights, Camera
I ported over my `Camera` class from Intersect/Illuminate and modified it to also compute the projection matrix. The camera gets initialized/reinitialized in `Realtime::resizeGL` (change in width/height), `Realtime::sceneChanged` (change in camera data), and `Realtime::settingsChanged` (change in near/far plane).

I used the classes `Cone`, `Cube`, `Cylinder`, and `Sphere` from Lab 8 for generating triangle meshes for all the primitives. In `Realtime::initializeGL`, I set up VBOs and VAOs for each of the primitives (each one has its own VBO and VAO) using `Realtime::setupVBOVAO`. When `Realtime::settingsChanged` gets called, it uses `Realtime::updateVBO` to bind each VBO and modify its data to the new mesh.

`Realtime::paintGL` performs the main rendering loop of declaring global uniforms (including arrays for the lights), then looping over the scene primitives to declare primitive-specific uniforms and then bind and draw their respective VAOs.
### Action!
**The value of the default framebuffer**, `m_defaultFBO`, gets initialized near the beginning of `Realtime::initializeGL`, and can be changed there to resolve any issues where a black screen is displayed when it shouldn't be.

I have a vertex/fragment shader pair (`phong.vert` and `phong.frag`) for computing Phong lighting and drawing to the FBO, and then another vertex/fragment shader pair (`texture.vert` and `texture.frag`) for reading from the FBO texture and drawing to the screen. To add support for point/spot lights, I augmented `phong.frag` to compute lighting effects conditional on a light type passed in as a struct field. `texture.frag` takes boolean uniforms for whether the per-pixel and kernel-based filters should be applied, and then applies the kernel-based filter (I chose **grayscale**) before the per-pixel one (I chose **box blur**) because doing the reverse would require another FBO for a second pass.

I added support for translational and rotational camera movement by adding the methods `Camera::translate` and `Camera::rotate` to the `Camera` class that perform the relevant transformations on the position and look vectors, respectively, which later affect how the view matrix gets recomputed. My translation speed is simply `m_speed = 5.0f` as specified. My horizontal rotation angle increment is `deltaX * M_PI / 3000`, while my vertical rotation angle increment is `deltaY * M_PI / 3000`, and I adjusted the constant factor to achieve a reasonable mouse sensitivity on my machine.

## Known Bugs
### Lights, Camera
N/A
### Action!
N/A

## Extra Credit
### Lights, Camera
N/A
### Action!
N/A
