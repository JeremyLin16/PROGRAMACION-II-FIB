#ifndef ENEMY_HH
#define ENEMY_HH

#include "window.hh"
#include "platform.hh"
#include <vector>

class Enemy {
public:
    enum Type {
        GOOMBA,
        KOOPA
    };

private:
    pro2::Pt pos_;
    pro2::Pt speed_;
    Type type_;
    bool alive_;
    bool moving_left_;
    int animation_frame_;
    
    // Sprite del Goomba
    static const std::vector<std::vector<int>> goomba_sprite_;
    static constexpr int sprite_width = 12;
    static constexpr int sprite_height = 12;
    
public:
    Enemy(pro2::Pt pos, Type type = GOOMBA);
    
    void update(const std::vector<Platform>& platforms);
    void paint(pro2::Window& window) const;
    
    // Rectángulo de colisión
    pro2::Rect get_rect() const {
        return {pos_.x - sprite_width/2, pos_.y - sprite_height/2,
                pos_.x + sprite_width/2, pos_.y + sprite_height/2};
    }
    
    pro2::Pt pos() const { return pos_; }
    bool is_alive() const { return alive_; }
    void kill() { alive_ = false; }
    
    // Verifica colisión con Mario
    bool check_collision_with_mario(pro2::Pt mario_pos, bool& jumped_on);
};

#endif
