#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Layer.h"

#include <vector>

namespace Hazel {

    /*
    图层Stack
    软件界面渲染分层进行 从前往后 界面渲染
    软件事件处理分层进行 从后往前 事件处理
    两个是相反的顺序,顶层最后渲染，最先处理事件
    */
    class HAZEL_API LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        //事件处理需要反向迭代器  界面渲染需要正向迭代器
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };

}