#pragma once

#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;
private:
    Hazel::ShaderLibrary m_ShaderLibrary;
    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;

    Hazel::Ref<Hazel::Shader> m_FlatColorShader;
    Hazel::Ref<Hazel::VertexArray> m_FlatColorVertexArray;

    Hazel::Ref<Hazel::Texture2D> m_Texture;
    Hazel::Ref<Hazel::Texture2D> m_BoxTexture;

    Hazel::OrthographicCameraController m_CameraController;

    glm::vec3 m_SquarePosition;
    float m_SquareMoveSpeed = 1.0f;
    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};