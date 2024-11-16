#include "collision.h"


GLboolean collision::CheckCollision_AABB_AABB(GameObject& one, GameObject& two)
{
    // x�᷽����ײ��
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // y�᷽����ײ��
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // ֻ��������������ײʱ����ײ
    return collisionX && collisionY;
}

Collision  collision::CheckCollision_Circle_AABB(BallObject& one, GameObject& two)
{
    // ��ȡԲ������ 
    glm::vec2 center(one.Position + one.Radius);
    // ����AABB����Ϣ�����ġ���߳���
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y
    );
    // ��ȡ�������ĵĲ�ʸ��
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
    glm::vec2 closest = aabb_center + clamped;
    // ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
    difference = closest - center;

    //����tuple
    if (glm::length(difference) <= one.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

Direction collision::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // ��
        glm::vec2(1.0f, 0.0f),  // ��
        glm::vec2(0.0f, -1.0f), // ��
        glm::vec2(-1.0f, 0.0f)  // ��
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}