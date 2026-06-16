#pragma once

#include <string>

#include "../character.h"

class TestObject : public Character
{
public:
	explicit TestObject(std::string character_id = "elves");
	~TestObject() override = default;
	 
	void on_input_snapshot(const InputSnapshot& input) override;
};

