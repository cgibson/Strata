//
//  GLRenderer.h
//  GLContextTest
//
//  Created by Chris Gibson on 12/17/11.
//  Copyright (c) 2011 Chris Gibson. All rights reserved.
//

#ifndef ENGINE_H
#define ENGINE_H

class Engine
{
public:
    
    // CORE
    Engine(){ _vpWidth = 0; _vpHeight = 0; }
    virtual ~Engine(){};
    virtual void init() = 0;
    
    // GRAPHICS
    void setScreen(int width, int height){ _vpWidth = width; _vpHeight = height; }
    void render(){ pre_frame(); frame(); post_frame(); }
    void render_and_update(long elapsed){ render(); update(elapsed); }
    
    // LOGIC
    virtual void update(long elapsed) = 0;
    
protected:
    
    // GRAPHICS
    virtual void pre_frame() = 0;
    virtual void post_frame() = 0;
    virtual void frame() = 0;
    
    int _vpWidth, _vpHeight;
};

#endif // ENGINE_H