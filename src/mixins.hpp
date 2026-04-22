#pragma once

// Набор небольших утилитарных миксинов и декораторов

#include <mutex>
#include <condition_variable>

namespace Common
{

/**
 * @brief The SignalDecorator class Добавляет функционал ожидания сигнала в класс
 */
class SignalDecorator
{
public:
    void waitSignal();
    void emitSignal();
    void emitForAll();

private:
    std::condition_variable m_signalCv;
    std::mutex              m_waitMx;
};

}
