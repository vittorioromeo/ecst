#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

TEST_MAIN()
{
    constexpr auto m0 = map::make();
    SA(map::valid(m0));

    constexpr auto m1 = map::insert(m0, pair::make(iv<0>, iv<10>));
    SA(map::valid(m1));

    SA(!map::valid(list::make(iv<0>)));

    constexpr auto m2 = map::emplace(m1, iv<10>, iv<20>);
    SA(map::valid(m2));

    SA(pair::fst_is(list::at(m2, sv<0>), iv<0>));
}
