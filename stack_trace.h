#ifndef STACK_STRCE_H
#define STACK_STRCE_H

#include <iosfwd>
#include <string>

#ifdef WIN32
#define OS_WIN
#endif

#ifdef _MSC_VER
#define COMPILER_MSVC
#endif

#if defined(OS_POSIX)
#include <unistd.h>
#endif

#if defined(OS_WIN)
struct _EXCEPTION_POINTERS;
#endif

// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef _MSC_VER
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

namespace debug {

bool EnableInProcessStackDumping();

class StackTrace
{
public:
    // Creates a stacktrace from the current location.
    StackTrace();

    // Creates a stacktrace from an existing array of instruction
    // pointers (such as returned by Addresses()).  |count| will be
    // trimmed to |kMaxTraces|.
    StackTrace(const void* const* trace, size_t count);

#if defined(OS_WIN)
    // Creates a stacktrace for an exception.
    // Note: this function will throw an import not found (StackWalk64) exception
    // on system without dbghelp 5.1.
    StackTrace(const _EXCEPTION_POINTERS* exception_pointers);
#endif

    ~StackTrace();

    // Gets an array of instruction pointer values. |*count| will be set to the
    // number of elements in the returned array.
    const void* const* Addresses(size_t* count) const;

    // Prints the stack trace to stderr.
    void Print() const;

    // Resolves backtrace to symbols and write to stream.
    void OutputToStream(std::ostream* os) const;

    // Resolves backtrace to symbols and returns as string.
    std::string ToString() const;

private:
    // From http://msdn.microsoft.com/en-us/library/bb204633.aspx,
    // the sum of FramesToSkip and FramesToCapture must be less than 63,
    // so set it to 62. Even if on POSIX it could be a larger value, it usually
    // doesn't give much more information.
    static const int kMaxTraces = 62;

    void* trace_[kMaxTraces];

    // The number of valid frames in |trace_|.
    size_t count_;
};

} //namespace debug

#endif
