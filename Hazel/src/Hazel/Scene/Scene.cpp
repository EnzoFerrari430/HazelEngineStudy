#include "hzpch.h"
#include "Scene.h"
#include "Components.h"

#include "Hazel/Renderer/Renderer2D.h"

#include "Entity.h"

namespace Hazel {

    //static void DoMath(const glm::mat4& transform)
    //{
    //
    //}

    Scene::Scene()
    {
        //{

        //    struct MeshComponent
        //    {
        //        float vertices[3];
        //    };

        //    struct TransformComponent
        //    {
        //        glm::mat4 Transform;

        //        TransformComponent() = default;
        //        TransformComponent(const TransformComponent&) = default;
        //        TransformComponent(const glm::mat4& transform)
        //            : Transform(transform) {}

        //        operator glm::mat4& () { return Transform; }
        //        operator const glm::mat4&() const { return Transform; }
        //    };

        //    // for test
        //    //create entity
        //    entt::entity entity = m_Registry.create();

        //    //add component
        //    TransformComponent& transform = m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
        //    MeshComponent& mesh = m_Registry.emplace<MeshComponent>(entity, 1.0f, 1.0f, 1.0f);

        //    transform = glm::mat4(2.0f);
        //    TransformComponent& transform2 = m_Registry.get<TransformComponent>(entity);

        //    // 轮询一个component
        //    auto view = m_Registry.view<TransformComponent>();
        //    for (auto entity : view)
        //    {
        //        TransformComponent& transform = view.get<TransformComponent>(entity);
        //    }

        //    // 轮询多个component的组合
        //    auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
        //    for (auto entity : group)
        //    {
        //        auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
        //    }

        //}
    }

    Scene::~Scene()
    {

    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }
    }

}