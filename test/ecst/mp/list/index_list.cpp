#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;
using namespace ecst::mp;

constexpr void idxlist()
{
    SA_SDT(           // .
        list::make(), // .
        list::make());

    SA_SDT(                             // .
        list::make_index_list(sz_v<0>), // .
        list::make());

    SA_SDT(                             // .
        list::make_index_list(sz_v<4>), // .
        list::make(sz_v<0>, sz_v<1>, sz_v<2>, sz_v<3>));

    SA_SDT(                                      // .
        list::make_index_list(sz_v<1>, sz_v<4>), // .
        list::make(sz_v<1>, sz_v<2>, sz_v<3>));

    SA_SDT(                                // .
        list::make_minimal_index_list_for( // .
            list::make(sz_v<1>, sz_v<4>)   // .
            ),                             // .
        list::make(sz_v<0>, sz_v<1>));

    SA_SDT(                                     // .
        list::make_minimal_index_list_for(      // .
            list::make(sz_v<1>, sz_v<4>),       // .
            list::make(sz_v<1>, sz_v<4>, iv<0>) // .
            ),                                  // .
        list::make(sz_v<0>, sz_v<1>));

    SA_SDT(                                                        // .
        list::make_minimal_index_list_for_by_unpacking(list::make( // .
            list::make(sz_v<1>, sz_v<4>),                          // .
            list::make(sz_v<1>, sz_v<4>, iv<0>))                   // .
            ),                                                     // .
        list::make(sz_v<0>, sz_v<1>));
}

constexpr void t5()
{
    STYP(                                          // .
        list::impl::make_index_sequence_list<0>(), // .
        list::make()                               // .
        );

    STYP(                                          // .
        list::impl::make_index_sequence_list<1>(), // .
        list::make(sz_v<0>)                        // .
        );

    STYP(                                          // .
        list::impl::make_index_sequence_list<2>(), // .
        list::make(sz_v<0>, sz_v<1>)               //
        );

    STYP(                                          // .
        list::impl::make_index_sequence_list<3>(), // .
        list::make(sz_v<0>, sz_v<1>,
            sz_v<2>) // .
        );
}

TEST_MAIN()
{
    idxlist();
    t5();

    return 0;
}
