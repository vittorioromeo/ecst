#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void t0()
{
    auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>);

    SA_SDT(                               // .
        (list::remove_at(l0, sv<0>)),     // .
        (list::make(iv<1>, iv<2>, iv<3>)) // .
        );

    SA_SDT(                               // .
        (list::remove_at(l0, sv<1>)),     // .
        (list::make(iv<0>, iv<2>, iv<3>)) // .
        );

    SA_SDT(                                     // .
        (list::remove_slice(l0, sv<1>, sv<3>)), // .
        (list::make(iv<0>, iv<3>))              // .
        );
}

TEST_MAIN()
{
    return 0;
}
