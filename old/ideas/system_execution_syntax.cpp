// Enable access to critical operations and system execution.
context.step(
    [](auto& proxy) {
        // Most general version.
        proxy.execute_systems_overload_detailed(
            ss::start_from(st::physics), [](s::physics& s, auto& executor) {
                // Only once, even with multiple subtasks.
                s.prepare();

                executor.for_subtasks([&s](auto& subtask, auto& data) {
                    s.process(data);
                    auto out = subtask.output();
                    do_something(out);
                });
            });
    },
    // customizable refresh step
    [](auto& proxy) {
        proxy.execute_deferred([](auto& system, auto& executor) {
            // binds defer proxy
            executor.for_deferred_fns([](auto& d_fn) { d_fn(); });
        });

        proxy.kill_entities([](auto& system, auto& executor) {
            executor.for_killed_eids([](auto eid) {
                std::cout << "unsubscribing " << eid << " from " << system
                          << "\n";

                return action::unsubscribe{}; // default
                // return action::ignore{};      // dangerous
            });
        });

        proxy.match_entities([](auto& system, auto eid, auto action) {
            if constexpr(matched(action))
            {
            }
            else if constexpr(unmatched(action))
            {
            }
        });
    });

namespace start_policy
{
    template <typename... TSystemTags>
    auto from(TSystemTags... sts);

    auto from_all_independent();
} // namespace start_policy

template <typename TStartPolicy, typename TF>
void execute_systems_detailed(TStartPolicy sp, TF&& f)
{
    _system_runner.run(sp, FWD(f));
}

void execute_systems_overload_detailed(TStartPolicy sp, TFs&&... fs)
{
    auto overload = make_overload(FWD(fs)...);
    execute_systems_detailed(sp, overload);
}

template <typename TF>
auto adapt_for_all_subtasks(TF&& f)
{
    return [f = FWD(f)](auto& s, auto& executor)
    {
        executor.for_subtasks([&s](auto& data) { f(s, data); });
    });
}
