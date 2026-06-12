#pragma once

#include "../../engine/scene/scene.h"

#include "../test_map.h"
#include "../test_object.h"
#include "../test_object_factory.h"
#include "../test_ui.h"
#include "../../engine/ui/button.h"
#include <memory>

// Added for button
#include <iostream>

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

	// Added for button
	// Lets application send raw SDL events to this scene, like mouse movement
	void handle_sdl_event(const SDL_Event& event);

private:
	void spawn_test_object();
	void spawn_test_ui();
	void destroy_tracked_objects();
	// Added for button
	// Owns test button for this scene, starts as nullptr when created, used when button resets
	std::unique_ptr<Button> _test_button;

	TestObjectFactory _test_object_factory;
	TestObject* _test_object = nullptr;
	TestMap* _test_map = nullptr;
	TestUi* _test_ui = nullptr;
	Rect _rect{460, 180, 360, 360};
	bool _contain = false;
};
