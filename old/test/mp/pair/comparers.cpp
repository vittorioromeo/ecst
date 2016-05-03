#include "../../../utils/test_utils.hpp"

using namespace vrm::core;
using namespace ecst::mp;

TEST_MAIN()
{
    constexpr auto p0 = pair::make(iv<0>, iv<99>);
    constexpr auto p1 = pair::make(iv<99>, iv<0>);

    auto is_fst_zero = [](auto p)
    {
        return pair::fst_comparer(p, iv<0>);
    };

    auto is_fst_nn = [](auto p)
    {
        return pair::fst_comparer(p, iv<99>);
    };

    auto is_snd_zero = [](auto p)
    {
        return pair::snd_comparer(p, iv<0>);
    };

    auto is_snd_nn = [](auto p)
    {
        return pair::snd_comparer(p, iv<99>);
    };

#define SADT(...) SA(decltype(__VA_ARGS__){})
#define SADTF(...) SA(!decltype(__VA_ARGS__){})

    SA(pair::fst_is(p0, iv<0>));

    SADT(is_fst_zero(p0));
    SADTF(is_fst_zero(p1));

    SADTF(is_fst_nn(p0));
    SADT(is_fst_nn(p1));

    SADTF(is_snd_zero(p0));
    SADT(is_snd_zero(p1));

    SADT(is_snd_nn(p0));
    SADTF(is_snd_nn(p1));
}
