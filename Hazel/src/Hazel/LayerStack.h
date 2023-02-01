#pragma once

#include "Hazel/Core.h"
#include "Hazel/Layer.h"

#include <vector>

namespace Hazel {

    /*
    ͼ��Stack
    ���������Ⱦ�ֲ���� ��ǰ���� ������Ⱦ
    ����¼�����ֲ���� �Ӻ���ǰ �¼�����
    �������෴��˳��,���������Ⱦ�����ȴ����¼�
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
        //�¼�������Ҫ���������  ������Ⱦ��Ҫ���������
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex = 0;
    };

}