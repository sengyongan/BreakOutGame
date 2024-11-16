#include "game.h"

#include"tool/resource_manager.h"
#include"tool/debug.h"
#include"Collision/collision.h"
#include"Window/window.h"
// ��ʼ������Ĵ�С
const glm::vec2 PLAYER_SIZE(100, 20);
// ��ʼ�����������
const float PLAYER_VELOCITY(5000.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(1200.0f , -4200.0f);
// ��İ뾶
const GLfloat BALL_RADIUS = 12.5f;

//instance
Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);
Game& GetGameRef() {
    return Breakout;
}

Game::Game(GLuint width, GLuint height)
    :Width(width), Height(height)
{
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
}

void Game::Init()
{
    ///�����ʲ�
    // ��������
    ResourceManager::LoadTexture("resources/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("resources/textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("resources/textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("resources/textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("resources/textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("resources/textures/particle.png", GL_TRUE, "particle");
    ResourceManager::LoadTexture("resources/textures/powerup_speed.png", true, "powerup_speed");
    ResourceManager::LoadTexture("resources/textures/powerup_sticky.png", true, "powerup_sticky");
    ResourceManager::LoadTexture("resources/textures/powerup_increase.png", true, "powerup_increase");
    ResourceManager::LoadTexture("resources/textures/powerup_confuse.png", true, "powerup_confuse");
    ResourceManager::LoadTexture("resources/textures/powerup_chaos.png", true, "powerup_chaos");
    ResourceManager::LoadTexture("resources/textures/powerup_passthrough.png", true, "powerup_passthrough");
    glCheckError();
    // ���عؿ�
    this->Levels.clear();
    GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    glCheckError();
    //��Ƶ

    //SoundEngine->play2D("resources/audio/breakout.mp3", false);
    SoundEngine->play2D("resources/audio/breakout.mp3", GL_TRUE);
    //����
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("resources/fonts/ocraext.TTF", 24);
    ///����shader
    // ������ɫ��
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");
    ResourceManager::LoadShader("shaders/traigle.vs", "shaders/traigle.fs", nullptr, "traigle");
    ResourceManager::LoadShader("shaders/post_processing.vs", "shaders/post_processing.fs", nullptr, "postprocessing");

    // ������ɫ��
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    //0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f
    // //0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f
    glm::mat4 projection = Window::GetProjection();
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    ResourceManager::GetShader("traigle").Use();
    ResourceManager::GetShader("traigle").SetMatrix4("projection", projection);
    glCheckError();
    // ����ר������Ⱦ�Ŀ���
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    TraigleRenderer = new SpriteTraigleRenderer(ResourceManager::GetShader("traigle"));
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"),this->Width, this->Height);
    //Effects->Chaos = true;
    ///����gameobject
    //�������
    glm::vec2 playerPos = glm::vec2(
        this->Width / 2 - PLAYER_SIZE.x / 2,
        this->Height - PLAYER_SIZE.y
    );
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    glCheckError();
    //��������
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
    //��������
    Particles = new ParticleGenerator(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        500
    );
}

void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // �ƶ�����
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0)
                Player->Position.x -= velocity;
            if (Ball->Stuck)//������̶�����û�з��䣩
                Ball->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
            if (Ball->Stuck)
                Ball->Position.x += velocity;
        }
        if (this->Keys[GLFW_KEY_SPACE])//���¿ո񣬷�������
            Ball->Stuck = false;
        //resetEnum
        if (this->Keys[GLFW_KEY_TAB] && !this->KeysProcessed[GLFW_KEY_TAB]) {
            this->State = GAME_MENU;
            isStartEnum = true;
            this->KeysProcessed[GLFW_KEY_TAB] = GL_TRUE;
        }
    }
    else if (this->State == GAME_MENU) {
        if (this->Keys[GLFW_KEY_Q] && !this->KeysProcessed[GLFW_KEY_Q]) {
            this->State = GAME_ACTIVE;
            isStartEnum = false;
            this->KeysProcessed[GLFW_KEY_Q] = GL_TRUE;
        }
        if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W]) {
            if (++Level == 4) {
                Level = 0;
            }
            this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
        }
        if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S]) {
            if (--Level == -1) {
                Level = 3;
            }
            this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
        }
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {
            ResetGame(Level);
            this->State = GAME_ACTIVE;
        }
    }
}

void Game::Update(GLfloat dt)
{
    if (this->State == GAME_ACTIVE) {

        //������������
        Ball->Move(dt, this->Width);
        // �����ײ
        this->DoCollisions();
        // Update particles
        Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
        // update PowerUps
        PowerUps.UpdatePowerUps(dt, Ball, Player, Effects);
        //������Ч
        if (ShakeTime > 0.0f)//
        {
            ShakeTime -= dt;
            if (ShakeTime <= 0.0f)//��Чʱ�����
                Effects->Shake = false;
        }
        // ���Ƿ�Ӵ��ײ��߽磿
        if (Ball->Position.y >= this->Height) 
        {
            this->ResetLevel();
            this->ResetPlayer();
            GameDataLV.DeathCount++;
        }
    }
}

void Game::Render()
{
    //if (this->State == GAME_ACTIVE)
    //{
        //����
        Effects->BeginRender();

        // ��Ⱦ����
        Renderer->DrawSprite(ResourceManager::GetTexture("background"),
            glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
        );
        // ��Ⱦ�ؿ�������ש�飩
        this->Levels[this->Level].Draw(*Renderer);
        //��Ⱦ���
        Player->Draw(*Renderer);
        // Draw particles   
        Particles->Draw();//��Ⱦ������
        //��Ⱦ����
        Ball->Draw(*Renderer);
        //��Ⱦ����
        for (PowerUp& powerUp : PowerUps.PowerUps)
            if (!powerUp.Destroyed)
                powerUp.Draw(*Renderer);
  
        Effects->EndRender();
        // Render postprocessing quad
        Effects->Render(glfwGetTime());
        //
        std::stringstream ss, ss1, ss2;
        ss << this->Level;
        Text->RenderText("Level:" + ss.str(), 5.0f, 5.0f, 0.5f, glm::vec3(1.0, 1.0, 0.0));
        ss1 << this->GameDataLV.KillBrickCount;
        Text->RenderText("KillBrickCount:" + ss1.str(), 5.0f, 30.0f, 0.5f, glm::vec3(1.0, 1.0, 0.0));
        ss2 << this->GameDataLV.DeathCount;
        Text->RenderText("DeathCount:" + ss2.str(), 5.0f, 55.0f, 0.5f, glm::vec3(1.0, 1.0, 0.0));
        if (isStartEnum) {
            ResetGameEnum();
        }
    //}
}
void Game::DoCollisions()
{
    //������ש����ײ���
    for (GameObject& box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = collision::CheckCollision_Circle_AABB(*Ball, box);
            if (std::get<0>(collision)) // ���collision �� true
            {
                // ���ש�鲻��ʵ�ľ�����ש��
                if (!box.IsSolid)
                {
                    box.DecreaseHealth();
                    box.DecreaseColor();
                    if (box.CurHealth <= 0) {
                        box.Destroyed = GL_TRUE;
                        PowerUps.SpawnPowerUps(box);
                        SoundEngine->play2D("resources/audio/bleep.mp3", false);
                        GameDataLV.KillBrickCount++;
                    }
                }
                // �����ʵ�ĵ�ש���򼤻�shake��Ч
                if (!box.Destroyed) {
                     ShakeTime = 0.01f;
                     Effects->Shake = true;
                     SoundEngine->play2D("resources/audio/solid.wav", false);
                }
                
                // ��ײ����
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (!(Ball->PassThrough && box.IsSolid))//�����ô����̶�ש����ߣ���ש���ǹ̶���,�ǾͲ���ִ����ײ������ִ��
                {
                    if (dir == LEFT || dir == RIGHT) // ˮƽ������ײ
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // ��תˮƽ�ٶ�
                        // �ض�λ
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->Position.x += penetration; // ��������???
                        else
                            Ball->Position.x -= penetration; // ��������
                    }
                    else // ��ֱ������ײ
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // ��ת��ֱ�ٶ�
                        // �ض�λ
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->Position.y -= penetration; // ��������
                        else
                            Ball->Position.y += penetration; // ��������
                    }
                }
            }
        }

    }
    //����
    for (PowerUp& powerUp : PowerUps.PowerUps)
    {
        if (!powerUp.Destroyed)
        {   //���������Ļ
            if (powerUp.Position.y >= this->Height)
                powerUp.Destroyed = GL_TRUE;
            // �����뵲��Ӵ�����������
            if (collision::CheckCollision_AABB_AABB(*Player, powerUp))
            {   
                PowerUps.ActivatePowerUp(powerUp, Ball, Player, Effects);//ִ��Ч��
                powerUp.Destroyed = GL_TRUE;
                powerUp.Activated = GL_TRUE;
                SoundEngine->play2D("resources/audio/powerup.wav", false);
            }
        }
    }

    //��������ײ���
    Collision result = collision::CheckCollision_Circle_AABB(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // ��������˵�����ĸ�λ�ã������������ĸ�λ�����ı��ٶ�
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);//��������뵲�����ĵľ���͵���İ�߳��İٷֱ�
        // ���ݽ���ƶ�
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);//�µķ��� * �ٶ�

        // if Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
        Ball->Stuck = Ball->Sticky;
        SoundEngine->play2D("resources/audio/bleep.wav", false);
    }
}
void Game::ResetLevel()
{
    Level++;
    switch(this->Level)
    {
        case 0:
            this->Levels[0].Load("levels/one.lvl", this->Width, this->Height / 2);
            break;
        case 1:
            this->Levels[1].Load("levels/two.lvl", this->Width, this->Height / 2);
            break;
        case 2:
            this->Levels[2].Load("levels/three.lvl", this->Width, this->Height / 2);
            break;
        case 3:
            this->Levels[3].Load("levels/four.lvl", this->Width, this->Height / 2);
            break;
        default:
            Level = 0;
            this->Levels[0].Load("levels/one.lvl", this->Width, this->Height / 2);
            break;
    }
}

void Game::ResetPlayer()
{
    // reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

    // also disable all active powerups�����ü��������
    Effects->Chaos = Effects->Confuse = false;
    Ball->PassThrough = Ball->Sticky = false;
    Player->Color = glm::vec3(1.0f);
    Ball->Color = glm::vec3(1.0f);
}

void Game::ResetGameEnum()
{
    //std::stringstream ss, ss1, ss2;
    //ss << this->Level;
    Text->RenderText("ResetGameEnum", Width/2 - 100.0f, Height / 2 + -100.0f, 1.0f, glm::vec3(1.0, 0.0, 1.0));
    Text->RenderText("Press w s to switch the start level", Width/2 - 250.0f, Height / 2 + -50.0f, 1.0f,glm::vec3(0.0, 1.0, 0.0));
    Text->RenderText("enter reset the level", Width/2 - 160.0f, Height / 2 , 1.0f,glm::vec3(0.0, 1.0, 0.0));
    Text->RenderText("Q Exit Menu", Width/2 - 100.0f, Height / 2 + 50.0f, 1.0f, glm::vec3(0.0, 1.0, 0.0));
}

void Game::ResetGame(int selectLevel)
{
    SoundEngine->stopAllSounds();
    Game::Init();
    Level = selectLevel;
    GameDataLV.DeathCount = 0;
    GameDataLV.KillBrickCount = 0;
    PowerUps.PowerUps.clear();
    isStartEnum = false;

    //����
    Window::GetInstance().SetWindowSize();

}


