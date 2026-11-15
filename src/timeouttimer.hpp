#pragma once

#include <stdint.h>
#include <memory>
#include <functional>

namespace Common {

/**
 * @brief The TimeoutTimer class    Timeout timer to handle timeouts in some cases, like algo or thread work debugging
 */
class TimeoutTimer
{
public:
    explicit TimeoutTimer(const std::string& timerName = "Generic");
    ~TimeoutTimer();

    // Argument - timer name
    using timerCallback_t = std::function<void(const std::string&)>;

    /**
     * @brief start         Start timeout thread, checking every Nms for elapsed. Calc table is in notes
     * @param timeoutMs     Wait time in ms
     * @param timeoutCbk    Callback to call on timeout
     * @throws              std::runtime_error If timeoutCbk not set
     * @note            Elapsed checking table:
     *          +---------------------------------------+
     *          | Timeout, ms     |  Check period, ms   |
     *          +---------------------------------------+
     *          | < 1000          |  10                 |
     *          | < 100'000       |  500                |
     *          | < 10'000'000    |  5'000              |
     *          | >= 10'000'000   |  1'000'000          |
     *          +---------------------------------------+
     */
    void start(uint64_t timeoutMs = 1000, timerCallback_t&& timeoutCbk = {}) noexcept(false);

    /**
     * @brief cancel Cancel timeout callback and stop timer
     */
    void cancel();

private:
    struct Impl;
    std::shared_ptr<Impl> d;

    void processTimeout(timerCallback_t&& timeoutCbk);
};

} // namespace Common
