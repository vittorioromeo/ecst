#include "../../utils/test_utils.hpp"

using namespace vrm::core;

TEST_MAIN()
{
    ecst::thread_pool pool;
    std::atomic<sz_t> c{0};
    std::atomic<int> acc{0};

    int test_acc = 0;
    for(int i = 0; i < 100; ++i)
    {
        test_acc += i;
        pool.post([i, &c, &acc]
            {
                acc += i;
                ++c;
            });
    }

    while(c != 100)
    {
        ecst::this_thread::sleep_for(ecst::chrono::milliseconds(1));
    }

    TEST_ASSERT_NS(c == 100);
    TEST_ASSERT_NS(acc == test_acc);
}
