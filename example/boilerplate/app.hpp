#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ecst/config.hpp>
#include <ecst/aliases.hpp>

namespace example
{
    namespace boilerplate
    {
        class app
        {
        protected:
            sf::RenderWindow& _window;

        public:
            inline app(sf::RenderWindow& window) noexcept : _window{window}
            {
            }

            inline void stop() noexcept
            {
                return _window.close();
            }

            template <typename... Ts>
            inline void render(Ts&&... xs) noexcept
            {
                _window.draw(FWD(xs)...);
            }

            inline auto& window() noexcept
            {
                return _window;
            }
            inline const auto& window() const noexcept
            {
                return _window;
            }
        };
    }
}
