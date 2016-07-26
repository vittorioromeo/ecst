#include "../../utils/test_utils.hpp"
#include <ecst/utils/function_queue.hpp>

struct test_struct
{
    test_struct()
    {
        std::cout << "ctor\n";
    }
    ~test_struct()
    {
        std::cout << "dtor\n";
    }
};

template <typename TSignature, std::size_t TBufferSize>
struct function_queue_test_adapter
{
private:
    std::vector<std::function<TSignature>> _fn_vec;
    ecst::fixed_function_queue<TSignature, TBufferSize> _fn_queue;

public:
    template <typename TF>
    void emplace(TF&& f)
    {
        _fn_vec.emplace_back(FWD(f));
        _fn_queue.emplace(FWD(f));
    }

    template <typename... Ts>
    void call_all(Ts&&... xs)
    {
        for(auto& f : _fn_vec)
        {
            f(FWD(xs)...);
        }

        _fn_queue.call_all(FWD(xs)...);
    }
};

static volatile int ctors;
static volatile int copy_ctors;
static volatile int move_ctors;
static volatile int dtors;

struct cpyx
{
    cpyx()
    {
        ++ctors;
    }
    ~cpyx()
    {
        ++dtors;
    }

    cpyx(const cpyx&)
    {
        ++copy_ctors;
    }

    cpyx(cpyx&&) = delete;
};


struct cpyxm
{
    cpyxm()
    {
        ++ctors;
    }
    ~cpyxm()
    {
        ++dtors;
    }

    cpyxm(const cpyxm&) = delete;

    cpyxm(cpyxm&&)
    {
        ++move_ctors;
    }
};

void cypx_reset()
{
    ctors = dtors = copy_ctors = move_ctors = 0;
}

#define cypx_test(xctors, xcopy_ctors, xmove_ctors, xdtors) \
    {                                                       \
        TEST_ASSERT_OP(ctors, ==, xctors);                  \
        TEST_ASSERT_OP(copy_ctors, ==, xcopy_ctors);        \
        TEST_ASSERT_OP(move_ctors, ==, xmove_ctors);        \
        TEST_ASSERT_OP(dtors, ==, xdtors);                  \
    }

void tests()
{

    {
        int acc = 0;
        int one = 1;

        function_queue_test_adapter<void(int), 512> ta;

        ta.emplace([&acc](int x)
            {
                acc += x;
            });
        ta.emplace([&acc, one](int)
            {
                acc += one;
            });
        ta.emplace([&acc](int x)
            {
                acc -= x;
            });
        ta.emplace([&acc, one](int)
            {
                acc -= one;
            });

        ta.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
    }

    {
        int acc = 0;

        struct tx
        {
            int& _acc_ref;
            bool _dec{true};

            tx(int& acc_ref) : _acc_ref(acc_ref)
            {
                ++_acc_ref;
            }
            tx(const tx& x) : _acc_ref(x._acc_ref), _dec{false}
            {
            }

            ~tx()
            {
                if(_dec)
                {
                    --_acc_ref;
                }
            }
        };

        {
            function_queue_test_adapter<void(int), 512> ta;
            tx c_tx(acc);

            ta.emplace([&acc](int x)
                {
                    acc += x;
                });
            ta.emplace([&acc, my_tx = tx{acc} ](int)
                {
                    acc += 1;
                });
            ta.emplace([&acc](int x)
                {
                    acc -= x;
                });
            ta.emplace([&acc, c_tx](int)
                {
                    acc -= 1;
                });

            ta.call_all(5);
        }

        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
    }
}

void copy_tests()
{
    {
        cypx_reset();
        std::cout << "a\n";
        cypx_test(0, 0, 0, 0);

        cpyx px;
        std::cout << "b\n";
        cypx_test(1, 0, 0, 0);

        int acc = 0;
        int one = 1;

        ecst::fixed_function_queue<void(int), 512> ta;

        ta.emplace([&acc, px](int x)
            {
                acc += x;
            });
        std::cout << "c\n";
        std::cout << copy_ctors << "\n";
        cypx_test(1, 2, 0, 1);


        ta.emplace([&acc, one, px](int)
            {
                acc += one;
            });
        std::cout << "d\n";
        cypx_test(1, 4, 0, 2);

        ta.emplace([&acc, lel = std::vector<int>{} ](int x)
            {
                acc -= x;
            });
        ta.emplace([&acc, one](int)
            {
                acc -= one;
            });



        ta.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);

        std::cout << "e\n";
        cypx_test(1, 4, 0, 2);
        acc = 0;
        one = 1;

        auto ta2 = ta;
        std::cout << "f\n";

        cypx_test(1, 6, 0, 2);
        ta2.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
        std::cout << "g\n";
        cypx_test(1, 6, 0, 2);
    }

    cypx_test(1, 6, 0, 7);
}

void move_tests()
{
    {
        cypx_reset();
        std::cout << "a\n";
        cypx_test(0, 0, 0, 0);

        cpyxm px;
        std::cout << "b\n";
        cypx_test(1, 0, 0, 0);

        int acc = 0;
        int one = 1;

        ecst::fixed_function_queue<void(int), 512> ta;

        ta.emplace([&acc, px = std::move(px) ](int x)
            {
                acc += x;
            });
        std::cout << "c\n";
        std::cout << move_ctors << "\n";
        cypx_test(1, 0, 2, 1);


        ta.emplace([&acc, one, px = std::move(px) ](int)
            {
                acc += one;
            });
        std::cout << "d\n";
        cypx_test(1, 0, 4, 2);
        ta.emplace([&acc, lel = std::vector<int>{} ](int x)
            {
                acc -= x;
            });
        ta.emplace([&acc, one](int)
            {
                acc -= one;
            });



        ta.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);

        std::cout << "e\n";
        cypx_test(1, 0, 4, 2);
        acc = 0;
        one = 1;

        auto ta2 = std::move(ta);
        std::cout << "f\n";

        cypx_test(1, 0, 6, 2);
        ta2.call_all(5);
        std::cout << acc << "\n";
        TEST_ASSERT(acc == 0);
        std::cout << "g\n";

        cypx_test(1, 0, 6, 2);
    }

    cypx_test(1, 0, 6, 7);
}

int main()
{
    // std::cout << sizeof(void*) << "\n";
    // return 0;

    tests();
    copy_tests();
    move_tests();

    test_struct xxx;

    ecst::fixed_function_queue<void(int), 512> f0;


    f0.emplace([ xxx, aaa = 100 ](int x)
        {
            std::cout << x << " hi!\n";
        });

    f0.emplace([ xxx, bbb = std::vector<int>{0} ](int x)
        {
            std::cout << x << " hi again!\n";
        });

    f0.emplace([&xxx](int x)
        {
            std::cout << x << " hi byref!\n";
        });


    f0.call_all(5);
}
