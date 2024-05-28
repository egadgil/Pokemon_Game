#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <climits>
#include <iostream>
#include "parse_file.h"

pokemon_move_struct pokemon_moves[528239];
pokemon_struct pokemon[1093];
char *types[19];
move_struct moves[845];
pokemon_species_struct species[899];
experience_struct experience[601];
pokemon_stats__struct pokemon_stats[6553];
stats__struct stats[9];
pokemon_types__struct pokemon_types[1676];
type_name_struct typeNames[194];

static const char *int_to_str(int i) {
    static char s[20][12]; 
    static int next = 0; 

   
    snprintf(s[next], sizeof(s[next]), "%d", i);

    const char *result = s[next];


    next = (next + 1) % 20;

    return result;
}

void parse_file(bool print, const char *filename) 
{
 
  FILE *f;
char line[800];
int i;

struct stat buf;
char *path;
int path_len;
 i  = snprintf(NULL, 0, "%s/.poke327/pokedex/pokedex/data/csv/", getenv("HOME")) + 1;

  path = (char *) malloc(i);
  snprintf(path, i, "%s/.poke327/pokedex/pokedex/data/csv/", getenv("HOME"));


  if (stat(path, &buf)) {
    free(path);
    path = NULL;
  }

  if (!path && !stat("/share/cs327", &buf)) {
    path = strdup("/share/cs327/pokedex/pokedex/data/csv/");
  } else if (!path) {
    
  }

  
  path_len = strlen(path);

  path = (char *) realloc(path, path_len + strlen("pokemon.csv") + 1);
 
  
 
  strcpy(path + path_len, "pokemon.csv");
  
  f = fopen(path, "r");


  path = (char *) realloc(path, path_len + 1);

   fgets(line, sizeof(line), f);

   
for (int i = 1; i < 1093; i++) {
    if (fgets(line, 800, f) == NULL) break; // End loop if no line is read

    int field = 0; 
    char* tokenStart = line; 
    for (char* p = line; ; p++) {
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p; 
            *p = '\0'; 

          
            switch (field) {
                case 0: pokemon[i].id = atoi(tokenStart); break;
                case 1: strncpy(pokemon[i].identifier, tokenStart, 30); pokemon[i].identifier[29] = '\0'; break;
                case 2: pokemon[i].species_id = atoi(tokenStart); break;
                case 3: pokemon[i].height = atoi(tokenStart); break;
                case 4: pokemon[i].weight = atoi(tokenStart); break;
                case 5: pokemon[i].base_experience = atoi(tokenStart); break;
                case 6: pokemon[i].order = atoi(tokenStart); break;
                case 7: pokemon[i].is_default = atoi(tokenStart); break;
               
            }

            if (prevChar == '\n' || prevChar == '\0') break; // End of line

            field++; // Move to the next field for the next token
            tokenStart = p + 1; // Set the start of the next token
        }
    }
}
   
    // Close the file after processing
    
if (print && strcmp(filename, "pokemon.csv") == 0) {
     for (i = 1; i < 1093; i++) {
          std::cout << int_to_str(pokemon[i].id) << ","
              << pokemon[i].identifier << ","
              << int_to_str(pokemon[i].species_id) << ","
              << int_to_str(pokemon[i].height) << ","
              << int_to_str(pokemon[i].weight) << ","
              << int_to_str(pokemon[i].base_experience) << ","
              << int_to_str(pokemon[i].order) << ","
              << int_to_str(pokemon[i].is_default) << std::endl;
    }}
     fclose(f);   
    


  path = (char *) realloc(path, path_len + strlen("moves.csv") + 1);
  strcpy(path + path_len, "moves.csv");
  
  f = fopen(path, "r");

  //No null byte copied here, so path is not technically a string anymore.
  path = (char *) realloc(path, path_len + 1);

 fgets(line, sizeof(line), f);
  
  for (int i = 1; i <= 845; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break; // End loop if no line is read

    int field = 0; // To track which field we're currently populating
    char* tokenStart = line; // Start of the current token
    for (char* p = line; ; p++) {
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p; // Save the current delimiter
            *p = '\0'; // Temporarily null-terminate the current token

            switch (field) {
                case 0: moves[i].id = atoi(tokenStart); break;
                case 1: strncpy(moves[i].identifier, tokenStart, sizeof(moves[i].identifier) - 1); 
                        moves[i].identifier[sizeof(moves[i].identifier) - 1] = '\0'; // Ensure null-termination
                        break;
                case 2: moves[i].generation_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 3: moves[i].type_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 4: moves[i].power = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 5: moves[i].pp = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 6: moves[i].accuracy = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 7: moves[i].priority = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 8: moves[i].target_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 9: moves[i].damage_class_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 10: moves[i].effect_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 11: moves[i].effect_chance = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 12: moves[i].contest_type_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 13: moves[i].contest_effect_id = *tokenStart ? atoi(tokenStart) : INT_MAX; break;
                case 14: moves[i].super_contest_effect_id = (*tokenStart != '\n' && *tokenStart) ? atoi(tokenStart) : INT_MAX; break;
                // Handle additional fields as needed
            }

            if (prevChar == '\n' || prevChar == '\0') break; // End of line

            field++; // Move to the next field for the next token
            tokenStart = p + 1; // Set the start of the next token
        }
    }
}

fclose(f);

  
  if (print && strcmp(filename, "moves.csv") == 0) {

    for (i = 1; i < 845; i++) {
        std::cout << int_to_str(moves[i].id) << "," << moves[i].identifier << "," << int_to_str(moves[i].generation_id) << ","
                  << int_to_str(moves[i].type_id) << "," << int_to_str(moves[i].power) << "," << int_to_str(moves[i].pp) << ","
                  << int_to_str(moves[i].accuracy) << "," << int_to_str(moves[i].priority) << ","
                  << int_to_str(moves[i].target_id) << "," << int_to_str(moves[i].damage_class_id) << ","
                  << int_to_str(moves[i].effect_id) << "," << int_to_str(moves[i].effect_chance) << ","
                  << int_to_str(moves[i].contest_type_id) << "," << int_to_str(moves[i].contest_effect_id) << ","
                  << int_to_str(moves[i].super_contest_effect_id) << std::endl;
    }
  }

 path = (char *) realloc(path, path_len + strlen("pokemon_moves.csv") + 1);
strcpy(path + path_len, "pokemon_moves.csv");

f = fopen(path, "r");
if (!f) {
    // Handle error
    std::cerr << "Failed to open file: " << path << std::endl;
    exit(EXIT_FAILURE);
}

// Ensure path is null-terminated after modification
path = (char *) realloc(path, path_len + 1);

// Skip header line
fgets(line, sizeof(line), f);

// Process each line of the file
for (int i = 1; i < 528239; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break; 

    int field = 0; 
    char* tokenStart = line; 
    for (char* p = line; ; p++) {
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p; 
            *p = '\0'; 

            switch (field) {
                case 0: pokemon_moves[i].pokemon_id = atoi(tokenStart); break;
                case 1: pokemon_moves[i].version_group_id = atoi(tokenStart); break;
                case 2: pokemon_moves[i].move_id = atoi(tokenStart); break;
                case 3: pokemon_moves[i].pokemon_move_method_id = atoi(tokenStart); break;
                case 4: pokemon_moves[i].level = atoi(tokenStart); break;
                case 5: pokemon_moves[i].order = atoi(tokenStart); break;
                
            }

            if (prevChar == '\n' || prevChar == '\0') break; 

            field++; 
            tokenStart = p + 1; 
        }
    }
}

if (print && strcmp(filename, "pokemon_moves.csv") == 0) {
    for (int i = 1; i < 528239; i++) {
        std::cout << pokemon_moves[i].pokemon_id << "," 
                  << pokemon_moves[i].version_group_id << ","
                  << pokemon_moves[i].move_id << "," 
                  << pokemon_moves[i].pokemon_move_method_id << ","
                  << pokemon_moves[i].level << "," 
                  << pokemon_moves[i].order << std::endl;
    }
}

fclose(f);
path = (char *) realloc(path, path_len + strlen("pokemon_species.csv") + 1);
strcpy(path + path_len, "pokemon_species.csv");
  
f = fopen(path, "r");
if (!f) {
    std::cerr << "Failed to open file: " << path << std::endl;
    exit(EXIT_FAILURE);
}


path = (char *) realloc(path, path_len + 1);


fgets(line, sizeof(line), f);
  

for (int i = 1; i < 899; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break;

    int field = 0; 
    char* tokenStart = line; 
    for (char* p = line; ; p++) {
       
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p;
            *p = '\0'; 
            
            switch (field) {
                case 0: species[i].id = atoi(tokenStart); break;
                case 1: strncpy(species[i].identifier, tokenStart, sizeof(species[i].identifier) - 1);
                        species[i].identifier[sizeof(species[i].identifier) - 1] = '\0'; break;
                case 2: species[i].generation_id = atoi(tokenStart); break;
                case 3: species[i].evolves_from_species_id = atoi(tokenStart); break;
                case 4: species[i].evolution_chain_id = atoi(tokenStart); break;
                case 5: species[i].color_id = atoi(tokenStart); break;
                case 6: species[i].shape_id = atoi(tokenStart); break;
                case 7: species[i].habitat_id = atoi(tokenStart); break;
                case 8: species[i].gender_rate = atoi(tokenStart); break;
                case 9: species[i].capture_rate = atoi(tokenStart); break;
                case 10: species[i].base_happiness = atoi(tokenStart); break;
                case 11: species[i].is_baby = atoi(tokenStart); break;
                case 12: species[i].hatch_counter = atoi(tokenStart); break;
                case 13: species[i].has_gender_differences = atoi(tokenStart); break;
                case 14: species[i].growth_rate_id = atoi(tokenStart); break;
                case 15: species[i].forms_switchable = atoi(tokenStart); break;
                case 16: species[i].is_legendary = atoi(tokenStart); break;
                case 17: species[i].is_mythical = atoi(tokenStart); break;
                case 18: species[i].order = atoi(tokenStart); break;
                case 19: species[i].conquest_order = atoi(tokenStart); break;
               
            }

            if (prevChar == '\n' || prevChar == '\0') break; 

            field++; 
            tokenStart = p + 1; 
        }
    }
}

if (print && strcmp(filename, "pokemon_species.csv") == 0) {
    for (int i = 1; i < 899; i++) {
     
         std::cout << species[i].id << ","
                  << species[i].identifier << ","
                  << species[i].generation_id << ","
                  << species[i].evolves_from_species_id << ","
                  << species[i].evolution_chain_id << ","
                  << species[i].color_id << ","
                  << species[i].shape_id << ","
                  << species[i].habitat_id << ","
                  << species[i].gender_rate << ","
                  << species[i].capture_rate << ","
                  << species[i].base_happiness << ","
                  << species[i].is_baby << ","
                  << species[i].hatch_counter << ","
                  << species[i].has_gender_differences << ","
                  << species[i].growth_rate_id << ","
                  << species[i].forms_switchable << ","
                  << species[i].is_legendary << ","
                  << species[i].is_mythical << ","
                  << species[i].order << ","
                  << species[i].conquest_order
                  << std::endl;
    }
}

fclose(f);
  
path = (char *) realloc(path, path_len + strlen("experience.csv") + 1);
if (!path) {
    std::cerr << "Memory allocation failed." << std::endl;
    exit(EXIT_FAILURE);
}
strcpy(path + path_len, "experience.csv");


f = fopen(path, "r");
if (!f) {
    std::cerr << "Failed to open file: " << path << std::endl;
    exit(EXIT_FAILURE);
}


path = (char *) realloc(path, path_len + 1);

// Skip header line
fgets(line, sizeof(line), f);


for (int i = 1; i <= 601; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break; 

    int field = 0; 
    char* tokenStart = line; 
    for (char* p = line; ; p++) {
   
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p;
            *p = '\0'; 

           
            switch (field) {
                case 0: experience[i].growth_rate_id = atoi(tokenStart); break;
                case 1: experience[i].level = atoi(tokenStart); break;
                case 2: experience[i].experience = atoi(tokenStart); break;
            
            }

            if (prevChar == '\n' || prevChar == '\0') break; 

            field++; 
            tokenStart = p + 1; 
        }
    }
}
fclose(f);


if (print && strcmp(filename, "type_names.csv") == 0) {
   
    std::cout << "Experience data (displaying in console instead of overwriting 'experience.csv'):" << std::endl;
    for (int i = 1; i <= 601; i++) {
        std::cout << experience[i].growth_rate_id << ","
                  << experience[i].level << ","
                  << experience[i].experience << std::endl;
    }
   
}



path = (char *) realloc(path, path_len + strlen("type_names.csv") + 1);
    if (!path) {
        std::cerr << "Memory allocation failed." << std::endl;
        exit(EXIT_FAILURE);
    }
   strcpy(path + path_len, "type_names.csv");


    // Open the file
    f = fopen(path, "r");
    if (!f) {
        std::cerr << "Failed to open file: " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    
    fgets(line, sizeof(line), f);

    int index = 0;
    while (fgets(line, sizeof(line), f) && index < 2000) {
        int field = 0;
        char* tokenStart = line;
        for (char* p = line;; p++) {
            if (*p == ',' || *p == '\n' || *p == '\0') {
                char prevChar = *p;
                *p = '\0'; 
                
                switch (field) {
                    case 0: typeNames[index].type_id = atoi(tokenStart); break;
                    case 1: typeNames[index].local_language_id = atoi(tokenStart); break;
                    case 2: strncpy(typeNames[index].name, tokenStart, sizeof(typeNames[index].name)-1); typeNames[index].name[sizeof(typeNames[index].name)-1] = '\0'; break;
                }
                
                if (prevChar == '\n' || prevChar == '\0') break; 
                
                field++; 
                tokenStart = p + 1; 
            }
        }
        index++;
    }
    fclose(f);
    
       if (print && strcmp(filename, "type_names.csv")==0 ) {
    for (int i = 0; i < index; i++) {
        std::cout << typeNames[i].type_id << ","
                  << typeNames[i].local_language_id << ","
                  << '"' << typeNames[i].name << '"' << std::endl;
    }

   
}

path = (char *) realloc(path, path_len + strlen("pokemon_stats.csv") + 1);
if (!path) {
    std::cerr << "Memory allocation failed." << std::endl;
    exit(EXIT_FAILURE);
}
strcpy(path + path_len, "pokemon_stats.csv");

f = fopen(path, "r");
if (!f) {
    std::cerr << "Failed to open file: " << path << std::endl;
    exit(EXIT_FAILURE);
}


path = (char *) realloc(path, path_len + 1);


fgets(line, sizeof(line), f);

// Process each record
for (int i = 1; i < 6553; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break; 

    int field = 0; 
    char* tokenStart = line;
    for (char* p = line; ; p++) {

        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p;
            *p = '\0'; 

      
            switch (field) {
                case 0: pokemon_stats[i].pokemon_id = atoi(tokenStart); break;
                case 1: pokemon_stats[i].stat_id = atoi(tokenStart); break;
                case 2: pokemon_stats[i].base_stat = atoi(tokenStart); break;
                case 3: pokemon_stats[i].effort = atoi(tokenStart); break;
              
            }

            if (prevChar == '\n' || prevChar == '\0') break;

            field++; 
            tokenStart = p + 1; 
        }
    }
}
fclose(f);


if (print && strcmp(filename, "pokemon_stats.csv") == 0) {

    std::cout << "Printing pokemon_stats to console instead of overwriting the file." << std::endl;
    for (int i = 1; i < 6553; i++) {
        std::cout << pokemon_stats[i].pokemon_id << ","
                  << pokemon_stats[i].stat_id << ","
                  << pokemon_stats[i].base_stat << ","
                  << pokemon_stats[i].effort << std::endl;
    }
   
}


path = (char *) realloc(path, path_len + strlen("stats.csv") + 1);
if (!path) {
    std::cerr << "Memory allocation failed." << std::endl;
    exit(EXIT_FAILURE);
}
strcpy(path + path_len, "stats.csv");


f = fopen(path, "r");
if (!f) {
    std::cerr << "Failed to open file: " << path << std::endl;
    exit(EXIT_FAILURE);
}


path = (char *) realloc(path, path_len + 1);


fgets(line, sizeof(line), f);


for (int i = 1; i <= 9; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break; // Stop if no line is read

    int field = 0;
    char* tokenStart = line;
    for (char* p = line; ; p++) {
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p;
            *p = '\0';

            switch (field) {
                case 0: stats[i].id = atoi(tokenStart); break;
                case 1: stats[i].damage_class_id = atoi(tokenStart); break;
                case 2: strncpy(stats[i].identifier, tokenStart, sizeof(stats[i].identifier)); stats[i].identifier[sizeof(stats[i].identifier) - 1] = '\0'; break;
                case 3: stats[i].is_battle_only = atoi(tokenStart); break;
                case 4: stats[i].game_index = atoi(tokenStart); break;
            }

            if (prevChar == '\n' || prevChar == '\0') break;

            field++;
            tokenStart = p + 1;
        }
    }
}
fclose(f);

  
   if (print && strcmp(filename, "stats.csv")==0 ) {
    f = fopen("stats.csv", "w");
    for (i = 1; i < 9; i++) {
       std::cout << int_to_str(stats[i].id) << ","
              << int_to_str(stats[i].damage_class_id) << ","
              << stats[i].identifier << ","
              << int_to_str(stats[i].is_battle_only) << ","
              << int_to_str(stats[i].game_index) << std::endl;
    }
    fclose(f);
  }
  

path = (char *) realloc(path, path_len + strlen("pokemon_types.csv") + 1);
if (!path) {
    std::cerr << "Memory allocation failed for pokemon_types.csv" << std::endl;
    exit(EXIT_FAILURE);
}
strcpy(path + path_len, "pokemon_types.csv");


f = fopen(path, "r");
if (!f) {
    std::cerr << "Failed to open file: " << path << std::endl;
    exit(EXIT_FAILURE);
}


path = (char *) realloc(path, path_len + 1);


fgets(line, sizeof(line), f);


for (int i = 1; i < 1676; i++) {
    if (fgets(line, sizeof(line), f) == NULL) break;

    int field = 0;
    char* tokenStart = line;
    for (char* p = line; ; p++) {
        if (*p == ',' || *p == '\n' || *p == '\0') {
            char prevChar = *p;
            *p = '\0'; 

            switch (field) {
                case 0: pokemon_types[i].pokemon_id = atoi(tokenStart); break;
                case 1: pokemon_types[i].type_id = atoi(tokenStart); break;
                case 2: pokemon_types[i].slot = atoi(tokenStart); break;
            }

            if (prevChar == '\n' || prevChar == '\0') break;

            field++;
            tokenStart = p + 1;
        }
    }
}
fclose(f);

  
 if (print && strcmp(filename, "pokemon_types.csv")==0 ) {
    f = fopen("pokemon_types.csv", "w");
    for (i = 1; i < 1676; i++) {
       std::cout << int_to_str(pokemon_types[i].pokemon_id) << ","
              << int_to_str(pokemon_types[i].type_id) << ","
              << int_to_str(pokemon_types[i].slot) << std::endl;}
    fclose(f);
  }


}

