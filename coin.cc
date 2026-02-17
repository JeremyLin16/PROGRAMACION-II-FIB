#include "coin.hh"
#include "utils.hh"  // ← IMPORTANTE: Para paint_sprite
#include <cmath>

using namespace pro2;

// Definir constantes de color
const int _ = -1;
const int h = 0x000000;  // Negro
const int y = 0xFFD700;  // Dorado

// Sprite de la moneda
const std::vector<std::vector<int>> Coin::sprite_ = {
    { -1, -1,  h,  h,  h,  h, -1, -1 },
    { -1,  h,  y,  y,  y,  y,  h, -1 },
    {  h,  y,  y,  y,  y,  y,  y,  h },
    {  h,  y,  y,  y,  y,  y,  y,  h },
    {  h,  y,  y,  y,  y,  y,  y,  h },
    {  h,  y,  y,  y,  y,  y,  y,  h },
    { -1,  h,  y,  y,  y,  y,  h, -1 },
    { -1, -1,  h,  h,  h,  h, -1, -1 },
};

Coin::Coin(Pt pos)
    : pos_(pos), 
      initial_pos_(pos),
      collected_(false),
      animation_frame_(0) {}

void Coin::paint(Window& window) const {
    if (collected_) return;
    
    const Pt draw = {pos_.x - sprite_size / 2, pos_.y - sprite_size / 2};
    paint_sprite(window, draw, sprite_, false);
}

void Coin::update() {
    if (collected_) return;
    
    animation_frame_++;
    float offset = std::sin(animation_frame_ * animation_speed) * animation_amplitude;
    pos_.y = initial_pos_.y + static_cast<int>(offset);
}

bool Coin::check_collision(Pt mario_pos) {
    if (collected_) return false;
    
    const int collision_radius = 15;
    int dx = std::abs(mario_pos.x - pos_.x);
    int dy = std::abs(mario_pos.y - pos_.y);
    
    return (dx < collision_radius && dy < collision_radius);
}