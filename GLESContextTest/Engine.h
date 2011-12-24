//
//  GLRenderer.h
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 PDI/DreamWorks Animation. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>


@protocol Engine <NSObject>

- (void)render;
- (void)update;
- (void)init;
- (void)teardown;
- (void)pre_frame;
- (void)post_frame;

@end

