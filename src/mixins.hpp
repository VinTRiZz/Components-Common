#pragma once

// Набор небольших утилитарных миксинов и декораторов

#include <thread>
#include <mutex>
#include <condition_variable>

namespace Common
{

class SignalDecorator
{
public:
    void waitSignal() {
        std::unique_lock<std::mutex> lock(m_waitMx);
        m_signalCv.wait(lock);
    }

    void emitSignal() {
        std::unique_lock<std::mutex> lock(m_waitMx);
        m_signalCv.notify_one();
    }

    void emitForAll() {
        std::unique_lock<std::mutex> lock(m_waitMx);
        m_signalCv.notify_all();
    }

private:
    std::condition_variable m_signalCv;
    std::mutex              m_waitMx;
};

}
