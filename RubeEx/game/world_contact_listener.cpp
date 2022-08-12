//
//  world_contact_listener.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/5/3.
//

#include "world_contact_listener.hpp"
#include "player.hpp"
#include "interactive_tileobject.hpp"

void WorldContactListener::BeginContact(b2Contact* contact) {
    b2Fixture *fixA = contact->GetFixtureA();
    b2Fixture *fixB = contact->GetFixtureB();

    int cDef = fixA->GetFilterData().categoryBits | fixB->GetFilterData().categoryBits;
    switch (cDef)
    {
        case Player::MARIO_HEAD_BIT | Player::BRICK_BIT:
            
        case Player::MARIO_HEAD_BIT | Player::COIN_BIT:
            break;
            
        case Player::ENEMY_HEAD_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::ENEMY_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::MARIO_BIT | Player::ENEMY_BIT:
            
            break;
            
        case Player::MARIO_BIT | Player::OBJECT_BIT:
        
            break;
            
        case Player::ENEMY_BIT | Player::ENEMY_BIT:
        {
            
            break;
        }
        case Player::ITEM_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::ITEM_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::FIREBALL_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::ENEMY_BIT | Player::PLATFORM_BIT:
            
            break;
    }
}

void WorldContactListener::EndContact(b2Contact* contact) {
      
    b2Fixture *fixA = contact->GetFixtureA();
    b2Fixture *fixB = contact->GetFixtureB();

    int cDef = fixA->GetFilterData().categoryBits | fixB->GetFilterData().categoryBits;

    switch (cDef)
    {
        case Player::MARIO_HEAD_BIT | Player::BRICK_BIT:
            
        case Player::MARIO_HEAD_BIT | Player::COIN_BIT:
            
            break;
            
        case Player::ENEMY_HEAD_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::ENEMY_BIT | Player::OBJECT_BIT:
            break;
            
        case Player::MARIO_BIT | Player::ENEMY_BIT:
            
            break;
            
        case Player::MARIO_BIT | Player::OBJECT_BIT:
        
            break;
            
        case Player::ENEMY_BIT | Player::ENEMY_BIT:
        
            break;
    
        case Player::ITEM_BIT | Player::OBJECT_BIT:
            
            break;
            
        case Player::ITEM_BIT | Player::MARIO_BIT:
            
            break;
            
        case Player::FIREBALL_BIT | Player::OBJECT_BIT:
            
            break;
    }
}
