#include "../../utils/test_utils.hpp"

void t0()
{
    // ecst::fixed_set<100> ss;
    vrm::core::fixed_vector_sparse_set<unsigned int, 100> ss;

    TEST_ASSERT(ss.empty());
    TEST_ASSERT_OP(ss.size(), ==, 0);

    TEST_ASSERT(ss.add(0) == true);
    TEST_ASSERT(ss.add(0) == false);
    TEST_ASSERT(ss.add(0) == false);

    TEST_ASSERT(!ss.empty());
    TEST_ASSERT_OP(ss.size(), ==, 1);
    TEST_ASSERT(ss.has(0));
    TEST_ASSERT(!ss.has(1));
    TEST_ASSERT(!ss.has(2));

    TEST_ASSERT(ss.add(1) == true);
    TEST_ASSERT(ss.add(1) == false);

    TEST_ASSERT(!ss.empty());
    TEST_ASSERT_OP(ss.size(), ==, 2);
    TEST_ASSERT(ss.has(0));
    TEST_ASSERT(ss.has(1));
    TEST_ASSERT(!ss.has(2));

    TEST_ASSERT(ss.add(2) == true);
    TEST_ASSERT(ss.add(2) == false);

    TEST_ASSERT(!ss.empty());
    TEST_ASSERT_OP(ss.size(), ==, 3);
    TEST_ASSERT(ss.has(0));
    TEST_ASSERT(ss.has(1));
    TEST_ASSERT(ss.has(2));
}

TEST_MAIN()
{
    t0();
}
