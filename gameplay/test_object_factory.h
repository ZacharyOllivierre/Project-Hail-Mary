#pragma once

#include "test_object.h"

#include <memory>

class TestObjectFactory
{
public:
	std::unique_ptr<TestObject> create() const
	{
		return std::make_unique<TestObject>();
	}
};
