//
//  world_contact_listener.hpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/3.
//

#ifndef world_contact_listener_hpp
#define world_contact_listener_hpp

#include "Box2D.h"
class WorldContactListener : public b2ContactListener
{
    void EndContact(b2Contact* contact);
    void BeginContact(b2Contact* contact);
};

#endif /* world_contact_listener_hpp */
