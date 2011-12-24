//
//  GL20Renderer.m
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 PDI/DreamWorks Animation. All rights reserved.
//

#import "GL20Renderer.h"

#import "Shaders.h"

#import "glm/glm.hpp"
#import "glm/gtc/matrix_transform.hpp"
#import "glm/gtc/type_ptr.hpp"

using namespace glm;

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

@interface GL20Renderer (PrivateMethods)
- (BOOL) loadShaders;
@end

@implementation GL20Renderer

// Create an ES 2.0 context
- (id <GLRenderer>) init
{
    self = [super init];
	if (self)
	{
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        // Check to see if the creation of this context was successful
        if (!context || ![EAGLContext setCurrentContext:context] || ![self loadShaders])
		{
            [self release];
            return nil;
        }
		
		// Create default framebuffer object.
		glGenFramebuffers(1, &defaultFramebuffer);
		glGenRenderbuffers(1, &colorRenderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
	}
	
	return self;
}

- (void)render {
    
    // Replace the implementation of this method to do your own custom drawing
    
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
	
    [EAGLContext setCurrentContext:context];
    
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    
    glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	
	// use shader program
	glUseProgram(program);
    
    mat4 proj, modelview, modelviewProj;	
    proj = glm::ortho(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    modelview = glm::rotate(glm::mat4(1.0f), rotz, vec3(0,0,1));
    modelviewProj = (proj * modelview);
    
	rotz += 3.0f;
	
	// update uniform values
	glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX], 1, GL_FALSE, glm::value_ptr(modelviewProj));
	
	// update attribute values
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, squareVertices);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 1, 0, squareColors); //enable the normalized flag
    glEnableVertexAttribArray(ATTRIB_COLOR);
	
	// Validate program before drawing. This is a good check, but only really necessary in a debug build.
	// DEBUG macro must be defined in your debug configurations if that's not already the case.
/*
#if defined(DEBUG)
	if (![self validateProgram:program])
	{
		NSLog(@"Failed to validate program: %d", program);
		return;
	}
#endif
*/	
	// draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (BOOL)loadShaders {
	
	GLuint vertShader, fragShader;
	NSString *vertShaderPathname, *fragShaderPathname;
	
	// create shader program
	program = glCreateProgram();
	
	// create and compile vertex shader
	vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"template" ofType:@"vsh"];
	if (!compileShader(&vertShader, GL_VERTEX_SHADER, 1, vertShaderPathname)) {
		destroyShaders(vertShader, fragShader, program);
		return NO;
	}
	
	// create and compile fragment shader
	fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"template" ofType:@"fsh"];
	if (!compileShader(&fragShader, GL_FRAGMENT_SHADER, 1, fragShaderPathname)) {
		destroyShaders(vertShader, fragShader, program);
		return NO;
	}
	
	// attach vertex shader to program
	glAttachShader(program, vertShader);
	
	// attach fragment shader to program
	glAttachShader(program, fragShader);
	
	// bind attribute locations
	// this needs to be done prior to linking
	glBindAttribLocation(program, ATTRIB_VERTEX, "position");
	glBindAttribLocation(program, ATTRIB_COLOR, "color");
	
	// link program
	if (!linkProgram(program)) {
		destroyShaders(vertShader, fragShader, program);
		return NO;
	}
	
	// get uniform locations
	uniforms[UNIFORM_MODELVIEW_PROJECTION_MATRIX] = glGetUniformLocation(program, "modelViewProjectionMatrix");
	
	// release vertex and fragment shaders
	if (vertShader) {
		glDeleteShader(vertShader);
		vertShader = 0;
	}
	if (fragShader) {
		glDeleteShader(fragShader);
		fragShader = 0;
	}
	
	return YES;
}

- (BOOL) resizeFromLayer:(CAEAGLLayer *)layer
{
	// Allocate color buffer backing based on the current layer size
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
	
    return YES;
}

- (void) dealloc
{
	// tear down GL
	if (defaultFramebuffer)
	{
		glDeleteFramebuffers(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}
	
	if (colorRenderbuffer)
	{
		glDeleteRenderbuffers(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}
	
	// realease the shader program object
	if (program)
	{
		glDeleteProgram(program);
		program = 0;
	}
	
	// tear down context
	if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	
	[context release];
	context = nil;
	
	[super dealloc];
}

@end
