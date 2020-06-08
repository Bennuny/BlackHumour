//
//  3DWorld.hpp
//  3DWorld
//
//  Created by 付备 on 2020/6/8.
//  Copyright © 2020 Ben. All rights reserved.
//

#ifndef _DWorld_hpp
#define _DWorld_hpp

class ThreeDWorld {

public:
    ThreeDWorld(unsigned int w, unsigned int h);
    
    ~ThreeDWorld();
    
    void Render();

private:
    unsigned int    _windowWidth;
    unsigned int    _windowHeight;
};

#endif /* _DWorld_hpp */
