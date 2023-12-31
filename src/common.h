#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include <GL/gl.h>

#include "font10x16.h"	

#include "def.h"

//util
extern void die(const char *fmt, ...);
extern void loadTextureFromFile(const char *file, bool alpha,GLuint tex);
extern void doFps();

//text
extern void initFonts(void);
extern void deinitFonts(void);
extern void drawTextScale(int x, int y, int r, int g, int b, int align,int scale,char *format, ...);

//sprite
extern void SpriteDeinit();
extern void Spriteinit();
extern void SpriteDraw(GLuint texture, float x,float y,float scalex,float scaley, float rotate, float r,float g,float b);

//opengl
extern void cleanup();
extern int  init();
extern void resize_cb(int w, int h);
extern void inputScene();
extern void doScene();

//shader-util
extern GLuint compileShader(const char* source, GLuint shaderType);
extern GLuint compileProgram_default(const char* vtxFile, const char* fragFile) ;
extern void printUniformAndAttribute(GLuint program);

//input
extern int doInput(void);

//matrix
extern const Matrix IDENTITY_MATRIX;
extern void MrotateZ(const Matrix* m, float angle);
extern void Mscale(const Matrix* m, float x, float y, float z) ;
extern void Mtranslate(const Matrix* m, float x, float y, float z);
extern Matrix orthogonal(float left, float right, float bottom, float top);

//glext
extern bool initGLExtensions();
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;



