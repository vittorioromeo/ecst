#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void pick()
{
    constexpr auto l0 = list::make(iv<0>, iv<10>, iv<20>);

    SA_SDT(                    // .
        (list::at(l0, iv<0>)), // .
        (iv<0>)                // .
        );

    SA_SDT(                    // .
        (list::at(l0, iv<1>)), // .
        (iv<10>)               // .
        );

    SA_SDT(                    // .
        (list::at(l0, iv<2>)), // .
        (iv<20>)               // .
        );

    SA_SDT(                         // .
        (list::pick_at(l0, iv<0>)), // .
        (list::make(iv<0>))         // .
        );

    SA_SDT(                         // .
        (list::pick_at(l0, iv<1>)), // .
        (list::make(iv<10>))        // .
        );

    SA_SDT(                         // .
        (list::pick_at(l0, iv<2>)), // .
        (list::make(iv<20>))        // .
        );

    SA_SDT(                                // .
        (list::pick_at(l0, iv<0>, iv<1>)), // .
        (list::make(iv<0>, iv<10>))        // .
        );

    SA_SDT(                                // .
        (list::pick_at(l0, iv<1>, iv<0>)), // .
        (list::make(iv<10>, iv<0>))        // .
        );
}

constexpr void basic_pick_by_unpacking()
{
    constexpr auto l0 = list::make(iv<0>, iv<10>, iv<20>);

    SA_SDT(                                                  // .
        (list::pick_at_by_unpacking(l0, list::make(iv<0>))), // .
        (list::make(iv<0>))                                  // .
        );

    SA_SDT(                                                  // .
        (list::pick_at_by_unpacking(l0, list::make(iv<1>))), // .
        (list::make(iv<10>))                                 // .
        );

    SA_SDT(                                                  // .
        (list::pick_at_by_unpacking(l0, list::make(iv<2>))), // .
        (list::make(iv<20>))                                 // .
        );

    SA_SDT(                                                         // .
        (list::pick_at_by_unpacking(l0, list::make(iv<0>, iv<1>))), // .
        (list::make(iv<0>, iv<10>))                                 // .
        );

    SA_SDT(                                                         // .
        (list::pick_at_by_unpacking(l0, list::make(iv<1>, iv<0>))), // .
        (list::make(iv<10>, iv<0>))                                 // .
        );
}


constexpr void t8()
{
    constexpr auto l0 =
        list::make(int_v<0>, int_v<1>, int_v<2>, int_v<3>, int_v<4>);

    STYP(                            // .
        list::pick_at(l0, int_v<0>), // .
        list::make(int_v<0>)         // .
        );

    STYP(                            // .
        list::pick_at(l0, int_v<1>), // .
        list::make(int_v<1>)         // .
        );

    STYP(                            // .
        list::pick_at(l0, int_v<2>), // .
        list::make(int_v<2>)         // .
        );

    STYP(                                      // .
        list::pick_at(l0, int_v<0>, int_v<1>), // .
        list::make(int_v<0>, int_v<1>)         // .
        );

    STYP(                                      // .
        list::pick_at(l0, int_v<4>, int_v<0>), // .
        list::make(int_v<4>, int_v<0>)         // .
        );
}

TEST_MAIN()
{
    pick();
    basic_pick_by_unpacking();
    t8();

    return 0;
}
