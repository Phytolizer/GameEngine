#include "MatrixStack.hpp"
#include <glm/gtc/matrix_transform.hpp>
MatrixStack::MatrixStack() = default;
void MatrixStack::Push()
{
    m_matrices.push(glm::mat3(1.0f));
}
void MatrixStack::Pop()
{
    m_matrices.pop();
}
void MatrixStack::Translate(glm::vec2 where)
{
    m_matrices.top() = glm::translate(glm::mat4(1.0f), glm::vec3(where, 1.0f)) *
                       m_matrices.top();
}
void MatrixStack::Rotate(float theta)
{
    m_matrices.top() =
        glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0.0f, 0.0f, 1.0f)) *
        m_matrices.top();
}
void MatrixStack::Scale(glm::vec2 scale)
{
    m_matrices.top() =
        glm::scale(glm::mat4(1.0f), glm::vec3(scale, 1.0f)) * m_matrices.top();
}
glm::mat4 &MatrixStack::Top()
{
    return m_matrices.top();
}