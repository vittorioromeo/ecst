#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

constexpr void fold()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>);
    auto res = list::fold_l(iv<0>,
        [](auto axl, auto axx)
        {
            return iv<axl + axx>;
        },
        l0);

    SA(res == iv<0 + 1 + 2 + 3 + 4>);
}

constexpr void fold_wi()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>);
    auto res = list::fold_l_with_index(iv<0>,
        [](auto i, auto axl, auto axx)
        {
            return iv<axl + axx + i>;
        },
        l0);

    SA(res == iv<0 + 1 + 2 + 3 + 4 + 0 + 1 + 2 + 3 + 4>);
}

constexpr void fold_wi_2()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>);
    auto res = list::fold_l_with_index(list::v<>,
        [](auto, auto axl, auto axx)
        {
            return list::cat(axl, list::make(axx));
        },
        l0);

    SA_SDT(res, list::make(iv<0>, iv<1>, iv<2>, iv<3>, iv<4>));
}


constexpr void fold_wi_3()
{
    constexpr auto l0 = list::make();

    auto res = list::fold_l_with_index(list::v<>,
        [](auto, auto axl, auto axx)
        {
            return list::cat(axl, list::make(axx));
        },
        l0);

    SA_SDT(res, list::make());
}

constexpr void fold_eq()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>);
    constexpr auto l1 = list::make(iv<0>, iv<1>, iv<2>);

    auto res = list::fold_l_with_index(bool_v<true>,
        [](auto, auto acc, auto axx0, auto axx1)
        {
            return bool_v<(acc && axx0 == axx1)>;
        },
        l0, l1);

    SA_SDT(res, bool_v<true>);
}

constexpr void fold_neq()
{
    constexpr auto l0 = list::make(iv<0>, iv<1>, iv<2>);
    constexpr auto l1 = list::make(iv<0>, iv<1>, iv<10>);

    auto res = list::fold_l_with_index(bool_v<true>,
        [](auto, auto acc, auto axx0, auto axx1)
        {
            return bool_v<(acc && axx0 == axx1)>;
        },
        l0, l1);

    SA_SDT(res, bool_v<false>);
}

TEST_MAIN()
{
    fold();
    fold_wi();
    fold_wi_2();
    fold_wi_3();
    fold_eq();
    fold_neq();

    return 0;
}
