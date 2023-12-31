#include "common.h"

extern int keyboard[MAX_KEYBOARD_KEYS];
extern bool quit ;
extern Uint32 fps_current,fps_lasttime; 

extern const char *g_vshader_src;
extern const char *g_fshader_src;
extern const char *g_vshader_font_src;
extern const char *g_fshader_font_src;
extern const char *g_vshader_test_src;
extern const char *g_fshader_test_src;
extern const char *g_vshader_post_src;
extern const char *g_fshader_post_src;
extern const char *g_vshader_fx_src;
extern const char *g_fshader_fx_src;

SDL_GLContext *g_ctx = NULL;
SDL_Window* window = NULL;
int SCREEN_W,SCREEN_H;

RGBColorf col[10]={
{0.0,0.0,0.0},//0 black
{1.0,1.0,1.0},//1 white
{1.0,0.0,0.0},//2 red
{0.0,1.0,0.0},//3 green
{0.0,0.0,1.0},//4 blue
{1.0,1.0,0.0},//5 yellow
{0.0,1.0,1.0},//6 cyan
{1.0,0.0,1.0},//7 magenta
{1.0,0.49,0.0},//8 orange
{0.57,0.43,0.68},//9 violet
};

//sprite 2D
Matrix proj;
GLuint sprite,spr;
GLuint vao,vbo,programId;
GLint u_col,u_model,u_proj,u_tex;

//font 2D bitmap 
GLuint fvao,fvbo,programfontId;   
GLint u_ftex,u_fcol;   

//test starfield (using dummy vao & gl_VertexID)
GLuint tvao,programtest;
GLint u_time,u_res;     
float stime=0.0f;

//frambuffer render scene in texture & post-fx sinwave or blur shake
GLuint fbo,fbo_texture,rbo_depth;
GLuint vbo_fbo_vertices,pvao;
GLuint programpost,programfx;
GLuint u_ptex,u_fxtex,u_fxtime,u_shake,u_move,u_pres;
float qtime=0.0f,move=0.0f;
int shake =0,framebuf=1,postfx=-1;

void resize_cb(int w, int h) {
	
	glViewport(0, 0, w, h);		
	SCREEN_W=w;
	SCREEN_H=h;
	Spriteinit();	
	
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_W, SCREEN_H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  	glBindTexture(GL_TEXTURE_2D, 0);

  	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
  	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_W, SCREEN_H);
  	glBindRenderbuffer(GL_RENDERBUFFER, 0);	
}

static void create_window(int width, int height) {
	
	int windowFlags;

	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,5);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
   	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
   	
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	
	windowFlags =  SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL;
	
	window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
										
	if (!window)
		 die("Failed to create window: %s", SDL_GetError());

	printf("Create windows size:%dx%d \n",width, height);

	SDL_SetWindowFullscreen( window,0);
	
	g_ctx = SDL_GL_CreateContext(window);
    	SDL_GL_MakeCurrent(window, g_ctx);
            
        if (!initGLExtensions())
	{
		die("Failed to init GL!");
	}
	
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SwapWindow(window); // make apitrace output nicer
    
	resize_cb(width, height);		
}

int  init(){

	printf("init\n");
	
	fps_lasttime = SDL_GetTicks(); 

	proj = IDENTITY_MATRIX;

	SCREEN_W=800;
	SCREEN_H=600;
	
	create_window(SCREEN_W, SCREEN_H);
		
	glGenTextures(1, &sprite);
	loadTextureFromFile("./ressources/test.png",true,sprite);
	
	glGenTextures(1, &spr);
	loadTextureFromFile("./ressources/block.png",false,spr);
	
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        
	programId = compileProgram_default(g_vshader_src,g_fshader_src);
	
	u_model   = glGetUniformLocation(programId, "model");
	u_proj   = glGetUniformLocation(programId, "projection");	
	u_col   = glGetUniformLocation(programId, "spriteColor");	
	u_tex   = glGetUniformLocation(programId, "image");	
   
        glUseProgram(programId);

	proj = orthogonal(0.0f, 800.0f,600.0f ,0.0f);
		
        glUniformMatrix4fv(u_proj, 1, GL_FALSE, &(proj.m[0])); //"projection"       
   	glUniform1i(u_tex, 0);	
   	
   	glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
    
   	Spriteinit();
   	
   	printUniformAndAttribute(programId);
   	
   	programfontId = compileProgram_default(g_vshader_font_src,g_fshader_font_src);

	u_fcol   = glGetUniformLocation(programfontId, "Fontcolor");	
	u_ftex   = glGetUniformLocation(programfontId, "image");	
   	
   	glUseProgram(programfontId);

   	glUniform1i(u_ftex, 0);	
   	
   	glGenVertexArrays(1, &fvao);
        glGenBuffers(1, &fvbo);

    	glBindVertexArray(fvao);
    	glBindBuffer(GL_ARRAY_BUFFER, fvbo);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 4, NULL, GL_DYNAMIC_DRAW);
    	
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindVertexArray(0);
    	
   	printUniformAndAttribute(programfontId);
   	
   	// starfield shader
   	programtest = compileProgram_default(g_vshader_test_src,g_fshader_test_src);
   	
   	u_res   = glGetUniformLocation(programtest, "u_res");	
	u_time  = glGetUniformLocation(programtest, "u_time");	
		
   	glUseProgram(programtest); 
   	  
   	glUniform1f(u_time, stime );
	glUniform2f(u_res,(float)SCREEN_W,(float)SCREEN_H );
		   
	glGenVertexArrays(1, &tvao);
	
        printUniformAndAttribute(programtest);       
   	
   	initFonts();
   	
   	glActiveTexture(GL_TEXTURE0);
  	glGenTextures(1, &fbo_texture);
  	glBindTexture(GL_TEXTURE_2D, fbo_texture);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  //	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_W, SCREEN_H, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  	glBindTexture(GL_TEXTURE_2D, 0);
  	
    	glGenRenderbuffers(1, &rbo_depth);
  	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
  	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_W, SCREEN_H);
  	glBindRenderbuffer(GL_RENDERBUFFER, 0);
  	
  	/* Framebuffer to link everything together */
  	glGenFramebuffers(1, &fbo);
  	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
  	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
  	
  	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    		die("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  	
  	glBindFramebuffer(GL_FRAMEBUFFER, 0);
   	
   	GLfloat fbo_vertices[] = {
    		-1, -1,
    		 1, -1,
    		-1,  1,
     		1,  1,
  	};
  	
  	glGenVertexArrays(1, &pvao);
  	glGenBuffers(1, &vbo_fbo_vertices);
  	glBindVertexArray(pvao);
  	glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
  	glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_vertices), fbo_vertices, GL_STATIC_DRAW);
  	
  	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    	glBindBuffer(GL_ARRAY_BUFFER, 0);
    	glBindVertexArray(0);  
  
  	programpost = compileProgram_default(g_vshader_post_src,g_fshader_post_src);
  	u_ptex   = glGetUniformLocation(programpost, "fbo_texture");
  	glUseProgram(programpost); 
  	glUniform1i(u_ptex, 0);	
  	
  	programfx = compileProgram_default(g_vshader_fx_src,g_fshader_fx_src);
  	u_fxtex   = glGetUniformLocation(programfx, "fbo_texture");
  	u_fxtime  = glGetUniformLocation(programfx, "time");	
  	u_shake   = glGetUniformLocation(programfx, "shake");
  	u_move    = glGetUniformLocation(programfx, "move");	
  	u_pres    = glGetUniformLocation(programtest, "u_res");	
  		
  	glUseProgram(programfx); 
  	glUniform1i(u_fxtex, 0);
  	glUniform1f(u_fxtime, qtime );
  	glUniform1i(u_shake,shake); 
  	glUniform1f(u_move, move );
  	glUniform2f(u_pres, (float)SCREEN_W,(float)SCREEN_H );
  	
  	glUseProgram(0); 
	return 0;
}

void deinit() {
	
	SDL_GL_MakeCurrent(window, g_ctx);
    	SDL_GL_DeleteContext(g_ctx);

    	g_ctx = NULL;
    	SDL_DestroyWindow(window);    		   
}

void cleanup(){ 

	printf("cleanup\n");
	if (vao)
        	glDeleteVertexArrays(1, &vao);

    	if (vbo)
        	glDeleteBuffers(1, &vbo);
		
    	vao = 0;
    	vbo = 0;
        if (fvao)
        	glDeleteVertexArrays(1, &fvao);

    	if (fvbo)
        	glDeleteBuffers(1, &fvbo);
		
    	fvao = 0;
    	fvbo = 0;
    	
        if(sprite)glDeleteTextures(1, &sprite);
        
        if(spr)glDeleteTextures(1, &spr);
        
        spr=0;
        sprite=0;
        
        if (tvao)
        	glDeleteVertexArrays(1, &tvao);
        tvao = 0;
        
	if (pvao)
        	glDeleteVertexArrays(1, &pvao);
        pvao = 0;
        
        if(programId)glDeleteProgram(programId);
        if(programfontId)glDeleteProgram(programfontId);
        
        if(programtest)glDeleteProgram(programtest);
        if(programpost)glDeleteProgram(programpost);
        if(programfx)glDeleteProgram(programfx);
                
        glDeleteRenderbuffers(1, &rbo_depth);
  	glDeleteTextures(1, &fbo_texture);
  	glDeleteFramebuffers(1, &fbo);
     	glDeleteBuffers(1, &vbo_fbo_vertices);
  
        SpriteDeinit();  
              
        deinitFonts();
        
	deinit();	
}


void inputScene(){

	if ( keyboard[SDL_SCANCODE_ESCAPE] == 1) quit=true;
		
	if ( keyboard[SDL_SCANCODE_TAB] == 1){
		framebuf=-framebuf;
		keyboard[SDL_SCANCODE_TAB] = 0;
	} 
		
	if ( keyboard[SDL_SCANCODE_SPACE] == 1){
		postfx=-postfx;
		keyboard[SDL_SCANCODE_SPACE] = 0;
	} 
	
	if ( keyboard[SDL_SCANCODE_A] == 1){
		printf("shake\n");
		shake=!shake;
		if(shake)qtime = 5.01f;			
		keyboard[SDL_SCANCODE_A] = 0;
	} 
}

void prepareScene(void)
{
	if(framebuf>0)glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDisable(GL_DEPTH_TEST); 
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);	
}

void presentScene(void)
{
	SDL_GL_SwapWindow(window);
}

void draw(){

    glUseProgram(programtest);   
      	glBindVertexArray(fvao);
   	glUniform1f(u_time, (float)stime );
	glUniform2f(u_res,(float)SCREEN_W,(float)SCREEN_H  );		
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    	glBindVertexArray(0);
    
    glUseProgram(0);
    
    static int rot=0;
    static int ystep=0;
    static int pas=1;
    rot++;
    if(rot>360)rot=0;
    for(int i =0;i<10;i++)  			
    SpriteDraw(spr, 0.0f+i*50, (float)ystep ,40.0f,30.0f,0.0f,col[i].r,col[i].g, col[i].b);
    
    SpriteDraw(spr, 200.0f,150.0f,100.0f,50.0f,(float)rot, 1.0f, 1.0f, 1.0f);
    SpriteDraw(sprite, 235.0f,165.0f,30.0f,30.0f,(float)rot, 1.0f, 1.0f, 1.0f);
    SpriteDraw(sprite, 200.0f,200.0f,300.0f,400.0f,45.0f,0.0f, 1.0f, 0.0f);
    
    ystep+=pas;
    if(ystep>100 || ystep<0)pas=-pas;
    
    drawTextScale(16, 16, 255, 255, 255, 0, 1.0f, "fps:%d",fps_current);  
    drawTextScale(SCREEN_W/2,16, 255, 255,255, 0, 1.0f, "framebuffer:%s postfx:%s shakefx:%s",framebuf>0?"on":"off",postfx>0?"on":"off",shake>0?"on":"off");       
}

void drawpost(){

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);

	if(postfx>0){
		// add post effect blur shake or sinwave
		glUseProgram(programfx);
		glUniform1i(u_shake,shake);
		glUniform1f(u_fxtime, qtime );
		glUniform2f(u_res,(float)SCREEN_W,(float)SCREEN_H  );	
		glUniform1f(u_move, move );
		if (qtime > 0.0f)
    		{
        		qtime -= (float)FPS60*10;
        		if (qtime <= 0.0f)
            			shake=0;
    		} 
	}
	else glUseProgram(programpost); //render fbo_texture without effect
		
	glBindVertexArray(pvao);
        glBindTexture(GL_TEXTURE_2D, fbo_texture);	
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);        
}

void doScene() {

 	inputScene();
 
   	stime+=FPS60;
	move+=FPS60; 
	 
	prepareScene();		
	draw();
	if(framebuf>0)drawpost();
	presentScene();
}

