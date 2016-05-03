#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

TEST_MAIN()
{
    constexpr auto m0 = map::make();
    constexpr auto m1 = map::insert(m0, pair::make(iv<0>, iv<10>));
    constexpr auto m2 = map::emplace(m1, iv<10>, iv<20>);

    constexpr auto fbi0 = map::find_all_indices_by_key(m2, iv<0>);
    SA(list::size(fbi0) == sv<1>);
    SA(list::at(fbi0, sv<0>) == sv<0>);

    constexpr auto fbi1 = map::find_all_indices_by_key(m2, iv<10>);
    SA(list::size(fbi1) == sv<1>);
    SA(list::at(fbi1, sv<0>) == sv<1>);

    constexpr auto fb0 = map::find_first_index_by_key(m2, iv<0>);
    SA(fb0 == sv<0>);

    constexpr auto fb1 = map::find_first_index_by_key(m2, iv<10>);
    SA(fb1 == sv<1>);

    constexpr auto fb2 = map::find_first_index_by_key(m2, iv<500>);
    SA(fb2 == null_v);

    constexpr auto v0 = map::find_first_by_key(m2, iv<0>);
    SA(v0 == iv<10>);

    constexpr auto v1 = map::find_first_by_key(m2, iv<10>);
    SA(v1 == iv<20>);

    constexpr auto v2 = map::find_first_by_key(m2, iv<500>);
    SA(v2 == null_v);
}
