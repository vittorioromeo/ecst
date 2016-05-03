#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void replace_at()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>);

    SA_SDT( // .
        (list::replace_slice_by_unpacking(
            l0, sv<0>, sv<0>, list::make(iv<99>))), // .
        (l0)                                        // .
        );

    SA_SDT( // .
        (list::replace_slice_by_unpacking(
            l0, sv<0>, sv<1>, list::make(iv<99>))),      // .
        (list::make(iv<99>, iv<1>, iv<2>, iv<3>, iv<4>)) // .
        );

    SA_SDT( // .
        (list::replace_slice_by_unpacking(
            l0, sv<0>, sv<5>, list::make(iv<99>))), // .
        (list::make(iv<99>))                        // .
        );

    SA_SDT(                                              // .
        (list::replace_slice(l0, sv<0>, sv<0>, iv<99>)), // .
        (l0)                                             // .
        );

    SA_SDT(                                              // .
        (list::replace_slice(l0, sv<0>, sv<1>, iv<99>)), // .
        (list::make(iv<99>, iv<1>, iv<2>, iv<3>, iv<4>)) // .
        );

    SA_SDT(                                              // .
        (list::replace_slice(l0, sv<0>, sv<5>, iv<99>)), // .
        (list::make(iv<99>))                             // .
        );

    SA_SDT(                                              // .
        (list::replace_at(l0, sv<0>, iv<99>)),           // .
        (list::make(iv<99>, iv<1>, iv<2>, iv<3>, iv<4>)) // .
        );

    SA_SDT(                                              // .
        (list::replace_at(l0, sv<1>, iv<99>)),           // .
        (list::make(iv<0>, iv<99>, iv<2>, iv<3>, iv<4>)) // .
        );

    SA_SDT(                                              // .
        (list::replace_at(l0, sv<2>, iv<99>)),           // .
        (list::make(iv<0>, iv<1>, iv<99>, iv<3>, iv<4>)) // .
        );

    SA_SDT(                                              // .
        (list::replace_at(l0, sv<3>, iv<99>)),           // .
        (list::make(iv<0>, iv<1>, iv<2>, iv<99>, iv<4>)) // .
        );
}

constexpr void t4()
{
    constexpr auto l0 = list::make(int_v<0>, int_v<1>, int_v<2>, int_v<3>);

    ECST_S_ASSERT(list::size(l0) == 4);

    STYP(                                   // .
        list::take_all_before(l0, sz_v<0>), // .
        list::empty_v                       // .
        );

    STYP(                                  // .
        list::take_all_after(l0, sz_v<0>), // .
        list::make(int_v<1>, int_v<2>,
            int_v<3>) // .
        );

    STYP(                                        // .
        list::replace_at(l0, sz_v<0>, int_v<0>), // .
        list::make(int_v<0>, int_v<1>, int_v<2>,
            int_v<3>) // .
        );

    STYP(                                         // .
        list::replace_at(l0, sz_v<0>, int_v<99>), // .
        list::make(int_v<99>, int_v<1>, int_v<2>,
            int_v<3>) // .
        );

    STYP(                                         // .
        list::replace_at(l0, sz_v<3>, int_v<99>), // .
        list::make(int_v<0>, int_v<1>, int_v<2>,
            int_v<99>) // .
        );
}

TEST_MAIN()
{
    replace_at();
    t4();

    return 0;
}
