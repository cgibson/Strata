//
//  STEngine.cpp
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 Chris Gibson. All rights reserved.
//

#include "STEngine.h"
#include "ResourceManager-Interface.h"
#include "Shaders.h"

#include <string>

#include "TriMesh.h"
#include "Loader.h"

#include "Types.h"

using std::string;

using namespace mesh;
CompressedTriMesh *meshPtr;
Texture2D *texPtr;

STEngine::STEngine(void * resourceMngrHandle, int width, int height): _width(width), _height(height) {
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
    
    // Load mesh
    string objFileName = string(cppGetResourceFilePath(_resourceMngrHandle, (void*)"sphere", (void*)"obj"));
    TriMesh tmpMesh = loadObj(objFileName);
    meshPtr = new CompressedTriMesh(tmpMesh, GL_TRIANGLES);
    
    // Load Texture
    texPtr = cppGetTexture2D((void*)"ice_melt_ice.png");
    
    // OpenGL Stuff
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
}

void
STEngine::pre_frame() {
    
}

void
STEngine::post_frame() {
    
}

void
STEngine::frame() {
    
    glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);
    glViewport(0, 0, _vpWidth, _vpHeight);
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	
	// use shader program
    glUseProgram(_program);
    
    vec3 eye(0,2.0,-3);
    vec3 lookAt(0);
    
    mat4 proj, view, modelview, modelviewProj, normal;	
    //proj = glm::ortho(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    proj = glm::perspective(45.0f, (float)_width / (float)_height, 0.1f, 100.0f);
    view = glm::lookAt(eye, lookAt, vec3(0,1,0));
    modelview = glm::rotate(glm::mat4(1.0f), _rotZ, vec3(0,1,0));
    modelviewProj = (proj * view * modelview);
    
    normal = glm::inverse(view * modelview);
    normal = glm::transpose(normal);
    
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(modelviewProj));
	glUniformMatrix4fv(uniforms[NORMAL_MATRIX], 1, GL_FALSE, glm::value_ptr(normal));
	glUniformMatrix4fv(uniforms[MODELVIEW_MATRIX], 1, GL_FALSE, glm::value_ptr(view * modelview));
    
    // Set texture 0 to the correct uniform
    GLint baseImageLoc = glGetUniformLocation(_program, "baseTexture");
    glUniform1i(baseImageLoc, 0);
    
    // Set to first texture (0)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texPtr->textureID);
    
    if (meshPtr)
        meshPtr->draw();
    
    /*
    view = glm::translate(view, vec3(0, 0, 3));
    modelviewProj = (proj * view * modelview);
    
    normal = glm::inverse(view * modelview);
    normal = glm::transpose(normal);
    
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(modelviewProj));
	glUniformMatrix4fv(uniforms[NORMAL_MATRIX], 1, GL_FALSE, glm::value_ptr(normal));
	glUniformMatrix4fv(uniforms[MODELVIEW_MATRIX], 1, GL_FALSE, glm::value_ptr(view * modelview));
                   
    if (meshPtr)
        meshPtr->draw();
     //*/
    
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
}

void
STEngine::update(long elapsed) {
    _rotZ += 0.5f;
}


bool
STEngine::initGlBuffers() {
    
    printf("Building Gl Buffers\n");
    // Create default framebuffer object.
    glGenFramebuffers(1, &_defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);
    
    glGenRenderbuffers(1, &_colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);
    
    glGenRenderbuffers(1, &_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);
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
	vertShaderPathname = string(cppGetResourceFilePath(_resourceMngrHandle, (void*)"template2", (void*)"vsh"));
	if (!compileShader(&vertShader, GL_VERTEX_SHADER, 1, vertShaderPathname)) {
		destroyShaders(vertShader, fragShader, _program);
		return false;
	}
	
	// create and compile fragment shader
	fragShaderPathname = string(cppGetResourceFilePath(_resourceMngrHandle, (void*)"template2", (void*)"fsh"));
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
	glBindAttribLocation(_program, ATTRIB_NORMAL, "normal");
	//glBindAttribLocation(_program, ATTRIB_NORMAL, "uv");
	
	// link program
	if (!linkProgram(_program)) {
		destroyShaders(vertShader, fragShader, _program);
		return false;
	}
	
	// get uniform locations
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
	uniforms[NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
	uniforms[MODELVIEW_MATRIX] = glGetUniformLocation(_program, "modelViewMatrix");
	
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