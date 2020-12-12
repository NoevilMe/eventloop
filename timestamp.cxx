#include "timestamp.h"

namespace muduo {
namespace event_loop {

Timestamp::Timestamp() : nanoseconds_since_epoch_(0) {}

Timestamp::Timestamp(int64_t nanoseconds)
    : nanoseconds_since_epoch_(nanoseconds) {}

Timestamp::~Timestamp() {}

bool Timestamp::valid() const { return nanoseconds_since_epoch_ > 0; }

int64_t Timestamp::nanoseconds_since_epoch() const {
    return nanoseconds_since_epoch_;
}

int64_t Timestamp::microseconds_since_epoch() const {
    return nanoseconds_since_epoch_ / kNanoSecondsPerMicroSecond;
}

int64_t Timestamp::milliseconds_since_epoch() const {
    return nanoseconds_since_epoch_ / kNanoSecondsPerMilliSecond;
}

time_t Timestamp::seconds_since_epoch() const {
    return static_cast<time_t>(nanoseconds_since_epoch_ /
                               kNanoSecondsPerSecond);
}

std::string Timestamp::ToString() const {
    char buf[32] = {0};
    int64_t seconds = nanoseconds_since_epoch_ / kNanoSecondsPerSecond;
    int64_t nanoseconds = nanoseconds_since_epoch_ % kNanoSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%lld.%09lld", seconds, nanoseconds);
    return buf;
}

struct timespec Timestamp::TimespecNow() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts;
}

Timestamp Timestamp::Now() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return FromTimespec(ts);
}

Timestamp Timestamp::FromMicroSecondsSinceEpoch(int64_t microseconds) {
    return Timestamp(microseconds * kNanoSecondsPerMicroSecond);
}

Timestamp Timestamp::FromMilliSecondsSinceEpoch(int64_t milliseconds) {
    return Timestamp(milliseconds * kNanoSecondsPerMilliSecond);
}

Timestamp Timestamp::FromUTC(time_t t) { return FromUTC(t, 0); }

Timestamp Timestamp::FromUTC(time_t t, int nanoseconds) {
    return Timestamp(static_cast<int64_t>(t) * kNanoSecondsPerSecond +
                     nanoseconds);
}

Timestamp Timestamp::FromTimespec(const struct timespec &ts) {
    return Timestamp(static_cast<int64_t>(ts.tv_sec) * kNanoSecondsPerSecond +
                     ts.tv_nsec);
}

} // namespace event_loop
} // namespace muduo