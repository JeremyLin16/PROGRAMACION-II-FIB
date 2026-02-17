#include "powerup.hh"
#include "utils.hh"
#include <cmath>

using namespace pro2;

const int _ = -1;

// Sprite genérico de power-up (se colorea según el tipo)
const std::vector<std::vector<int>> PowerUp::powerup_sprite_ = {
    {_, _, _, 1, 1, 1, 1, _, _, _},
    {_, _, 1, 1, 1, 1, 1, 1, _, _},
    {_, 1, 1, 1, 1, 1, 1, 1, 1, _},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {_, 1, 1, 1, 1, 1, 1, 1, 1, _},
    {_, _, 1, 1, 1, 1, 1, 1, _, _},
    {_, _, _, 1, 1, 1, 1, _, _, _},
};

PowerUp::Config PowerUp::get_config(Type type) {
    switch (type) {
        case STAR:
            return {300, 0xFFD700, "Star (Invincible)"};  // Dorado, 5 segundos
        case MUSHROOM:
            return {240, 0xFF0000, "Mushroom (Speed)"};   // Rojo, 4 segundos
        case FEATHER:
            return {360, 0x00FF00, "Feather (Jump)"};     // Verde, 6 segundos
        default:
            return {180, 0xFFFFFF, "Unknown"};
    }
}

PowerUp::PowerUp(Pt pos, Type type)
    : pos_(pos), type_(type), collected_(false), animation_frame_(0) {}

void PowerUp::paint(pro2::Window& window) const {
    if (collected_) return;
    
    Config cfg = get_config(type_);
    const Pt top_left = {pos_.x - sprite_size/2, pos_.y - sprite_size/2};
    
    // Crear sprite con el color apropiado
    std::vector<std::vector<int>> colored_sprite = powerup_sprite_;
    for (auto& row : colored_sprite) {
        for (int& pixel : row) {
            if (pixel == 1) {
                pixel = cfg.color;
            }
        }
    }
    
    paint_sprite(window, top_left, colored_sprite, false);
}

void PowerUp::update() {
    if (collected_) return;
    animation_frame_++;
    
    // Rotación lenta (opcional, puede simplificarse)
}

bool PowerUp::check_collision(Pt mario_pos) {
    if (collected_) return false;
    
    const int collision_radius = 15;
    int dx = std::abs(mario_pos.x - pos_.x);
    int dy = std::abs(mario_pos.y - pos_.y);
    
    return (dx < collision_radius && dy < collision_radius);
}
