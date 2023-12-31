#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Uint32 fps_current=0;
Uint32 fps_frames = 0;
Uint32 fps_lasttime; 

void die(const char *fmt, ...) {
	char buffer[4096];

	va_list va;
	va_start(va, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, va);
	va_end(va);

	fputs(buffer, stderr);
	fputc('\n', stderr);
	fflush(stderr);

	exit(EXIT_FAILURE);
}

void loadTextureFromFile(const char *file, bool alpha,GLuint tex)
{    
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    printf("%d %d %d \n",width, height, nrChannels);
    // now generate texture
    //textureGenerate(width, height, data);
    unsigned int format = GL_RGB;
    if(alpha)format = GL_RGBA;
    
    // create Texture
    glBindTexture(GL_TEXTURE_2D,tex);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
        
    // and finally free image data
    stbi_image_free(data);
}

void doFps(){

		
	Uint32 fps_tmp=SDL_GetTicks();

	fps_frames++;
   	if (fps_lasttime < fps_tmp - FPS_INTERVAL*1000)
   	{
      		fps_lasttime = fps_tmp;
      		fps_current = fps_frames;
      		fps_frames = 0;  			
      		//printf("fps:%d\n",    fps_current);				
   	}  
	
}

