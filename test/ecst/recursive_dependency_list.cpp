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
    constexpr auto st_a = tag::system::v<s_a>;
    constexpr auto st_b = tag::system::v<s_b>;
    constexpr auto st_c = tag::system::v<s_c>;

    // Define movement system signature
    constexpr auto ssig_a =               // .
        ss::make(st_a)                    // .
            .parallelism(ips::none::v()); // .

    // Define render system signature
    constexpr auto ssig_b =              // .
        ss::make(st_b)                   // .
            .parallelism(ips::none::v()) // .
            .dependencies(st_a);         // .

    // Define render system signature
    constexpr auto ssig_c =              // .
        ss::make(st_c)                   // .
            .parallelism(ips::none::v()) // .
            .dependencies(st_b);         // .

    // Define system signature list
    constexpr auto ssl = sls::make(ssig_a, ssig_b, ssig_c);

    SA(bh::length(sls::recursive_dependency_id_list(ssl, mp::wrap(ssig_a))) ==
        bh::size_c<0>);


    SA_SDT(                                                         // .
        (sls::recursive_dependency_id_list(ssl, mp::wrap(ssig_a))), // .
        (mp::list::empty_v)                                         // .
        );



    SA(bh::length(sls::recursive_dependency_id_list(ssl, mp::wrap(ssig_b))) ==
        bh::size_c<1>);

    SA_SDT(                                                         // .
        (sls::recursive_dependency_id_list(ssl, mp::wrap(ssig_b))), // .
        (mp::list::v<bh::size_t<0>>)                                // .
        );



    SA(bh::length(sls::recursive_dependency_id_list(ssl, mp::wrap(ssig_c))) ==
        bh::size_c<2>);

    SA_SDT(                                                         // .
        (sls::recursive_dependency_id_list(ssl, mp::wrap(ssig_c))), // .
        (mp::list::v<bh::size_t<0>, bh::size_t<1>>)                 // .
        );
}
