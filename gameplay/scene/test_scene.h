#pragma once

#include "../../engine/scene/scene.h"

#include "../test/test_map.h"
#include "../test/test_object.h"
#include "../test/test_object_factory.h"
#include "../test/test_ui.h"

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
	void spawn_test_object();
	void spawn_test_ui();
	void destroy_tracked_objects();

	TestObjectFactory _test_object_factory;
	TestObject* _test_object = nullptr;
	TestMap* _test_map = nullptr;
	TestUi* _test_ui = nullptr;
	Rect _rect{460, 180, 360, 360};
	bool _contain = false;
};
