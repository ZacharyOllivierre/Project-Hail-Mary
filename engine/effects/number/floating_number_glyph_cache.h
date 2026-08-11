#pragma once

#include "../../core/geometry/vector2.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace engine::effects
{
    enum class FloatingNumberColor : std::size_t
    {
        White,
        Black,
        Yellow,
        Green,
        Red,
        Blue,
        LightBlue,
        Orange,
        Purple,
        Count
    };

    using FloatingNumberTexturePtr = std::shared_ptr<SDL_Texture>;

    struct FloatingNumberGlyph
    {
        FloatingNumberTexturePtr texture;
        engine::core::Vector2 source_size;
    };


    class FloatingNumberGlyphCache
    {
    public:
        static constexpr std::size_t k_color_count = static_cast<std::size_t>(FloatingNumberColor::Count);

    private:
        std::array<std::unordered_map<char, FloatingNumberGlyph>, k_color_count> _glyphs;
        SDL_Renderer* _renderer = nullptr;
        TTF_Font* _font = nullptr;
        std::uint64_t _font_generation = 0;
    };
}