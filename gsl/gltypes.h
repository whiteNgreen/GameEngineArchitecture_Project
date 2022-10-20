#ifndef GLTYPES_H
#define GLTYPES_H

// We put this test inside this file, instead of having it in each file that needs gl.h
// OpenGL types and declarations on different architectures

//Visual Studio - for some reason it (sometimes) does not like to include gl/GL.h
#ifdef _WIN64
using GLfloat = float;
using GLdouble = double;
using GLint = int;
using GLuint = unsigned int;
using GLenum = unsigned int;

#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009
#else

//minGW on windows
#ifdef _WIN32   //_WIN32 is defined if _WIN64 is defined, so must test _WIN64 first
#include <GL/gl.h>
#endif

#endif //#ifdef _WIN64

#ifdef linux
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <opengl/gl.h>
#endif

#endif // GLTYPES_H
