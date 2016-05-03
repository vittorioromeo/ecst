#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;

struct fake_component0
{
};

struct fake_component1
{
};

TEST_MAIN()
{
    auto c0t = signature::component::tag<fake_component0>;
    (void)c0t;

    auto c1t = signature::component::tag<fake_component1>;
    (void)c1t;
}
