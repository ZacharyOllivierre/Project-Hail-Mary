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

        _bullet_attributes.bullet_speed = 800.0f;
        _bullet_attributes.max_age = 0.3f;
        _bullet_attributes.bullet_size = {24.0f, 24.0f};
        _bullet_attributes.damage = 30.0f;
        _bullet_attributes.damage_cooldown_sec = 0.3f;

        set_added_slots(5);
        set_consumed_runes(1);
        set_consumption_fire_interval_seconds(1.0f);
        set_consume_type(WeaponConsumeType::FireNested);
    }
};

class TestWeaponRune : public WeaponRune
{
public:
    TestWeaponRune() : WeaponRune()
    {
        _wand_attributes.bullet_count = 2;
        _wand_attributes.spread_style = SpreadStyle::Uniform;
        _wand_attributes.spread_degrees = 45;
        _wand_attributes.shot_style = ShotStyle::Simultaneous;
        _wand_attributes.shot_delay_sec = 0.12f;
        _wand_attributes.spawn_distance = 32.0f;

        _bullet_attributes.bullet_speed = 400.0f;
        _bullet_attributes.max_age = 10.0f;
        _bullet_attributes.damage = 1.0f;

        set_added_slots(1);
        set_consumed_runes(1);
        set_consumption_fire_interval_seconds(1.0f);
        set_consume_type(WeaponConsumeType::FireNested);
    }
};

class NestedTestWeaponRune : public WeaponRune
{
public:
    NestedTestWeaponRune() : WeaponRune()
    {
        _wand_attributes.bullet_count = 2;
        _wand_attributes.spread_style = SpreadStyle::Circular;
        _wand_attributes.spread_degrees = 20.0f;
        _wand_attributes.shot_style = ShotStyle::Simultaneous;
        _wand_attributes.shot_delay_sec = 0.08f;
        _wand_attributes.spawn_distance = 40.0f;

        _bullet_attributes.bullet_speed = 200.0f;
        _bullet_attributes.max_age = 10.0f;
        _bullet_attributes.damage = 7.0f;

        set_added_slots(1);
        set_consumed_runes(1);
        set_consumption_fire_interval_seconds(1.0f);
        set_consume_type(WeaponConsumeType::FireNested);
    }
};