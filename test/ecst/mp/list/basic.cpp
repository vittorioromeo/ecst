#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

constexpr void basic_operations()
{
    SA_SDT(             // .
        (list::make()), // .
        (list::v<>)     // .
        );

    constexpr auto l0 = list::make(iv<0>, iv<1>);

    SA_SDT(                        // .
        (l0),                      // .
        (list::make(iv<0>, iv<1>)) // .
        );

    SA(!list::empty(l0));
    SA(list::size(list::v<>) == 0);
    SA(list::size(l0) == 2);
    SA(list::empty(list::make()));
    SA(list::empty(list::v<>));

    constexpr auto l1 = list::cat(l0, l0);

    SA_SDT(                                      // .
        (l1),                                    // .
        (list::make(iv<0>, iv<1>, iv<0>, iv<1>)) // .
        );

    SA_SDT(                                           // .
        (l1),                                         // .
        (list::v<int_<0>, int_<1>, int_<0>, int_<1>>) // .
        );

    SA(!list::empty(l1));
    SA(list::size(l1) == 4);

    SA_SDT(                    // .
        (list::at(l1, iv<0>)), // .
        (iv<0>)                // .
        );

    SA_SDT(                    // .
        (list::at(l1, iv<1>)), // .
        (iv<1>)                // .
        );

    SA_SDT(                    // .
        (list::at(l1, iv<2>)), // .
        (iv<0>)                // .
        );

    SA_SDT(                    // .
        (list::at(l1, iv<3>)), // .
        (iv<1>)                // .
        );

    constexpr auto l2 = list::make(iv<2>, iv<2>);

    SA_SDT(                               // .
        (list::append(l2, iv<0>)),        // .
        (list::make(iv<2>, iv<2>, iv<0>)) // .
        );

    SA_SDT(                               // .
        (list::prepend(l2, iv<0>)),       // .
        (list::make(iv<0>, iv<2>, iv<2>)) // .
        );
}

TEST_MAIN()
{
    basic_operations();
    return 0;
}
