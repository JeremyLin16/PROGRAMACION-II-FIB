#ifndef SPECIALBLOCK_HH
#define SPECIALBLOCK_HH

#include "window.hh"
#include "powerup.hh"
#include <vector>
#include <memory>

class SpecialBlock {
public:
    enum Type {
        QUESTION,   // Bloque "?" con power-up
        BRICK,      // Bloque de ladrillo rompible
        SOLID       // Bloque sólido no rompible
    };

private:
    int left_, right_, top_, bottom_;
    Type type_;
    bool activated_;  // Si ya fue golpeado
    int animation_offset_;  // Para animación cuando se golpea
    
    // Qué contiene el bloque (si es QUESTION)
    PowerUp::Type contains_powerup_;
    bool has_powerup_;
    
    static const std::vector<std::vector<int>> question_texture_;
    static const std::vector<std::vector<int>> brick_texture_;
    static const std::vector<std::vector<int>> empty_texture_;

public:
    SpecialBlock(int left, int right, int top, int bottom, Type type,
                 bool has_powerup = false, 
                 PowerUp::Type powerup = PowerUp::STAR);
    
    void paint(pro2::Window& window) const;
    void update();
    
    // Verifica si Mario golpeó el bloque desde abajo
    bool check_hit_from_below(pro2::Pt mario_pos, pro2::Pt mario_last_pos);
    
    // Activar el bloque (golpeado)
    PowerUp* activate();  // Retorna el power-up si tiene
    
    bool is_activated() const { return activated_; }
    
    pro2::Rect get_rect() const {
        return {left_, top_, right_, bottom_};
    }
    
    // Para colisiones normales (como Platform)
    bool has_crossed_floor_downwards(pro2::Pt plast, pro2::Pt pcurr) const;
    bool is_pt_inside(pro2::Pt pt) const;
    int top() const { return top_; }
};

#endif
