#pragma once

#include "../../engine/scene/scene.h"

#include "../test_obj.h"
#include "../test2_obj.h"
#include <memory>

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
	std::shared_ptr<TestObj> _t_obj;
	std::shared_ptr<Test2Obj> _t2_obj;
	Rect _rect{460, 180, 360, 360};
	bool _contain = false;
};
