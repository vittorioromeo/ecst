struct Manager
{
    template <typename TSystemType>
    auto get_systems_in_parallel_with(TSystemType)
    {
        return ...;
    }
};

struct System
{
    template <typename THandle>
    auto access_handle(THandle h)
    {
        if(any_conflict(
               manager.get_systems_in_parallel_with(*this), get_systems_of(h)))
        {
            // Compile-time error or something.
        }
    }
};

struct Game
{
    ecs::handle _player;

    void process()
    {
        ecs::system<some_sig>.process([](auto s, auto&, auto&, auto&)
            {
                auto accessed_player_handle(s.access_handle(_player));

                // The above line may not compile if the system is parallelized
                // with some other system.
            });
    }
};
