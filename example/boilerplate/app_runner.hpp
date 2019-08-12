// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ecst/aliases.hpp>
#include <ecst/config.hpp>
#include <functional>
#include <memory>
#include <string>
#include <vrm/core/assert.hpp>

namespace example
{
    namespace boilerplate
    {
        template <typename T>
        class app_runner
        {
        private:
            sf::RenderWindow _window;

            // TODO: use unique_ptr if compiler stops crashing...
            T* _app = nullptr;
            std::function<void()> _run_fn;

        public:
            template <typename... Ts>
            inline app_runner(const std::string& title, std::size_t width,
                std::size_t height, Ts&&... xs) noexcept
            {
                _run_fn = [this, title, width, height, &xs...] {
                    if(_window.isOpen()) _window.close();

                    _window.create(sf::VideoMode(width, height), title,
                        sf::Style::Default);

#ifdef EXAMPLE_LIMIT_FRAMERATE
                    _window.setVerticalSyncEnabled(true);
                    _window.setFramerateLimit(60);
#endif
                    delete _app;
                    _app = new T(_window, FWD(xs)...);
                };
            }

            ~app_runner()
            {
                delete _app;
            }

            inline auto& app() noexcept
            {
                ECST_ASSERT_OP(_app, !=, nullptr);
                return *_app;
            }

            inline void run() noexcept
            {
                this->_run_fn();
            }
        };
    } // namespace boilerplate
} // namespace example
