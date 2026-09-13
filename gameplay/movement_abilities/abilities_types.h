#pragma once

// TODO implement iframes

enum class MovementAbilityType
{
    Roll,
    Dash,
    Teleport
};

struct MovementAbilityConfig
{
    float distance = 0.0f;
    float duration = 0.0f;
    float cooldown = 0.0f;
    float max_range = 0.0f;
};

[[nodiscard]] constexpr MovementAbilityConfig movement_ability_config(MovementAbilityType type) noexcept
{
    switch (type)
    {
    case MovementAbilityType::Roll:
        return {.distance = 150.0f, .duration = 0.35f, .cooldown = 0.65f, .max_range = 0.0f};
    case MovementAbilityType::Dash:
        return {.distance = 100.0f, .duration = 0.10f, .cooldown = 0.45f, .max_range = 0.0f};
    case MovementAbilityType::Teleport:
        return {.distance = 0.0f, .duration = 0.05f, .cooldown = 0.75f, .max_range = 350.0f};
    }

    return {};
}