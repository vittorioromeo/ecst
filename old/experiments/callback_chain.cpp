#define BOOST_THREAD_VERSION 4
#define BOOST_RESULT_OF_USE_DECLTYPE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION

#include <iostream>
#include <mutex>
#include <boost/thread/future.hpp>

std::mutex cout_mutex;

auto task(char x)
{
    return boost::async(boost::launch::async, [x]
        {
            {
                std::lock_guard<decltype(cout_mutex)> l(cout_mutex);
                std::cout << "starting " << x << "\n";
            }

            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

            {
                std::lock_guard<decltype(cout_mutex)> l(cout_mutex);
                std::cout << "completed " << x << "\n";
            }
        });
}

int main()
{
    using namespace boost;

    task('A')
        .then([](auto&&)
            {
                when_all(when_all(task('B'), task('C'))
                             .then([](auto&&)
                                 {
                                     task('D').get();
                                 }),
                    task('E'))
                    .then([](auto&&)
                        {
                            task('F').get();
                        })
                    .get();
            })
        .get();

    return 0;
}