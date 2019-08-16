// global parallelism settings
ecst::settings(inner_par::allowed, inner_par::disallowed)

    // system settings
    sys_sig(parallelism::none,

        parallelism::basic(entity_threshold(sz_v<10000>),
                      entities_per_subtask(sz_v<5000>), max_subtasks(sz_v<4>)),

        parallelism::automatic)

    // example automatic parallelism idea
    system_context::execute()
{
    if(par.first_execution())
    {
        par.set_entity_threshold(guess_entity_threshold());
        par.set_max_tasks(hardware_concurrency());

        start_benchmark();
        impl();
        auto bres = end_benchmark();

        par.history.add(bres);
    }
    else
    {
        // ... ?
    }
}
