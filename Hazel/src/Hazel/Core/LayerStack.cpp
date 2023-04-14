#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

    LayerStack::LayerStack()
    {
    }

    // LayerStack是一个类的成员变量的时候，当父类实例被析构时，该析构函数会被调用到
    // 内存就不会泄露
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

    // 内存泄露
    // Pop只是Pop了指针， 但是申请的内存还没有释放
    // 这个函数不应该被调用到
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