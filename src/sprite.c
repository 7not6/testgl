#include "common.h"

extern GLuint vao;
extern GLuint vbo;
extern GLuint programId;
extern GLint u_col;
extern GLint u_model;
extern GLint u_proj;
extern GLint u_tex;

void SpriteDeinit()
{
    glDeleteVertexArrays(1, &vao);
}

void Spriteinit()
{
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteDraw(GLuint texture, float x,float y,float scalex,float scaley, float rotate, float r,float g,float b)
{
    float angle = (float)(rotate * (3.14159265359 / 180));

    glUseProgram(programId);
    
    Matrix model = IDENTITY_MATRIX;  
    
    Mscale(&model, scalex, scaley, 1.0f);  
    Mtranslate(&model, -0.5f * scalex, -0.5f * scaley, 0.0f);
    MrotateZ(&model, -angle);
    Mtranslate(&model, 0.5f * scalex, 0.5f * scaley, 0.0f);
    Mtranslate(&model, x,  y, 0.0f);

    glUniformMatrix4fv(u_model, 1, GL_FALSE, &(model.m[0])); //"model"
    glUniform4f(u_col, r,g,b,1.0f); //"spriteColor"
    glUniform1i(u_tex, 0);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);    
}
