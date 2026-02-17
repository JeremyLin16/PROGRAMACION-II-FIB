#include "specialblock.hh"
#include "utils.hh"

using namespace pro2;

const int _ = -1;
const int Y = 0xFFD700;  // Amarillo (?)
const int R = 0x8B4513;  // Marrón (ladrillo)
const int G = 0x808080;  // Gris (vacío)
const int B = 0x000000;  // Negro

// Textura bloque "?" (11x11)
const std::vector<std::vector<int>> SpecialBlock::question_texture_ = {
    {Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y},
    {Y, Y, Y, B, B, B, B, B, Y, Y, Y},
    {Y, Y, B, B, B, B, B, B, B, Y, Y},
    {Y, Y, B, B, B, B, B, B, B, Y, Y},
    {Y, Y, Y, Y, Y, B, B, B, Y, Y, Y},
    {Y, Y, Y, Y, B, B, B, Y, Y, Y, Y},
    {Y, Y, Y, Y, B, B, B, Y, Y, Y, Y},
    {Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y},
    {Y, Y, Y, Y, B, B, B, Y, Y, Y, Y},
    {Y, Y, Y, Y, B, B, B, Y, Y, Y, Y},
    {Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y},
};

// Textura ladrillo (11x11)
const std::vector<std::vector<int>> SpecialBlock::brick_texture_ = {
    {R, R, R, R, R, B, R, R, R, R, R},
    {R, R, R, R, R, B, R, R, R, R, R},
    {B, B, B, B, B, B, B, B, B, B, B},
    {R, R, R, B, R, R, R, R, R, B, R},
    {R, R, R, B, R, R, R, R, R, B, R},
    {B, B, B, B, B, B, B, B, B, B, B},
    {R, R, R, R, R, B, R, R, R, R, R},
    {R, R, R, R, R, B, R, R, R, R, R},
    {B, B, B, B, B, B, B, B, B, B, B},
    {R, R, R, B, R, R, R, R, R, B, R},
    {R, R, R, B, R, R, R, R, R, B, R},
};

// Textura bloque vacío (11x11)
const std::vector<std::vector<int>> SpecialBlock::empty_texture_ = {
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
    {G, G, G, G, G, G, G, G, G, G, G},
};

SpecialBlock::SpecialBlock(int left, int right, int top, int bottom, Type type,
                           bool has_powerup, PowerUp::Type powerup)
    : left_(left), right_(right), top_(top), bottom_(bottom),
      type_(type), activated_(false), animation_offset_(0),
      contains_powerup_(powerup), has_powerup_(has_powerup) {}

void SpecialBlock::paint(pro2::Window& window) const {
    Pt topleft = {left_, top_ + animation_offset_};
    
    const auto& texture = (type_ == QUESTION && !activated_) ? question_texture_ :
                         (type_ == BRICK) ? brick_texture_ :
                         empty_texture_;
    
    paint_sprite(window, topleft, texture, false);
}

void SpecialBlock::update() {
    // Animación de rebote cuando se golpea
    if (animation_offset_ < 0) {
        animation_offset_++;
    }
}

bool SpecialBlock::check_hit_from_below(Pt mario_pos, Pt mario_last_pos) {
    if (activated_ && type_ == QUESTION) return false;
    
    // Mario debe estar debajo del bloque y moviéndose hacia arriba
    bool below_last_frame = mario_last_pos.y >= bottom_;
    bool crossing_now = mario_pos.y < bottom_;
    bool horizontally_aligned = mario_pos.x >= left_ - 10 && mario_pos.x <= right_ + 10;
    
    if (below_last_frame && crossing_now && horizontally_aligned) {
        animation_offset_ = -5;  // Rebote hacia arriba
        return true;
    }
    
    return false;
}

PowerUp* SpecialBlock::activate() {
    if (activated_) return nullptr;
    
    activated_ = true;
    
    if (type_ == QUESTION && has_powerup_) {
        // Crear power-up en la posición del bloque
        int powerup_x = (left_ + right_) / 2;
        int powerup_y = top_ - 15;
        return new PowerUp({powerup_x, powerup_y}, contains_powerup_);
    }
    
    return nullptr;
}

bool SpecialBlock::has_crossed_floor_downwards(Pt plast, Pt pcurr) const {
    // Si Mario estaba encima del bloque y ahora lo atravesó
    return plast.y <= top_ && pcurr.y > top_ &&
           pcurr.x >= left_ && pcurr.x <= right_;
}

bool SpecialBlock::is_pt_inside(Pt pt) const {
    return pt.x >= left_ && pt.x <= right_ &&
           pt.y >= top_ && pt.y <= bottom_;
}
