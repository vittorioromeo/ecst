#include <thread>
#include <mutex>
#include <chrono>

#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY
#include <boost/thread/future.hpp>

#define THREAD_POOL_USE_BOOST_FUTURE 1
#include <thread_pool/thread_pool.hpp>

using namespace std::literals;

using ts =
    tp::ThreadPoolSettings<tp::PackagedTaskAdaptorBoost<boost::packaged_task>>;

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

void bf_test()
{
    auto mkf = [](auto s, auto xms)
    {
        return boost::async(boost::launch::async, [s, xms]
            {
                df(s, xms);
            });
    };

    auto f0 = mkf(0, 1000ms);
    auto f1 = mkf(1, 1000ms);
    auto f2 = mkf(2, 1000ms);
    auto f3 = mkf(3, 1000ms);

    auto fwa = boost::when_all( // .
        std::move(f0),          // .
        std::move(f1),          // .
        std::move(f2),          // .
        std::move(f3));

    auto fwat = fwa.then([](auto)
        {
            {
                std::lock_guard<decltype(mtx)> l(mtx);
                std::cout << "All done!\n";
            }
        });

    fwat.get();
}



void bf_test_tp()
{
    auto mkf = [](auto s, auto xms)
    {
        return _tp.process([s, xms]
            {
                df(s, xms);
            });
    };

    auto f0 = mkf(0, 1000ms);
    auto f1 = mkf(1, 1000ms);
    auto f2 = mkf(2, 1000ms);
    auto f3 = mkf(3, 1000ms);

    auto fwa = boost::when_all( // .
        std::move(f0),          // .
        std::move(f1),          // .
        std::move(f2),          // .
        std::move(f3));

    auto fwat = fwa.then([](auto)
        {
            {
                std::lock_guard<decltype(mtx)> l(mtx);
                std::cout << "All done!\n";
            }
        });

    fwat.get();
}


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

                df(s, xms);
            });
    };

    std::vector<boost::future<void>> vf;

    for(auto i(0); i < 1000; ++i)
    {
        auto f0 = mkf(i, 1ms);
        vf.emplace_back(std::move(f0));
    }

    auto fwa = boost::when_all(std::begin(vf), std::end(vf));

    auto fwat = fwa.then([](auto)
        {
            {
                std::lock_guard<decltype(mtx)> l(mtx);
                std::cout << "All done!\n";
            }
        });

    fwat.get();
}

int main()
{
    bf_test();
    bf_test_tp();
    bf_test_tp2();


    return 0;
}