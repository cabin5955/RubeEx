//
//  game_b2rube.hpp
//  RubeEx
//
//  Created by chen caibin on 2022/6/16.
//

#ifndef game_b2rube_hpp
#define game_b2rube_hpp

#include "glad.h"
#include <vector>
#include "scene.h"
#include <string>
#include "resource_manager.hpp"
#include "sprite_renderer.hpp"
#include "text_renderer.hpp"
#include "director.hpp"
#include "button.hpp"
#include "color_renderer.hpp"
#include "color.h"
#include "global.hpp"
#include "utils.h"
#include "mesh.hpp"
#include "libex.h"
#include "Box2D.h"
#include "b2debugdraw.h"
#include "b2dJson.h"
#include "b2dJsonImageEx.h"

class GameB2Rube:public IScene
{
public:
    
    typedef struct Player {
        glm::vec2 position;
        float speed;
        bool canJump;
    } Player;
    
    struct RubeParameters {
        std::string m_filename;
        float m_driveJointSpeed;
        bool m_doDebugDraw;
        RubeParameters( std::string fn, float ds, bool ddd ) : m_filename(fn), m_driveJointSpeed(ds), m_doDebugDraw(ddd) {}
    };
    
    // Initialize game state (load all shaders/textures/levels)
    void Init(unsigned int width, unsigned int height);
    void OnEnter();
    void OnExit();
    // GameLoop
    void KeyboardInput(ExKeyCode keycode, ExKeyAction action);
    void Update(GLfloat dt);
    void Render();
    
    void Release();
    
    static GameB2Rube* GetInstance()
    {
        if(s_instance == nullptr)
        {
            s_instance = new GameB2Rube();
        }
        return s_instance;
    }
    
private:
    GLfloat deltaTime;
    b2World* world;
    
    //drive joint info
    std::vector<b2Joint*> driveJoints;
    float driveSpeed;
    b2Vec2 currentViewCenter;
    
    //images loaded from scene file
    std::vector<b2dJsonImageEx*> images;
    
    SpriteRenderer      *spriteRenderer;
    SpriteRenderer      *uiRenderer;
    ColorRenderer       *colorRenderer;

    Player player;
    Button *GoButton_Right,*GoButton_Left,*JumpButton;
    
    Camera2D camera;
    
    std::vector<RubeParameters> m_rubeParameters;
    
    GameB2Rube(){}
    
    ~GameB2Rube(){}

    GameB2Rube(const GameB2Rube&);
    GameB2Rube& operator =(const GameB2Rube&);
    
    static GameB2Rube *s_instance;
};

#endif /* game_b2rube_hpp */
