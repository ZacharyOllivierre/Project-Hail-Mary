#pragma once

#include "status_effect.h"
#include "attack_info.h"

#include <vector>

// Todo add effect interactions on entity
class StatusEffectManager
{
public:
    void add_effect(std::shared_ptr<StatusEffect> effect, CombatReceiver &target)
    {
        if (!effect)
            return;

        std::shared_ptr<StatusEffect> instance = effect->make_new_instance();
        if (!instance)
            return;

        instance->on_apply(target);
        _effects.push_back(std::move(instance));
    }

    void update(CombatReceiver &target, double delta)
    {
        const std::vector<std::shared_ptr<StatusEffect>> active_effects = _effects;

        for (const auto &effect : active_effects)
        {
            if (effect)
                effect->update(target, delta);
        }

        erase_finished(target);
    }

    void clear()
    {
        _effects.clear();
    }

private:
    void erase_finished(CombatReceiver &target)
    {
        for (auto it = _effects.begin(); it != _effects.end();)
        {
            auto &effect = *it;

            if (effect && !effect->finished())
            {
                it++;
                continue;
            }

            if (effect)
                effect->on_remove(target);

            it = _effects.erase(it);
        }
    }

private:
    std::vector<std::shared_ptr<StatusEffect>> _effects;
};