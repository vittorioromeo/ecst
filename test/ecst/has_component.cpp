#include "./settings_generator.hpp"
#include <ecst.hpp>

namespace example
{
    using vrm::core::sz_t;
    using vrm::core::uint;

    namespace c
    {
        struct c0
        {
        };
        struct c1
        {
        };
    } // namespace c

    namespace ct
    {
        constexpr auto c0 = ecst::tag::component::v<c::c0>;
        constexpr auto c1 = ecst::tag::component::v<c::c1>;
    } // namespace ct

    namespace s
    {
        struct s0
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid) {
                    TEST_ASSERT(data.has(ct::c0, eid));
                    TEST_ASSERT(!data.has(ct::c1, eid));

                    data.defer([eid](auto& proxy) {
                        proxy.remove_component(ct::c0, eid);
                        proxy.add_component(ct::c1, eid);
                    });
                });
            }
        };

        struct s1
        {
            template <typename TData>
            void process(TData& data)
            {
                data.for_entities([&](auto eid) {
                    TEST_ASSERT(!data.has(ct::c0, eid));
                    TEST_ASSERT(data.has(ct::c1, eid));

                    data.defer([eid](auto& proxy) {
                        proxy.remove_component(ct::c1, eid);
                        proxy.add_component(ct::c0, eid);
                    });
                });
            }
        };
    } // namespace s

#define SYS_TAG(x)                                     \
    namespace s                                        \
    {                                                  \
        struct x;                                      \
    }                                                  \
    namespace st                                       \
    {                                                  \
        constexpr auto x = ecst::tag::system::v<s::x>; \
    }

    SYS_TAG(s0)
    SYS_TAG(s1)

    namespace ecst_setup
    {
        constexpr auto make_csl()
        {
            namespace c = example::c;
            namespace sc = ecst::sig::component;
            namespace slc = ecst::sig_list::component;

            return slc::make(sc::make(ct::c0).contiguous_buffer(),
                sc::make(ct::c1).contiguous_buffer());
        }

        constexpr auto entity_count = ecst::sz_v<100>;

        constexpr auto make_ssl()
        {
            using ecst::sz_v;

            namespace c = example::c;
            namespace s = example::s;
            namespace ss = ecst::sig::system;
            namespace sls = ecst::sig_list::system;
            namespace ips = ecst::inner_par::strategy;
            namespace ipc = ecst::inner_par::composer;

            constexpr auto test_p = ipc::none_below_threshold::v(
                ecst::sz_v<10>, ips::split_evenly_fn::v_cores());

            constexpr auto ssig_s0 =     // .
                ss::make(st::s0)         // .
                    .parallelism(test_p) // .
                    .write(ct::c0);      // .

            constexpr auto ssig_s1 =
                ss::make(st::s1).parallelism(test_p).write(ct::c1);

            return sls::make(ssig_s0, ssig_s1);
        }
    } // namespace ecst_setup

    namespace sea = ::ecst::sys_exec;

    auto execute_systems = [](auto& proxy) {
        proxy.execute_systems_from(st::s0, st::s1)( // .
            sea::all().for_subtasks(
                [](auto& s, auto& data) { s.process(data); }));
    };

    auto test_impl_f = [](auto& ctx) {
        ctx.step([&](auto& proxy) {
            for(sz_t ie = 0; ie < ecst_setup::entity_count; ++ie)
            {
                auto e = proxy.create_entity();
                TEST_ASSERT(!proxy.has_component(ct::c0, ecst::entity_id(ie)));
                TEST_ASSERT(!proxy.has_component(ct::c1, ecst::entity_id(ie)));
                proxy.add_component(ct::c0, e);
            }
        });

        ctx.step(execute_systems);
        ctx.step(execute_systems);

        ctx.step([&](auto& proxy) {
            for(sz_t ie = 0; ie < ecst_setup::entity_count; ++ie)
            {
                TEST_ASSERT(proxy.has_component(ct::c0, ecst::entity_id(ie)));
                TEST_ASSERT(!proxy.has_component(ct::c1, ecst::entity_id(ie)));
            }
        });
    };
} // namespace example

int main()
{
    using namespace example;
    using namespace example::ecst_setup;

    test::run_tests(test_impl_f, entity_count, make_csl(), make_ssl());
    return 0;
}
