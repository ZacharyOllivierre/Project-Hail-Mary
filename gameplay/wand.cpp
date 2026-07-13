#include "wand.h"

vector<unique_ptr<Projectile>> Wand::attack(const ::engine::core::Vector2 &origin, const ::engine::core::Vector2 &direction)
{
    _origin = origin;
    vector<unique_ptr<Projectile>> projectiles;

    make_bullets(projectiles, direction);

    return projectiles;
}

void Wand::make_bullets(vector<unique_ptr<Projectile>> &projectiles, const ::engine::core::Vector2 &direction)
{
    const int count = std::max(1, _wand_attributes.bullet_count);
    ::engine::core::Vector2 aim = direction.normalized();

    for (int i = 0; i < count; i++)
    {
        // Get angle for current
        float angle = calculate_bullet_angle(i);

        // Calculate shot direction
        ::engine::core::Vector2 shot_direction = aim.rotated(angle);

        // Update bullet velocity via shot direction
        _bullet_attributes.bullet_velocity = shot_direction * _bullet_attributes.bullet_speed;

        // Todo update start position (origin) for each spread type and count
        // For now all starting from given origin
        _bullet_attributes.start_position = _origin;

        projectiles.push_back(std::make_unique<Bullet>(_bullet_attributes));
    }
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
        angle = calc_random_spread_angle(index);
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

float Wand::calc_random_spread_angle(int num)
{
    float r = static_cast<float>(std::rand()) / RAND_MAX;

    float &sd = _wand_attributes.spread_degrees;
    return -sd * 0.5f + r * sd;
}
