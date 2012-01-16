//
//  ResourceManager.m
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 Chris Gibson. All rights reserved.
//

#import "ResourceManager.h"


const char * cppGetResourceFileContents(void * resourceName, void * resourceType)
{
    return NULL;
    
}

const char* cppGetResourceFilePath(void * resourceName, void * resourceType)
{
    NSString * path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:(char*)resourceName] 
                                                      ofType:[NSString stringWithUTF8String:(char*)resourceType]];
    
    return [path UTF8String];
}

Texture2D * cppGetTexture2D(void * textureName)
{
    Texture2D *tex = new Texture2D();
    
    // Generate texture
    glGenTextures(1, &tex->textureID);
    
    // Bind texture
    glBindTexture(GL_TEXTURE_2D, tex->textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    
    NSString *path = [[NSString alloc] initWithUTF8String: (char*)textureName];
    path = [[NSBundle mainBundle] pathForResource:path ofType:@""];
    
    // Grab the data from the texture
    NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
    
    UIImage *image = [[UIImage alloc] initWithData:texData];
    
    if (image == nil)
        return NULL;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    // Get the image size
    GLuint width = CGImageGetWidth(image.CGImage);
    GLuint height = CGImageGetHeight(image.CGImage);
    
    // Allocate memory for image
    void *imgData = malloc( height * width * 4 );
    CGContextRef imgcontext = CGBitmapContextCreate(
                                                    imgData, width, height, 8, 4 * width, colorSpace,
                                                    kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big );
    
    CGColorSpaceRelease( colorSpace );
    CGContextClearRect(imgcontext, CGRectMake(0, 0, width, height));
    CGContextTranslateCTM( imgcontext, 0, height-height);
    CGContextDrawImage(imgcontext, CGRectMake(0, 0, width, height), image.CGImage);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
    
    CGContextRelease(imgcontext);
    
    free(imgData);
    [image release];
    [texData release];
    
    tex->width = width;
    tex->height = height;
    
    return tex;
}