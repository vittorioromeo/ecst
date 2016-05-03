#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

constexpr void eq()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>);
    constexpr auto l1 = list::make(iv<0>, iv<1>, iv<2>);
    constexpr auto l2 = list::make(iv<0>, iv<1>, iv<2>, iv<3>);

    SA_SDT(list::equal_size(l0, l1), bool_v<true>);
    SA_SDT(list::equal(l0, l1), bool_v<true>);
    SA_SDT(list::equal(l0, l2), bool_v<false>);
    SA_SDT(list::equal(l1, l2), bool_v<false>);

    SA(list::equal(
        list::make(iv<0>, iv<1>, iv<2>), list::make(iv<0>, iv<1>, iv<2>)));

    SA(list::equal(list::make(), list::make()));

    SA(!(list::equal(list::make(iv<0>), list::make())));
}

TEST_MAIN()
{
    eq();
    return 0;
}
