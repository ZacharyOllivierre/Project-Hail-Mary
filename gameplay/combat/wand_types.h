#pragma once

#include "../../engine/core/geometry/vector2.h"
#include "status_effect.h"

#include <functional>
#include <memory>
#include <string>

class BulletBehaviorSet;

struct Bullet_Attributes
{
    float bullet_speed = 500.0f;
    engine::core::Vector2 bullet_velocity;

    float max_age = 20.0f;

    engine::core::Vector2 start_position;
    engine::core::Vector2 bullet_size = {24.0f, 24.0f};

    float damage = 100.0f;

    // ToDo redo status effect system
    std::shared_ptr<StatusEffect> status_effect = nullptr;
    std::vector<std::function<void(BulletBehaviorSet &)>> bullet_behavior_appenders;

    float damage_cooldown_sec = 0.3;

    // Sound keys for bullet event hooks, empty = no sound
    std::string sound_on_fire;
    std::string sound_on_collision;
    std::string sound_on_entity_collision;
    std::string sound_on_death;
    std::string sound_during_flight;
};

enum class SpreadStyle
{
    Uniform,
    Circular,
    Random
};

enum class ShotStyle
{
    Simultaneous,
    Sequential,
    ReverseSequential
};

struct WandAttributes
{
    // Need to implement these two
    float cooldown_seconds;
    float mana_cost;

    int bullet_count = 1;

    SpreadStyle spread_style = SpreadStyle::Uniform;
    float spread_degrees = 180.0;

    ShotStyle shot_style = ShotStyle::Simultaneous;
    float first_shot_delay = 0.0f;
    float shot_delay_sec = 0.1f;

    float spawn_distance = 32.0f;

    // Temp : Currently set for wand to have poison in wand constructor
    std::vector<std::shared_ptr<StatusEffect>> status_effect;
    float effect_chance = 0.5f; // chance for every bullet to roll for status effect
};

/*
ShotDescriptor is a per shot intention record, not a live projectile. It carries the bullet template,
the relative spawn offset, and the spawn delay. The room scene resolves the final world position when the
shot actually fires so delayed shots still spawn relative to the character’s current position.
*/
struct ShotDescriptor
{
    Bullet_Attributes bullet_attributes;

    // Bullet position position relative to character pos
    engine::core::Vector2 spawn_offset;
    engine::core::Vector2 shot_direction;

    float spawn_delay_sec;
};
