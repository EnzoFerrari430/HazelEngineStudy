#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

/*
LayerStack保存每一个渲染图层(called Layer)
渲染的时候从前往后渲染，保证最后要渲染的图层在最上层
事件处理的时候从后往前处理，保证最上层的图层先处理事件
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