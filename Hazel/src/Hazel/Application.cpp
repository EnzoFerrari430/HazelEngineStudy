#include "hzpch.h"
#include "Application.h"

#include "Hazel/Renderer/Renderer.h"

#include "Hazel/Input.h" 

namespace Hazel {

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_VertexArray.reset(VertexArray::Create());
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
        };
        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position", false},
            {ShaderDataType::Float4, "a_Color", false}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        std::shared_ptr<IndexBuffer> indexBuffer;
        uint32_t indices[3] = { 0, 1, 2 };
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;
            out vec4 v_Color;

            void main()
            {
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            in vec4 v_Color;
            out vec4 color;

            void main()
            {
                color = v_Color;
            }
        )";

        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));





        m_BlueVertexArray.reset(VertexArray::Create());
        float blueVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f,  0.75f, 0.0f
        };
        std::shared_ptr<VertexBuffer> blueVertexBuffer;
        blueVertexBuffer.reset(VertexBuffer::Create(blueVertices, sizeof(blueVertices)));
        BufferLayout blueLayout = {
            {ShaderDataType::Float3, "a_Position", false}
        };
        blueVertexBuffer->SetLayout(blueLayout);
        m_BlueVertexArray->AddVertexBuffer(blueVertexBuffer);

        uint32_t blueIndices[6] = { 0, 1, 2, 2, 3, 0 };
        std::shared_ptr<IndexBuffer> blueIndexBuffer;
        blueIndexBuffer.reset(IndexBuffer::Create(blueIndices, sizeof(blueIndices) / sizeof(uint32_t)));
        m_BlueVertexArray->SetIndexBuffer(blueIndexBuffer);

        std::string blueVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        std::string blueFragmentSrc = R"(
            #version 330 core

            out vec4 color;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Shader(blueVertexSrc, blueFragmentSrc));
    }
    
    Application::~Application()
    {
    }

    void Application::PushLayer(Layer* layer)
    {
        m_Layerstack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_Layerstack.PushOverlay(layer);
        layer->OnAttach();
    }

    //事件集中调度
    void Application::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

        //HZ_CORE_INFO("{0}", event);

        //事件处理，从后往前
        for (auto it = m_Layerstack.end(); it != m_Layerstack.begin();)
        {
            (*--it)->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    /*
    TODO:
    这里要对逻辑和渲染进行区分
    OnLogical
    OnRender
    两个步骤
    */
    void Application::Run()
    {
        while (m_Running)
        {
            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            RenderCommand::Clear();

            Renderer::BeginScene();
            m_BlueShader->Bind();
            Renderer::Submit(m_BlueVertexArray);

            m_Shader->Bind();
            Renderer::Submit(m_VertexArray);
            Renderer::EndScene();
            
            //图像渲染 从前往后
            for (Layer* layer : m_Layerstack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_Layerstack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
            
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

}
