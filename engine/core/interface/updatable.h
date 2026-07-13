#pragma once

namespace engine::core
{

class Updatable
{
public:
    virtual ~Updatable() = default;
    virtual void update(double delta) = 0;
};
}
