#pragma once

#include "Hazel/Core/Core.h"
#include "hzpch.h"

namespace Hazel {

    // Events in Hazel are currently blocking, meaning when an event occurs it
    // immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of eht update stage.

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WIndowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    //对事件进行分类， 用于过滤
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication        = BIT(0),
        EventCategoryInput              = BIT(1),
        EventCategoryKeyboard           = BIT(2),
        EventCategoryMouse              = BIT(3),
        EventCategoryMouseButton        = BIT(4)
    };

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                                virtual EventType GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class HAZEL_API Event
    {
        friend class EventDispatcher;
    public:
        bool Handled = false;  //是否被处理

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }
        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class HAZEL_API EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_Event(event) {}

        // F will be deduced by the compiler
        template <typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    // fmt/ostream.h provides std::ostream support including formatting of user-defined types that have an overloaded 
    // insertion operator (operator<<).In order to make a type formattable via std::ostream you should provide a formatter 
    // specialization inherited from ostream_formatter :
    // 用于spdlog输出自定义的事件信息
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}