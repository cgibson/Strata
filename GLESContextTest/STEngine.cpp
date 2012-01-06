//
//  STEngine.cpp
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 PDI/DreamWorks Animation. All rights reserved.
//

#include "STEngine.h"
#include "ResourceManager-Interface.h"
#include "Shaders.h"

#include <string>

using namespace glm;
using std::string;

// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,
	NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];

// attribute index
enum {
	ATTRIB_VERTEX,
	ATTRIB_COLOR,
	NUM_ATTRIBUTES
};

STEngine::STEngine(void * resourceMngrHandle) {
    printf("Building Engine\n");
    _resourceMngrHandle = resourceMngrHandle;
    _rotZ = 0.0f;
}

STEngine::~STEngine() {
    printf("Teardown\n");
    // tear down GL
	if (_defaultFramebuffer)
	{
		glDeleteFramebuffers(1, &_defaultFramebuffer);
		_defaultFramebuffer = 0;
	}
	
	if (_colorRenderbuffer)
	{
		glDeleteRenderbuffers(1, &_colorRenderbuffer);
		_colorRenderbuffer = 0;
	}
	
	// realease the shader program object
	if (_program)
	{
		glDeleteProgram(_program);
		_program = 0;
	}
}

void
STEngine::init() {
    printf("GETTING STARTED\n");
}

void
STEngine::pre_frame() {
    
}

void
STEngine::post_frame() {
    
}

void
STEngine::frame() {
    const GLfloat squareVertices[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        -0.5f,  0.5f,
        0.5f,   0.5f,
    };
    const GLubyte squareColors[] = {
        255, 255,   0, 255,
        0,   255, 255, 255,
        0,     0,   0,   0,
        255,   0, 255, 255,
    };
    
    glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);
    glViewport(0, 0, _vpWidth, _vpHeight);
    
    glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	
	// use shader program
    glUseProgram(_program);
    
    mat4 proj, modelview, modelviewProj;	
    proj = glm::ortho(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    modelview = glm::rotate(glm::mat4(1.0f), _rotZ, vec3(0,0,1));
    modelviewProj = (proj * modelview);
	
	// update uniform values
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(modelviewProj));
	
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, 0, squareColors); //enable the normalized flag
    glEnableVertexAttribArray(ATTRIB_COLOR);
    
    
	// Validate program before drawing. This is a good check, but only really necessary in a debug build.
	// DEBUG macro must be defined in your debug configurations if that's not already the case.
#if defined(DEBUG)
	if (!validateProgram(_program))
	{
		printf("Failed to validate program: %d", _program);
		return;
	}
#endif
	
	// draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
}

void
STEngine::update(long elapsed) {
    _rotZ += 3.0f;
}


bool
STEngine::initGlBuffers() {
    
    printf("Building Gl Buffers\n");
    // Create default framebuffer object.
    glGenFramebuffers(1, &_defaultFramebuffer);
    glGenRenderbuffers(1, &_colorRenderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
    
    return true;
}

bool
STEngine::initGlShaders() {
	
    printf("Initializing Shaders\n");
	GLuint vertShader, fragShader;
	string vertShaderPathname, fragShaderPathname;
	
	// create shader program
	_program = glCreateProgram();
	
	// create and compile vertex shader
	vertShaderPathname = string(cppGetResourceFilePath(_resourceMngrHandle, (void*)"template", (void*)"vsh"));
	if (!compileShader(&vertShader, GL_VERTEX_SHADER, 1, vertShaderPathname)) {
		destroyShaders(vertShader, fragShader, _program);
		return false;
	}
	
	// create and compile fragment shader
	fragShaderPathname = string(cppGetResourceFilePath(_resourceMngrHandle, (void*)"template", (void*)"fsh"));
	if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, 1, fragShaderPathname)) {
		destroyShaders(vertShader, fragShader, _program);
		return false;
	}
	
	// attach vertex shader to program
	glAttachShader(_program, vertShader);
	
	// attach fragment shader to program
	glAttachShader(_program, fragShader);
	
	// bind attribute locations
	// this needs to be done prior to linking
	glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
	glBindAttribLocation(_program, ATTRIB_COLOR, "color");
	
	// link program
	if (!linkProgram(_program)) {
		destroyShaders(vertShader, fragShader, _program);
		return false;
	}
	
	// get uniform locations
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
	
	// release vertex and fragment shaders
	if (vertShader) {
		glDeleteShader(vertShader);
		vertShader = 0;
	}
	if (fragShader) {
		glDeleteShader(fragShader);
		fragShader = 0;
	}
    
    return true;
}