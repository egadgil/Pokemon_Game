#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <cstdlib> 
enum stat_index {
  hp,
  attack,
  defense,
  special_attack,
  special_defense,
  speed
};

enum gender {
  male,
  female
};
class Pokemon {
public:
    
    enum gender {
        male,
        female
    };

    Pokemon(int level);

  
    const char* get_species() const;
    int get_level() const;
    int get_hp() const;
    int get_attack() const;
    int get_defense() const;
    int get_special_attack() const;
    int get_special_defense() const;
    int get_speed() const;
    const char* get_move(int i) const;

   
    static gender get_gender(); 
    static bool isShiny();

private:
    int level;
    int pokemon_index;
    int poke_moves[2];
    int poke_iv[6];
    int poke_statistics[6];
    gender poke_gender;
    bool is_shiny;
};

#endif 
