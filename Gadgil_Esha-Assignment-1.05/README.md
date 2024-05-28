
Assignment-1.05 User Interface 

In this week's assignment we have implemented a user inteface where you can control the moves of the player character represented by @ using various commands.
Credit -Referenced Akhilesh Nevatia's code on Github for queue building 
Key(s) Action
7 or y Attempt to move PC one cell to the upper left.
8 or k Attempt to move PC one cell up.
9 or u Attempt to move PC one cell to the upper right.
6 or l Attempt to move PC one cell to the right.
3 or n Attempt to move PC one cell to the lower right.
2 or j Attempt to move PC one cell down.
1 or b Attempt to move PC one cell to the lower left.
4 or h Attempt to move PC one cell to the left.
> Attempt to enter a Pokémart or Pokémon Center. Works only if standing on a building. Leads to a user interface for the appropriate building. You may simply add a placeholder for this for now, which you exit with a <.
5 or space or . Rest for a turn. NPCs still move.
t Display a list of trainers on the map, with their symbol and position relative to the PC (e.g.: “t, 2 north and 14 west”).
up arrow When displaying trainer list, if entire list does not fit in screen and not currently at top of list, scroll list up.
down arrow When displaying trainer list, if entire list does not fit in screen and not currently at bottom of list, scroll list down.
escape When displaying trainer list, return to character control.
Q Quit the game. Your main game loop will become something like: while (!quit_game) { ... }


To run the code, just type "make " after going inside the folder and then run the executable by typing ./main2 in the terminal.




