class action_proxy
{
private:
    context& _context;

public:
    auto create_entity()
    {
        auto x = _context.create();
        _context.refresh_state.to_match.add(x);
    }

    void kill_entity(entity_id);

    template <typename, typename>
    auto add_component(entity_id, init_fn);

    template <typename>
    auto remove_component(entity_id);
};

class system_state
{
private:
    std::vector<std::function<void(action_proxy)>> _deferred_fns;
};

void context::refresh()
{
    for_systems([](auto& s)
        {
            for(auto& f : s._deferred_fns)
            {
                f();
            }
        });

    for(auto x : refresh_state.to_kill)
    {
        for_systems([](auto& s)
            {
                s.unsubscribe(x);
            });

        refresh_state.to_match.remove(x);
        entity_storage.reclaim(x);
    }

    for_systems([](auto& s)
        {
            for(auto tm_eid : refresh_state.to_match)
            {
                if(matches(s, tm_eid))
                {
                    s.subscribe(tm_eid);
                }
                else
                {
                    s.unsubscribe(tm_eid);
                }
            }
        });
}