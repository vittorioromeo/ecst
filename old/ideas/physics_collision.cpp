struct TaskGroup
{
    template <typename TF>
    void process(TF f)
    {
        for(auto& t : my_tasks)
        {
            apply(f, unwrap_signature<my_signature>(t));
        }
    }
};

struct Manager
{
    template <typename TSystem, typename TSignature>
    auto get_matching(TSystem&, const TSignature&)
    {
        std::vector<task> tasks;

        if(TSystem::can_subdivide())
        {
            build_tasks<parallelism_degree>(tasks, ...);
        }
        else
        {
            build_tasks<1>(tasks, ...);
        }

        return tasks;
    }
};

struct SPhysics
{
    template <typename TManager>
    void process(TManager& m)
    {
        // TODO: SIMD?
        m.get_matching(*this, physics_signature)
            .process([](auto& c_pos, auto& c_vel, auto& c_acc)
                {
                    c_vel += c_acc;
                    c_pos += c_vel;
                });
    }
};

struct SCollision
{
    // http://stackoverflow.com/questions/22469304/how-to-evenly-distribute-work-for-an-all-pairs-test-of-n-objects-over-n-threads
    // (?)

    contact_list _final_contact_list;

    template <typename TManager>
    void process(TManager& m)
    {
        // Get entities divided in subtasks
        m.get_matching(*this, collision_signature)
            .process_task<collision_task_data>(
                [](auto& data, auto& xc_pos, auto& xc_hitbox)
                {
                    data.contacts.clear();

                    // Get all entities in a single task
                    m.get_matching(*this, collision_signature)
                        .process_unsubdivided([](auto& yc_pos, auto& yc_hitbox)
                            {
                                if(collision(
                                       xc_pos, yc_pos, xc_hitbox, yc_hitbox))
                                {
                                    data.contacts.emplace_back(
                                        xc_pos, yc_pos, metadata);
                                }
                            });
                })
            .final_step([](auto& tasks)
                {
                    contact_list cl;
                    for(auto& t : tasks) cl.merge_with(t.contacts);

                    _final_contact_list = cl;
                });
    }
};

struct SContacts
{
    template <typename TManager>
    void process(TManager& m)
    {
        // SCollision has definitely finished processing.
        const auto& s_collision = m.get(type_v<SCollision>);
        const auto& cl = s_collision._final_contact_list;

        // alternative syntax:
        const auto& deps = get_my_dependencies();
        const auto& s_collision = deps.get(type_v<SCollision>);
        const auto& cl = s_collision._final_contact_list;

        // TODO: bad idea! Make a copy of _final_contact_list during DAG
        // traversal to avoid data-races. Maybe specify "return type" during
        // system definition in `init()`?;



        // Subdivision strategy
        auto subdivider = const_list_subdivider(cl);

        // For each list subdivision -> for each list element -> lambda
        m.generic_subdivide(subdivider, [](const auto& c)
            {
                process_contact(c);
            });
    }

    // "Await" return type...
    template <typename TManager>
    void process(const contact_list& cl, TManager& m)
    {
    }
};

namespace ct_dag
{
    struct null_node
    {
    };

    template <typename T, typename TNextNodes>
    struct node
    {
        using type = T;

        // TODO: store dependencies or nexts?
        using next_nodes = TNextNodes;
        using dependencies = TDependencies;
    };

    template <typename T>
    auto make_node(T)
    {
        return ...;
    }

    template <typename T, typename TNextNodes>
    auto make_node(T, TNextNodes)
    {
        return ...;
    }

    template <typename TNode>
    auto add_neighbor_to_node(TNode n, TNode nb)
    {
        auto neighbors = get_neighbords(n);
        auto added = add_to_neighbors(neighbors, nb);
        return make_node(TNode::type{}, added);
    }
}

template <typename Ts>
auto steps(Ts... xs)
{
    // 0) Make root node
    // 1) Find all steps without dependencies and add them to root
    // 2)
    // 3) while(remaining steps)
    // 3.1)     take one of them
    // 3.2)     find parent node
    // 3.3)         attach to parent
    // 3.4)         remove from node list

    // Execution (?):
    // 0) Start from root
    // 1) Get all nodes that depend on root
    // 2) Process all of them
    // 3) As soon as a node has finished processing...
    // 3.1)     Mark it as done
    // 3.2)     Check if there is any node dependent on it whose dependencies
    //          are all done
    // 3.3)     If so, process it

    // Execution runtime ideas:
    // * Have a bit for every node (bitset)
    // * Upon beginning, set all bits to 0
    // * Start processing from root
    // * When a node finished, set its bit to 1, check nodes depending on it
    // * When all bits are set, processing is done, pass to post-processing step
}

void processing_mock(TF continuation_fn)
{
    std::mutex done_mutex;
    std::bitset<node_count> done;
    // or maybe... (is reading from std::array thread safe?)
    std::array<bool, node_count> done;
    // or maybe...
    std::array<atomic<bool>, node_count> done;

    atomic<bool> has_called_continuation;

    auto recurse = []
    {
        // Put unparallelizable nodes first
        auto curr_nodes = sort_by_parallelizability(curr_nodes);

        static_for(curr_nodes, [](auto& n)
            {
                // Don't process if dependencies not satisfied
                if(!n.dependencies_satisfied(done))
                {
                    return;
                }

                auto& nd = get_node_data(n);

                // Continuation after a node is done processing
                auto next_step = []
                {
                    auto lock = done_mutex.lock();
                    done[n] = true;

                    if(!all_done())
                    // alternatively: `if(has_next_nodes())`
                    {
                        // Optimization: recurse only on
                        // forward-neigbors of
                        // n (instead of iterating all nodes)
                        recurse();
                    }
                    else
                    {
                        // Final continuation (get out of processing mock)
                        if(!has_called_continuation)
                        {
                            continuation_fn();
                            has_called_continuation = true;
                        }
                    }
                };

                static_if(is_parallelizable(n))
                {
                    // Asynchronous continuation
                    nd.process().then([]
                        {
                            next_step();
                        });
                }
                static_else
                {
                    // TODO: remove?
                    // Synchronous continuation
                    nd.process();
                    next_step();
                };


            });
    };

    /*auto recurse2 = [](auto parent)
    {
        auto nlist = all_nodes_depending_on(parent);

        static_if(empty(nlist), [](auto)
        {
            return;
        });

        static_for(nlist, [](auto& n)
        {
            auto& nd = get_node_data(n);
            nd.process();
        };

        nlist.join_all([]{ recurse2(
    };

    recurse2(root);*/
}

void full_processing_mock()
{
    processing_mock([]
        {
            post_process();
        });
}

struct Game
{
    void init()
    {
        auto s_physics = make_system( // .
            type_v<SPhysics>,         // .
            depends_on(nothing),      // .
            subdivide::yes, parallelize::yes);

        auto s_collision = make_system( // .
            type_v<SCollision>,         // .
            depends_on(s_physics),      // .
            subdivide::yes, parallelize::yes);

        auto s_contacts = make_processing_step( // .
            type_v<SContacts>,                  // .
            depends_on(s_collision),            // .
            subdivide::yes, parallelize::yes);

        auto steps_dag = steps(s_physics, s_collision, s_contacts);

        auto mgr = make_manager(steps_dag);
    }

    void process()
    {
        mgr.process().then([]()
            {
            });
    }
};
