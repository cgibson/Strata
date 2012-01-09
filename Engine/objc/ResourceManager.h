//
//  ResourceManager.h
//  GLESContextTest
//
//  Created by Chris Gibson on 1/2/12.
//  Copyright (c) 2012 Chris Gibson. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ResourceManager-Interface.h"

@interface ResourceManager : NSObject

- (NSString*) getResourceFilePath: (char*)resourceName type: (char*)resourceType;

@end