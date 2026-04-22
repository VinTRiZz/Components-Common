#include "mixins.hpp"

void Common::SignalDecorator::waitSignal() {
    std::unique_lock<std::mutex> lock(m_waitMx);
    m_signalCv.wait(lock);
}

void Common::SignalDecorator::emitSignal() {
    std::unique_lock<std::mutex> lock(m_waitMx);
    m_signalCv.notify_one();
}

void Common::SignalDecorator::emitForAll() {
    std::unique_lock<std::mutex> lock(m_waitMx);
    m_signalCv.notify_all();
}
