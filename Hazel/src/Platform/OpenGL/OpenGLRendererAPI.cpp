#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include "stb_image_write.h"
#include <glad/glad.h>

namespace Hazel {

    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRendererAPI::ScreenShot(const char* filename)
    {
        int SCR_WIDTH = 1280;
        int SCR_HEIGHT = 720;
        int color_width = SCR_WIDTH;
        int color_height = SCR_HEIGHT;
        int color_x = 0;
        int color_y = 0;
        unsigned char* bytes = (unsigned char*)malloc(SCR_WIDTH * SCR_HEIGHT * 4);
        glReadPixels(color_x, color_y, color_width, color_height, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
        GLenum err = glGetError();

        stbi_flip_vertically_on_write(1);
        stbi_write_png(filename, color_width, color_height, 4, bytes, SCR_WIDTH * 4);

        free(bytes);
    }

}