#pragma once

// Template area for brainstorming wand components

// Base modifier - modifies base value (bullet, wand, character structs)
// On entity collision modifier - modifier that effects interaction on entity collision
// On fire modifier
// Flight time modifier - modifies bullet during flight time

enum class ModifierOperation
{
    Add,
    Subtract,
    Multiply,
    Divide,
    Set,
};

template <typename T, typename V>
struct Modifier
{
    V T::*member;
    ModifierOperation operation;
    V value;

    void apply(T &object)
    {
        V &stat = object.*member;

        switch (operation)
        {
        case ModifierOperation::Add:
            stat += value;
            break;
        case ModifierOperation::Subtract:
            stat -= value;
            break;
        case ModifierOperation::Multiply:
            stat *= value;
            break;
        case ModifierOperation::Divide:
            stat /= value;
            break;
        case ModifierOperation::Set:
            stat = value;
            break;
        }
    }
};