#pragma once

#include "entt/entt.hpp"

#include "Hazel/Core/Timestep.h"

namespace Hazel {

    class Scene
    {
    public:
        Scene();
        ~Scene();

        entt::entity CreateEntity();

        // TEMP
        entt::registry& Reg() { return m_Registry; }

        void OnUpdate(Timestep ts);
    private:
        // 保存所有components与entities(identifiers)的容器
        // components保存所有数据，与entities的identifiers关联起来
        entt::registry m_Registry;
    };

}