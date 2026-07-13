#include "wand.h"

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
    float spread_degrees = _wand_attributes.spread_degrees /
                           static_cast<float>(_wand_attributes.bullet_count);
    float angle;

    if (_wand_attributes.bullet_count == 1)
    {
        angle = 0.0;
    }
    else
    {
        angle = -spread_degrees * 0.5 + num * (spread_degrees / (_wand_attributes.bullet_count - 1));
    }
    return angle;
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
    switch (_wand_attributes.shot_style)
    {
    case ShotStyle::Instant:
        return 0.0f;
    case ShotStyle::Sequential:
        return _wand_attributes.first_shot_delay + index * _wand_attributes.shot_delay_sec;
    default:
    }
    return 0.0f;
}