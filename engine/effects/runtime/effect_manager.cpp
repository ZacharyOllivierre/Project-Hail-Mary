#include "effect_manager.h"

#include "../../animation/animation_manager.h"
#include "../../scene/scene.h"

#include <SDL.h>

#include <utility>

namespace engine::effects
{

bool EffectManager::register_animation_effect(
	const std::vector<engine::resources::AnimationEffectBuildRequest>& requests)
{
	for (const engine::resources::AnimationEffectBuildRequest& request : requests)
	{
		if (!register_animation_effect(request))
			return false;
	}

	return true;
}

bool EffectManager::register_animation_effect(
	const engine::resources::AnimationEffectBuildRequest& request)
{
	if (request.effect_key.empty())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Register animation effect failed: effect key is empty.");
		return false;
	}

	if (request.animation_key.empty())
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Register animation effect failed: animation key is empty.");
		return false;
	}

	if (request.default_size.x < 0.0f || request.default_size.y < 0.0f
		|| ((request.default_size.x == 0.0f) != (request.default_size.y == 0.0f)))
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Register animation effect failed: default size must provide positive width and height.");
		return false;
	}

	if (!engine::animation::AnimationManager::instance()->find_definition(
		request.animation_key))
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Register animation effect failed: animation definition does not exist: %s",
			request.animation_key.c_str());
		return false;
	}

	AnimationEffectDefinition definition;
	definition.effect_key = request.effect_key;
	definition.animation_key = request.animation_key;
	definition.default_size = request.default_size;
	definition.angle_degrees = request.default_angle_degrees;
	_animation_effect_definitions[request.effect_key] = std::move(definition);
	return true;
}

const AnimationEffectDefinition* EffectManager::find_animation_effect_definition(
	std::string_view key) const
{
	const auto iterator = _animation_effect_definitions.find(std::string(key));
	if (iterator == _animation_effect_definitions.end())
		return nullptr;

	return &iterator->second;
}

void EffectManager::clear_content() noexcept
{
	_animation_effect_definitions.clear();
}

bool EffectManager::dispatch(const AnimationEffectSpawnRequest& request)
{
	if (!_active_scene)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Spawn animation effect failed: there is no active scene.");
		return false;
	}

	const AnimationEffectDefinition* definition =
		find_animation_effect_definition(request.effect_key);
	if (!definition)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
			"Spawn animation effect failed: definition does not exist: %s",
			request.effect_key.c_str());
		return false;
	}

	std::unique_ptr<AnimationEffect> effect =
		_animation_effect_factory.create(request, *definition);
	if (!effect)
		return false;

	if (_active_scene->add_object(std::move(effect)))
		return true;

	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
		"Spawn animation effect failed: active scene rejected the effect.");
	return false;
}

void EffectManager::bind_active_scene(engine::scene::Scene& scene) noexcept
{
	_active_scene = &scene;
}

void EffectManager::unbind_active_scene(const engine::scene::Scene& scene) noexcept
{
	if (_active_scene == &scene)
		_active_scene = nullptr;
}

}
