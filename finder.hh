#ifndef FINDER_HH
#define FINDER_HH

#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "geometry.hh"

template <class T>
class Finder {
private:
    // Configuración del grid espacial
    static constexpr int SPACE_SIZE = 20000;
    static constexpr int CELL_SIZE = 1000;  // Tamaño de cada celda
    static constexpr int GRID_DIM = SPACE_SIZE / CELL_SIZE; // 20x20 celdas
    
    // Grid: matriz 2D de sets de punteros a objetos
    // Cada celda contiene los objetos que la intersectan
    std::vector<std::vector<std::set<const T*>>> grid_;
    
    // Mapa para recordar el rectángulo de cada objeto
    // Necesario para update() y remove()
    std::map<const T*, pro2::Rect> object_rects_;
    
    // Obtener las coordenadas de celda (cx, cy) para un punto (x, y)
    std::pair<int, int> get_cell(int x, int y) const {
        int cx = std::max(0, std::min(x / CELL_SIZE, GRID_DIM - 1));
        int cy = std::max(0, std::min(y / CELL_SIZE, GRID_DIM - 1));
        return {cx, cy};
    }
    
    // Obtener todas las celdas que un rectángulo intersecta
    std::vector<std::pair<int, int>> get_cells(pro2::Rect rect) const {
        std::pair<int, int> min_cell = get_cell(rect.left, rect.top);
        std::pair<int, int> max_cell = get_cell(rect.right, rect.bottom);
        
        std::vector<std::pair<int, int>> cells;
        for (int cy = min_cell.second; cy <= max_cell.second; ++cy) {
            for (int cx = min_cell.first; cx <= max_cell.first; ++cx) {
                cells.push_back({cx, cy});
            }
        }
        return cells;
    }
    
    // Verificar si dos rectángulos intersectan
    // Los límites están incluidos en el rectángulo
    bool intersects(pro2::Rect a, pro2::Rect b) const {
        return !(a.right < b.left || a.left > b.right ||
                 a.bottom < b.top || a.top > b.bottom);
    }
    
public:
    // Constructor: inicializa el grid vacío
    Finder() : grid_(GRID_DIM, std::vector<std::set<const T*>>(GRID_DIM)) {}
    
    // Añadir un objeto al finder
    void add(const T* t) {
        pro2::Rect rect = t->get_rect();
        object_rects_[t] = rect;
        
        // Añadir el objeto a todas las celdas que intersecta
        std::vector<std::pair<int, int>> cells = get_cells(rect);
        for (const auto& cell : cells) {
            grid_[cell.second][cell.first].insert(t);
        }
    }
    
    // Actualizar la posición de un objeto
    void update(const T* t) {
        // Remover de las celdas del rectángulo viejo
        auto it = object_rects_.find(t);
        if (it != object_rects_.end()) {
            pro2::Rect old_rect = it->second;
            std::vector<std::pair<int, int>> old_cells = get_cells(old_rect);
            for (const auto& cell : old_cells) {
                grid_[cell.second][cell.first].erase(t);
            }
        }
        
        // Añadir a las celdas del rectángulo nuevo
        pro2::Rect new_rect = t->get_rect();
        object_rects_[t] = new_rect;
        std::vector<std::pair<int, int>> new_cells = get_cells(new_rect);
        for (const auto& cell : new_cells) {
            grid_[cell.second][cell.first].insert(t);
        }
    }
    
    // Remover un objeto del finder
    void remove(const T* t) {
        auto it = object_rects_.find(t);
        if (it != object_rects_.end()) {
            pro2::Rect rect = it->second;
            
            // Remover de todas las celdas que intersectaba
            std::vector<std::pair<int, int>> cells = get_cells(rect);
            for (const auto& cell : cells) {
                grid_[cell.second][cell.first].erase(t);
            }
            
            object_rects_.erase(it);
        }
    }
    
    // Consultar objetos que intersectan con un rectángulo
    std::set<const T*> query(pro2::Rect qrect) const {
        std::set<const T*> result;
        
        // Obtener las celdas que el rectángulo de consulta intersecta
        std::vector<std::pair<int, int>> cells = get_cells(qrect);
        
        // Recolectar candidatos de todas las celdas relevantes
        // Usamos un set para evitar duplicados (un objeto puede estar en múltiples celdas)
        std::set<const T*> candidates;
        for (const auto& cell : cells) {
            const auto& cell_objects = grid_[cell.second][cell.first];
            candidates.insert(cell_objects.begin(), cell_objects.end());
        }
        
        // Verificar intersección real con cada candidato
        for (const T* obj : candidates) {
            if (intersects(obj->get_rect(), qrect)) {
                result.insert(obj);
            }
        }
        
        return result;
    }
};

#endif
