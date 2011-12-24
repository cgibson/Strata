//
//  GL20Renderer.h
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 PDI/DreamWorks Animation. All rights reserved.
//
#import "GLRenderer.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface GL20Renderer : NSObject <GLRenderer>
{
@private
	EAGLContext *context;
	
	// The pixel dimensions of the CAEAGLLayer
	GLint backingWidth;
	GLint backingHeight;
	
	// The OpenGL names for the framebuffer and renderbuffer used to render to this view
	GLuint defaultFramebuffer, colorRenderbuffer;
	
	/* the shader program object */
	GLuint program;
	
	GLfloat rotz;
	
}

- (void) render;
- (BOOL) resizeFromLayer:(CAEAGLLayer *)layer;

@end