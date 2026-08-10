#include "wand.h"

#include "grid/behavior_rune.h"
#include "grid/stat_rune.h"
#include "grid/weapon_rune.h"

#include <algorithm>

/*TODO Move effect into weapon runes*/

Wand::Wand()
    : _rune_line(1), _debug_data(_wand_attributes, _bullet_attributes)
{
    seed_test_runes();
}

vector<ShotDescriptor> Wand::attack(const engine::core::Vector2 &direction)
{
    std::vector<ShotDescriptor> shots;

    std::vector<RuneWeaponNode> weapon_nodes = _rune_line.evaluate_weapons();
    for (const RuneWeaponNode &weapon_node : weapon_nodes)
    {
        append_weapon_shots(weapon_node, direction, 0.0f, shots);
    }

    if (!weapon_nodes.empty())
    {
        _wand_attributes = weapon_nodes.front().loadout.wand_attributes;
        _bullet_attributes = weapon_nodes.front().loadout.bullet_attributes;
    }
    return shots;
}

WandDebugData &Wand::debug_data() noexcept
{
    return _debug_data;
}

ShotDescriptor Wand::make_shot(
    const RuneLoadout &loadout,
    const engine::core::Vector2 &direction,
    int index,
    float delay_offset)
{
    WandAttributes wand_attributes = loadout.wand_attributes;
    Bullet_Attributes bullet_attributes = loadout.bullet_attributes;

    engine::core::Vector2 aim = direction.normalized();
    const float angle = calculate_bullet_angle(wand_attributes, index);
    const engine::core::Vector2 shot_direction = aim.rotated(angle);

    bullet_attributes.bullet_velocity = shot_direction * bullet_attributes.bullet_speed;
    bullet_attributes.status_effect = roll_for_effect(wand_attributes);

    bullet_attributes.bullet_behavior_appenders = loadout.bullet_behavior_appenders;

    return ShotDescriptor{
        .bullet_attributes = bullet_attributes,
        .spawn_offset = shot_direction * wand_attributes.spawn_distance,
        .shot_direction = shot_direction,
        .spawn_delay_sec = delay_offset + get_shot_delay(wand_attributes, index)};
}

int Wand::append_weapon_shots(
    const RuneWeaponNode &weapon_node,
    const engine::core::Vector2 &direction,
    float delay_offset,
    std::vector<ShotDescriptor> &out_shots)
{
    const RuneLoadout &loadout = weapon_node.loadout;
    const WandAttributes &wand_attributes = loadout.wand_attributes;
    const int bullet_count = std::max(1, wand_attributes.bullet_count);
    const float child_fire_delay = weapon_node.consumption.consumption_fire_interval_seconds;

    int emitted_count = 0;

    for (int shot_index = 0; shot_index < bullet_count; ++shot_index)
    {
        ShotDescriptor parent_shot = make_shot(loadout, direction, shot_index, delay_offset);
        out_shots.push_back(parent_shot);
        ++emitted_count;

        for (const RuneWeaponNode &child_weapon : weapon_node.children)
        {
            emitted_count += append_weapon_shots(
                child_weapon,
                parent_shot.shot_direction,
                parent_shot.spawn_delay_sec + child_fire_delay,
                out_shots);
        }
    }

    return emitted_count;
}

std::shared_ptr<StatusEffect> Wand::roll_for_effect(const WandAttributes &wand_attributes)
{
    if (wand_attributes.status_effect.empty())
    {
        return nullptr;
    }

    if ((rand() % 100) / 100.0 > wand_attributes.effect_chance)
    {
        return nullptr;
    }

    int effectIndex = rand() % wand_attributes.status_effect.size();
    return wand_attributes.status_effect[effectIndex]->make_new_instance();
}

float Wand::calculate_bullet_angle(const WandAttributes &wand_attributes, int index)
{
    float angle = 0.0f;

    switch (wand_attributes.spread_style)
    {
    case SpreadStyle::Uniform:
        angle = calc_uniform_spread_angle(wand_attributes, index);
        break;

    case SpreadStyle::Circular:
        angle = calc_circular_spread_angle(wand_attributes, index);
        break;

    case SpreadStyle::Random:
        angle = calc_random_spread_angle(wand_attributes);
        break;
    default:
        break;
    }

    return angle;
}

float Wand::calc_uniform_spread_angle(const WandAttributes &wand_attributes, int num)
{
    if (wand_attributes.bullet_count == 1)
        return 0.0f;

    float total_spread = wand_attributes.spread_degrees;

    return -total_spread * 0.5f +
           num * (total_spread / (wand_attributes.bullet_count - 1));
}

float Wand::calc_circular_spread_angle(const WandAttributes &wand_attributes, int num)
{
    return num * (360.0 / static_cast<float>(wand_attributes.bullet_count));
}

float Wand::calc_random_spread_angle(const WandAttributes &wand_attributes)
{
    float r = static_cast<float>(std::rand()) / RAND_MAX;

    float spread_degrees = wand_attributes.spread_degrees;
    return -spread_degrees * 0.5f + r * spread_degrees;
}

float Wand::get_shot_delay(const WandAttributes &wand_attributes, int index)
{
    float delay = 0.0f;
    switch (wand_attributes.shot_style)
    {
    case ShotStyle::Simultaneous:
        delay = 0.0f;
        break;

    case ShotStyle::Sequential:
        delay = index * wand_attributes.shot_delay_sec;
        break;

    case ShotStyle::ReverseSequential:
        delay = (wand_attributes.bullet_count - 1 - index) * wand_attributes.shot_delay_sec;
        break;
    }

    return wand_attributes.first_shot_delay + delay;
}

void Wand::seed_test_runes()
{
    // Weapon consumption example
    // (void)_rune_line.set_rune(0, std::make_shared<Fireball>());
    // (void)_rune_line.set_rune(1, std::make_shared<BulletCountUpRune>(1));
    // (void)_rune_line.set_rune(2, std::make_shared<WallStickRune>(5, 1));
    // (void)_rune_line.set_rune(3, std::make_shared<HomingRune>(300));
    // (void)_rune_line.set_rune(4, std::make_shared<ShotgunRune>());
    // (void)_rune_line.set_rune(5, std::make_shared<BounceRune>(10));
    // (void)_rune_line.set_rune(6, std::make_shared<SpreadStyleChangeRune>(SpreadStyle::Circular));
    // (void)_rune_line.set_rune(7, std::make_shared<ShotgunRune>());

    // Behavior before weapon test
    // (void)_rune_line.set_rune(0, std::make_shared<BounceRune>(2));
    // (void)_rune_line.set_rune(1, std::make_shared<Fireball>());

    // Stat before weapon test
    // (void)_rune_line.set_rune(0, std::make_shared<BulletCountUpRune>(2));
    // (void)_rune_line.set_rune(1, std::make_shared<Fireball>());

    // Wall stick test
    // (void)_rune_line.set_rune(1, std::make_shared<Fireball>());
    // (void)_rune_line.set_rune(2, std::make_shared<WallStickRune>(3, 1));
    // (void)_rune_line.set_rune(3, std::make_shared<BounceRune>(5));
    // // (void)_rune_line.set_rune(4, std::make_shared<CurveRune>(600));
    // (void)_rune_line.set_rune(4, std::make_shared<HomingRune>(600));
    // (void)_rune_line.set_rune(5, std::make_shared<PierceRune>(7));

    // Deceleration Test
    // (void)_rune_line.set_rune(1, std::make_shared<Fireball>());
    // (void)_rune_line.set_rune(2, std::make_shared<DecelerationRune>(500, 100));
    // (void)_rune_line.set_rune(3, std::make_shared<HomingRune>(600));

    // Bullet Audio Test
    (void)_rune_line.set_rune(1, std::make_shared<Fireball>());
    (void)_rune_line.set_rune(2, std::make_shared<WallStickRune>(3, 1));
    (void)_rune_line.set_rune(3, std::make_shared<BounceRune>(5));
}
