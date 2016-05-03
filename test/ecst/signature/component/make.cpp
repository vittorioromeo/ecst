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
    constexpr auto c0t = signature::component::tag<fake_component0>;
    (void)c0t;

    constexpr auto c1t = signature::component::tag<fake_component1>;
    (void)c1t;

    constexpr auto c0_signature = signature::component::make_by_tag(c0t);
    (void)c0_signature;

    constexpr auto c1_signature = signature::component::make_by_tag(c1t);
    (void)c1_signature;

    constexpr auto cc0_signature =
        signature::component::make<fake_component0>();
    (void)cc0_signature;

    constexpr auto cc1_signature =
        signature::component::make<fake_component1>();
    (void)cc1_signature;
}
