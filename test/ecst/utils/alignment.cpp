#include "../../utils/test_utils.hpp"
#include <ecst/utils/alignment.hpp>

using namespace ecst;

#define ATEST(a, b) TEST_ASSERT(aligned_ptr<8>((char*)a) == (char*)b)
#define BTEST(a, b) TEST_ASSERT(aligned_ptr<32>((char*)a) == (char*)b)

TEST_MAIN()
{
    ATEST(0, 0);

    ATEST(1, 8);
    ATEST(2, 8);
    ATEST(3, 8);
    ATEST(4, 8);
    ATEST(5, 8);
    ATEST(6, 8);
    ATEST(7, 8);
    ATEST(8, 8);

    ATEST(9, 16);
    ATEST(10, 16);
    ATEST(11, 16);
    ATEST(12, 16);
    ATEST(13, 16);
    ATEST(14, 16);
    ATEST(15, 16);
    ATEST(16, 16);

    ATEST(17, 24);

    // -

    BTEST(0, 0);

    BTEST(1, 32);
    BTEST(2, 32);
    BTEST(3, 32);
    BTEST(4, 32);
    BTEST(4, 32);
    BTEST(5, 32);
    BTEST(6, 32);
    BTEST(7, 32);
    BTEST(8, 32);
    // ...
    BTEST(30, 32);
    BTEST(31, 32);
    BTEST(32, 32);

    BTEST(33, 64);
    BTEST(34, 64);
    BTEST(35, 64);
    // ...
    BTEST(62, 64);
    BTEST(63, 64);
    BTEST(64, 64);

    BTEST(65, 96);
}
