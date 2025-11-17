#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <functional>
#include <vector>

template<typename... Args>
class Signal
{
public:
    using Slot = std::function<void(Args...)>;

    void connect(const Slot& slot)
    {
        m_slots.push_back(slot);
    }

    void emit(Args... args)
    {
        for(const auto& slot : m_slots)
            slot(args...);
    }

private:
    std::vector<Slot> m_slots;
};

#define HANDLE_EVENT(sender, signal, receiver, callback)        \
    sender->signal.connect(                                     \
        [receiver](auto&&... args) { callback(args...); }       \
    )
#endif //SIGNAL_HPP