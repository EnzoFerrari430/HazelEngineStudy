#include <Hazel.h>

#include <glm/gtc/matrix_transform.hpp>


class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f)
        , m_CameraPosition(0.0f), m_SquarePosition(0.0f)
    {
        m_VertexArray.reset(Hazel::VertexArray::Create());
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
        };
        std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position", false},
            {Hazel::ShaderDataType::Float4, "a_Color", false}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
        uint32_t indices[3] = { 0, 1, 2 };
        indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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

        m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));





        m_BlueVertexArray.reset(Hazel::VertexArray::Create());
        float blueVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };
        std::shared_ptr<Hazel::VertexBuffer> blueVertexBuffer;
        blueVertexBuffer.reset(Hazel::VertexBuffer::Create(blueVertices, sizeof(blueVertices)));
        Hazel::BufferLayout blueLayout = {
            {Hazel::ShaderDataType::Float3, "a_Position", false}
        };
        blueVertexBuffer->SetLayout(blueLayout);
        m_BlueVertexArray->AddVertexBuffer(blueVertexBuffer);

        uint32_t blueIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<Hazel::IndexBuffer> blueIndexBuffer;
        blueIndexBuffer.reset(Hazel::IndexBuffer::Create(blueIndices, sizeof(blueIndices) / sizeof(uint32_t)));
        m_BlueVertexArray->SetIndexBuffer(blueIndexBuffer);

        std::string blueVertexSrc = R"(
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

        std::string blueFragmentSrc = R"(
            #version 330 core

            in vec3 v_Position;
            out vec4 color;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Hazel::Shader(blueVertexSrc, blueFragmentSrc));
    }

    void OnUpdate(Hazel::Timestep ts) override
    {
        HZ_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            m_CameraRotation += m_CameraRotationSpeed * ts;


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


        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Hazel::Renderer::BeginScene(m_Camera);

        for (int y = 0; y < 20; ++y) {
            for (int x = 0; x < 20; ++x) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition + pos) * scale; //矩阵右乘，一般先缩放，再旋转，最后位移
                Hazel::Renderer::Submit(m_BlueShader, m_BlueVertexArray, transform);
            }
        }

        Hazel::Renderer::Submit(m_Shader, m_VertexArray);
        Hazel::Renderer::EndScene();

    }

    void OnImGuiRender() override
    {

    }

private:
    std::shared_ptr<Hazel::Shader> m_Shader;
    std::shared_ptr<Hazel::VertexArray> m_VertexArray;

    std::shared_ptr<Hazel::Shader> m_BlueShader;
    std::shared_ptr<Hazel::VertexArray> m_BlueVertexArray;

    Hazel::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraMoveSpeed = 5.0f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 30.0f;

    glm::vec3 m_SquarePosition;
    float m_SquareMoveSpeed = 1.0f;
};

class SandBox : public Hazel::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
    }

    ~SandBox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandBox();
}