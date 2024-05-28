#include <math.h>

#include "pokemon.h"
#include "db_parse.h"

#include <unistd.h>
#include <ncurses.h>
#include <cctype>
#include <cstdlib>
#include <climits>

Pokemon::Pokemon(int level) : level(level){
  int i;
  pokemon_index = (rand() % 898) + 1;


  do {
  

    poke_moves[0] = pokemon_moves[(rand() % 528238) + 1].move_id;
  }
  while(!((pokemon_moves[(rand() % 528238) + 1].pokemon_id == pokemon[pokemon_index].species_id) &&
          (pokemon_moves[(rand() % 528238) + 1].pokemon_move_method_id == 1)));

  do {
   

    poke_moves[1] = pokemon_moves[(rand() % 528238) + 1].move_id;
  }
  while(!((pokemon_moves[(rand() % 528238) + 1].pokemon_id == pokemon[pokemon_index].species_id) &&
          (pokemon_moves[(rand() % 528238) + 1].pokemon_move_method_id == 1)) ||
          (((rand() % 528238) + 1 )== ((rand() % 528238) + 1)));

  for(i = 0; i < 6; i++) {
    poke_iv[i] = rand() % 16;

    if(i == 0) {
      poke_statistics[i] = (((((pokemon_stats[1 + (pokemon_index - 1) * 6].base_stat + poke_iv[i]) * 2) * level) / 100) + level + 10);
    }
    else {
      poke_statistics[i] = (((((pokemon_stats[1 + (pokemon_index - 1) * 6].base_stat + poke_iv[i]) * 2) * level) / 100) + 5);
    }
  }

 
   poke_gender = Pokemon::get_gender();
    is_shiny = Pokemon::isShiny();
}

const char *Pokemon::get_species() const {
  return species[pokemon_index].identifier;
}

int Pokemon::get_level() const {
  return level;
}

int Pokemon::get_hp() const {
  return poke_statistics[hp];
}

int Pokemon::get_attack() const {
  return poke_statistics[attack];
}

int Pokemon::get_defense() const {
  return poke_statistics[defense];
}

int Pokemon::get_special_attack() const {
  return poke_statistics[special_attack];
}

int Pokemon::get_special_defense() const {
  return poke_statistics[special_defense];
}

int Pokemon::get_speed() const {
  return poke_statistics[speed];
}

const char *Pokemon::get_move(int i) const {
  return moves[poke_moves[i]].identifier;
}

Pokemon::gender Pokemon::get_gender() {
    return static_cast<gender>(rand() % 2); 
}

bool Pokemon::isShiny() {
    return (rand() % 8192) == 0; 
}