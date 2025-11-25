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

        void clearSlots()
        {
            m_slots.clear();
        }

    private:
        std::vector<Slot> m_slots;
    };

    #define HANDLE_EVENT(sender, signal, receiver, callback)        \
        sender->signal.connect(                                     \
            [receiver](auto&&... args) { callback(args...); }       \
        )

    #define SIGNAL(name, ...) Signal<__VA_ARGS__> name;
    #define SIGNAL_NAMESPACE_BEGIN \
        struct SignalRegistry {     \
        public:

    #define SIGNAL(name, ...) Signal<__VA_ARGS__> name;

    #define SIGNAL_NAMESPACE_END \
        } m_signals;             \
    public:                      \
        auto& signals() { return m_signals; }

    #define HANDLE_SIGNAL(sender, signal, receiver, callback) \
        (sender)->signals().signal.connect(                    \
            [receiver](auto&&... args) { callback(args...); } \
        )

    #define CALL_SIGNAL(signal, ...) \
        m_signals.signal.emit(__VA_ARGS__)

    #endif //SIGNAL_HPP

