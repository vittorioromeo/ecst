#include "../../utils/test_utils.hpp"

using namespace vrm::core;

TEST_MAIN()
{
    std::atomic<int> x{0};

    ecst::thread_pool pool;
    pool.post([&x]
        {
            x = 100;
        });

    while(x != 100)
    {
        ecst::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    TEST_ASSERT_NS(x == 100);
}
