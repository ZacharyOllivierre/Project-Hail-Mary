#pragma once

#include "effect.h"
#include "../tools/singleton.h"
#include "../resources/resource_types.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <optional>

class AnimationManager;

struct EffectDefinition
{
	std::string _effect_key;
	std::string _animation_key;
	double _angle_degrees = 0.0;
	Vector2 _default_size;
};

struct EffectSpawnRequest
{
	std::string _effect_key;
	Vector2 _position;
	std::optional<Vector2> _size;
	std::optional<double> _angle_degrees;
};

class EffectManager : public Singleton<EffectManager>
{
	friend Singleton<EffectManager>;

public:
	bool register_effect(const EffectBuildRequest& request);
	bool register_effect(const std::vector<EffectBuildRequest>& requests);

	const EffectDefinition* find_definition(const std::string_view& key) const;

	std::unique_ptr<Effect> create_effect(const EffectSpawnRequest& request) const;


private:
	std::unordered_map<std::string, EffectDefinition> _definitions;
};

