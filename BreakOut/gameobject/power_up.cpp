#include "power_up.h"

GLboolean ShouldSpawn(GLuint chance)
{
    GLuint random = rand() % chance;
    return random == 0;
}

GLboolean isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
    for (const PowerUp& powerUp : powerUps)
    {
        if (powerUp.Activated)
            if (powerUp.Type == type)
                return GL_TRUE;
    }
    return GL_FALSE;
}

PowerUp::PowerUp()
{
}

void PowerUp::SpawnPowerUps(GameObject& block)
{
     if (ShouldSpawn(10)) // 1 in 75 chance
        this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 2.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
    if (ShouldSpawn(10))
        this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 2.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
    if (ShouldSpawn(10))
        this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 2.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
    if (ShouldSpawn(10))
        this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 2.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
    if (ShouldSpawn(10)) // Negative powerups should spawn more often
        this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 1.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
    if (ShouldSpawn(3))
        this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 3.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void PowerUp::UpdatePowerUps(GLfloat dt, BallObject* ball, GameObject* player, PostProcessor* effects)
{
    for (PowerUp& powerUp : this->PowerUps)
    {
        powerUp.Position += powerUp.Velocity * dt * 5.0f;
        if (powerUp.Activated)
        {
            powerUp.Duration -= dt;

            if (powerUp.Duration <= 0.0f)
            {
                // 之后会将这个道具移除
                powerUp.Activated = GL_FALSE;
                // 停用效果
                if (powerUp.Type == "sticky")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "sticky"))
                    {   // 仅当没有其他sticky效果处于激活状态时重置，以下同理
                        ball->Sticky = GL_FALSE;
                        player->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "pass-through")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
                    {
                        ball->PassThrough = GL_FALSE;
                        ball->Color = glm::vec3(1.0f);
                    }
                }
                else if (powerUp.Type == "confuse")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "confuse"))
                    {
                        effects->Confuse = GL_FALSE;
                    }
                }
                else if (powerUp.Type == "chaos")
                {
                    if (!isOtherPowerUpActive(this->PowerUps, "chaos"))
                    {
                        effects->Chaos = GL_FALSE;
                    }
                }
            }
        }
    }
    this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
        [](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
    ), this->PowerUps.end());
}

void PowerUp::ActivatePowerUp(PowerUp& powerUp, BallObject* ball, GameObject* player, PostProcessor* effects)
{
    // 根据道具类型发动道具
    if (powerUp.Type == "speed")
    {
        ball->Velocity *= 1.2;
    }
    else if (powerUp.Type == "sticky")
    {
        ball->Sticky = GL_TRUE;
        player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (powerUp.Type == "pass-through")
    {
        ball->PassThrough = GL_TRUE;
        ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (powerUp.Type == "pad-size-increase")
    {
        player->Size.x += 50;
    }
    else if (powerUp.Type == "confuse")
    {
        if (!effects->Chaos)
            effects->Confuse = GL_TRUE; // 只在chaos未激活时生效，chaos同理
    }
    else if (powerUp.Type == "chaos")
    {
        if (!effects->Confuse)
            effects->Chaos = GL_TRUE;
    }
}
