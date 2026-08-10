#pragma once

#include "../../engine/scene/scene.h"
#include "../characters/player_character.h"

class GameScene final : public engine::scene::Scene
{
public:
	GameScene() = default;
	~GameScene() override = default;

	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const engine::input::InputSnapshot& input, const std::vector<engine::input::InputEvent>& events) override;

	void on_enter() override;
	void on_exit() override;
	void reset() override;

private:
	void spawn_player();
	void spawn_map();
	void destroy_tracked_objects();
	PlayerCharacter* _player = nullptr;

	engine::core::GameObject* _map = nullptr;

	engine::core::Rect _rect{ 460, 180, 360, 360 };
	bool _contain = false;
};
