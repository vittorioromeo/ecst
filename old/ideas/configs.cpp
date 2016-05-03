class system_status
{
    // ... filled by macros and compile-time constants
};

// stuff:
bool system_status::hw_cores_known sz_t system_status::hw_cores bool
    system_status::cache_size_known sz_t system_status::cache_size

// user config

{
    namespace es = ecst::storage_strategy_options;
    auto ss = ecst::make_storage_strategy(        // .
    (                                             // .
        es::from(es::bytes<0>, es::array),        // .
        es::from(es::bytes<256>, es::ptr_array),  // .
        es::from(es::bytes<1024>, es::hash_table) // .
    );

    namespace ec = ecst::config_options;
    auto config = ecst::make_config               // .
    (                                             // .
        ec::multithreading(ec::enabled),          // .
        // ec::multithreading(ec::disabled),         // .
        ec::max_entities(ec::fixed(sz_v<10000>)), // .
        // ec::max_entities(ec::dynamic),            // .
        ec::storage_strategy(ss),                 // .
    );
}