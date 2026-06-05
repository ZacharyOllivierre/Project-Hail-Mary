#pragma once

class Updatable
{
public:
    virtual ~Updatable() = default;
    virtual void update(double delta) = 0;
};