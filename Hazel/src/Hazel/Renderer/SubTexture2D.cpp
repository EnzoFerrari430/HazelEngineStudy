#include "hzpch.h"

#include "SubTexture2D.h"

namespace Hazel {

    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
        : m_Texture(texture)
    {
        m_TexCoords[0] = { min.x, min.y };
        m_TexCoords[1] = { max.x, min.y };
        m_TexCoords[2] = { max.x, max.y };
        m_TexCoords[3] = { min.x, max.y };
    }

    // ����texture��ˮƽ��ת�� ����Ĵ����coords�ǻ�û�з�ת������
    // �ڲ�Ҫ���з�ת
    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize)
    {
        float sheetWidth = texture->GetWidth(), sheetHeight = texture->GetHeight();
        float spriteWidth = spriteSize.x, spriteHeight = spriteSize.y;

        // min and max
        glm::vec2 min = { coords.x / sheetWidth, (sheetHeight - coords.y - spriteHeight) / sheetHeight };
        glm::vec2 max = { (coords.x + spriteWidth) / sheetWidth, (sheetHeight - coords.y) / sheetHeight };

        return CreateRef<SubTexture2D>(texture, min, max);
    }

}