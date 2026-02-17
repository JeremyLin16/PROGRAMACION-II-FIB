# Part 1: Nou tipus d'objecte - Implementació

## Resum de la implementació

S'ha creat un nou tipus d'objecte coleccionable (estrelles daurades) que el Mario pot recollir durant el joc. Els objectes compleixen tots els requisits especificats.

## Fitxers nous creats

### 1. `collectible.hh`
Fitxer de capçalera que defineix la classe `Collectible` amb:
- Atributs privats per la posició, estat de recol·lecció i animació
- Mètodes públics per pintar, actualitzar i detectar col·lisions
- Sprite estàtic d'una estrella daurada

### 2. `collectible.cc`
Fitxer d'implementació que conté:
- Sprite d'una estrella daurada (10x10 píxels)
- Lògica d'animació amb moviment sinusoïdal vertical
- Detecció de col·lisions amb Mario

## Fitxers modificats

### 3. `game.hh`
- S'ha afegit `#include "collectible.hh"`
- Nou atribut `std::vector<Collectible> collectibles_` per emmagatzemar els objectes
- Nou atribut `int collected_count_` per comptar els objectes recollits
- Nou mètode públic `collected_count()` per consultar el comptador

### 4. `game.cc`
- **Constructor**: Inicialitza el comptador a 0 i crea diversos objectes coleccionables distribuïts pel nivell
  - 3 objectes a prop de l'inici (posicions fàcils)
  - 10+ objectes més endavant en el nivell
  - Alguns en posicions més difícils d'assolir
  
- **`update_objects()`**: 
  - Actualitza l'animació de tots els objectes
  - Detecta col·lisions amb ambdós Mario (mario_ i mario2_)
  - Incrementa el comptador quan es recull un objecte
  
- **`paint()`**: Dibuixa tots els objectes coleccionables (abans dels Mario per correcte ordre de capes)

## Característiques implementades

### ✅ Requisit 1: Distribució limitada
Els objectes estan distribuïts de forma limitada pel nivell:
- 3 estrelles a prop de l'inici
- ~13 estrelles més distribuïdes pel nivell
- Algunes en posicions més difícils d'accedir

### ✅ Requisit 2: Recollida automàtica
Quan el Mario passa per sobre (dins del radi de col·lisió de 15 píxels), l'objecte:
- Es marca com a recollit (`collected_ = true`)
- Desapareix de la pantalla (no es dibuixa si `collected_` és true)
- No es torna a actualitzar ni col·lisionar

### ✅ Requisit 3: Comptador d'objectes
- El comptador `collected_count_` s'incrementa cada vegada que es recull un objecte
- Es pot consultar amb el mètode `collected_count()`
- No es mostra per pantalla textualment (com s'especifica)

### ✅ Requisit 4: Animació
Implementat amb moviment de flotació sinusoïdal:
```cpp
float offset = std::sin(animation_frame_ * animation_speed) * animation_amplitude;
pos_.y = initial_pos_.y + static_cast<int>(offset);
```
- Amplitud: 8 píxels
- Velocitat: 0.08 radians per frame
- Moviment suau i natural que crida l'atenció del jugador

## Detalls tècnics

### Sprite de l'estrella
- Mida: 10x10 píxels
- Color: Daurada brillant (#FFD700)
- Estil: Estrella de 5 puntes
- Transparent: Fons transparent (-1)

### Col·lisions
- Detecció basada en distància Manhattan
- Radi de col·lisió: 15 píxels
- Comprova ambdós Mario (suport per 2 jugadors)

### Animació
- Flotació vertical sinusoïdal contínua
- No requereix sprites múltiples (rotació)
- Eficient i visualment atractiva

## Integració amb el joc

La classe `Collectible` s'integra perfectament amb l'arquitectura existent:
- Segueix el mateix patró que `Mario` i `Platform`
- Mètodes `paint()` i `update()` consistents
- Utilitza les mateixes estructures (`pro2::Pt`, `pro2::Window`)
- Compatibilitat amb el sistema de sprites existent

## Compilació

El projecte es compila correctament amb:
```bash
make clean
make
```

Tots els fitxers nous i modificats s'han integrat al Makefile automàticament gràcies a la regla `$(wildcard *.cc)`.

## Possibles extensions

Idees per a futures millores (opcionals):
- Diferents tipus d'objectes coleccionables
- Sons quan es recull un objecte
- Efectes visuals de recollida (partícules, puntuació flotant)
- Objectes especials amb efectes diferents
- Mostrar el comptador amb sprites de números
