#pragma once

enum class ModifierOperation
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Set,
    Min,
    Max
};

template <typename T, typename V>
struct Modifier
{
    V T::*member;
    ModifierOperation operation;
    V value;
};