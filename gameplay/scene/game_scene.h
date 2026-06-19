#pragma once

#include "../../engine/scene/scene.h"
#include "../character.h"

class GameScene final : public Scene
{
public:
	GameScene() = default;
	~GameScene() override = default;

	void on_update(double delta) override;
	void on_render(SDL_Renderer* renderer) override;
	void on_input(const InputSnapshot& input, const std::vector<InputEvent>& events) override;

	void on_enter() override;
	void on_exit() override;
	void reset() override;

private:
	void spawn_player();
	void spawn_map();
	void destroy_tracked_objects();
	void spawn_effect_for_character(const std::string& effect_id, const Vector2& position);

	Character* _player = nullptr;

	GameObject* _map = nullptr;

	Rect _rect{ 460, 180, 360, 360 };
	bool _contain = false;
};
