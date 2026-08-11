#pragma once

#include "rune.h"

#include <functional>

#include "../bullet_behavior/bullet_behavior_set.h"
#include "../wand_types.h"

// TODO Added slots not working

class WeaponRune : public Rune
{
protected:
    WeaponRune() : Rune(RuneType::Weapon) {}

public:
    void apply_weapon(RuneLoadout &loadout) const override
    {
        loadout.wand_attributes = _wand_attributes;
        loadout.bullet_attributes = _bullet_attributes;
        loadout.bullet_behavior_appenders.insert(
            loadout.bullet_behavior_appenders.end(),
            _bullet_behavior_appenders.begin(),
            _bullet_behavior_appenders.end());
    }

    [[nodiscard]] int added_slots() const noexcept
    {
        return _added_slots;
    }

    [[nodiscard]] WeaponConsumptionData consumption() const noexcept
    {
        return _consumption;
    }

protected:
    void set_added_slots(int added_slots) noexcept
    {
        _added_slots = added_slots > 0 ? added_slots : 0;
    }

    void set_consumed_runes(int consumed_runes) noexcept
    {
        _consumption.consumed_runes = consumed_runes > 0 ? consumed_runes : 0;
    }

    void set_consumption_fire_interval_seconds(float fire_interval_seconds) noexcept
    {
        _consumption.consumption_fire_interval_seconds = fire_interval_seconds > 0.0f ? fire_interval_seconds : 0.0f;
    }

    void set_consume_type(WeaponConsumeType consume_type) noexcept
    {
        _consumption.consume_type = consume_type;
    }

protected:
    WandAttributes _wand_attributes;
    Bullet_Attributes _bullet_attributes;
    std::vector<std::function<void(BulletBehaviorSet &)>> _bullet_behavior_appenders;

private:
    int _added_slots = 0;
    WeaponConsumptionData _consumption;
};

class ShotgunRune : public WeaponRune
{
public:
    ShotgunRune() : WeaponRune()
    {
        _wand_attributes.bullet_count = 2;
        _wand_attributes.spread_style = SpreadStyle::Uniform;
        _wand_attributes.spread_degrees = 60;
        _wand_attributes.shot_style = ShotStyle::Simultaneous;
        _wand_attributes.first_shot_delay = 0.0f;
        _wand_attributes.shot_delay_sec = 0.1f;
        _wand_attributes.spawn_distance = 32.0f;
        _wand_attributes.effect_chance = 0.6f;

        _bullet_attributes.bullet_speed = 1500.0f;
        _bullet_attributes.max_age = 0.5f;
        _bullet_attributes.bullet_size = {24.0f, 24.0f};
        _bullet_attributes.damage = 30.0f;
        _bullet_attributes.damage_cooldown_sec = 0.3f;

        set_added_slots(2);
        set_consumed_runes(3);
        set_consumption_fire_interval_seconds(0.1f);
        set_consume_type(WeaponConsumeType::FireNested);
    }
};

class Fireball : public WeaponRune
{
public:
    Fireball() : WeaponRune()
    {
        _wand_attributes.bullet_count = 5;
        _wand_attributes.spread_style = SpreadStyle::Uniform;
        _wand_attributes.spread_degrees = 10;
        _wand_attributes.shot_style = ShotStyle::Simultaneous;
        _wand_attributes.first_shot_delay = 0.2f;
        _wand_attributes.shot_delay_sec = 0.5f;
        _wand_attributes.spawn_distance = 32.0f;
        _wand_attributes.effect_chance = 0.6f;

        _bullet_attributes.bullet_speed = 600.0f;
        _bullet_attributes.max_age = 20.0f;
        _bullet_attributes.bullet_size = {48.0f, 48.0f};
        _bullet_attributes.damage = 100.0f;
        _bullet_attributes.damage_cooldown_sec = 0.5f;

        set_added_slots(7);
        set_consumed_runes(5);
        set_consumption_fire_interval_seconds(0.2f);
        set_consume_type(WeaponConsumeType::FireNested);

        // Sound
        // _bullet_attributes.sound_on_collision = "";
        // _bullet_attributes.sound_on_entity_collision = "";
        _bullet_attributes.sound_on_fire = "fireball_shoot";
        _bullet_attributes.sound_on_death = "fireball_explosion";
        _bullet_attributes.sound_during_flight = "fireball_flight";
    }
};