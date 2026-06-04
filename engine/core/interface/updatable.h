#pragma once

class Updatable
{
public:
    virtual ~Updatable() = default;
    virtual void on_update(double delta) = 0;
}