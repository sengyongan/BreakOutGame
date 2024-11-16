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


// 代表了游戏的当前状态
enum GameState {//游戏中，游戏菜单，游戏胜利
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
    GameState  State;// 游戏状态
    GLboolean  Keys[1024];
    GLuint     Width, Height;

    // 构造函数/析构函数
    Game(GLuint width, GLuint height);
    ~Game();
    // 初始化游戏状态（加载所有的着色器/纹理/关卡）
    void Init();
    // 游戏循环
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    void DoCollisions();//球体和其他物体检测碰撞

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
    SpriteRenderer* Renderer;//渲染器

    std::vector<GameLevel> Levels;
    GLuint                 Level = 0;//当前关卡

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

