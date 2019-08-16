#include "../../utils/test_utils.hpp"
#include "../settings_generator.hpp"
#include <ecst.hpp>

using namespace vrm::core;

struct c0
{
    int _v;
};

struct s0
{
};

TEST_MAIN()
{
    constexpr auto ct0 = ecst::tag::component::v<c0>;
    constexpr auto st0 = ecst::tag::system::v<s0>;

    constexpr auto cs0 = ecst::sig::component::make(ct0);
    constexpr auto ss0 = ecst::sig::system::make(st0);

    constexpr auto csl = ecst::sig_list::component::make(cs0);
    constexpr auto ssl = ecst::sig_list::system::make(ss0);

    auto test_impl = [](auto& ctx)
    {
        using ct = ECST_DECAY_DECLTYPE(ctx);
        using ct_handle = typename ct::handle;

        {
            ct_handle h;
            TEST_ASSERT(!ctx.valid_handle(h));
        }

        {
            ct_handle h{ecst::ctx::entity::uninitialized_handle};
            (void)h;
        }

        auto t = ctx.step([](auto& proxy)
            {
                auto e0 = proxy.create_entity();
                auto h0 = proxy.create_handle(e0);
                TEST_ASSERT(proxy.valid_handle(h0));
                TEST_ASSERT(proxy.alive(h0));

                auto h1 = proxy.create_entity_and_handle();
                TEST_ASSERT(proxy.valid_handle(h1));
                TEST_ASSERT(proxy.alive(h1));

                return std::make_tuple(e0, h0);
            });

        auto e0 = std::get<0>(t);
        auto h0 = std::get<1>(t);

        TEST_ASSERT(ctx.valid_handle(h0));
        TEST_ASSERT(ctx.alive(h0));

        ctx.step([&](auto& proxy)
            {
                TEST_ASSERT(proxy.valid_handle(h0));
                TEST_ASSERT(proxy.alive(h0));

                proxy.kill_entity(e0);

                // Changes are not committed until the end of a step.
                TEST_ASSERT(proxy.valid_handle(h0));
                TEST_ASSERT(proxy.alive(h0));
            });

        // Handle is still valid, but not alive.
        TEST_ASSERT(ctx.valid_handle(h0));
        TEST_ASSERT(!ctx.alive(h0));
    };

    test::run_tests(test_impl, ecst::sz_v<10>, csl, ssl);
}
