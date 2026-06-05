#pragma once

#include "animation.h"
#include "../core/game_object.h"
#include "../core/interface/updatable.h"

#include <memory>

class Effect : public GameObject, public Updatable
{
public:
	Effect(std::string effect_key, std::string animation_key, std::unique_ptr<Animation> animation);
	~Effect() override = default;

	void submit_render_commands(std::vector<RenderCommand>& out_commands) const override;
	void update(double delta) override;

	std::unique_ptr<Effect> clone() const;

	void set_angle(double angle_degrees);

private:
	std::string _effect_key;
	std::string _animation_key;
	double _angle_degrees = 0.0;
	std::unique_ptr<Animation> _animation;
};
