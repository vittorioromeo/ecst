#include <bitset>

namespace entity_policy
{
    struct stateful
    {
        using id_type = int;
        using bitset_type = std::bitset<256>;

        struct entity
        {
            id_type _id;
            bitset_type _bitset;
        };

        struct system
        {
            bitset_type _bitset;
        };

        void manager_before_update()
        {
        }

        void manager_update()
        {
            // * O(n_entities * n_systems)
            // * branch in tight loop

            for(auto& e : get_all_entities())
                for(auto& s : get_all_systems())
                    if(e.matches(s)) s.act_on(e);
        }
    };

    struct stateless
    {
        using id_type = int;

        struct sparse_int_set
        {
        };

        struct entity
        {
            id_type _id;
        };

        struct system
        {
            sparse_int_set _entities;
        };

        void manager_before_update()
        {
            for(auto& e : get_killed_entities())
                for(auto& s : get_all_systems()) s.remove(e);

            for(auto& e : get_new_entities())
                for(auto& s : get_all_systems()) s.add(e);
        }

        void manager_update()
        {
            for(auto& e : get_all_entities())
                for(auto& s : get_all_systems())
                    if(e.matches(s)) s.act_on(e);
        }
    };
}