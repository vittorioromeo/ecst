#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

TEST_MAIN()
{
    auto p0 = pair::make(iv<0>, iv<1>);

    SA_SDT(              // .
        (pair::fst(p0)), // .
        (iv<0>)          // .
        );

    SA_SDT(              // .
        (pair::snd(p0)), // .
        (iv<1>)          // .
        );

    auto p1 = pair::replace_snd(p0, iv<3>);

    SA_SDT(              // .
        (pair::fst(p1)), // .
        (iv<0>)          // .
        );

    SA_SDT(              // .
        (pair::snd(p1)), // .
        (iv<3>)          // .
        );

    return 0;
}
