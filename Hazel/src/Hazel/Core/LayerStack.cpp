#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

    LayerStack::LayerStack()
    {
    }

    // LayerStack��һ����ĳ�Ա������ʱ�򣬵�����ʵ��������ʱ�������������ᱻ���õ�
    // �ڴ�Ͳ���й¶
    LayerStack::~LayerStack()
    {
        for (Layer* layer : m_Layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
        ++m_LayerInsertIndex;
    }

    void LayerStack::PushOverlay(Layer* layer)
    {
        m_Layers.emplace_back(layer);
    }

    // �ڴ�й¶
    // Popֻ��Pop��ָ�룬 ����������ڴ滹û���ͷ�
    // ���������Ӧ�ñ����õ�
    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            --m_LayerInsertIndex;
        }
    }

    void LayerStack::PopOverlay(Layer* layer)
    {
        auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
        if (it != m_Layers.end())
            m_Layers.erase(it);
    }

}