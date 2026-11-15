#include "timeouttimer.hpp"

#include <thread>
#include <chrono>

namespace Common {

struct TimeoutTimer::Impl
{
    std::string m_name;
    std::atomic_bool m_isWorking {false};
    std::atomic_bool m_isCanceled {false};
    std::mutex       m_completeMx; // To process outer thread stop
};

TimeoutTimer::TimeoutTimer(const std::string &timerName) :
    d {new Impl}
{
    d->m_name = timerName;
}

TimeoutTimer::~TimeoutTimer()
{
    cancel();
    std::lock_guard<std::mutex> lock(d->m_completeMx); // Wait for thread to complete
}

void TimeoutTimer::cancel()
{
    d->m_isCanceled.store(true, std::memory_order_relaxed);
}

void TimeoutTimer::processTimeout(timerCallback_t &&timeoutCbk)
{
    if (timeoutCbk) {
        timeoutCbk(d->m_name);
    } else {
        throw std::runtime_error(d->m_name + " (TimeoutTimer): timeout");
    }
}

void TimeoutTimer::start(uint64_t timeoutMs, timerCallback_t&& timeoutCbk) noexcept(false)
{
    d->m_isCanceled.store(false, std::memory_order_relaxed);
    if (timeoutMs < 1) {
        processTimeout(std::move(timeoutCbk));
        return;
    }

    std::thread([this, timeoutMs, cbk = std::move(timeoutCbk)]() mutable {
        uint64_t skipTimeCountTmp {};
        if (timeoutMs < 1000) { skipTimeCountTmp = 10; } else
        if (timeoutMs < 100'000) { skipTimeCountTmp = 500; } else
        if (timeoutMs < 10'000'000) { skipTimeCountTmp = 5'000; } else
        { skipTimeCountTmp = 1'000'000; }
        const auto& skipTimeCount = skipTimeCountTmp; // To avoid editing

        std::lock_guard<std::mutex> lock(d->m_completeMx); // To make sure thread will not emit signals on parent removal
        for (uint64_t currentVal = 0;
             (currentVal < timeoutMs) && !d->m_isCanceled.load(std::memory_order_relaxed);
             currentVal+= skipTimeCount) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::milliseconds(std::min(currentVal + skipTimeCount, timeoutMs - currentVal)));
        }
        if (!d->m_isCanceled.load(std::memory_order_relaxed)) {
            processTimeout(std::move(cbk));
        }
    }).detach();
}

} // namespace Common
