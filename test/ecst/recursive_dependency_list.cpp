#include "../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;

struct s_a
{
};

struct s_b
{
};

struct s_c
{
};

TEST_MAIN()
{
    using namespace ecst::mp;

    namespace ss = signature::system;
    namespace sls = signature_list::system;
    namespace ips = inner_parallelism::strategy;

    // Define system tags
    constexpr auto st_a = ss::tag<s_a>;
    constexpr auto st_b = ss::tag<s_b>;
    constexpr auto st_c = ss::tag<s_c>;

    // Define movement system signature
    constexpr auto ssig_a = ss::make_by_tag( // .
        st_a,                                // .
        ips::none::v,                        // .
        ss::no_dependencies,                 // .
        ss::no_component_use,                // .
        ss::output::none                     // .
        );

    // Define render system signature
    constexpr auto ssig_b = ss::make_by_tag( // .
        st_b,                                // .
        ips::none::v,                        // .
        ss::depends_on<s_a>,                 // .
        ss::no_component_use,                // .
        ss::output::none                     // .
        );

    // Define render system signature
    constexpr auto ssig_c = ss::make_by_tag( // .
        st_c,                                // .
        ips::none::v,                        // .
        ss::depends_on<s_b>,                 // .
        ss::no_component_use,                // .
        ss::output::none                     // .
        );

    // Define system signature list
    constexpr auto ssl = sls::make(ssig_a, ssig_b, ssig_c);



    SA(bh::length(sls::recursive_dependency_id_list(ssl, ssig_a)) ==
        bh::size_c<0>);

    SA_SDT(                                               // .
        (sls::recursive_dependency_id_list(ssl, ssig_a)), // .
        (mp::list::empty_v)                               // .
        );



    SA(bh::length(sls::recursive_dependency_id_list(ssl, ssig_b)) ==
        bh::size_c<1>);

    SA_SDT(                                               // .
        (sls::recursive_dependency_id_list(ssl, ssig_b)), // .
        (mp::list::v<bh::size_t<0>>)                      // .
        );



    SA(bh::length(sls::recursive_dependency_id_list(ssl, ssig_c)) ==
        bh::size_c<2>);

    SA_SDT(                                               // .
        (sls::recursive_dependency_id_list(ssl, ssig_c)), // .
        (mp::list::v<bh::size_t<0>, bh::size_t<1>>)       // .
        );

    /*
    SA_SDT( // .
        (), // .
        ()  // .
        );
    */
}
