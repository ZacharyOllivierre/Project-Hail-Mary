#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <functional>

class Button
{
public:
    enum class Status
    {
        Idle = 0,
        Hovered,
        Pushed
    };

public:
    // Create a basic button with a renderer target and button rectangle.
    explicit Button(SDL_Renderer* renderer, SDL_Rect rect_button);

    // Create a button with text texture and optional press/release sound effects.
    // `rect_message` is the destination area for `texture_message`.
    Button(SDL_Renderer* renderer, SDL_Rect rect_button, SDL_Rect rect_message, SDL_Texture* texture_message,
        Mix_Chunk* sound_effect_down, Mix_Chunk* sound_effect_up);

    // Create a color-based button and override idle/hover/pushed/frame colors.
    Button(SDL_Renderer* renderer, SDL_Rect rect_button, SDL_Rect rect_message, SDL_Texture* texture_message,
        Mix_Chunk* sound_effect_down, Mix_Chunk* sound_effect_up,
        SDL_Color color_idle, SDL_Color color_hovered, SDL_Color color_pushed, SDL_Color color_frame);

    // Create a texture-based button with separate textures for each visual state.
    Button(SDL_Renderer* renderer, SDL_Rect rect_button, SDL_Rect rect_message, SDL_Texture* texture_message,
        Mix_Chunk* sound_effect_down, Mix_Chunk* sound_effect_up,
        SDL_Texture* texture_idle, SDL_Texture* texture_hovered, SDL_Texture* texture_pushed);

    // Disable copy to avoid accidental sharing of non-owning raw pointers.
    Button(const Button&) = delete;
    // Disable copy assignment for the same ownership/lifetime reason.
    Button& operator=(const Button&) = delete;
    // Allow move construction to transfer button state.
    Button(Button&&) = default;
    // Allow move assignment to transfer button state.
    Button& operator=(Button&&) = default;
    // Use default destructor; this class does not own external SDL resources.
    ~Button() = default;

    // Draw the button and optional message texture based on current status.
    void render();
    // Process one SDL event; returns true if the button handled/consumed it.
    bool handle_event(const SDL_Event& event);
    // Update the button rectangle used for hit testing and rendering.
    void set_button_rect(SDL_Rect new_rect_button);
    // Update the destination rectangle used when rendering message texture.
    void set_message_rect(SDL_Rect new_rect_message);
    // Set or replace the optional message texture rendered on top of the button.
    void set_message_texture(SDL_Texture* new_texture_message);
    // Enable or disable interaction; disabling resets press state to idle.
    void set_enabled(bool enabled);
    // Return whether the button is currently enabled for input handling.
    [[nodiscard]] bool is_enabled() const;
    // Return how many successful clicks have been registered.
    [[nodiscard]] int click_count() const;
    // Reset the internal click counter to zero.
    void reset_click_count();
    // Set the callback executed when a valid click release happens inside the button.
    void set_on_click(std::function<void()> func);
    // Return the current visual/input status (Idle, Hovered, or Pushed).
    [[nodiscard]] Status get_status()const;

private:

    // Return true when point (`x`, `y`) lies inside the button rectangle.
    bool check_cursor_hit(int x, int y)const;
    // Recalculate hover status from cursor position; returns true if redraw is needed.
    bool update_hover_status(int x, int y);
    // Throw `std::invalid_argument` when `ptr` is null, using `err_msg`.
    void init_assert(const void* ptr, const char* err_msg)const;

private:

    // External resources are non-owning; caller must keep them alive.
    SDL_Renderer* renderer = nullptr;

    SDL_Rect rect_button{};
    SDL_Rect rect_message{};

    Mix_Chunk* sound_effect_down = nullptr;
    Mix_Chunk* sound_effect_up = nullptr;

    SDL_Texture* texture_message = nullptr;
    SDL_Texture* texture_idle = nullptr;
    SDL_Texture* texture_hovered = nullptr;
    SDL_Texture* texture_pushed = nullptr;

    SDL_Color color_frame = { 0, 0, 0, 255 };

    SDL_Color color_idle = { 180, 180, 180, 255 };
    SDL_Color color_hovered = { 200, 200, 200, 255 };
    SDL_Color color_pushed = { 130, 130, 130, 255 };

    Status status = Status::Idle;

    std::function<void()> on_click;

    bool have_texture = false;
    bool enabled = true;
    bool is_pressing = false;

    int time_on_click = 0;

};
