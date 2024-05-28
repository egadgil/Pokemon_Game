#ifndef DB_PARSE_H
# define DB_PARSE_H

struct pokemon_struct {
  int id;
  char identifier[30];
  int species_id;
  int height;
  int weight;
  int base_experience;
  int order;
  int is_default;
};

struct move_struct {
  int id;
  char identifier[50];
  int generation_id;
  int type_id;
  int power;
  int pp;
  int accuracy;
  int priority;
  int target_id;
  int damage_class_id;
  int effect_id;
  int effect_chance;
  int contest_type_id;
  int contest_effect_id;
  int super_contest_effect_id;
};

struct pokemon_move_struct {
  int pokemon_id;
  int version_group_id;
  int move_id;
  int pokemon_move_method_id;
  int level;
  int order;
};

struct levelup_struct {
  int level;
  int move;
};

struct pokemon_species_struct {
  int id;
  char identifier[30];
  int generation_id;
  int evolves_from_species_id;
  int evolution_chain_id;
  int color_id;
  int shape_id;
  int habitat_id;
  int gender_rate;
  int capture_rate;
  int base_happiness;
  int is_baby;
  int hatch_counter;
  int has_gender_differences;
  int growth_rate_id;
  int forms_switchable;
  int is_legendary;
  int is_mythical;
  int order;
  int conquest_order;
};

struct experience_struct {
  int growth_rate_id;
  int level;
  int experience;
};

struct pokemon_stats__struct {
  int pokemon_id;
  int stat_id;
  int base_stat;
  int effort;
};

struct stats__struct {
  int id;
  int damage_class_id;
  char identifier[30];
  int is_battle_only;
  int game_index;
};

struct pokemon_types__struct {
  int pokemon_id;
  int type_id;
  int slot;
};

struct type_name_struct {
    int type_id;
    int local_language_id;
    char name[100]; // Adjust based on expected maximum length
};



char *next_token(char *start, char delim);

void parse_file(bool print, const char *filename) ;

#endif
