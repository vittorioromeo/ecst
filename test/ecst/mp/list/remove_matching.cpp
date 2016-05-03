#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void t2()
{
    auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>);
    auto l1 = list::remove_matching(l0, [](auto xx)
        {
            return bool_v<xx % 2 == 0>;
        });

    SA_SDT(                        // .
        (decltype(l1){}),          // .
        (list::make(iv<1>, iv<3>)) // .
        );
}

constexpr void t0()
{
    auto l0 = list::make(iv<0>, iv<2>, iv<4>, iv<6>, iv<8>);

    auto rmi0 = list::find_indices_of_matching(l0, negate([](auto)
                                                       {
                                                           return bool_v<false>;
                                                       }));
    (void)rmi0;
    SA_SDT(                                             // .
        (rmi0),                                         // .
        (list::make(sv<0>, sv<1>, sv<2>, sv<3>, sv<4>)) // .
        );

    auto rmi1 = list::find_indices_of_matching(l0, negate([](auto)
                                                       {
                                                           return bool_v<true>;
                                                       }));
    (void)rmi1;
    SA_SDT(            // .
        (rmi1),        // .
        (list::make()) // .
        );

    auto rmi2 = list::find_indices_of_matching(l0, [](auto zx)
        {
            return bool_v<(zx > 3)>;
        });

    (void)rmi2;
    SA(list::size(decltype(rmi2){}) == 3);
    SA_SDT(                               // .
        (rmi2),                           // .
        (list::make(sv<2>, sv<3>, sv<4>)) // .
        );



    auto rmi3 =
        list::find_indices_of_matching(l0, negate([](auto zx)
                                               {
                                                   return bool_v<(zx > 3)>;
                                               }));

    (void)rmi3;
    SA(list::size(decltype(rmi3){}) == 2);
    SA_SDT(                        // .
        (rmi3),                    // .
        (list::make(sv<0>, sv<1>)) // .
        );
}

constexpr void t1()
{
    auto l0 = list::make(iv<0>, iv<2>, iv<4>, iv<6>, iv<8>);

    auto rmi0 = list::find_matching(l0, negate([](auto)
                                            {
                                                return bool_v<false>;
                                            }));
    (void)rmi0;
    SA_SDT(                                             // .
        (rmi0),                                         // .
        (list::make(iv<0>, iv<2>, iv<4>, iv<6>, iv<8>)) // .
        );

    auto rmi1 = list::find_matching(l0, negate([](auto)
                                            {
                                                return bool_v<true>;
                                            }));
    (void)rmi1;
    SA_SDT(            // .
        (rmi1),        // .
        (list::make()) // .
        );

    auto rmi2 = list::find_matching(l0, [](auto zx)
        {
            return bool_v<(zx > 3)>;
        });

    (void)rmi2;
    SA(list::size(decltype(rmi2){}) == 3);
    SA_SDT(                               // .
        (rmi2),                           // .
        (list::make(iv<4>, iv<6>, iv<8>)) // .
        );



    auto rmi3 = list::find_matching(l0, negate([](auto zx)
                                            {
                                                return bool_v<(zx > 3)>;
                                            }));

    (void)rmi3;
    SA(list::size(decltype(rmi3){}) == 2);
    SA_SDT(                        // .
        (rmi3),                    // .
        (list::make(iv<0>, iv<2>)) // .
        );
}

TEST_MAIN()
{
    t0();
    t1();
    t2();

    return 0;
}
