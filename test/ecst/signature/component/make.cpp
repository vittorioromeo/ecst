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
    constexpr auto c0t = tag::component::v<fake_component0>;
    (void)c0t;

    constexpr auto c1t = tag::component::v<fake_component1>;
    (void)c1t;

    constexpr auto c0_signature = signature::component::make(c0t);
    (void)c0_signature;

    constexpr auto c1_signature = signature::component::make(c1t);
    (void)c1_signature;
}
