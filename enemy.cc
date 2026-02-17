#include "enemy.hh"
#include "utils.hh"
#include <cmath>

using namespace pro2;

// Colores para el Goomba
const int _ = -1;  // Transparente
const int B = 0x8B4513;  // Marrón
const int D = 0x654321;  // Marrón oscuro
const int W = 0xFFFFFF;  // Blanco (ojos)
const int K = 0x000000;  // Negro (pupilas)

// Sprite del Goomba (12x12)
const std::vector<std::vector<int>> Enemy::goomba_sprite_ = {
    {_, _, _, B, B, B, B, B, B, _, _, _},
    {_, _, B, B, B, B, B, B, B, B, _, _},
    {_, B, B, B, B, B, B, B, B, B, B, _},
    {_, B, W, W, B, B, B, B, W, W, B, _},
    {B, B, W, K, B, B, B, B, W, K, B, B},
    {B, B, W, W, B, B, B, B, W, W, B, B},
    {B, B, B, B, B, B, B, B, B, B, B, B},
    {B, D, D, B, B, B, B, B, B, D, D, B},
    {D, D, D, D, B, B, B, B, D, D, D, D},
    {D, D, D, D, D, B, B, D, D, D, D, D},
    {_, D, D, D, D, D, D, D, D, D, D, _},
    {_, _, D, D, D, D, D, D, D, D, _, _},
};

Enemy::Enemy(Pt pos, Type type)
    : pos_(pos), speed_{-2, 0}, type_(type), 
      alive_(true), moving_left_(true), animation_frame_(0) {}

void Enemy::update(const std::vector<Platform>& platforms) {
    if (!alive_) return;
    
    animation_frame_++;
    
    // Aplicar gravedad
    speed_.y += 1;  // Gravedad
    
    // Actualizar posición
    Pt old_pos = pos_;
    pos_.x += speed_.x;
    pos_.y += speed_.y;
    
    // Comprobar colisiones con plataformas
    bool grounded = false;
    for (const Platform& platform : platforms) {
        if (platform.has_crossed_floor_downwards(old_pos, pos_)) {
            pos_.y = platform.top();
            speed_.y = 0;
            grounded = true;
        }
    }
    
    // Si no está en el suelo o choca con algo, girar
    if (!grounded && old_pos.y <= pos_.y) {
        // No hay suelo debajo, girar
        speed_.x = -speed_.x;
        moving_left_ = !moving_left_;
    }
    
    // Detectar colisión con bordes de plataformas (simplificado)
    // Si se mueve demasiado hacia abajo, probablemente se cayó del borde
    if (pos_.y > old_pos.y + 20) {
        speed_.x = -speed_.x;
        moving_left_ = !moving_left_;
        pos_.x = old_pos.x;
    }
}

void Enemy::paint(pro2::Window& window) const {
    if (!alive_) return;
    
    const Pt top_left = {pos_.x - sprite_width/2, pos_.y - sprite_height/2};
    paint_sprite(window, top_left, goomba_sprite_, !moving_left_);
}

bool Enemy::check_collision_with_mario(Pt mario_pos, bool& jumped_on) {
    if (!alive_) return false;
    
    Rect enemy_rect = get_rect();
    Rect mario_rect = {mario_pos.x - 6, mario_pos.y - 15, 
                       mario_pos.x + 6, mario_pos.y + 1};
    
    // Verificar intersección
    bool intersects = !(enemy_rect.right < mario_rect.left || 
                       enemy_rect.left > mario_rect.right ||
                       enemy_rect.bottom < mario_rect.top || 
                       enemy_rect.top > mario_rect.bottom);
    
    if (intersects) {
        // Determinar si Mario saltó sobre el enemigo
        // Si Mario está cayendo y su posición está por encima del centro del enemigo
        jumped_on = (mario_pos.y < pos_.y - sprite_height/4);
        return true;
    }
    
    return false;
}