#include "../../utils/test_utils.hpp"

using namespace vrm::core;

TEST_MAIN()
{
    (void)ecst::hardware::status::core_count();
    (void)ecst::hardware::status::core_count_known();
}
