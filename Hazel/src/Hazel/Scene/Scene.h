#pragma once

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/EditorCamera.h"

#include "entt/entt.hpp"

namespace Hazel {

    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateRuntime(Timestep ts);  // 游戏运行时更新函数
        void OnUpdateEditor(Timestep ts, EditorCamera& camera);   // 编辑状态下更新函数
        void OnViewportResize(uint32_t width, uint32_t height);

        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        // 保存所有components与entities(identifiers)的容器
        // components保存所有数据，与entities的identifiers关联起来
        entt::registry m_Registry;

        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}