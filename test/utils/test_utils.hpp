#pragma once

#include <cassert>
#include <ecst.hpp>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>

#define TEST_CONST __attribute__((const))
#define TEST_MAIN(...) int TEST_CONST main(__VA_ARGS__)

#define TEST_IMPL_DEPARENS(...) __VA_ARGS__

#define SA_SAME(value, type) \
    static_assert(           \
        std::is_same<TEST_IMPL_DEPARENS value, TEST_IMPL_DEPARENS type>{}, "")

#define SA_TYPE(value, type) \
    static_assert(std::is_same<decltype value, TEST_IMPL_DEPARENS type>{}, "")

#define VRM_CORE_LIKELY(...) __builtin_expect(!!(__VA_ARGS__), 1)

namespace test_impl
{
    namespace impl
    {
        inline auto& get_ostringstream() noexcept
        {
            static std::ostringstream oss;
            return oss;
        }

        inline auto& clear_and_get_ostringstream() noexcept
        {
            auto& oss(get_ostringstream());
            oss.str("");
            return oss;
        }

        [[noreturn]] inline void fail() noexcept
        {
            std::cout << get_ostringstream().str() << std::endl;
            std::terminate();
        }

        template <typename TStream>
        inline void output_line(TStream& s, int line)
        {
            s << "line: " << line << "\n";
        }

        template <typename TStream>
        inline void output_expr(TStream& s, const char* expr)
        {
            s << "expr: " << expr << "\n";
        }

        template <typename TStream, typename T>
        inline void output_result(TStream& s, const T& lhs_result)
        {
            s << "result: " << lhs_result << "\n";
        }

        template <typename TStream>
        inline void output_expected(TStream& s, const char* expected)
        {
            s << "expected: " << expected << "\n";
        }

        template <typename TStream, typename T>
        inline void output_expected(
            TStream& s, const char* expected, const T& rhs_result)
        {
            s << "expected: " << expected << " -> " << rhs_result << "\n";
        }

        template <typename TF>
        inline auto do_test(bool x, TF&& f)
        {
            if(VRM_CORE_LIKELY(x)) return;

            auto& error(impl::clear_and_get_ostringstream());
            f(error);
            impl::fail();
        }
    } // namespace impl

    template <typename T>
    inline auto test_expr(
        int line, bool x, T&& lhs_result, const char* expr) noexcept
    {
        return impl::do_test(x, [&](auto& s) {
            impl::output_line(s, line);
            impl::output_expr(s, expr);
            impl::output_result(s, lhs_result);
        });
    }

    template <typename TLhs, typename TRhs>
    inline auto test_op(int line, bool x, TLhs&& lhs_result, TRhs&& rhs_result,
        const char* expr, const char* expected)
    {
        return impl::do_test(x, [&](auto& s) {
            impl::output_line(s, line);
            impl::output_expr(s, expr);
            impl::output_result(s, lhs_result);
            impl::output_expected(s, expected, rhs_result);
        });
    }

    inline auto test_expr_ns(int line, bool x, const char* expr) noexcept
    {
        return impl::do_test(x, [&](auto& s) {
            impl::output_line(s, line);
            impl::output_expr(s, expr);
        });
    }

    inline auto test_op_ns(
        int line, bool x, const char* expr, const char* expected)
    {
        return impl::do_test(x, [&](auto& s) {
            impl::output_line(s, line);
            impl::output_expr(s, expr);
            impl::output_expected(s, expected);
        });
    }
} // namespace test_impl

#define TEST_ASSERT(expr)                                  \
    do                                                     \
    {                                                      \
        auto _t_x(expr);                                   \
                                                           \
        test_impl::test_expr(__LINE__, _t_x, expr, #expr); \
    } while(false)

#define TEST_ASSERT_OP(lhs, op, rhs)                                           \
    do                                                                         \
    {                                                                          \
        using ct = std::common_type_t<decltype(lhs), decltype(rhs)>;           \
                                                                               \
        auto _t_xl(lhs);                                                       \
        auto _t_xr(rhs);                                                       \
                                                                               \
        auto _t_x(ct(_t_xl) op ct(_t_xr));                                     \
                                                                               \
        test_impl::test_op(__LINE__, _t_x, _t_xl, _t_xr, #lhs #op #rhs, #rhs); \
    } while(false)

#define TEST_ASSERT_NS(expr)                            \
    do                                                  \
    {                                                   \
        auto _t_x(expr);                                \
                                                        \
        test_impl::test_expr_ns(__LINE__, _t_x, #expr); \
    } while(false)

#define TEST_ASSERT_NS_OP(lhs, op, rhs)                             \
    do                                                              \
    {                                                               \
        auto _t_xl(lhs);                                            \
        auto _t_x(_t_xl op rhs);                                    \
                                                                    \
        test_impl::test_op_ns(__LINE__, _t_x, #lhs #op #rhs, #rhs); \
    } while(false)

#define DEPARENS(...) __VA_ARGS__

#define SA(...) static_assert(__VA_ARGS__, "")

#define SA_SDT(a, b)                                                \
    static_assert(std::is_same<std::decay_t<decltype(DEPARENS(a))>, \
                      std::decay_t<decltype(DEPARENS(b))>>{},       \
        "")

#define STYP(...) static_assert(same_type(__VA_ARGS__), "");


template <int TI>
constexpr vrm::core::int_<TI> iv{};

template <vrm::core::sz_t TI>
constexpr vrm::core::sz_t_<TI> sv{};
