
#include "stack_trace.h"
#include <boost/thread.hpp>

void b()
{
    debug::StackTrace st;
    st.Print();

    std::string str = st.ToString();
}

void a()
{
    b();
}

int main(int argc, char* argv[])
{
    a();

    boost::thread(b).join();

    return 0;
}

