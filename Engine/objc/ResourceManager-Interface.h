//
//  ResourceManager-Interface.h
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 Chris Gibson. All rights reserved.
//

#ifndef GLESContextTest_ResourceManager_Interface_h
#define GLESContextTest_ResourceManager_Interface_h

#include <OpenGLES/ES2/gl.h>

typedef struct Texture2D {
    GLuint textureID;
    uint width;
    uint height;
} Texture2D;

// C plus plus boilerplating
#ifdef __cplusplus
extern "C"
{
#endif

    const char * cppGetResourceFilePath(void * resourceName, void * resourceType);
    const char * cppGetResourceFileContents(void * resourceName, void * resourceType);
    Texture2D * cppGetTexture2D(void * textureName);
    
// C plus plus boilerplating
#ifdef __cplusplus
}
#endif

#endif
