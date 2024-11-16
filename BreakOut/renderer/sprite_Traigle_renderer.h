#pragma once

#include "../tool/shader.h"
#include "../tool/texture.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SpriteTraigleRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteTraigleRenderer(const Shader& shader);
    // Destructor
    ~SpriteTraigleRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
private:
    unsigned int VBO, VAO;
    Shader       shader;
};

