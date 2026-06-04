#include "effect.h"
#include "animation_manager.h"

Effect::Effect(std::string effect_key, std::string animation_key, std::unique_ptr<Animation> animation) 
    :GameObject(DepthLayer::EffectFront),_effect_key(effect_key), _animation_key(animation_key), _animation(std::move(animation))
{}

void Effect::on_render(SDL_Renderer* renderer)
{
	_animation->render(renderer,GameObject::rect(),_angle_degrees);
}

void Effect::on_update(double delta)
{
	_animation->update(delta);
	if (_animation->is_finished())
		GameObject::destroy();
}

std::unique_ptr<Effect> Effect::clone() const
{
    auto animation = AnimationManager::instance()->create_animation(_animation_key);

    auto effect = std::make_unique<Effect>(
        _effect_key,
        _animation_key,
        std::move(animation)
    );

    effect->set_world_position(position());
    effect->set_size(size());
    effect->_angle_degrees = _angle_degrees;

    return effect;
}
