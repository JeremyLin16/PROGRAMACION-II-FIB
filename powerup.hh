#ifndef POWERUP_HH
#define POWERUP_HH

#include "window.hh"
#include <vector>

class PowerUp {
public:
    enum Type {
        STAR,          // Invencibilidad
        MUSHROOM,      // Super velocidad
        FEATHER        // Super salto
    };
    
    struct Config {
        int duration_frames;  // Duración del efecto en frames
        int color;           // Color del sprite
        std::string name;    // Nombre del power-up
    };

private:
    pro2::Pt pos_;
    Type type_;
    bool collected_;
    int animation_frame_;
    
    static const std::vector<std::vector<int>> powerup_sprite_;
    static constexpr int sprite_size = 10;
    
    // Configuración de cada tipo de power-up
    static Config get_config(Type type);
    
public:
    PowerUp(pro2::Pt pos, Type type);
    
    void paint(pro2::Window& window) const;
    void update();
    
    bool check_collision(pro2::Pt mario_pos);
    void collect() { collected_ = true; }
    bool is_collected() const { return collected_; }
    
    Type get_type() const { return type_; }
    Config get_config() const { return get_config(type_); }
    
    pro2::Rect get_rect() const {
        return {pos_.x - sprite_size/2, pos_.y - sprite_size/2,
                pos_.x + sprite_size/2, pos_.y + sprite_size/2};
    }
};

// Estructura para efectos temporales activos
struct TimedEffect {
    PowerUp::Type type;
    int frames_remaining;
    
    TimedEffect(PowerUp::Type t, int frames) 
        : type(t), frames_remaining(frames) {}
};

#endif
