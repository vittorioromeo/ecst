namespace ecst
{
    namespace impl
    {
        struct instance_base
        {
            system _system;
        };
    }

    // processes entities
    struct instance_entity : impl::instance_base
    {
        set _subscribed;
        state_manager _sm;
    };

    // processes raw component data
    struct instance_component : impl::instance_base
    {
    };

    struct data_proxy_component
    {
    private:
        void execute_simd(TCTL ctl, TF f, sz_t i_begin, sz_t i_end)
        {
            float* data_ptrs = ctl.chunk().data()...;
            auto casted_data_ptrs = reinterpret_cast<sse_float4_whatever*>(data_ptrs);

            for(auto i = i_begin; i < i_end; ++i)
            {
                f(casted_data_ptrs[i]...);
            }
        }

    public:
        void for_components_simd(TComponentTagList ctl, TF f)
        {   
            static_assert(all ctl in system signature);
            static_assert(all ctl components are sizeof(float) or sizeof(double));
            static_assert(all ctl component chunks are arrays or vectors);

            // maybe:
            // resize_all_ctl_to_nearest_multiple_of_4();
            // and don't have execute_sequential

            assert(all ctl component chunks have same capacity);

            auto floored_end_indices = floor(ctl.chunk().count() / 4)...;

            // process as simd where possible
            execute_simd(ctl, f, 0, floored_end_indices);

            // process remaining data sequentially
            execute_sequential(ctl, f, floored_end_indices * 4, ctl.chunk.count()); 
        } 
    };
}

auto make_ssl()
{
    auto s0 = ss::make_entity_system()
        .usualstuff();

    // not related to entities
    auto s1 = ss::make_component_system()
        .usualstuff();
}

struct s1
{
    template<typename TData>
    void process(TData& d)
    {
        // compile-time error, no subscribers
        // d.for_entities(...)

        d.for_components_simd(list(ct::position, ct::velocity), 
            [](auto& pos4, auto& vel4)
            {
                simd_add(pos4, vel4);

                // note: no proxy
            });
    }
}

// TODO:
// parallel executor simply return split IDs and tasks
// instance_entity uses split IDs to calculate entity IDs
// instance_component uses split IDs to split workload