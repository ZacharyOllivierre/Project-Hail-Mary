#pragma once

#include <string>

namespace engine::input
{

enum class InputAction
{
    Left,
    Right,
    Up,
    Down,

    Confirm,
    Cancel,
    Pause,

    Attack,
    Special,
    Backspace,
    DeleteKey,
    Home,
    End,
    Tab,

    DebugCollision,

    Exit,

    Count
};

enum class InputContext
{
    Gameplay,
    UI,
    Dialogue,
    Debug
};

enum class InputDevice
{
    Keyboard,
    Mouse,
    Gamepad,
    Unknown
};

enum class InputEventType
{
    Pressed,
    Released,
    MouseWheel,
    TextInput,
    TextEditing
};

struct InputEvent
{
    InputAction action = InputAction::Count;
    InputEventType type = InputEventType::Released;
    InputDevice device = InputDevice::Unknown;
    int wheel_x = 0;
    int wheel_y = 0;
    std::string text;
};
}
