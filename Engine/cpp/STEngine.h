//
//  STEngine.h
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 PDI/DreamWorks Animation. All rights reserved.
//

#ifndef GLESContextTest_STEngine_h
#define GLESContextTest_STEngine_h

#include "Engine.h"
// LATER CHANGED TO A COMPILE-TIME DEFINE BASED ON PLATFORM
#if 1

#include <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>

#endif

#import "glm/glm.hpp"
#import "glm/gtc/matrix_transform.hpp"
#import "glm/gtc/type_ptr.hpp"



class STEngine : public Engine
{
public:
    STEngine(void * resourceMngrHandle);
    virtual ~STEngine();    
    virtual void init();
    
    // LOGIC
    virtual void update(long elapsed);
    
    // GRAPHICS
    bool initGlBuffers();
    bool initGlShaders();
    
    GLuint getColorRenderbuffer(){ return _colorRenderbuffer; }
    
protected:
    
    // GRAPHICS
    virtual void pre_frame();
    virtual void frame();
    virtual void post_frame();
    
	GLuint _defaultFramebuffer, _colorRenderbuffer;
    
    float _rotZ;
    GLuint _program;
    
    void * _resourceMngrHandle;
};

#endif