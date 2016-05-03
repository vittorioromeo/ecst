#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

constexpr void t0()
{
    constexpr auto l0 = list::make();

    ECST_S_ASSERT(list::size(l0) == 0);

    ECST_S_ASSERT(!list::contains(l0, int_v<0>));
    ECST_S_ASSERT(!list::contains(l0, int_v<1>));
    ECST_S_ASSERT(!list::contains(l0, int_v<2>));
    ECST_S_ASSERT(!list::contains(l0, int_v<3>));
}

constexpr void t1()
{
    constexpr auto l0 = list::make(int_v<0>, int_v<1>, int_v<2>, int_v<3>);

    ECST_S_ASSERT(list::size(l0) == 4);

    ECST_S_ASSERT(list::contains(l0, int_v<0>));
    ECST_S_ASSERT(list::contains(l0, int_v<1>));
    ECST_S_ASSERT(list::contains(l0, int_v<2>));
    ECST_S_ASSERT(list::contains(l0, int_v<3>));
    ECST_S_ASSERT(!list::contains(l0, int_v<4>));
}

template <int TI>
struct bigger_int_than
{
    template <typename T>
    constexpr auto operator()(T x) const
    {
        return bool_v<(decltype(x){} > TI)>;
    }
};

constexpr void t2()
{
    constexpr auto l0 = list::make(int_v<0>, int_v<1>, int_v<2>, int_v<3>);

    ECST_S_ASSERT(list::size(l0) == 4);

    ECST_S_ASSERT(list::contains(l0, int_v<0>));
    ECST_S_ASSERT(list::contains(l0, int_v<1>));
    ECST_S_ASSERT(list::contains(l0, int_v<2>));
    ECST_S_ASSERT(list::contains(l0, int_v<3>));
    ECST_S_ASSERT(!list::contains(l0, int_v<4>));

    ECST_S_ASSERT(list::contains_matching(l0, bigger_int_than<0>{}));
    ECST_S_ASSERT(list::contains_matching(l0, bigger_int_than<1>{}));
    ECST_S_ASSERT(list::contains_matching(l0, bigger_int_than<2>{}));
    ECST_S_ASSERT(!list::contains_matching(l0, bigger_int_than<3>{}));
    ECST_S_ASSERT(!list::contains_matching(l0, bigger_int_than<4>{}));
}

constexpr void t3()
{
    constexpr auto l0 = list::make(int_v<0>, int_v<1>, int_v<2>, int_v<3>);

    SA(list::contains_all_by_unpacking(l0, // .
        list::make()));

    SA(list::contains_all_by_unpacking(l0, // .
        list::make(iv<0>)));

    SA(list::contains_all_by_unpacking(l0, // .
        list::make(iv<0>, iv<1>)));

    SA(list::contains_all_by_unpacking(l0, // .
        list::make(iv<1>, iv<0>)));

    SA(list::contains_all_by_unpacking(l0, // .
        l0));

    SA(!list::contains_all_by_unpacking(l0, // .
        list::make(iv<0>, iv<100>)));


    SA(list::contains_all(l0));

    SA(list::contains_all(l0, // .
        iv<0>));

    SA(list::contains_all(l0, // .
        iv<0>, iv<1>));

    SA(list::contains_all(l0, // .
        iv<1>, iv<0>));

    SA(!list::contains_all(l0, // .
        iv<0>, iv<100>));
}

constexpr void t4()
{
    SA_SDT(                                      // .
        (list::cat_unique(                       // .
            list::make(iv<0>, iv<1>, iv<2>),     // .
            list::make(iv<1>, iv<2>, iv<3>)      // .
            )),                                  //.
        (list::make(iv<0>, iv<1>, iv<2>, iv<3>)) // .
        );
}

TEST_MAIN()
{
    t0();
    t1();
    t2();
    t3();
    t4();

    return 0;
}
