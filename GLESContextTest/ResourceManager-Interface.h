//
//  ResourceManager-Interface.h
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 PDI/DreamWorks Animation. All rights reserved.
//

#ifndef GLESContextTest_ResourceManager_Interface_h
#define GLESContextTest_ResourceManager_Interface_h

#ifdef __cplusplus
extern "C"
{
    const char * cppGetResourceFilePath(void * self, void * resourceName, void * resourceType);
    const char * cppGetResourceFileContents(void * self, void * resourceName, void * resourceType);
}
#else
const char * cppGetResourceFilePath(void * self, void * resourceName, void * resourceType);
const char * cppGetResourceFileContents(void * self, void * resourceName, void * resourceType);
#endif

#endif
