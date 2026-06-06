#include "../geometry/vector2.h"

class Movable
{
public:
    virtual ~Movable() = default;

    virtual void move_by(const Vector2& offset) noexcept = 0;
};

/*void move_by(const Vector2& offset) noexcept
{
    _world_rect.set_position(_world_rect.position() + offset);
}*/