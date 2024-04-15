#include "ExampleLayer.h"

#include <ImGui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
    : Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
    , m_SquarePosition(0.3f, 0.3f, 0.0f)
{
    m_VertexArray = Hazel::VertexArray::Create();
    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
    vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
    Hazel::BufferLayout layout = {
        {Hazel::ShaderDataType::Float3, "a_Position", false},
        {Hazel::ShaderDataType::Float4, "a_Color", false}
    };
    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
    uint32_t indices[3] = { 0, 1, 2 };
    indexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_VertexArray->SetIndexBuffer(indexBuffer);

    std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    std::string fragmentSrc = R"(
            #version 330 core

            in vec4 v_Color;
            in vec3 v_Position;
            out vec4 color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

    m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);





    m_FlatColorVertexArray = Hazel::VertexArray::Create();
    float flatColorVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    Hazel::Ref<Hazel::VertexBuffer> flatColorVertexBuffer;
    flatColorVertexBuffer = Hazel::VertexBuffer::Create(flatColorVertices, sizeof(flatColorVertices));
    Hazel::BufferLayout flatColorLayout = {
        {Hazel::ShaderDataType::Float3, "a_Position", false},
        {Hazel::ShaderDataType::Float2, "a_TexCoord", false},
    };
    flatColorVertexBuffer->SetLayout(flatColorLayout);
    m_FlatColorVertexArray->AddVertexBuffer(flatColorVertexBuffer);

    uint32_t flatColorIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Hazel::Ref<Hazel::IndexBuffer> flatColorIndexBuffer;
    flatColorIndexBuffer = Hazel::IndexBuffer::Create(flatColorIndices, sizeof(flatColorIndices) / sizeof(uint32_t));
    m_FlatColorVertexArray->SetIndexBuffer(flatColorIndexBuffer);

    std::string flatColorShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    std::string flatColorShaderFragmentSrc = R"(
            #version 330 core

            in vec3 v_Position;
            out vec4 color;
            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0f);
            }
        )";

    m_FlatColorShader = Hazel::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

    auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

    m_BoxTexture = Hazel::Texture2D::Create("assets/textures/container.jpg");
    m_Texture = Hazel::Texture2D::Create("assets/textures/awesomeface.png");

    textureShader->Bind();
    textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("res/fonts/msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(Hazel::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
        m_SquarePosition.x -= m_SquareMoveSpeed * ts;
    else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
        m_SquarePosition.x += m_SquareMoveSpeed * ts;

    if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
        m_SquarePosition.y += m_SquareMoveSpeed * ts;
    else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
        m_SquarePosition.y -= m_SquareMoveSpeed * ts;


    Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    Hazel::RenderCommand::Clear();

    Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
    glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

    m_FlatColorShader->Bind();//先绑定，否则(x = 0, y = 0)方块在第一帧时设置uniform失败
    m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

    for (int y = 0; y < 20; ++y)
    {
        for (int x = 0; x < 20; ++x)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, -0.2f); // 添加深度信息
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition + pos) * scale; //矩阵右乘，一般先缩放，再旋转，最后位移
            Hazel::Renderer::Submit(m_FlatColorShader, m_FlatColorVertexArray, transform);
        }
    }

    auto textureShader = m_ShaderLibrary.Get("Texture");

    textureShader->Bind();
    textureShader->SetFloat4("u_Color", glm::vec4(1.0f));
    textureShader->SetFloat("u_TilingFactor", 1.0f);
    m_BoxTexture->Bind();
    Hazel::Renderer::Submit(textureShader, m_FlatColorVertexArray, glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, -0.1f}) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    m_Texture->Bind(); //不用每一帧都绑定一下，静态贴图绑定一次就好了
    Hazel::Renderer::Submit(textureShader, m_FlatColorVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    // Triangle
    //Hazel::Renderer::Submit(m_Shader, m_VertexArray);

    Hazel::Renderer::EndScene();

}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color1", glm::value_ptr(m_SquareColor));
    ImGui::End();

    //static bool show = true;
    //ImGui::ShowDemoWindow(&show);
}

void ExampleLayer::OnEvent(Hazel::Event& e)
{
    m_CameraController.OnEvent(e);
}
