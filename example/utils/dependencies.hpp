// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <ecst.hpp>

namespace example
{
    // Type aliases.
    using ft = float;
    using vrm::core::sz_t;

    // SFML type aliases.
    using sfc = sf::Color;
    using vec2f = sf::Vector2f;
    using vec2i = sf::Vector2i;
    using vec2u = sf::Vector2u;
} // namespace example

#include "../boilerplate.hpp"
#include "./pres_utils.hpp"
#include "./trig.hpp"
