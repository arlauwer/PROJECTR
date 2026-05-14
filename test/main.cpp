// test_main.cpp

#include "Log.hpp"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

#ifdef NDEBUG
    Log::level = Log::Level::Error;
#else
    Log::level = Log::Level::Info;
#endif

    return RUN_ALL_TESTS();
}