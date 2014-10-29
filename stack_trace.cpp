#include "stack_trace.h"

#include <algorithm>
#include <sstream>

namespace debug {

StackTrace::StackTrace(const void* const* trace, size_t count) {
    count = std::min(count, arraysize(trace_));
    if (count)
        memcpy(trace_, trace, count * sizeof(trace_[0]));
    count_ = count;
}

StackTrace::~StackTrace() {
}

const void *const *StackTrace::Addresses(size_t* count) const {
    *count = count_;
    if (count_)
        return trace_;
    return NULL;
}

std::string StackTrace::ToString() const {
    std::stringstream stream;
    OutputToStream(&stream);
    return stream.str();
}

} //namespace debug

