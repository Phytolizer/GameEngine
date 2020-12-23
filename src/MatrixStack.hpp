#pragma once

#include <glm/glm.hpp>
#include <stack>

class MatrixStack
{
    std::stack<glm::mat4> m_matrices;

  public:
    MatrixStack();
    void Push();
    void Pop();
    void Translate(glm::vec2 where);
    void Rotate(float theta);
    void Scale(glm::vec2 scale);
    glm::mat4 &Top();
};
