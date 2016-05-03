#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

constexpr void basic_mutations()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>);

    SA_SDT(                                   // .
        (list::insert_at(l0, sv<0>, iv<99>)), // .
        (list::make(iv<99>, iv<0>, iv<1>))    // .
        );

    SA_SDT(                                   // .
        (list::insert_at(l0, sv<1>, iv<99>)), // .
        (list::make(iv<0>, iv<99>, iv<1>))    // .
        );

    SA_SDT(                                   // .
        (list::insert_at(l0, sv<2>, iv<99>)), // .
        (list::make(iv<0>, iv<1>, iv<99>))    // .
        );

    SA_SDT(                                           // .
        (list::insert_at(l0, sv<0>, iv<99>, iv<88>)), // .
        (list::make(iv<99>, iv<88>, iv<0>, iv<1>))    // .
        );

    SA_SDT(                                           // .
        (list::insert_at(l0, sv<1>, iv<99>, iv<88>)), // .
        (list::make(iv<0>, iv<99>, iv<88>, iv<1>))    // .
        );

    SA_SDT(                                           // .
        (list::insert_at(l0, sv<2>, iv<99>, iv<88>)), // .
        (list::make(iv<0>, iv<1>, iv<99>, iv<88>))    // .
        );

    constexpr auto l1 = list::make(iv<0>, iv<1>, iv<2>, iv<3>);

    SA_SDT(                                              // .
        (list::push_front(l1, iv<99>)),                  // .
        (list::make(iv<99>, iv<0>, iv<1>, iv<2>, iv<3>)) // .
        );

    SA_SDT(                                              // .
        (list::push_back(l1, iv<99>)),                   // .
        (list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<99>)) // .
        );

    SA_SDT(                               // .
        (list::pop_front(l1)),            // .
        (list::make(iv<1>, iv<2>, iv<3>)) // .
        );

    SA_SDT(                               // .
        (list::pop_back(l1)),             // .
        (list::make(iv<0>, iv<1>, iv<2>)) // .
        );

    SA_SDT(                           // .
        (list::pop_front(l1, sv<2>)), // .
        (list::make(iv<2>, iv<3>))    // .
        );

    SA_SDT(                          // .
        (list::pop_back(l1, sv<2>)), // .
        (list::make(iv<0>, iv<1>))   // .
        );
}

TEST_MAIN()
{
    basic_mutations();
    return 0;
}
