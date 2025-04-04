#pragma once

#include <glm/glm.hpp>

namespace Hazel {

    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar);

        void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);//proj

        void SetPosition(const glm::vec3& position);
        const glm::vec3& GetPosition() const { return m_Position; }

        void SetRotation(float rotation);
        float GetRotation() const { return m_Rotation; }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
    private:
        void RecalculateViewMatrix();
    private:
        glm::mat4x4 m_ProjectionMatrix;
        glm::mat4x4 m_ViewMatrix;
        glm::mat4x4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = glm::vec3(0.0f);
        float m_Rotation = 0.0f;
    };

}