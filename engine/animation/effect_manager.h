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

enum class EffectAnchor
{
	TopLeft,
	TopCenter,
	TopRight,
	CenterLeft,
	Center,
	CenterRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

struct EffectDefinition
{
	std::string effect_key;
	std::string animation_key;
	double angle_degrees = 0.0;
	Vector2 default_size;
};

struct EffectSpawnRequest
{
	std::string effect_key;
	// World-space position of the selected playback anchor.
	Vector2 position;
	EffectAnchor anchor = EffectAnchor::TopLeft;
	std::optional<Vector2> size;
	std::optional<double> angle_degrees;
	std::optional<SpriteFlip> flip;
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
