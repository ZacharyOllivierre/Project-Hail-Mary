#pragma once

#include "../animation/animation_effect_factory.h"
#include "../effect_types.h"
#include "../../resources/resource_types.h"
#include "../../tools/singleton.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace engine::scene
{
class Scene;
class SceneManager;
}

namespace engine::effects
{

class EffectService;

class EffectManager : public engine::tools::Singleton<EffectManager>
{
	friend engine::tools::Singleton<EffectManager>;
	friend class EffectService;
	friend class engine::scene::SceneManager;

public:
	bool register_animation_effect(
		const engine::resources::AnimationEffectBuildRequest& request);
	bool register_animation_effect(
		const std::vector<engine::resources::AnimationEffectBuildRequest>& requests);

	[[nodiscard]] const AnimationEffectDefinition* find_animation_effect_definition(
		std::string_view key) const;
	void clear_content() noexcept;

private:
	[[nodiscard]] bool dispatch(const AnimationEffectSpawnRequest& request);
	void bind_active_scene(engine::scene::Scene& scene) noexcept;
	void unbind_active_scene(const engine::scene::Scene& scene) noexcept;

	std::unordered_map<std::string, AnimationEffectDefinition> _animation_effect_definitions;
	AnimationEffectFactory _animation_effect_factory;
	engine::scene::Scene* _active_scene = nullptr;
};

}
