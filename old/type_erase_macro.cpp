// TODO:

#define ERASED(x)                                                         \
    using VRM_PP_CAT(VRM_PP_EXPAND(x), _tt) = decltype(VRM_PP_EXPAND(x)); \
    struct VRM_PP_CAT(e_, VRM_PP_EXPAND(x), _tt) : VRM_PP_CAT(x, _tt)     \
    {                                                                     \
        constexpr VRM_PP_CAT(e_, VRM_PP_EXPAND(x), _tt)()                 \
        {                                                                 \
        }                                                                 \
    };                                                                    \
    constexpr VRM_PP_CAT(e_, VRM_PP_EXPAND(x), _tt)                       \
        VRM_PP_CAT(erased_, VRM_PP_EXPAND(x));


#define MAKE_ERASED_SLS_STEP0_BODY(i, d, a) ERASED(VRM_PP_EXPAND(a));
#define MAKE_ERASED_SLS_STEP1_BODY(i, d, a) \
    VRM_PP_CAT(erased_, VRM_PP_EXPAND(a)) VRM_PP_COMMA_IF(i)

#define MAKE_ERASED_SLS(...)                                     \
    VRM_PP_FOREACH_REVERSE(                                      \
        MAKE_ERASED_SLS_STEP0_BODY, VRM_PP_EMPTY(), __VA_ARGS__) \
    return sls::make(VRM_PP_FOREACH_REVERSE(                     \
        MAKE_ERASED_SLS_STEP1_BODY, VRM_PP_EMPTY(), __VA_ARGS__))

#if 0
            MAKE_ERASED_SLS(               // .
                ssig_acceleration,         // .
                ssig_velocity,             // .
                ssig_keep_in_bounds,       // .
                ssig_spatial_partition,    // .
                ssig_collision,            // .
                ssig_solve_contacts,       // .
                ssig_render_colored_circle // .
                );
#else


#endif
