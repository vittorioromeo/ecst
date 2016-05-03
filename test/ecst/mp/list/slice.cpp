#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void slices()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>);

    SA(list::valid_interval(l0, iv<0>, iv<5>));
    SA(list::valid_interval(l0, iv<0>, iv<0>));
    SA(!list::valid_interval(l0, iv<-1>, iv<5>));
    SA(!list::valid_interval(l0, iv<0>, iv<6>));

    SA_SDT(                                           // .
        (list::make_interval_sequence(iv<0>, iv<0>)), // .
        (list::make())                                // .
        );

    SA_SDT(                                           // .
        (list::make_interval_sequence(iv<0>, iv<1>)), // .
        (list::make(sv<0>))                           // .
        );

    SA_SDT(                                           // .
        (list::make_interval_sequence(iv<0>, iv<2>)), // .
        (list::make(sv<0>, sv<1>))                    // .
        );

    SA_SDT(                                           // .
        (list::make_interval_sequence(iv<1>, iv<4>)), // .
        (list::make(sv<1>, sv<2>, sv<3>))             // .
        );

    SA_SDT(                              // .
        (list::slice(l0, iv<0>, iv<0>)), // .
        (list::make())                   // .
        );

    SA_SDT(                                       // .
        (list::slice(l0, iv<0>, list::size(l0))), // .
        (l0)                                      // .
        );

    SA_SDT(                              // .
        (list::slice(l0, iv<0>, iv<2>)), // .
        (list::make(iv<0>, iv<1>))       // .
        );

    SA_SDT(                              // .
        (list::slice(l0, iv<2>, iv<4>)), // .
        (list::make(iv<2>, iv<3>))       // .
        );
}

constexpr void t3()
{
    constexpr auto l0 = list::make(int_v<0>, int_v<1>, int_v<2>, int_v<3>);

    ECST_S_ASSERT(list::size(l0) == 4);

    STYP(                      // .
        list::at(l0, sz_v<0>), // .
        int_v<0>);

    STYP(                      // .
        list::at(l0, sz_v<1>), // .
        int_v<1>);

    STYP(                      // .
        list::at(l0, sz_v<2>), // .
        int_v<2>);

    STYP(                      // .
        list::at(l0, sz_v<3>), // .
        int_v<3>);

    STYP(                                 // .
        list::take_all_from(l0, sz_v<0>), // .
        l0                                // .
        );

    STYP(                                 // .
        list::take_all_from(l0, sz_v<1>), // .
        list::make(int_v<1>, int_v<2>,
            int_v<3>) // .
        );

    STYP(                                 // .
        list::take_all_from(l0, sz_v<2>), // .
        list::make(int_v<2>, int_v<3>)
        // .
        );

    STYP(                                 // .
        list::take_all_from(l0, sz_v<3>), // .
        list::make(int_v<3>)              // .
        );

    STYP(                                 // .
        list::take_all_from(l0, sz_v<4>), // .
        list::make()                      // .
        );
}

TEST_MAIN()
{
    slices();
    t3();

    return 0;
}
