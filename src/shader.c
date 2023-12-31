
const char *g_vshader_src =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
        "TexCoords = vertex.zw;\n"
	"gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
    "}";

const char *g_fshader_src =
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D image;\n"
    "uniform vec4 spriteColor;\n"
    "void main() {\n"
        "color = vec4(spriteColor.xyz, 1.0) * texture(image, TexCoords);\n"
    "}";

const char *g_vshader_font_src =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n"
    "out vec2 TexCoords;\n"
    "void main() {\n"
        "TexCoords = vertex.zw;\n"
	"gl_Position = vec4(vertex.xy, 0.0, 1.0);\n"
    "}";
    
const char *g_fshader_font_src =
    "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D image;\n"
    "uniform vec4 Fontcolor;\n"
    "void main() {\n"
    	"float alpha = texture(image, TexCoords).a;\n"
        "color = vec4(Fontcolor.xyz, alpha);\n"
    "}"; 
    
  // dummy vshader
const char *g_vshader_test_src =
    "#version 330 core\n"
    "const vec2 quadVertices[4] = vec2[4]( vec2( -1.0, -1.0), vec2( -1.0, 1.0), vec2( 1.0, -1.0), vec2( 1.0, 1.0));\n"
    "void main() {\n"
    	"gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);\n"
    "}"; 
    
// starfield fshader from shadertoys    
const char *g_fshader_test_src =
// starfield fshader from shadertoys
#if 1 
"#version 330 core\n" 
"float rand (in vec2 uv) { return fract(sin(dot(uv,vec2(12.4124,48.4124)))*48512.41241); }\n"
"const vec2 O = vec2(0.,1.);\n"
"float noise (in vec2 uv) {\n"
"	vec2 b = floor(uv);\n"
"	return mix(mix(rand(b),rand(b+O.yx),.5),mix(rand(b+O),rand(b+O.yy),.5),.5);\n"
"}\n"
"\n"
"#define DIR_RIGHT -1.\n"
"#define DIR_LEFT 1.\n"
"#define DIRECTION DIR_LEFT\n"
"\n"
"#define LAYERS 8\n"
"#define SPEED 40.\n"
"#define SIZE 5.\n"
"out vec4 fragColor;\n"
    "uniform float u_time;\n"
    "uniform vec2 u_res;\n"
"\n"
"void main()\n"
"{\n"
"	vec2 uv = gl_FragCoord.xy / u_res.xy;\n"
"   \n"
"    \n"
"    float stars = 0.;\n"
"	float fl, s;\n"
"	for (int layer = 0; layer < LAYERS; layer++) {\n"
"		fl = float(layer);\n"
"		s = (400.-fl*20.);\n"
"		stars += step(.1,pow(noise(mod(vec2(uv.x*s + u_time*SPEED*DIRECTION - fl*100.,uv.y*s),u_res.x)),18.)) * (fl/float(LAYERS));\n"
"	}\n"
"\n"
"	fragColor = vec4( vec3(stars), 1.0 );\n"
"}";
#else
    "#version 330 core\n" 
    "#define LAYERS 8.\n"
    "#define SPEED 40.\n"
    "#define r(u) fract(sin(dot(u,vec2(12.7,48)))*4e5)\n"
    "out vec4 o;\n"
    "uniform float u_time;\n"
    "uniform vec2 u_res;\n"
    "vec2 O = vec2(1,0);\n"
    "vec2 u = gl_FragCoord.xy;\n"
    "float noise ( vec2 b) {\n"
    "	b = ceil(b); \n"
    "	return ( r(b)+r(b+O)+r(b+O.yx)+r(b+1.) )/4.;\n"
    "}\n"
    "void main()\n"
    "{\n"
    "   vec2 R = u_res;\n"
    "	u /= R;\n"
    "   o -= o;\n"
    "	for (float l = 0.; l < LAYERS; l++) \n"
    "		o += l/LAYERS * step(.88, noise(mod (u*(20.-l)*20. + O*(u_time*SPEED - l*100.), R.x)));\n"  
    "}";   
#endif  

const char *g_vshader_post_src =
"#version 330 core\n"
"layout (location = 0) in vec2 v_coord;\n"
"out vec2 TexCoords;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(v_coord, 0.0, 1.0);\n"
"    TexCoords = (v_coord + 1.0) / 2.0;\n"
"}";  


const char *g_fshader_post_src =
"#version 330 core\n"
"out vec4 FragColor;  \n"
"in vec2 TexCoords;\n"
"uniform sampler2D fbo_texture;\n"
"void main()\n"
"{ \n"
"    FragColor = texture(fbo_texture, TexCoords);\n"
"}";


 const char *g_vshader_fx_src =
"#version 330 core\n"
"layout (location = 0) in vec2 v_coord;\n"
"out vec2 TexCoords;\n"
"uniform float time;\n"
"uniform bool shake;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(v_coord, 0.0, 1.0);\n"
"    TexCoords = (v_coord + 1.0) / 2.0;\n"
"	if (shake)\n"
"	{\n"
"	   float strength = 0.01;\n"
"          gl_Position.x += cos(time * 10) * strength;  \n"      
"          gl_Position.y += cos(time * 15) * strength;  \n"
"	}\n"
"}";  

const char *g_fshader_fx_src =
#if 0
"#version 330 core\n"
"out vec4 FragColor;  \n"
"in vec2 TexCoords;\n"
"uniform sampler2D fbo_texture;\n"
"void main()\n"
"{ \n"
"    FragColor = texture(fbo_texture, TexCoords);\n"
    "float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;\n"
    "FragColor = vec4(average, average, average, 1.0);\n"
"}";
#else
"#version 330 core\n"
"out vec4 FragColor;  \n"
"in vec2 TexCoords;\n"
"uniform sampler2D fbo_texture;\n"
"uniform bool shake;\n"
"uniform float move;\n"
"const float offset = 1.0 / 300.0;  \n"
" \n"
"void main() \n"
"{ \n"
"if(shake){\n"
"    vec2 offsets[9] = vec2[]( \n"
"        vec2(-offset,  offset), // top-left \n"
"        vec2( 0.0f,    offset), // top-center \n"
"        vec2( offset,  offset), // top-right \n"
"        vec2(-offset,  0.0f),   // center-left \n"
"        vec2( 0.0f,    0.0f),   // center-center \n"
"        vec2( offset,  0.0f),   // center-right \n"
"        vec2(-offset, -offset), // bottom-left \n"
"        vec2( 0.0f,   -offset), // bottom-center \n"
"        vec2( offset, -offset)  // bottom-right   \n"  
"    );\n"
" \n"
"float kernel[9] = float[](\n"
"    1.0 / 16, 2.0 / 16, 1.0 / 16,\n"
"    2.0 / 16, 4.0 / 16, 2.0 / 16,\n"
"    1.0 / 16, 2.0 / 16, 1.0 / 16  \n"
");\n"
"     \n"
"    vec3 sampleTex[9]; \n"
"    for(int i = 0; i < 9; i++) \n"
"    { \n"
"        sampleTex[i] = vec3(texture(fbo_texture, TexCoords.st + offsets[i])); \n"
"    } \n"
"    vec3 col = vec3(0.0); \n"
"    for(int i = 0; i < 9; i++) \n"
"        col += sampleTex[i] * kernel[i]; \n"
"     \n"
"    FragColor = vec4(col, 1.0); \n"
"}else {\n"
    "vec2 cp = -1.0 + 2.0 * TexCoords;\n"
    "float cl = length(cp);\n"
    "vec2 uv = TexCoords + (cp / cl) * cos(cl * 12.0 - move * 4.0) * 0.02;\n"
    "vec3 col = texture(fbo_texture, uv).xyz;    \n"
"FragColor = vec4(col, 1.0);\n"
"}\n"

"}";
#endif

/*
SIN WAV
    "vec2 uv = TexCoords;\n"
    "uv.x += sin(uv.y*10.0+move)/10.0;    \n"
"FragColor = texture(fbo_texture, uv);\n"
*/

