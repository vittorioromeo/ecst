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

// Define component tags
constexpr auto ctag_physics = tag::component::v<c_physics>;
constexpr auto ctag_draw = tag::component::v<c_draw>;
constexpr auto ctag_collision = tag::component::v<c_collision>;

constexpr auto TEST_CONST setup_components()
{
    namespace sc = signature::component;
    namespace slc = signature_list::component;

    // Define component signature list
    return slc::make(ctag_physics, ctag_draw, ctag_collision);
}

// Define system tags
constexpr auto stag_movement = tag::system::v<s_movement>;
constexpr auto stag_render = tag::system::v<s_render>;
constexpr auto stag_collreact = tag::system::v<s_collreact>;

constexpr auto TEST_CONST setup_systems()
{
    namespace ss = signature::system;
    namespace sls = signature_list::system;
    namespace ips = inner_parallelism::strategy;

    // Define movement system signature
    constexpr auto ssig_movement =       // .
        ss::make(stag_movement)          // .
            .parallelism(ips::none::v()) // .
            .write(ctag_physics);        // .

    // Define render system signature
    constexpr auto ssig_render =         // .
        ss::make(stag_render)            // .
            .parallelism(ips::none::v()) // .
            .dependencies(stag_movement) // .
            .read(ctag_physics);         // .


    // Define render system signature
    constexpr auto ssig_collreact =      // .
        ss::make(stag_collreact)         // .
            .parallelism(ips::none::v()) // .
            .dependencies(stag_movement) // .
            .read(ctag_physics);         // .

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

    constexpr auto res =                          // .
        ecst::settings::make()                    // .
            .allow_inner_parallelism()            // .
            .fixed_entity_limit(ecst::sz_v<1000>) // .
            .component_signatures(csl)            // .
            .system_signatures(ssl)               // .
            .scheduler(cs::scheduler<ss::s_atomic_counter>);

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
            proxy.execute_systems_from(stag_movement)(
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
