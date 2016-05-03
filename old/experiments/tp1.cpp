#include <thread>
#include <mutex>
#include <chrono>
#include <future>
#include <iostream>

#include <thread_pool/thread_pool.hpp>

using namespace std::literals;

using ts =
    tp::ThreadPoolSettings<tp::PackagedTaskAdaptorStd<std::packaged_task>>;

using tptp = tp::ThreadPool<ts>;

tptp _tp;
std::mutex mtx;

auto slp = [](auto x)
{
    std::this_thread::sleep_for(x);
};

auto df = [](auto ys, auto yms)
{
    {
        std::lock_guard<decltype(mtx)> l(mtx);
        std::cout << "Starting " << ys << "\n";
    }

    slp(yms);

    {
        std::lock_guard<decltype(mtx)> l(mtx);
        std::cout << "Finished " << ys << "\n";
    }
};

void bf_test_tp2()
{
    auto mkf = [](auto s, auto xms)
    {
        return _tp.process([s, xms]
            {
                // TODO: correctly 100%'s all cores
                /*
                volatile int kk = 0;

                for(volatile int jj= 0; jj < 100; ++jj)
                for(volatile int k = 0; k < 10000000; ++k)
                {
                    kk += 1;
                }
                */
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
                df(s, xms);
            });
    };

    std::vector<std::future<void>> vf;

    for(auto i(0); i < 1000; ++i)
    {
        auto f0 = mkf(i, 1ms);
        vf.emplace_back(std::move(f0));
    }

    for(auto& f : vf) f.get();
}

void bf_test_tp3()
{
    auto mkf = [](auto s, auto xms)
    {
        return _tp.try_process([s, xms]
            {
                // TODO: correctly 100%'s all cores
                /*
                volatile int kk = 0;

                for(volatile int jj= 0; jj < 100; ++jj)
                for(volatile int k = 0; k < 10000000; ++k)
                {
                    kk += 1;
                }
                */
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
                df(s, xms);
            });
    };

    std::vector<std::future<void>> vf;

    for(auto i(0); i < 1000; ++i)
    {
        auto f0 = mkf(i, 1ms);
        vf.emplace_back(std::move(std::get<1>(f0)));
    }

    for(auto& f : vf) f.get();
}


int main()
{
    // bf_test_tp2();
    bf_test_tp3();
    return 0;
}