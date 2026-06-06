#include "../geometry/rect.h"

class Collidable
{
public:
    virtual ~Collidable() = default;

    [[nodiscard]] virtual Rect collision_rect() const noexcept = 0;

    [[nodiscard]] virtual bool is_trigger() const noexcept
    {
        return false;
    }
};