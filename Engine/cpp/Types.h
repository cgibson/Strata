//
//  Types.h
//  ModelViewTest
//
//  Created by Chris Gibson on 1/8/12.
//  Copyright (c) 2012 Chris Gibson. All rights reserved.
//

#ifndef ModelViewTest_Types_h
#define ModelViewTest_Types_h

#ifdef __APPLE__
	#include <OpenGLES/ES2/gl.h>
	#include <QuartzCore/QuartzCore.h>
#else
	//#include <GLee.h>
	//#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	//#include <GL/glext.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace glm;

// uniform index
enum {
	UNIFORM_MODELVIEW_PROJECTION_MATRIX,
    NORMAL_MATRIX,
    MODELVIEW_MATRIX,
	NUM_UNIFORMS
};

static GLint uniforms[NUM_UNIFORMS];

// attribute index
enum {
	ATTRIB_VERTEX,
	ATTRIB_NORMAL,
    ATTRIB_UV,
    ATTRIB_COLOR,
	NUM_ATTRIBUTES
};

#endif
