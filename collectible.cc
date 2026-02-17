#include "collectible.hh"
#include "utils.hh"
#include <cmath>
using namespace pro2;

// Definición de colores
const int _ = -1;  // Transparente
const int Y = 0xFFD700;  // Dorado brillante
const int O = 0xFFA500;  // Naranja (sombra)

// Sprite de una estrella (10x10)
// clang-format off
const std::vector<std::vector<int>> Collectible::star_sprite_ = {
    {_, _, _, _, Y, Y, _, _, _, _},
    {_, _, _, Y, Y, Y, Y, _, _, _},
    {_, _, Y, Y, Y, Y, Y, Y, _, _},
    {Y, Y, Y, Y, Y, Y, Y, Y, Y, Y},
    {_, Y, Y, Y, Y, Y, Y, Y, Y, _},
    {_, _, Y, Y, Y, Y, Y, Y, _, _},
    {_, _, Y, Y, _, _, Y, Y, _, _},
    {_, Y, Y, _, _, _, _, Y, Y, _},
    {_, Y, _, _, _, _, _, _, Y, _},
    {Y, _, _, _, _, _, _, _, _, Y},
};
// clang-format on

Collectible::Collectible(Pt pos)
    : initial_pos_(pos), 
      pos_(pos), 
      collected_(false),
      animation_frame_(0) {}

void Collectible::paint(pro2::Window& window) const {
    // No dibujamos si ya ha sido recogido
    if (collected_) {
        return;
    }
    
    // Calcular la posición del sprite (centrado en pos_)
    const Pt top_left = {pos_.x - sprite_size / 2, pos_.y - sprite_size / 2};
    paint_sprite(window, top_left, star_sprite_, false);
}

void Collectible::update() {
    // No actualizamos si ya ha sido recogido
    if (collected_) {
        return;
    }
    
    // Animación de flotación sinusoidal
    animation_frame_++;
    
    // Calcular el desplazamiento vertical usando una función sinusoidal
    float offset = std::sin(animation_frame_ * animation_speed) * animation_amplitude;
    
    // Actualizar la posición vertical
    pos_.y = initial_pos_.y + static_cast<int>(offset);
}

bool Collectible::check_collision(Pt mario_pos) {
    // No colisionamos si ya ha sido recogido
    if (collected_) {
        return false;
    }
    
    // Radio de colisión simple (distancia Manhattan)
    const int collision_radius = 15;
    
    int dx = std::abs(mario_pos.x - pos_.x);
    int dy = std::abs(mario_pos.y - pos_.y);
    
    // Si Mario está lo suficientemente cerca, se considera una colisión
    return (dx < collision_radius && dy < collision_radius);
}
