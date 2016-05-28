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

        d.for_components_simd<position, velocity>(...)
    }
}