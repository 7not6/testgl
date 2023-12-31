#include "common.h"

GLuint compileShader(const char* source, GLuint shaderType) {
	// Create ID for shader
	GLuint result = glCreateShader(shaderType);
	// Define shader text
	glShaderSource(result, 1, &source, NULL);
	// Compile shader
	glCompileShader(result);

	//Check vertex shader for errors
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv( result, GL_COMPILE_STATUS, &shaderCompiled );
	if( shaderCompiled != GL_TRUE ) {
		printf( "Error compilation: %d\n",result );
		
		GLint logLength;
		glGetShaderiv(result, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(result, logLength, &logLength, log);
			
			printf("Shader compile log: %s\n" ,log );
			free(log);
		}
		glDeleteShader(result);
		result = 0;
	} else {
		printf( "Shader compile ok Id = %d\n", result);
	}
	return result;
}

GLuint compileProgram_default(const char* vtxFile, const char* fragFile) {

	GLuint programID = 0;
	GLuint vtxShaderId, fragShaderId;

	programID = glCreateProgram();

	vtxShaderId = compileShader(vtxFile, GL_VERTEX_SHADER);

	fragShaderId = compileShader(fragFile, GL_FRAGMENT_SHADER);

	if(vtxShaderId && fragShaderId) {
		// Associate shader with program
		glAttachShader(programID, vtxShaderId);
		glAttachShader(programID, fragShaderId);
		glLinkProgram(programID);
		glValidateProgram(programID);

		// Check the status of the compile/link
		GLint logLen;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0) {
			char* log = (char*) malloc(logLen * sizeof(char));
			// Show any errors as appropriate
			glGetProgramInfoLog(programID, logLen, &logLen, log);
			printf( "Prog Info Log:%s\n" , log );
			free(log);
		}	
	}
	
	if(vtxShaderId) {
		glDeleteShader(vtxShaderId);
	}
	if(fragShaderId) {
		glDeleteShader(fragShaderId);
	}
	return programID;
}


void printUniformAndAttribute(GLuint program){

	// recuperer le nombre total d'uniforms
	GLint n= 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &n);

	// recuperer la longueur max occuppee par un nom d'uniform
	GLint length_max= 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length_max);
	// allouer une chaine de caractere
	char name[4096];
 	// recuperer les infos de chaque uniform
	for(int index= 0; index < n; index++)
	{
	    GLint glsl_size;
	    GLenum glsl_type;
	    glGetActiveUniform(program, index, length_max, NULL, &glsl_size, &glsl_type, name);
	    
	    // et son identifiant
	    GLint location= glGetUniformLocation(program, name);
	    
	    printf("uniform %d '%s': location %d, type %x, array_size %d\n", index, name, location, glsl_type, glsl_size);
	}	
 
	// recuperer le nombre total d'attributs
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &n);

	// recuperer la longueur max occuppee par un nom d'attribut
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length_max);
 
	// recuperer les infos de chaque attribut
	for(int index= 0; index < n; index++)
	{
	    GLint glsl_size;
	    GLenum glsl_type;
	    glGetActiveAttrib(program, index, length_max, NULL, &glsl_size, &glsl_type, name);
	    
	    // et son identifiant
	    GLint location= glGetAttribLocation(program, name);
	    
	    printf("attribute %d '%s': location %d, type %x, array_size %d\n", index, name, location, glsl_type, glsl_size);
	}
}

