#ifndef COLLECTIBLE_HH
#define COLLECTIBLE_HH

#include "window.hh"
#include <vector>

class Collectible {
 private:
    pro2::Pt initial_pos_;  // Posición inicial (para la animación)
    pro2::Pt pos_;          // Posición actual
    bool collected_;        // Si ya ha sido recogido
    int animation_frame_;   // Frame de animación
    
    static const std::vector<std::vector<int>> star_sprite_;
    static constexpr int sprite_size = 10;  // Tamaño del sprite
    static constexpr float animation_amplitude = 8.0f;  // Amplitud de la flotación
    static constexpr float animation_speed = 0.08f;     // Velocidad de la animación

 public:
    // Constructor
    Collectible(pro2::Pt pos);
    
    // Dibuja el objeto coleccionable
    void paint(pro2::Window& window) const;
    
    // Actualiza la animación
    void update();
    
    // Comprueba si Mario ha tocado este objeto
    bool check_collision(pro2::Pt mario_pos);
    
    // Marca el objeto como recogido
    void collect() {
        collected_ = true;
    }
    
    // Verifica si ya ha sido recogido
    bool is_collected() const {
        return collected_;
    }
    
    // Obtiene la posición actual
    pro2::Pt pos() const {
        return pos_;
    }

    pro2::Rect get_rect() const {
        const int half = 4;
        return {pos_.x - half, pos_.y - half,
                pos_.x + half, pos_.y + half};
    }
};

#endif
