//
//  GL20Renderer.h
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 PDI/DreamWorks Animation. All rights reserved.
//
#import "GLRenderer.h"
#import "STEngine.h"
#import "ResourceManager.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@interface GL20Renderer : NSObject <GLRenderer>
{
@private
	EAGLContext *context;
    ResourceManager *resourceMngr;
    STEngine *engine;
	
	// The pixel dimensions of the CAEAGLLayer
	GLint backingWidth;
	GLint backingHeight;
	
}

- (void) render;
- (BOOL) resizeFromLayer:(CAEAGLLayer *)layer;

@end