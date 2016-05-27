#include <atomic>
#include <iostream>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <cassert>
#include <condition_variable>
#include <future>

namespace etp
{
    /**
        * @brief The FixedFunction<R(ARGS...), STORAGE_SIZE> class implements
        * functional object.
        * This function is analog of 'std::function' with limited capabilities:
        *  - It supports only move semantics.
        *  - The size of functional objects is limited to storage size.
        * Due to limitations above it is much faster on creation and copying
     * than
        * std::function.
        */
    template <typename SIGNATURE, size_t STORAGE_SIZE = 64>
    class FixedFunction;

    template <typename R, typename... ARGS, size_t STORAGE_SIZE>
    class FixedFunction<R(ARGS...), STORAGE_SIZE>
    {

        typedef R (*func_ptr_type)(ARGS...);

    public:
        FixedFunction()
            : m_function_ptr(nullptr), m_method_ptr(nullptr),
              m_alloc_ptr(nullptr)
        {
        }

        /**
         * @brief FixedFunction Constructor from functional object.
         * @param object Functor object will be stored in the internal storage
         * using move constructor. Unmovable objects are prohibited explicitly.
         */
        template <typename FUNC>
        FixedFunction(FUNC&& object)
            : FixedFunction()
        {
            typedef typename std::remove_reference<FUNC>::type unref_type;

            static_assert(sizeof(unref_type) < STORAGE_SIZE,
                "functional object doesn't fit into internal storage");
            static_assert(std::is_move_constructible<unref_type>::value,
                "Should be of movable type");

            m_method_ptr = [](
                void* object_ptr, func_ptr_type, ARGS... args) -> R
            {
                return static_cast<unref_type*>(object_ptr)
                    ->
                    operator()(args...);
            };

            m_alloc_ptr = [](void* storage_ptr, void* object_ptr)
            {
                if(object_ptr)
                {
                    unref_type* x_object = static_cast<unref_type*>(object_ptr);
                    new(storage_ptr) unref_type(std::move(*x_object));
                }
                else
                {
                    static_cast<unref_type*>(storage_ptr)->~unref_type();
                }
            };

            m_alloc_ptr(&m_storage, &object);
        }

        /**
         * @brief FixedFunction Constructor from free function or static member.
         */
        template <typename RET, typename... PARAMS>
        FixedFunction(RET (*func_ptr)(PARAMS...))
            : FixedFunction()
        {
            m_function_ptr = func_ptr;
            m_method_ptr = [](void*, func_ptr_type f_ptr, ARGS... args) -> R
            {
                return static_cast<RET (*)(PARAMS...)>(f_ptr)(args...);
            };
        }

        FixedFunction(FixedFunction&& o) : FixedFunction()
        {
            moveFromOther(o);
        }

        FixedFunction& operator=(FixedFunction&& o)
        {
            moveFromOther(o);
            return *this;
        }

        ~FixedFunction()
        {
            if(m_alloc_ptr) m_alloc_ptr(&m_storage, nullptr);
        }

        /**
         * @brief operator () Execute stored functional object.
         * @throws std::runtime_error if no functional object is stored.
         */
        R operator()(ARGS... args)
        {
            if(!m_method_ptr) throw std::runtime_error("call of empty functor");
            return m_method_ptr(&m_storage, m_function_ptr, args...);
        }

    private:
        FixedFunction& operator=(const FixedFunction&) = delete;
        FixedFunction(const FixedFunction&) = delete;

        union
        {
            typename std::aligned_storage<STORAGE_SIZE, sizeof(size_t)>::type
                m_storage;
            func_ptr_type m_function_ptr;
        };

        typedef R (*method_type)(
            void* object_ptr, func_ptr_type free_func_ptr, ARGS... args);
        method_type m_method_ptr;

        typedef void (*alloc_type)(void* storage_ptr, void* object_ptr);
        alloc_type m_alloc_ptr;

        void moveFromOther(FixedFunction& o)
        {
            if(this == &o) return;

            if(m_alloc_ptr)
            {
                m_alloc_ptr(&m_storage, nullptr);
                m_alloc_ptr = nullptr;
            }
            else
            {
                m_function_ptr = nullptr;
            }

            m_method_ptr = o.m_method_ptr;
            o.m_method_ptr = nullptr;

            if(o.m_alloc_ptr)
            {
                m_alloc_ptr = o.m_alloc_ptr;
                m_alloc_ptr(&m_storage, &o.m_storage);
            }
            else
            {
                m_function_ptr = o.m_function_ptr;
            }
        }
    };

    using task = FixedFunction<void(), 64>;

    class task_queue
    {
    public:
        using mutex_type = std::mutex;

    private:
        std::deque<task> _tasks;
        mutable mutex_type _mutex;

    public:
        void push(task&& t)
        {
            std::lock_guard<mutex_type> l(_mutex);
            _tasks.emplace_front(std::move(t));
        }

        auto try_pop(task& t)
        {
            std::lock_guard<mutex_type> l(_mutex);

            if(_tasks.empty())
            {
                return false;
            }

            t = std::move(_tasks.back());
            _tasks.pop_back();

            return true;
        }
    };

    class worker
    {
    private:
        std::atomic<bool> _running;
        std::thread _thread;
        task_queue* _queue;

        void run()
        {
            assert(_running);
            task t;

            while(true)
            {
                auto success = _queue->try_pop(t);

                if(success)
                {
                    t();
                }
                else if(_running)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                else
                {
                    return;
                }
            }
        }

    public:
        worker(task_queue& queue) : _queue{&queue}
        {
        }

        worker(const worker&) = delete;
        worker& operator=(const worker&) = delete;

        worker(worker&& rhs)
            : _running{rhs._running.load()}, _thread{std::move(rhs._thread)},
              _queue{rhs._queue}
        {
        }

        worker& operator=(worker&& rhs)
        {
            _running = rhs._running.load();
            _thread = std::move(rhs._thread);
            _queue = rhs._queue;

            return *this;
        }

        void start()
        {
            assert(!_running);

            _thread = std::thread([this]
                {
                    _running = true;
                    run();
                });
        }

        void stop()
        {
            assert(_running);

            _running = false;
            _thread.join();
        }
    };

    class pool
    {
    private:
        std::unique_ptr<task_queue> _queue;
        std::vector<worker> _workers;

    public:
        pool()
        {
            _queue = std::make_unique<task_queue>();
            auto hwc = std::thread::hardware_concurrency() - 1;

            for(decltype(hwc) i = 0; i < hwc; ++i)
            {
                _workers.emplace_back(*_queue);
            }

            for(auto& w : _workers)
            {
                w.start();
            }
        }

        ~pool()
        {
            for(auto& w : _workers)
            {
                w.stop();
            }
        }

        pool(const pool&) = delete;
        pool& operator=(const pool&) = delete;

        pool(pool&&) = default;
        pool& operator=(pool&) = default;

        template <typename TF>
        void post(TF&& f)
        {
            _queue->push(std::move(f));
        }

        template <typename TF>
        auto process(TF&& f)
        {
            std::packaged_task<void()> task([f = std::move(f)]
                {
                    return f();
                });

            auto fut = task.get_future();

            post(std::move(task));
            return fut;
        }
    };
}

int main()
{
    etp::pool p;

    auto testf = [](auto i)
    {
        return [i]
        {
            std::cout << "(" << i << ") started\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "(" << i << ") finished\n";
        };
    };

    std::vector<std::future<void>> futs;

    for(int i = 0; i < 100; ++i)
    {
        futs.emplace_back(p.process(testf(i)));
    }

    for(auto& f : futs)
    {
        f.get();
    }
}