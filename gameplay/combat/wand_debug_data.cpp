#include "wand_debug_data.h"

#include "../../thirdparty/imgui/imgui.h"

WandDebugData::WandDebugData(WandAttributes &wand_attributes, Bullet_Attributes &bullet_attributes) noexcept
{
    _wand_attributes = &wand_attributes;
    _bullet_attributes = &bullet_attributes;
}

// AI wrote ImGui UI
void WandDebugData::render_debugger()
{
    if (!_wand_attributes || !_bullet_attributes)
        return;

    ImGui::Text("Wand Debug");

    ImGui::DragFloat("Cooldown", &_wand_attributes->cooldown_seconds, 0.01f, 0.0f, 30.0f);
    ImGui::DragFloat("Mana Cost", &_wand_attributes->mana_cost, 0.1f, 0.0f, 1000.0f);
    ImGui::DragInt("Bullet Count", &_wand_attributes->bullet_count, 1.0f, 1, 1000);

    int spread_style = static_cast<int>(_wand_attributes->spread_style);
    if (ImGui::Combo("Spread Style", &spread_style, "Uniform\0Circular\0Random\0"))
    {
        _wand_attributes->spread_style = static_cast<SpreadStyle>(spread_style);
    }

    ImGui::DragFloat("Spread Degrees", &_wand_attributes->spread_degrees, 0.1f, 0.0f, 360.0f);

    int shot_style = static_cast<int>(_wand_attributes->shot_style);
    if (ImGui::Combo("Shot Style", &shot_style, "Simultaneous\0Sequential\0ReverseSequential\0"))
    {
        _wand_attributes->shot_style = static_cast<ShotStyle>(shot_style);
    }

    ImGui::DragFloat("First Shot Delay", &_wand_attributes->first_shot_delay, 0.01f, 0.0f, 20.0f);
    ImGui::DragFloat("Shot Delay", &_wand_attributes->shot_delay_sec, 0.01f, 0.0f, 20.0f);
    ImGui::DragFloat("Spawn Distance", &_wand_attributes->spawn_distance, 1.0f, 0.0f, 1000.0f);

    ImGui::Separator();
    ImGui::Text("Bullet Debug");
    ImGui::DragFloat("Speed", &_bullet_attributes->bullet_speed, 10.0f, 0.0f, 5000.0f);
    ImGui::DragFloat("Max Age", &_bullet_attributes->max_age, 0.1f, 0.0f, 120.0f);
    ImGui::DragFloat2("Bullet Size", &_bullet_attributes->bullet_size.x, 1.0f, 1.0f, 512.0f);
    ImGui::DragFloat("Damage", &_bullet_attributes->damage, 1.0f, 0.0f, 10000.0f);
    ImGui::DragFloat("Damage Cooldown", &_bullet_attributes->damage_cooldown_sec, 0.1f, 0.0f, 10.0f);
}