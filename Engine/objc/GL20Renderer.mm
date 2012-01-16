//
//  GL20Renderer.m
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 Chris Gibson. All rights reserved.
//

#import "GL20Renderer.h"

#import "Shaders.h"

#import "glm/glm.hpp"
#import "glm/gtc/matrix_transform.hpp"
#import "glm/gtc/type_ptr.hpp"


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
        engine = new STEngine(backingWidth, backingHeight);
        
        // Check to see if the creation of this context was successful
        if (!context || !engine || ![EAGLContext setCurrentContext:context] || !engine->initGlShaders())
		{
            NSLog(@"Context, engine or shaders screwed up...");
            [self release];
            return nil;
        }
        engine->initGlBuffers();
        
        engine->init();
	}
	
	return self;
}

- (void)render {
	
    [EAGLContext setCurrentContext:context];
    
    engine->render_and_update(12);
    
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (BOOL) resizeFromLayer:(CAEAGLLayer *)layer
{
	// Allocate color buffer backing based on the current layer size
    glBindRenderbuffer(GL_RENDERBUFFER, engine->getColorRenderbuffer());
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
    
    engine->setWindow(backingWidth, backingHeight);
    
    glBindRenderbuffer(GL_RENDERBUFFER, engine->getDepthRenderbuffer());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, engine->getDepthRenderbuffer());
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
    
    engine->setScreen(backingWidth, backingHeight);
	
    return YES;
}

- (void) dealloc
{
    delete engine;
	
	// tear down context
	if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	
	[context release];
	context = nil;
	
	[super dealloc];
}

@end
