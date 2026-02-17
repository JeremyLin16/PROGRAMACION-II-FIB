#include "game.hh"
using namespace pro2;

Game::Game(int width, int height)
    : mario_({width / 2, 150}),
      collected_count_(0),
      lives_(3),
      score_(0),
      finished_(false) {
    
    // Crear plataformas base
    platforms_.push_back(Platform(100, 300, 200, 211));
    platforms_.push_back(Platform(0, 200, 250, 261));
    platforms_.push_back(Platform(250, 400, 150, 161));
    
    // Crear plataformas distribuidas por el nivel (menos que antes para dejar espacio)
    for (int i = 1; i < 200; i++) {
        int x = 250 + i * 250;
        int y = 400 + (i % 10) * 50;  // Variación en altura
        platforms_.push_back(Platform(x, x + 150, y, y + 11));
    }
    
    // Añadir todas las plataformas al finder
    for (const Platform& p : platforms_) {
        platform_finder_.add(&p);
    }
    
    // ===== NUEVOS OBJETOS (Part 3) =====
    
    // Crear ENEMIGOS distribuidos por el nivel (usar std::list)
    for (int i = 2; i < 50; i++) {
        int x = 300 + i * 400;
        int y = 350 + (i % 10) * 50;
        enemies_.push_back(Enemy({x, y}, Enemy::GOOMBA));
    }
    
    // Añadir enemigos al finder
    for (const Enemy& e : enemies_) {
        enemy_finder_.add(&e);
    }
    
    // Crear BLOQUES ESPECIALES con power-ups
    special_blocks_.push_back(SpecialBlock(400, 440, 300, 340, 
                              SpecialBlock::QUESTION, true, PowerUp::STAR));
    special_blocks_.push_back(SpecialBlock(600, 640, 280, 320, 
                              SpecialBlock::QUESTION, true, PowerUp::MUSHROOM));
    special_blocks_.push_back(SpecialBlock(800, 840, 260, 300, 
                              SpecialBlock::QUESTION, true, PowerUp::FEATHER));
    
    // Añadir algunos bloques ladrillos decorativos
    for (int i = 5; i < 30; i++) {
        int x = 200 + i * 350;
        int y = 250 + (i % 5) * 60;
        special_blocks_.push_back(SpecialBlock(x, x + 40, y, y + 40, 
                                  SpecialBlock::BRICK, false));
    }
    
    // Añadir bloques al finder
    for (const SpecialBlock& b : special_blocks_) {
        block_finder_.add(&b);
    }
    
    // Crear coleccionables iniciales (menos que antes)
    collectibles_.push_back(Collectible({150, 180}));
    collectibles_.push_back(Collectible({200, 240}));
    collectibles_.push_back(Collectible({100, 360}));
    
    // Distribuir coleccionables por el nivel
    for (int i = 1; i < 100; i++) {
        collectibles_.push_back(Collectible({300 + i * 300, 350 + (i % 10) * 50}));
        if (i % 3 == 0) {
            collectibles_.push_back(Collectible({350 + i * 300, 300 + (i % 8) * 50}));
        }
    }
    
    // Añadir todos los coleccionables al finder
    for (const Collectible& c : collectibles_) {
        collectible_finder_.add(&c);
    }
}

void Game::process_keys(pro2::Window& window) {
    if (window.is_key_down(Keys::Escape)) {
        finished_ = true;
        return;
    }
    if(window.is_key_down('P')){
        if(!has_been_pressed){
            paused=!paused;
            has_been_pressed=true;
            return;
        }
    }
    else{has_been_pressed=false;}

}

void Game::update_objects(pro2::Window& window) {
    // Obtener el rectángulo visible de la cámara
    pro2::Rect camera_rect = window.camera_rect();
    
    // Expandir un poco el rectángulo para incluir objetos justo fuera de la pantalla
    // Esto evita pop-in visual y permite colisiones en los bordes
    const int margin = 200;
    pro2::Rect extended_rect = {
        camera_rect.left - margin,
        camera_rect.top - margin,
        camera_rect.right + margin,
        camera_rect.bottom + margin
    };
    
    // Obtener solo las plataformas cercanas usando el Finder
    std::set<const Platform*> nearby_platforms = platform_finder_.query(extended_rect);
    
    // Convertir a vector para pasarlo a Mario::update
    std::vector<Platform> nearby_platforms_vec;
    for (const Platform* p : nearby_platforms) {
        nearby_platforms_vec.push_back(*p);
    }
    
    // Actualizar Mario solo con plataformas cercanas
    mario_.update(window, nearby_platforms_vec);
    
    // Obtener solo los coleccionables cercanos usando el Finder
    std::set<const Collectible*> nearby_collectibles = collectible_finder_.query(extended_rect);
    
    // Actualizar y verificar colisiones solo con coleccionables cercanos
    for (const Collectible* c_ptr : nearby_collectibles) {
        for (Collectible& c : collectibles_) {
            if (&c == c_ptr) {
                c.update();
                if (c.check_collision(mario_.pos())) {
                    c.collect();
                    collected_count_++;
                }
                break;
            }
        }
    }
}

void Game::update_camera(pro2::Window& window) {
    const Pt pos = mario_.pos();
    const Pt cam = window.camera_center();

    const int left = cam.x - window.width() / 4;
    const int right = cam.x + window.width() / 4;
    const int top = cam.y - window.height() / 4;
    const int bottom = cam.y + window.height() / 4;

    int dx = 0, dy = 0;
    if (pos.x > right) {
        dx = pos.x - right;
    } else if (pos.x < left) {
        dx = pos.x - left;
    }
    if (pos.y < top) {
        dy = pos.y - top;
    } else if (pos.y > bottom) {
        dy = pos.y - bottom;
    }

    window.move_camera({dx, dy});
}

void Game::update(pro2::Window& window) {
    process_keys(window);
    if(!pause()){
        update_objects(window);
        update_enemies(window);
        update_powerups(window);
        update_special_blocks(window);
        update_effects();
        check_enemy_collisions();
        update_camera(window);
    }
}

void Game::paint(pro2::Window& window) {
    window.clear(sky_blue);
    
    // Obtener el rectángulo visible de la cámara
    pro2::Rect camera_rect = window.camera_rect();
    
    // Usar el Finder para obtener solo las plataformas visibles
    std::set<const Platform*> visible_platforms = platform_finder_.query(camera_rect);
    for (const Platform* p : visible_platforms) {
        p->paint(window);
    }
    
    // Dibujar bloques especiales visibles
    std::set<const SpecialBlock*> visible_blocks = block_finder_.query(camera_rect);
    for (const SpecialBlock* b : visible_blocks) {
        b->paint(window);
    }
    
    // Usar el Finder para obtener solo los coleccionables visibles
    std::set<const Collectible*> visible_collectibles = collectible_finder_.query(camera_rect);
    for (const Collectible* c : visible_collectibles) {
        c->paint(window);
    }
    
    // Dibujar power-ups visibles
    for (const PowerUp& p : powerups_) {
        if (!p.is_collected()) {
            pro2::Rect p_rect = p.get_rect();
            if (!(p_rect.right < camera_rect.left || p_rect.left > camera_rect.right ||
                  p_rect.bottom < camera_rect.top || p_rect.top > camera_rect.bottom)) {
                p.paint(window);
            }
        }
    }
    
    // Dibujar enemigos visibles
    std::set<const Enemy*> visible_enemies = enemy_finder_.query(camera_rect);
    for (const Enemy* e : visible_enemies) {
        e->paint(window);
    }
    
    // Mario siempre se dibuja (siempre está cerca de la cámara)
    mario_.paint(window);
    
    // HUD: Mostrar vidas y score
    // Aquí se podría añadir texto si tuviéramos una función de texto
}
// ===== IMPLEMENTACIÓN NUEVOS MÉTODOS (Part 3) =====

void Game::update_enemies(pro2::Window& window) {
    pro2::Rect camera_rect = window.camera_rect();
    const int margin = 300;
    pro2::Rect extended_rect = {
        camera_rect.left - margin,
        camera_rect.top - margin,
        camera_rect.right + margin,
        camera_rect.bottom + margin
    };
    
    // Obtener plataformas cercanas para los enemigos
    std::set<const Platform*> nearby_platforms = platform_finder_.query(extended_rect);
    std::vector<Platform> nearby_platforms_vec;
    for (const Platform* p : nearby_platforms) {
        nearby_platforms_vec.push_back(*p);
    }
    
    // Actualizar solo enemigos cercanos y eliminar muertos (std::list permite esto)
    auto it = enemies_.begin();
    while (it != enemies_.end()) {
        if (!it->is_alive()) {
            // Remover del finder antes de eliminar
            enemy_finder_.remove(&(*it));
            it = enemies_.erase(it);  // std::list permite borrado eficiente
        } else {
            it->update(nearby_platforms_vec);
            ++it;
        }
    }
}

void Game::update_powerups(pro2::Window& window) {
    // Actualizar power-ups
    for (PowerUp& p : powerups_) {
        if (!p.is_collected()) {
            p.update();
            
            // Verificar colisión con Mario
            if (p.check_collision(mario_.pos())) {
                p.collect();
                apply_powerup_effect(p.get_type());
                score_ += 100;
            }
            
            if (p.check_collision(mario_.pos())) {
                p.collect();
                apply_powerup_effect(p.get_type());
                score_ += 100;
            }
        }
    }
}

void Game::update_special_blocks(pro2::Window& window) {
    pro2::Pt mario_pos = mario_.pos();
    static pro2::Pt mario_last_pos = mario_pos;
    
    for (SpecialBlock& block : special_blocks_) {
        block.update();
        
        if (block.check_hit_from_below(mario_pos, mario_last_pos)) {
            PowerUp* new_powerup = block.activate();
            if (new_powerup != nullptr) {
                powerups_.push_back(*new_powerup);
                delete new_powerup;
                score_ += 50;
            }
        }
    }
    
    mario_last_pos = mario_pos;
}

void Game::update_effects() {
    // Actualizar efectos activos usando la queue
    std::queue<TimedEffect> updated_effects;
    
    while (!active_effects_.empty()) {
        TimedEffect effect = active_effects_.front();
        active_effects_.pop();
        
        effect.frames_remaining--;
        
        if (effect.frames_remaining > 0) {
            updated_effects.push(effect);
            active_effect_timers_[effect.type] = effect.frames_remaining;
        } else {
            // Efecto terminado, remover del map
            active_effect_timers_.erase(effect.type);
        }
    }
    
    active_effects_ = updated_effects;
}

void Game::check_enemy_collisions() {
    // Verificar colisiones de Mario con enemigos
    for (Enemy& enemy : enemies_) {
        if (!enemy.is_alive()) continue;
        
        bool jumped_on = false;
        
        // Verificar con mario_
        if (enemy.check_collision_with_mario(mario_.pos(), jumped_on)) {
            if (jumped_on || has_active_effect(PowerUp::STAR)) {
                // Mario salta sobre el enemigo o es invencible -> matar enemigo
                enemy.kill();
                score_ += 200;
                // Aquí se podría añadir un rebote pequeño a Mario
            } else {
                // Enemigo toca a Mario lateralmente -> perder vida
                if (!has_active_effect(PowerUp::STAR)) {
                    lives_--;
                    if (lives_ <= 0) {
                        finished_ = true;
                    }
                }
            }
        }
    }
}

void Game::apply_powerup_effect(PowerUp::Type type) {
    PowerUp temp_powerup({0, 0}, type);
    PowerUp::Config cfg = temp_powerup.get_config();
    
    // Añadir efecto a la queue
    active_effects_.push(TimedEffect(type, cfg.duration_frames));
    active_effect_timers_[type] = cfg.duration_frames;
    
    std::cout << "Power-up activado: " << cfg.name 
              << " (duración: " << cfg.duration_frames << " frames)" << std::endl;
}

bool Game::has_active_effect(PowerUp::Type type) const {
    return active_effect_timers_.find(type) != active_effect_timers_.end();
}
