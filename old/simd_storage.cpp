template <typename TComponentTagList>
struct compressed_simd_buffer
{
    struct metadata
    {
    };

    using components = components_of<TComponentTagList>...;

    std::tuple<std::vector<components>...> _vectors;
    std::vector<sz_t> _eid_mapping;
    sparse_set _used_eids;

    void compress()
    {
        // like remove_if
        for(d : dead_used_eids)
        {
            auto end = first_unused();
            swap(_vectors[d], _vectors[end]);
            swap(_eid_mapping[d], _eid_mapping[end]);
        }
    }
};

struct position
{
    vec2f _v;
};

struct velocity
{
    vec2f _v;
};

struct acceleration
{
    vec2f _v;
};

auto make_csl()
{
}