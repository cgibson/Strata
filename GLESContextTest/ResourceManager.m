//
//  ResourceManager.m
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 PDI/DreamWorks Animation. All rights reserved.
//

#import "ResourceManager.h"

@implementation ResourceManager

const char* cppGetResourceFilePath(void *self, void* resourceName, void* resourceType)
{
    NSString * path = [(id) self getResourceFilePath:resourceName type:resourceType];
    
    return [path UTF8String];
}

const char * cppGetResourceFileContents(void * self, void * resourceName, void * resourceType)
{
    return NULL;
}


- (NSString*) getResourceFilePath: (char*)resourceName type: (char*)resourceType
{
    NSString * resourcePath = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:resourceName] 
                                                              ofType:[NSString stringWithUTF8String:resourceType]];
    
    return resourcePath;
}

@end
