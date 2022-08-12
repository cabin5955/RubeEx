//
//  game_b2rube.cpp
//  RubeEx
//
//  Created by chen caibin on 2022/6/16.
//

#include "game_b2rube.hpp"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f

GameB2Rube* GameB2Rube::s_instance = nullptr;

void UpdateCameraCenter(Camera2D *camera, GameB2Rube::Player *player, int width, int height);

void GameB2Rube::Init(unsigned int width, unsigned int height)
{
    this->Width = width;
    this->Height = height;
    // 配置着色器
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                        static_cast<GLfloat>(this->Height),0.0f,
                                        -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("view", glm::mat4(1.0));
    spriteShader.setMat4("projection", projection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", glm::mat4(1.0));
    colorShader.setMat4("projection", projection);
    
    Shader uiShader = ResourceManager::GetShader("ui");
    uiShader.use();
    uiShader.setMat4("projection", projection);
    
    // 设置专用于渲染的控制
    spriteRenderer = new SpriteRenderer(spriteShader);
    colorRenderer = new ColorRenderer(colorShader);
    uiRenderer = new SpriteRenderer(uiShader);
    
    float scal = 1.0f;
    int btn_size = 64;
    GoButton_Right = new Button(this,glm::vec2(this->Width/2+btn_size*scal-btn_size*4,this->Height-128*scal),
                                glm::vec2(btn_size*scal,btn_size*scal),
                                0,
                                ResourceManager::GetTexture("arrow"),
                                0,0);
    
    GoButton_Left = new Button(this,glm::vec2(this->Width/2-btn_size*scal-btn_size*4,this->Height-128*scal),
                               glm::vec2(btn_size*scal,btn_size*scal),
                               180.0f*DEG2RAD,
                               ResourceManager::GetTexture("arrow"),
                               0,0);
    
    JumpButton = new Button(this,glm::vec2(this->Width-btn_size*4,this->Height-128*scal),
                            glm::vec2(btn_size*scal,btn_size*scal),
                            -90.0f*DEG2RAD,
                            ResourceManager::GetTexture("arrow"),
                            0,0);
    
    player.position = { 400, 280 };
    player.speed = 0;
    player.canJump = false;
    
    camera.target = player.position;
    camera.offset = { width/2.0f, height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void GameB2Rube::OnEnter(){
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
                                        static_cast<GLfloat>(this->Height),0.0f,
                                        -1.0f, 1.0f);
    
    Shader spriteShader = ResourceManager::GetShader("sprite");
    spriteShader.use();
    spriteShader.setMat4("projection", projection);
    
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("projection", projection);
    
    glm::mat4 rubeProjection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width/PPM),
                                          0.0f, static_cast<GLfloat>(this->Height/PPM),
                                          -1.0f, 1.0f);
    
    Shader rubeShader = ResourceManager::GetShader("rube");
    rubeShader.use();
    rubeShader.setMat4("projection", rubeProjection);
    
    m_rubeParameters.push_back( RubeParameters( "image_test.json", 30, true ) );
    
    b2dJson json;
    std::string errorMsg;
    world = json.readFromFile(m_rubeParameters[0].m_filename.c_str(), errorMsg);
    if ( world ) {
        g_debugDraw.SetFlags(b2Draw::e_shapeBit);
        world->SetDebugDraw( &g_debugDraw );
        
        //get named drive joints from the JSON info
        json.getJointsByName("drivejoint", driveJoints);
        
        //load images
        std::vector<b2dJsonImage*> srcImages;
        json.getAllImages(srcImages);
        
        driveSpeed = m_rubeParameters[0].m_driveJointSpeed;
        
        for (int i = 0; i < (int)srcImages.size(); i++) {
            b2dJsonImageEx* img = new b2dJsonImageEx(rubeShader, srcImages[i] );
            images.push_back( img );
        }
        
        if ( ! driveJoints.empty() )
            currentViewCenter = driveJoints[0]->GetBodyA()->GetPosition();
        else
            currentViewCenter = b2Vec2(0,0);
    }
}

void GameB2Rube::OnExit(){
    
}

void GameB2Rube::KeyboardInput(ExKeyCode keycode, ExKeyAction action)
{
    
}

void GameB2Rube::Update(GLfloat dt)
{
    float deltaTime = Global::GetFrameTime();
    float motorSpeed = 0.0f;
    if(GoButton_Right->mouseState == MOUSE_PRESSED)
    {
        player.position.x += PLAYER_HOR_SPD*deltaTime;
        motorSpeed = -1.0f;
    }
    else if(GoButton_Left->mouseState == MOUSE_PRESSED)
    {
        float step = PLAYER_HOR_SPD*deltaTime;
        player.position.x -= step;
        motorSpeed = 1.0f;
    }
    
    if(JumpButton->mouseState == MOUSE_PRESSED)
    {
        if (player.canJump)
        {
            player.speed = -PLAYER_JUMP_SPD;
            player.canJump = false;
        }
    }
    
    //UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
    world->Step(1/60.0f, 6, 2);
    
    motorSpeed *= driveSpeed;
    for (int i = 0; i < driveJoints.size(); i++) {
        b2Joint* joint = driveJoints[i];
        b2JointType type = joint->GetType();
        switch (type) {
            case e_revoluteJoint :
                ((b2RevoluteJoint*)joint)->SetMotorSpeed(motorSpeed);
                break;
            case e_wheelJoint :
                ((b2WheelJoint*)joint)->SetMotorSpeed(motorSpeed);
                break;
            default:;
        }
    }
    
    UpdateCameraCenter(&camera, &player, this->Width, this->Height);
}

void GameB2Rube::Render()
{
    Shader colorShader = ResourceManager::GetShader("color");
    colorShader.use();
    colorShader.setMat4("view", GetCameraMatrix2D(camera));

    //colorRenderer->DrawColor( ORANGE,glm::vec2(player.position.x - 20, player.position.y - 40), {40, 40});
    
    GoButton_Left->Draw(*uiRenderer);
    GoButton_Right->Draw(*uiRenderer);
    JumpButton->Draw(*uiRenderer);
    
    Camera2D camera_b2;
    float ZOOM = 1.0f;
    float my = (ZOOM-1.0f) * this->Height;
    //camera_b2.target = {camera.target.x/PPM, (this->Height-camera.target.y)/PPM};
    camera_b2.target = {currentViewCenter.x,currentViewCenter.y};
    camera_b2.offset = {camera.offset.x/PPM,(this->Height-camera.offset.y-my)/PPM};
    camera_b2.rotation = camera.rotation;
    camera_b2.zoom = camera.zoom;
    
    b2Vec2 location;
    if ( ! driveJoints.empty() )
        location = driveJoints[0]->GetBodyA()->GetPosition();
    else
        location = b2Vec2(0,0);
    
    currentViewCenter = 0.75f * currentViewCenter + 0.25f * location;
    glm::mat4 viewMatrix = GetCameraMatrix2D(camera_b2);
    world->DrawDebugData();
    g_debugDraw.Flush(viewMatrix);
    
    //draw images
    for (int i = 0; i < (int)images.size(); i++)
        images[i]->render(viewMatrix);
}

void GameB2Rube::Release()
{
    
}

void UpdateCameraCenter(Camera2D *camera, GameB2Rube::Player *player, int width, int height)
{
    camera->offset = { width/2.0f, height/2.0f };
    camera->target = player->position;
}
