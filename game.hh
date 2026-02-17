#ifndef GAME_HH
#define GAME_HH

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <iostream>
#include "Mario.hh"
#include "platform.hh"
#include "collectible.hh"
#include "enemy.hh"
#include "powerup.hh"
#include "specialblock.hh"
#include "finder.hh"
#include "window.hh"

class Game {
    Mario                      mario_;
    std::vector<Platform>      platforms_;
    std::vector<Collectible>   collectibles_;
    
    // NUEVOS OBJETOS (Part 3)
    std::list<Enemy>           enemies_;           // std::list para gestión dinámica
    std::vector<PowerUp>       powerups_;
    std::vector<SpecialBlock>  special_blocks_;
    
    // CONTENEDOR STL: Queue para efectos temporales activos
    std::queue<TimedEffect>    active_effects_;
    
    // CONTENEDOR STL: Map para tracking de efectos activos por tipo
    std::map<PowerUp::Type, int> active_effect_timers_;
    
    // Finders para optimizar consultas espaciales
    Finder<Platform>           platform_finder_;
    Finder<Collectible>        collectible_finder_;
    Finder<Enemy>              enemy_finder_;
    Finder<SpecialBlock>       block_finder_;
    
    int collected_count_;  // Contador de objetos recogidos
    int lives_;            // Vidas del jugador
    int score_;            // Puntuación

    bool finished_;
    bool paused=false;
    bool has_been_pressed=false;

    void process_keys(pro2::Window& window);
    void update_objects(pro2::Window& window);
    void update_camera(pro2::Window& window);
    
    // NUEVOS MÉTODOS (Part 3)
    void update_enemies(pro2::Window& window);
    void update_powerups(pro2::Window& window);
    void update_special_blocks(pro2::Window& window);
    void update_effects();
    void check_enemy_collisions();
    void apply_powerup_effect(PowerUp::Type type);
    bool has_active_effect(PowerUp::Type type) const;

 public:
    Game(int width, int height);

    
    void update(pro2::Window& window);
    void paint(pro2::Window& window);

    bool pause() const{
        return paused;
    }

    bool is_finished() const {
        return finished_;
    }

    int collected_count() const {
        return collected_count_;
    }
    
    int lives() const {
        return lives_;
    }
    
    int score() const {
        return score_;
    }
    
    // Debug: obtener estadísticas de objetos
    void print_stats(pro2::Window& window) const {
        pro2::Rect camera_rect = window.camera_rect();
        auto visible_platforms = platform_finder_.query(camera_rect);
        auto visible_collectibles = collectible_finder_.query(camera_rect);
        
        std::cout << "=== ESTADÍSTICAS DEL FINDER ===" << std::endl;
        std::cout << "Total plataformas: " << platforms_.size() << std::endl;
        std::cout << "Plataformas visibles: " << visible_platforms.size() << std::endl;
        std::cout << "Total coleccionables: " << collectibles_.size() << std::endl;
        std::cout << "Coleccionables visibles: " << visible_collectibles.size() << std::endl;
        std::cout << "Ratio renderizado: " 
                  << (visible_platforms.size() + visible_collectibles.size()) 
                  << "/" 
                  << (platforms_.size() + collectibles_.size())
                  << " = "
                  << (100.0 * (visible_platforms.size() + visible_collectibles.size()) / 
                     (platforms_.size() + collectibles_.size()))
                  << "%" << std::endl;
        std::cout << "===============================" << std::endl;
    }

 private:
    static constexpr int sky_blue = 0x5c94fc;
};

#endif