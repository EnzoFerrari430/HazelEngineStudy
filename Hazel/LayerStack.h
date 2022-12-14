#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

/*
LayerStack����ÿһ����Ⱦͼ��(called Layer)
��Ⱦ��ʱ���ǰ������Ⱦ����֤���Ҫ��Ⱦ��ͼ�������ϲ�
�¼������ʱ��Ӻ���ǰ������֤���ϲ��ͼ���ȴ����¼�
*/
namespace Hazel {

	class HAZEL_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() {}
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}