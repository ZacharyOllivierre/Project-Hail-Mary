#pragma once

#include "../../engine/scene/scene.h"

#include "../test_map.h"
#include "../test_object.h"

class TestScene final : public Scene
{
public:
	TestScene() = default;
	~TestScene() override = default;

	void on_update(double delta) override;
	void on_render(SDL_Renderer *renderer) override;
	void on_input(const InputSnapshot &input, const std::vector<InputEvent> &events) override;

	void on_enter() override;
	void on_exit() override;
	void reset() override;

private:
	void spawn_test_objects();
	void destroy_tracked_objects();

	TestObject* _test_object = nullptr;
	TestMap* _test_map = nullptr;
	Rect _rect{460, 180, 360, 360};
	bool _contain = false;
};
