// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <functional>
#include <string>
#include <memory>
#include <vrm/core/assert.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

namespace example
{
    namespace boilerplate
    {
        template <typename T>
        class app_runner
        {
        private:
            sf::RenderWindow _window;
            std::unique_ptr<T> _app;
            std::function<void()> _run_fn;

        public:
            template <typename... Ts>
            inline app_runner(const std::string& title, std::size_t width,
                std::size_t height, Ts&&... xs) noexcept
            {
                _run_fn = [this, title, width, height, &xs...]
                {
                    if(_window.isOpen()) _window.close();

                    _window.create(sf::VideoMode(width, height), title,
                        sf::Style::Default);

#ifdef EXAMPLE_LIMIT_FRAMERATE
                    _window.setVerticalSyncEnabled(true);
                    _window.setFramerateLimit(60);
#endif

                    _app = std::make_unique<T>(_window, FWD(xs)...);
                };
            }

            inline auto& app() noexcept
            {
                ECST_ASSERT_OP(_app.get(), !=, nullptr);
                return *_app;
            }

            inline void run() noexcept
            {
                this->_run_fn();
            }
        };
    }
}
