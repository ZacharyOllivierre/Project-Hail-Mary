#include "test_object.h"

#include "../../engine/input/input_state.h"

#include <utility>

TestObject::TestObject(std::string character_id)
	: Character(std::move(character_id), Vector2(200.0f, 200.0f), Vector2(100.0f, 100.0f))
{
}

void TestObject::on_input_snapshot(const InputSnapshot& input)
{
	if (input.state.is_just_released(InputAction::DeleteKey))
	{
		die();
		return;
	}

	Character::on_input_snapshot(input);
}
