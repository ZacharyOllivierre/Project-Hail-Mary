#include "effect.h"
#include "animation_manager.h"

Effect::Effect(std::string effect_key, std::string animation_key, std::unique_ptr<Animation> animation) 
    : GameObject(DepthLayer::EffectFront), _effect_key(effect_key), _animation_key(animation_key), _animation(std::move(animation))
{}

void Effect::submit_render_commands(std::vector<RenderCommand>& out_commands) const
{
	if (!_animation)
		return;

	RenderCommand render_command = make_render_command();
	if (_animation->build_render_command(world_rect(), _angle_degrees, render_command))
		out_commands.push_back(render_command);
}

void Effect::update(double delta)
{
	_animation->update(delta);
	if (_animation->is_finished())
		GameObject::destroy();
}

std::unique_ptr<Effect> Effect::clone() const
{
    std::unique_ptr<Animation> animation = AnimationManager::instance()->create_animation(_animation_key);

    std::unique_ptr<Effect> effect = std::make_unique<Effect>(
        _effect_key,
        _animation_key,
        std::move(animation)
    );

    effect->set_position(position());
    effect->set_size(size());
    effect->_angle_degrees = _angle_degrees;

    return effect;
}

void Effect::set_angle(double angle_degrees)
{
	_angle_degrees = angle_degrees;
}
