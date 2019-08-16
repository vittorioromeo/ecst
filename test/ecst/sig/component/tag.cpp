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
    auto c0t = tag::component::v<fake_component0>;
    (void)c0t;

    auto c1t = tag::component::v<fake_component1>;
    (void)c1t;
}
