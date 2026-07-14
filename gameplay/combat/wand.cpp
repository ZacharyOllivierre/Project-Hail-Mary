#include "wand.h"

Wand::Wand() : _debug_data(_wand_attributes, _bullet_attributes)
{
}

vector<ShotDescriptor> Wand::attack(const engine::core::Vector2 &direction)
{
    const int count = std::max(1, _wand_attributes.bullet_count);
    std::vector<ShotDescriptor> shots;
    shots.reserve(count);

    for (int i = 0; i < count; i++)
    {
        shots.push_back(make_shot(direction, i));
    }

    return shots;
}

WandDebugData &Wand::debug_data() noexcept
{
    return _debug_data;
}

ShotDescriptor Wand::make_shot(const engine::core::Vector2 &direction, int index)
{
    engine::core::Vector2 aim = direction.normalized();

    // Get angle for current
    const float angle = calculate_bullet_angle(index);
    const engine::core::Vector2 shot_direction = aim.rotated(angle);

    // Update bullet velocity via shot direction
    _bullet_attributes.bullet_velocity = shot_direction * _bullet_attributes.bullet_speed;

    return ShotDescriptor({_bullet_attributes,
                           shot_direction * _wand_attributes.spawn_distance,
                           get_shot_delay(index)});
}

float Wand::calculate_bullet_angle(int index)
{
    float angle = 0.0f;

    switch (_wand_attributes.spread_style)
    {
    case SpreadStyle::Uniform:
        angle = calc_uniform_spread_angle(index);
        break;

    case SpreadStyle::Circular:
        angle = calc_circular_spread_angle(index);
        break;

    case SpreadStyle::Random:
        angle = calc_random_spread_angle();
        break;
    default:
    }

    return angle;
}

float Wand::calc_uniform_spread_angle(int num)
{
    if (_wand_attributes.bullet_count == 1)
        return 0.0f;

    float total_spread = _wand_attributes.spread_degrees;

    return -total_spread * 0.5f +
           num * (total_spread / (_wand_attributes.bullet_count - 1));
}

float Wand::calc_circular_spread_angle(int num)
{
    return num * (360.0 / static_cast<float>(_wand_attributes.bullet_count));
}

float Wand::calc_random_spread_angle()
{
    float r = static_cast<float>(std::rand()) / RAND_MAX;

    float &sd = _wand_attributes.spread_degrees;
    return -sd * 0.5f + r * sd;
}

float Wand::get_shot_delay(int index)
{
    float delay;
    switch (_wand_attributes.shot_style)
    {
    case ShotStyle::Simultaneous:
        delay = 0.0f;
        break;

    case ShotStyle::Sequential:
        delay = index * _wand_attributes.shot_delay_sec;
        break;

    case ShotStyle::ReverseSequential:
        delay = (_wand_attributes.bullet_count - 1 - index) * _wand_attributes.shot_delay_sec;
        break;
    }

    return _wand_attributes.first_shot_delay + delay;
}