#pragma once

#include "floating_number_glyph_cache.h"
#include "../../core/game_object.h"
#include "../../core/interface/updatable.h"

#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

namespace engine::effects
{
    enum class FloatingNumberAlignment
    {
        Left,
        Center,
        Right
    };

    struct FloatingNumberEffectTimeRange
    {
        float start_progress = 0.0f;
        float end_progress = 1.0f;
    };

    struct FloatingNumberLinearMotion
    {
        engine::core::Vector2 offset;
        FloatingNumberEffectTimeRange time_range;
    };

    struct FloatingNumberArcMotion
    {
        engine::core::Vector2 offset;
        float arc_height = 0.0f;
        FloatingNumberEffectTimeRange time_range;
    };

    using FloatingNumberMotion = std::variant<FloatingNumberLinearMotion, FloatingNumberArcMotion>;

    struct FloatingNumberScale
    {
        float from_scale = 1.0f;
        float to_scale = 1.0f;
        FloatingNumberEffectTimeRange time_range;
    };

    struct FloatingNumberFade
    {
        uint8_t from_alpha = 255;
        uint8_t to_alpha = 0;
        FloatingNumberEffectTimeRange time_range;
    };

    struct FloatingNumberEffects
    {
        std::optional<FloatingNumberMotion> motion;
        std::optional<FloatingNumberScale> scale;
        std::optional<FloatingNumberFade> fade;
    };


    class FloatingNumberEffect final : public engine::core::GameObject, public engine::core::Updatable
    {
    public:
        using Callback = std::function<void(FloatingNumberEffect&)>;
    private:
    };
}