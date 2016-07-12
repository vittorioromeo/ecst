#include "../../utils/test_utils.hpp"
#include <ecst/utils.hpp>

using namespace ecst::impl;

TEST_MAIN()
{
    reverse_loop(0, 0, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(1, 1, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(1, 0, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(100, 0, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(0u, 0u, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(1u, 0u, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(100u, 0u, [](auto)
        {
            TEST_ASSERT(false);
        });

    reverse_loop(0u, 1u, [](auto)
        {
            TEST_ASSERT(true);
        });

    reverse_loop(0, 1, [](auto x)
        {
            TEST_ASSERT(x == 0);
        });

    reverse_loop(0, 5, [](auto x)
        {
            TEST_ASSERT(x >= 0 && x < 5);
        });
}
