#include "common.h"

extern GLuint fvao;
extern GLuint fvbo;   
extern GLint u_ftex;
extern GLint u_fcol;  
extern GLuint programfontId;
extern int SCREEN_W,SCREEN_H;

static char drawTextBuffer[MAX_LINE_LENGTH];
GLuint fontTexture;
float UV[FONT_BOXW*FONT_BOXH][4];

void setUVFonts(){

	int i,j,idx=0;	
	float sx,sy;
	
	for(j=0;j<FONT_BOXH;j++){
	
		for(i=0;i<FONT_BOXW;i++){
		
			sx=(float)(i*GLYPH_WIDTH);
			sy=(float)(j*GLYPH_HEIGHT);
			idx=i+j*FONT_BOXW;
			UV[idx][0]=(float)sx/(float)FONT_WIDTH;
			UV[idx][1]=(float)(sx+(float)GLYPH_WIDTH)/(float)FONT_WIDTH;
			UV[idx][2]=(float)sy/(float)FONT_HEIGHT;
			UV[idx][3]=(float)(sy+(float)GLYPH_HEIGHT)/(float)FONT_HEIGHT; 				
		}		
	}
}
	
void initFonts(void)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "init font10x16 texture" );	
	
	SDL_Surface *tmp = NULL;
	unsigned char tmpfont[FONT10X16_WIDTH*FONT10X16_HEIGHT*4];
	
	int i,j,n,ind=0;
	Uint32 *ptr=(Uint32*)tmpfont;
	unsigned char binary[8];
	
	for(j=0;j<FONT10X16_HEIGHT;j++){
	
		for(i=0;i<FONT10X16_WIDTH;i+=8){
		
			unsigned char val=font10x16[ind++];
			
			for(n = 0; n < 8; n++)
			 	binary[7-n] = (val >> n) & 1;
			 	
			for(n = 0; n < 8; n++){	
				if(binary[n] == 1)*ptr=0xffffffff;
				else *ptr=0;	
				ptr++;
			} 						
		}		
	}
	
	tmp = SDL_CreateRGBSurfaceFrom((void *)tmpfont, FONT10X16_WIDTH, FONT10X16_HEIGHT, 32,FONT10X16_WIDTH*4, 0xff000000, 0x00ff0000, 0x0000ff00,0x000000ff);
	
	if(NULL == tmp){
		fprintf(stderr, "Error SDL_CreateRGBSurface tmp: %s", SDL_GetError());	
		goto quit;
	}
		
        glGenTextures(1, &fontTexture);

	if (!fontTexture)
		die("Failed to create the font texture");

	glBindTexture(GL_TEXTURE_2D, fontTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FONT10X16_WIDTH,FONT10X16_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, tmp->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	SDL_FreeSurface(tmp);

	setUVFonts();
	
	quit:

	if (!fontTexture)printf("create font failed\n");
}

void deinitFonts(void)
{
	if (fontTexture)
		glDeleteTextures(1, &fontTexture);
}

void drawTextScale(int x, int y, int r, int g, int b, int align,int scale,char *format, ...)
{
	int i, len, c;

	va_list args;
	
	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf(drawTextBuffer, format, args);
	va_end(args);
	
	len = strlen(drawTextBuffer);
	
	switch (align)
	{
		case TEXT_RIGHT:
			x -= (len * GLYPH_WIDTH);
			break;
			
		case TEXT_CENTER:
			x -= (len * GLYPH_WIDTH) / 2;
			break;
	}

	glUseProgram(programfontId);   
	 	
	glUniform4f(u_fcol, (float)r/255.0f,(float)g/255.0f,(float)b/255.0f,1.0f);
	glUniform1i(u_ftex, 0);
	
	glActiveTexture(GL_TEXTURE0);
   	glBindVertexArray(fvao);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
			
	for (i = 0 ; i < len ; i++)
	{
		c = drawTextBuffer[i];
		
		int cy=c/GLYPH_HEIGHT;
		int cx=c-cy*GLYPH_HEIGHT;
		int idx= cx + cy*FONT_BOXW;				
		
		#define OGL_coord(a,b)  (((float)(a)*2.0f)/(float)(b))
	 
    		float dw = (float)GLYPH_WIDTH*scale;	 
 		float dh = (float)GLYPH_HEIGHT*scale; 
 		
	       	float dx=OGL_coord(x,SCREEN_W)-1.0f;
        	float dy=1.0f-OGL_coord(y+dh,SCREEN_H);
	
        	float odw= OGL_coord(dw,SCREEN_W);
        	float odh= OGL_coord(dh,SCREEN_H);                
  		float rx=dx+odw;
  		float ry=dy+odh;

		float u0=UV[idx][0],u1=UV[idx][1],v0=UV[idx][2],v1=UV[idx][3];

        	float vertices[4][4] = {
        	    { dx, dy, u0, v1 },            
        	    { dx, ry, u0, v0 },
        	    { rx, dy, u1, v1 },
        	    { rx, ry, u1, v0 },          
        	};
	
		glBindBuffer(GL_ARRAY_BUFFER, fvbo);
        	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        	glBindBuffer(GL_ARRAY_BUFFER, 0);
        	
        	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		x += GLYPH_WIDTH*scale;
	}
	
	glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);  
        
	glUseProgram(0);	
}

