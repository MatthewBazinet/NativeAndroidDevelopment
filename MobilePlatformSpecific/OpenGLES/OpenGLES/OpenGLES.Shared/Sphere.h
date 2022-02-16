#ifndef __HelloSphereNative__main__
#define __HelloSphereNative__main__

#include <stdio.h>
#ifdef __ANDROID__
#include <GLES/gl.h>
#elif __APPLE__
#include <OpenGLES/ES1/gl.h>
#endif

void Sphere_setupGL(double width, double height);
void Sphere_tearDownGL();
void Sphere_update();
void Sphere_prepare();
void Sphere_draw();

#endif /* defined(__HelloSphereNative__main__) */
