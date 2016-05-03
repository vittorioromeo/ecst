#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void t6()
{
    constexpr auto l0 = list::make(int_v<0>, int_v<0>, int_v<2>, int_v<3>);

    STYP(                                                      // .
        list::find_indices_of_matching(l0, equal_to<int_<0>>), // .
        list::make(sz_v<0>, sz_v<1>)                           // .
        );

    STYP(                                     // .
        list::find_if(l0, equal_to<int_<0>>), // .
        int_v<0>                              // .
        );
}

TEST_MAIN()
{
    t6();
    return 0;
}
