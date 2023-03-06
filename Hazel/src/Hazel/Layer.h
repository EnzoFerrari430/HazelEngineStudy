#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

#include "Hazel/Log.h"

namespace Hazel {

    class HAZEL_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {
            HZ_CORE_INFO("{0} EventType: {1}", __FUNCTION__, event.GetName());
        }

        inline const std::string& GetName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

}