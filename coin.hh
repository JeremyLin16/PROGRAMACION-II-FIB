#ifndef COIN_HH
#define COIN_HH

#include "window.hh"
#include <vector>

namespace pro2 {

class Coin {
private:
    Pt pos_;
    Pt initial_pos_;
    bool collected_;
    int animation_frame_;
    
    static const std::vector<std::vector<int>> sprite_;
    static constexpr int sprite_size = 8;
    static constexpr float animation_amplitude = 8.0f;
    static constexpr float animation_speed = 0.08f;

public:
    Coin(Pt pos);
    
    void paint(Window& window) const;
    void update();
    bool check_collision(Pt mario_pos);
    
    void collect() { collected_ = true; }
    bool is_collected() const { return collected_; }
    
    Rect get_rect() const {
        return {pos_.x - sprite_size/2, pos_.y - sprite_size/2,
                pos_.x + sprite_size/2, pos_.y + sprite_size/2};
    }
};

} // namespace pro2

#endif