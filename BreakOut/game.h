#pragma once

#include <irrKlang.h>
using namespace irrklang;
#include"renderer/sprite_renderer.h"
#include"renderer/sprite_Traigle_renderer.h"
#include"renderer/text_renderer.h"
#include"gameobject/ball_object.h"
#include"gameobject/power_up.h"
#include"post/particle_generator.h"
#include"post/post_processor.h"
#include"level/game_level.h"
#include <GLFW/glfw3.h>
#include<vector>


// ��������Ϸ�ĵ�ǰ״̬
enum GameState {//��Ϸ�У���Ϸ�˵�����Ϸʤ��
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
struct GameData {
    GLuint KillBrickCount = 0;
    GLuint DeathCount = 0;
};
// The Width of the screen
const GLuint SCREEN_WIDTH = 192 * 5;
// The height of the screen
const GLuint SCREEN_HEIGHT = 108 * 5;


class Game
{

public:
    GameState  State;// ��Ϸ״̬
    GLboolean  Keys[1024];
    GLuint     Width, Height;

    // ���캯��/��������
    Game(GLuint width, GLuint height);
    ~Game();
    // ��ʼ����Ϸ״̬���������е���ɫ��/����/�ؿ���
    void Init();
    // ��Ϸѭ��
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    void DoCollisions();//�����������������ײ

    // reset
    void ResetLevel();
    void ResetPlayer();

    //
    PostProcessor& GetPostProcessor() { return *Effects; }

    //enum
    bool isStartEnum;
    void ResetGameEnum();
    void ResetGame(int selectLevel);

public:
    SpriteTraigleRenderer* TraigleRenderer;
    SpriteRenderer* Renderer;//��Ⱦ��

    std::vector<GameLevel> Levels;
    GLuint                 Level = 0;//��ǰ�ؿ�

    GameObject* Player;

    BallObject* Ball;

    ParticleGenerator* Particles;

    PostProcessor* Effects;

    GLfloat            ShakeTime = 0.0f;

    PowerUp     PowerUps;

    ISoundEngine* SoundEngine = createIrrKlangDevice();

    TextRenderer* Text;

    //
    GameData GameDataLV;

    GLboolean KeysProcessed[1024];
};


Game& GetGameRef();

