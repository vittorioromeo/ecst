#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void invidx()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>);

    SA_SDT(                               // .
        (list::inverse_indices(l0)),      // .
        (list::make(sv<0>, sv<1>, sv<2>)) // .
        );

    SA_SDT(                                 // .
        (list::inverse_indices(l0, sv<0>)), // .
        (list::make(sv<1>, sv<2>))          // .
        );

    SA_SDT(                                        // .
        (list::inverse_indices(l0, sv<0>, sv<1>)), // .
        (list::make(sv<2>))                        // .
        );

    SA_SDT(                                               // .
        (list::inverse_indices(l0, sv<0>, sv<1>, sv<2>)), // .
        (list::make())                                    // .
        );

    SA_SDT(                                 // .
        (list::inverse_indices(l0, sv<1>)), // .
        (list::make(sv<0>, sv<2>))          // .
        );
}

constexpr void invpick()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>);

    SA_SDT(                               // .
        (list::pick_all_except_at(l0)),   // .
        (list::make(iv<0>, iv<1>, iv<2>)) // .
        );

    SA_SDT(                                    // .
        (list::pick_all_except_at(l0, sv<0>)), // .
        (list::make(iv<1>, iv<2>))             // .
        );

    SA_SDT(                                           // .
        (list::pick_all_except_at(l0, sv<0>, sv<1>)), // .
        (list::make(iv<2>))                           // .
        );

    SA_SDT(                                                  // .
        (list::pick_all_except_at(l0, sv<0>, sv<1>, sv<2>)), // .
        (list::make())                                       // .
        );

    SA_SDT(                                    // .
        (list::pick_all_except_at(l0, sv<1>)), // .
        (list::make(iv<0>, iv<2>))             // .
        );
}

TEST_MAIN()
{
    invidx();
    invpick();

    return 0;
}
