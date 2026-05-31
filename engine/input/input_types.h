#pragma once

enum class InputAction
{
    Left,
    Right,
    Up,
    Down,

    Confirm,
    Cancel,
    Pause,

    Jump,
    Attack,
    Special,
    Guard,
    Dash,

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
    Released
};

struct InputEvent
{
    InputAction action = InputAction::Count;
    InputEventType type = InputEventType::Released;
    InputDevice device = InputDevice::Unknown;
};
