#include "../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst;

struct c_physics
{
    int _x;
    int _y;
};

struct c_draw
{
    int _x;
    int _y;
};

struct c_collision
{
    int _x;
    int _y;
};

struct s_movement
{
    void process(float dt)
    {
        std::cout << "(" << dt << ") "
                  << "Process movement\n";
    }
};

struct s_render
{
    void process(float dt)
    {
        std::cout << "(" << dt << ") "
                  << "Process render\n";
    }
};

struct s_collreact
{
    void process(float dt)
    {
        std::cout << "(" << dt << ") "
                  << "Process collreact\n";
    }
};


constexpr auto TEST_CONST setup_components()
{
    namespace sc = signature::component;
    namespace slc = signature_list::component;

    // Define component tags
    constexpr auto ctag_physics = sc::tag<c_physics>;
    constexpr auto ctag_draw = sc::tag<c_draw>;
    constexpr auto ctag_collision = sc::tag<c_collision>;

    // Define component signatures
    constexpr auto csig_physics = sc::make_by_tag(ctag_physics);
    constexpr auto csig_draw = sc::make_by_tag(ctag_draw);
    constexpr auto csig_collision = sc::make_by_tag(ctag_collision);

    // Define component signature list
    constexpr auto component_siglist = // .
        slc::make(csig_physics, csig_draw, csig_collision);

    return component_siglist;
}

constexpr auto TEST_CONST setup_systems()
{
    namespace ss = signature::system;
    namespace sls = signature_list::system;
    namespace ips = inner_parallelism::strategy;

    // Define system tags
    constexpr auto stag_movement = ss::tag<s_movement>;
    constexpr auto stag_render = ss::tag<s_render>;
    constexpr auto stag_collreact = ss::tag<s_collreact>;

    // Define movement system signature
    constexpr auto ssig_movement = ss::make_by_tag( // .
        stag_movement,                              // .
        ips::none::v(),                             // .
        ss::no_dependencies,                        // .
        ss::component_use(                          // .
            ss::mutate<c_physics>                   // .
            ),                                      // .
        ss::output::none                            // .
        );

    // Define render system signature
    constexpr auto ssig_render = ss::make_by_tag( // .
        stag_render,                              // .
        ips::none::v(),                           // .
        ss::depends_on<                           // .
            s_movement                            // .
            >,                                    // .
        ss::component_use(                        // .
            ss::read<c_physics>                   // .
            ),                                    // .
        ss::output::none                          // .
        );

    // Define render system signature
    constexpr auto ssig_collreact = ss::make_by_tag( // .
        stag_collreact,                              // .
        ips::none::v(),                              // .
        ss::depends_on<                              // .
            s_movement                               // .
            >,                                       // .
        ss::component_use(                           // .
            ss::read<c_physics>                      // .
            ),                                       // .
        ss::output::none                             // .
        );

    // Define system signature list
    constexpr auto system_siglist = // .
        sls::make(ssig_movement, ssig_render, ssig_collreact);

    return system_siglist;
}

template <typename TCSL, typename TSSL>
constexpr auto TEST_CONST setup_settings(TCSL csl, TSSL ssl)
{
    namespace cs = ecst::settings;
    namespace ss = ecst::scheduler;

    constexpr auto ce_csl = decltype(csl){};
    constexpr auto ce_ssl = decltype(ssl){};

    constexpr auto res = ecst::settings::make(          // .
        cs::multithreaded(cs::allow_inner_parallelism), // .
        cs::fixed<1000>,                                // .
        ce_csl,                                         // .
        ce_ssl,                                         // .
        cs::scheduler<ss::s_atomic_counter>             // .
        );

    return res;
}

template <typename TS>
void test_cs(TS s)
{
    ecst::context::storage::component::dispatch<TS> cs;
    auto fake_metadata =
        ecst::context::storage::component::chunk_metadata_tuple_type<TS>{};

    auto& cs_c_physics =
        cs.template get<c_physics>(ecst::entity_id(0), fake_metadata);

    auto& cs_c_draw =
        cs.template get<c_draw>(ecst::entity_id(0), fake_metadata);

    auto& cs_c_collision =
        cs.template get<c_collision>(ecst::entity_id(0), fake_metadata);

    (void)cs;
    (void)s;
    (void)cs_c_physics;
    (void)cs_c_draw;
    (void)cs_c_collision;
}

TEST_MAIN()
{
    constexpr auto component_siglist = setup_components();
    constexpr auto system_siglist = setup_systems();
    constexpr auto context_settings =
        setup_settings(component_siglist, system_siglist);

    test_cs(context_settings);


    namespace sea = ::ecst::system_execution_adapter;

    auto ctx = ecst::context::make(context_settings);
    ctx.step([](auto& proxy)
        {
            proxy.execute_systems(
                sea::all().detailed([&](auto& system, auto& executor)
                    {
                        executor.for_subtasks([&](auto&)
                            {
                                std::cout << "Executing...\n";
                                system.process(15);
                                std::cout << "...finished.\n";
                            });
                    }));
        });
}
