//
//  GLRenderer.h
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 Chris Gibson. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol GLRenderer <NSObject>

- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer;

@end
