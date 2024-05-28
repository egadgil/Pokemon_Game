
Assignment-1.03 Path finding
So far, we had finalized printing our maps , now we are introducing npc characters, and printing maps for them using djikstra
*author credit- used Akhilesh Nevatia's code on github for queue building. 
Commands:
n: Move to the map immediately north of the current map and display it.
s: Move to the map immediately south of the current map and display it.
e: Move to the map immediately east of the current map and display it.
w: Move to the map immediately west of the current map and display it.
f x y: Fly to map (x, y) and display it.
q: Quit the game.

The levels of wild-spawned Pokémon will be a function of the distance from the center.
The frequency of Poké Marts and Pokémon Centers will decline as the player moves outwards from the center.
Map Generation and Gates:
Initially, all maps in the 401 × 401 array are NULL except for the center map at (200, 200).
Gates are used to connect maps, and when generating adjacent maps, gates should match up appropriately.
Map Storage and Revisiting:
No map should be generated until it is visited.
All visited maps should be saved so they can be revisited by going to the original coordinates.
The program displays the hiker and rival maps after calculating the shortest paths using djikstra's algorithm.
I also added the randomly placed player character.



To run the code, just type "make run" after going inside the folder




