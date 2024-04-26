#pragma once

#include <glm/glm.hpp>

namespace Hazel {

    // 对于Render 只需要一个matrix 所以Camera类尽量保持简约
    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
            : m_Projection(projection) {}

        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const { return m_Projection; }
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
    };

}