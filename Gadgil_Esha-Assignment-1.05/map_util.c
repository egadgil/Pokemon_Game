#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include "queue.h"
#include <unistd.h> // Include for usleep
#include <ncurses.h>
#include <termios.h>



typedef struct {
    int x;  
    int y; 
} PC;

typedef struct CurrMap
{
    int x;
    int y;
    void (*updateX)(struct CurrMap *, int);
    void (*updateY)(struct CurrMap *, int);
} CurrMap;
int numtrainers = 10;
#define SIGINT 2

void printLegend() {
    printf("~: Water\n");
    printf("^: Tree \n");
    printf("C: Pokémon Center\n");
    printf("M: Pokémon Mart\n");
    printf("Legend:\n");
    printf(".: Clearing\n");
    printf("#: Road\n");
    printf(": Tall Grass\n");
    
}
//makes sure we dont leave the 400x400 world



void updateX(CurrMap *currMap, int newX)
{
    if (newX < 0)
    {
        currMap->x = 0;
        printf("Cannot move further up\n");
    }
    else if (newX > 400)
    {
        currMap->x = 400;
        printf("Cannot move further down\n");
    }
    else
    {
        currMap->x = newX;
    }
}


void updateY(CurrMap *currMap, int newY)
{
    if (newY < 0)
    {
        currMap->y = 0;
        printf("Cannot move further west\n");
    }
    else if (newY > 400)
    {
        currMap->y = 400;
        printf("Cannot move further east\n");
    }
    else
    {
        currMap->y = newY;
    }
}
//sets weights according to assignment instructions
int getHikerWeight(char symbol, int row, int column) { 
    
    int weight = -1;

   
    if (symbol == '%' || symbol == '.' || symbol == '~') {
        weight = INT_MAX;  
    } else if (symbol == '#') {
        if(row==0 || row == 20 || column ==0 || column == 79) weight = INT_MAX;
        else weight = 10;
    } else if (symbol == 'M' || symbol == 'C') {
        weight = 50;
    } else if (symbol == ':') {
        weight = 15;
    } else if (symbol == '^') {
        weight = 10;
    } else {
        
        weight = INT_MAX;
    }

    return weight;
}

int getRivalWeight(char symbol, int row, int column){
    int weight = -1;
    if (symbol == '%' || symbol == '.' || symbol == '~') {
        weight = INT_MAX;  
    } else if (symbol == '#') {
        if(row==0 || row == 20 || column ==0 || column == 79) weight = INT_MAX;
        else weight = 10;
    } else if (symbol == 'M' || symbol == 'C') {
        weight = 50;
    } else if (symbol == ':') {
        weight = 20;
    } else if (symbol == '^') {
        weight = 10;
    } else {
       weight = INT_MAX;
    }

    return weight;

}
int getSwimmerWeight(char symbol, int row, int column) { 
    
    int weight = -1;

    // Determine weight based on the symbol
    if (symbol == '%' || symbol == '.' ) {
        weight = INT_MAX;  // Represents infinity
    } else if(symbol == '~'){
        weight = 7;  
     }else if (symbol == '#') {
         weight = INT_MAX;
    } else if (symbol == 'M' || symbol == 'C') {
        weight = INT_MAX;
    } else if (symbol == ':') {
        weight = INT_MAX;
    } else if (symbol == '^') {
        weight = INT_MAX;
    } else {
        // Handle any other symbols not listed (error condition)
        weight = INT_MAX;
    }

    return weight;
}

// OTHER CHARACTERS WEIGHT FUNCTION

int getOtherWeight(char symbol, int row, int column) { 
    
    int weight = -1;

    // Determine weight based on the symbol
    if (symbol == '%' || symbol == '.' ) {
        weight = INT_MAX;  // Represents infinity
    } else if(symbol == '~'){
        weight = INT_MAX;  
     }else if (symbol == '#') {
        if(row==0 || row == 20 || column ==0 || column == 79) weight = INT_MAX;
        else weight = 10;
    } else if (symbol == 'M' || symbol == 'C') {
        weight = 50;
    } else if (symbol == ':') {
        weight = 20;
    } else if (symbol == '^') {
        weight = 10;
    } else {
        // Handle any other symbols not listed (error condition)
        weight = INT_MAX;
    }

    return weight;
}
int game_running = 1;

// Signal handler for SIGINT (CTRL+C)
void sigint_handler(int signal) {
    printf("\nReceived SIGINT. Exiting...\n");
    game_running = 0;  // Set the flag to exit the game
}

int *printRmap(char map[21][80] ,int row , int col , int aroundr , int aroundc){

    
    int rival[21][80];
    PriorityQueue *pq = createPriorityQueue(20000); // MAX POSSIBLE STORAGE 
    
    
    int visited[21][80];
    
    for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            visited[i][j] = 0;
        }
    }

      for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            rival[i][j] = INT_MAX; // EVERYTHING AT INFINITE DISTANCE INITIALLY 
        }
    }

    insert(pq,row,col,0);

    rival[row][col] = 0; // distance of the source from itself is 0

    //To implement an 8 - directional search
    int aroundx[] = {0, 1, 0, -1, 1, -1, 1, -1};
    int aroundy[] = {1, 0, -1, 0, -1, 1, 1, -1};

    // I AM HERE 
    while(!isEmp(pq)){
        Element minElement = extractMin(pq);
        int x = minElement.row;
        int y = minElement.column;
        int weight = minElement.weight;
        if(visited[x][y] == 1 || x < 0 || y > 79 || x > 20 || y < 0) continue;
        
        visited[x][y] = 1;
        for(int i = 0 ; i < 8 ; i++){

           int newWeight = (weight + (getRivalWeight(map[x+aroundx[i]][y+aroundy[i]], x + aroundx[i], y + aroundy[i]))) ;
            
            if(visited[x+aroundx[i]][y+aroundy[i]]== 0 && rival[x+aroundx[i]][y+aroundy[i]] > newWeight) {
                rival[x+aroundx[i]][y+aroundy[i]] = newWeight<0 ? INT_MAX : newWeight ;
                insert(pq,x+aroundx[i],y+aroundy[i],rival[x+aroundx[i]][y+aroundy[i]]);
            }
            
        }
    }

   

    
    int *val = (int *)malloc(sizeof(int)*2);
     if (val == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
}
    int min = INT_MAX;
    for(int i =0 ; i< 8 ; i++){
         if((aroundr + aroundx[i])>=0 && (aroundr + aroundx[i])<21 && (aroundc + aroundy[i])>=0 && (aroundc + aroundy[i])<80){
            if(rival[aroundr+aroundx[i]][aroundc+aroundy[i]]!= INT_MAX && (min > rival[aroundr+aroundx[i]][aroundc+aroundy[i]]) && (map[aroundr+aroundx[i]][aroundc+aroundy[i]]!='@')){
                min = rival[aroundr+aroundx[i]][aroundc+aroundy[i]];
                val[0] = aroundr+aroundx[i];
                val[1] = aroundc+aroundy[i];
            }
         }
    }
    

    return val; // return value to which we need to go as array pointer 

}

int *printHmap(char map[21][80] ,int row , int col , int aroundr , int aroundc){


    //printf("HIKER MAP \n");
    // printing the hikermap using dijkstra's algorithm
    int hiker[21][80];
    PriorityQueue *pq = createPriorityQueue(20000); // MAX POSSIBLE STORAGE 
    
    
    int visited[21][80];
    // initializing visited array to 0
    for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            visited[i][j] = 0;
        }
    }

      for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            hiker[i][j] = INT_MAX; // EVERYTHING AT INFINITE DISTANCE INITIALLY 
        }
    }

    insert(pq,row,col,0);

    hiker[row][col] = 0; // distance of the source from itself is 0

    //To implement an 8 - directional search
    int aroundx[] = {0, 1, 0, -1, 1, -1, 1, -1};
    int aroundy[] = {1, 0, -1, 0, -1, 1, 1, -1};

    // I AM HERE 
    while(!isEmp(pq)){
        Element minElement = extractMin(pq);
        int x = minElement.row;
        int y = minElement.column;
        int weight = minElement.weight;
        if(visited[x][y] == 1 || x < 0 || y > 79 || x > 20 || y < 0) continue;
        
        visited[x][y] = 1;
        for(int i = 0 ; i < 8 ; i++){
           
            int newWeight = (weight + (getHikerWeight(map[x+aroundx[i]][y+aroundy[i]], x + aroundx[i], y + aroundy[i]))) ;
            
            if(visited[x+aroundx[i]][y+aroundy[i]]== 0 && hiker[x+aroundx[i]][y+aroundy[i]] > newWeight) {
                //hiker[x+aroundx[i]][y+aroundy[i]] =((weight + getHikerWeight(map[x+aroundx[i]][y+aroundy[i]] ,x + aroundx[i], y + aroundy[i]))%100);
                //printf("%c",map[x+aroundx[i]][y+aroundy[i]]);
                hiker[x+aroundx[i]][y+aroundy[i]] = (newWeight < 0) ? INT_MAX : newWeight;
                insert(pq,x+aroundx[i],y+aroundy[i],hiker[x+aroundx[i]][y+aroundy[i]]);
            }
            
        }
    }

   

   
    int *val = (int *)malloc(sizeof(int)*2);
     if (val == NULL) {
    printf("Memory allocation failed.\n");
    exit(EXIT_FAILURE);
}
    int min = INT_MAX;
    for(int i =0 ; i< 8 ; i++){
        if((aroundr + aroundx[i])>=0 && (aroundr + aroundx[i])<21 && (aroundc + aroundy[i])>=0 && (aroundc + aroundy[i])<80){
            if(hiker[aroundr+aroundx[i]][aroundc+aroundy[i]]!= INT_MAX && (min > hiker[aroundr+aroundx[i]][aroundc+aroundy[i]]) && (map[aroundr+aroundx[i]][aroundc+aroundy[i]]!='@')){
                min = hiker[aroundr+aroundx[i]][aroundc+aroundy[i]];
                val[0] = aroundr+aroundx[i];
                val[1] = aroundc+aroundy[i];
            }
        }
    }




    return val; // return value to which we need to go as array pointer 

 }





// Function to create an empty queue
 
 
char **printmap(char gate, int index , int mapx , int mapy,int numtrainers )
{
    PC playerCharacter;
    int rows = 21;
    int cols = 80;
    char map[rows][cols]; 

    time_t t;

    srand((unsigned)time(&t)); 
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i == 0 || i == (rows - 1) || j == 0 || j == (cols - 1))
            {
                map[i][j] = '%'; 
            }
            else
            {
                map[i][j] = '.'; 
                continue;
            }
        }
    }

      if (gate == 'w')
    {

        int dx[] = {-1, 1};
        int dy[] = {-1, -1};

        int Hstartx = index;
        int rowx = Hstartx;
        int rowy = 79;
        map[rowx][rowy] = '#';
        rowy--; 
        map[rowx][rowy] = '#';
        
 
        while (rowy != 0)
        {
             
            int dirn = (rand() % 2); 
             if ((map[rowx + dx[dirn]][rowy + dy[dirn]]) != '%')
            {
                rowx += dx[dirn];
                rowy += dy[dirn];
                map[rowx][rowy] = '#';}
            else{
                rowy--;
                map[rowx][rowy] = '#';
            }
            
            
        }
        if (rowy == 0)
        {
            map[rowx][rowy] = '#'; // exitgate for horz path
        }
    }
    else if (gate == 'e')
    {

        int dx[] = {-1,1};
        int dy[] = {1,1};

        int Hstartx = index;
        int rowx = Hstartx;
        int rowy = 0;
        map[rowx][rowy] = '#';
        rowy++; 
        map[rowx][rowy] = '#';
        

        while (rowy != 79)
        {
            int dirn = (rand() % 2); 
             if ((map[rowx + dx[dirn]][rowy + dy[dirn]]) != '%')
            {
                rowx += dx[dirn];
                rowy += dy[dirn];
                map[rowx][rowy] = '#';}
            else{
                rowy++;
                map[rowx][rowy] = '#';

            }
           
            
            
               
            
        }
        if (rowy == 79)
        {
            map[rowx][rowy] = '#'; 
        }
    }
    // prints center map
    else
    {

        int dx[] = {0, 1,-1};
        int dy[] = {1, 0,1};

        int Hstartx = rand() % 20;
        int rowx = Hstartx;
        int rowy = 0;
        map[rowx][rowy] = '#';
        rowy++; 
        map[rowx][rowy] = '#';
         
        
        int var = 0;
        while (rowy != 79)
        {
            int dirn = (rand() % 3); 
            if ((map[rowx + dx[dirn]][rowy + dy[dirn]]) != '%')
            {
                var++;
                rowx += dx[dirn];
                rowy += dy[dirn];
                map[rowx][rowy] = '#';
                
            }
            else
            {
                rowy++;
                map[rowx][rowy] = '#';
            }
        }
        if (rowy == 79)
        {
            map[rowx][rowy] = '#'; // exitgate for horz path
        }
    }

    if (gate == 'n')
    {

        int Hstarty = index;
        int coly = Hstarty;
        int colx = 0;
        map[colx][coly] = '#';
        colx++; // next line moving
        map[colx][coly] = '#';

        int cx[] = {0, 1, 0};
        int cy[] = {-1, 0, 1};

        while (colx != 20)
        {
            int dirn = (rand() % 3); // as we only want to go ahead in terms of cols
            if ((map[colx + cx[dirn]][coly + cy[dirn]]) != '%')
            {
                colx += cx[dirn];
                coly += cy[dirn];
                map[colx][coly] = '#';
            }
            else
            {
                colx++;
                map[colx][coly] = '#';
            }
        }
        if (colx == 20)
        {
            map[colx][coly] = '#'; 
        }
    }
    else if (gate == 's')
    {
        int Hstarty = index;
        int coly = Hstarty;
        int colx = 20; // last row go upi from there
        map[colx][coly] = '#';
        colx--; // next line moving
        map[colx][coly] = '#';

        int cx[] = {0, -1, 0};
        int cy[] = {-1, 0, 1};

        while (colx != 0)
        {
            int dirn = (rand() % 3); 
            if ((map[colx + cx[dirn]][coly + cy[dirn]]) != '%')
            {
                colx += cx[dirn];
                coly += cy[dirn];
                map[colx][coly] = '#';
            }
            else
            {
                colx--;
                map[colx][coly] = '#';
            }
        }
        if (colx == 0)
        {
            map[colx][coly] = '#'; 
        }
    }
    else
    {

        int Hstarty = 1 + rand() % 78;

        int coly = Hstarty;
        int colx = 0;
        map[colx][coly] = '#';
        colx++; // next line moving
        map[colx][coly] = '#';

        int cx[] = {0, 1, 0};
        int cy[] = {-1, 0, 1};

        while (colx != 20)
        {
            int dirn = (rand() % 3); 
            if ((map[colx + cx[dirn]][coly + cy[dirn]]) != '%')
            {
                colx += cx[dirn];
                coly += cy[dirn];
                map[colx][coly] = '#';
            }
            else
            {
                colx++;
                map[colx][coly] = '#';
            }
        }
        if (colx == 20)
        {
            map[colx][coly] = '#';
        }
    }

   


    int d = sqrt(pow(200 - mapx, 2) + pow(200 - mapy, 2));
    
    int prob = (((-45*d)/200) + 50);

   


    int num_pc= prob/10;
    for (int i=0; i<num_pc; i++){
        int ranpcy = rand() % 80;
        int ranpcx = rand() % 21;
        if (map[ranpcx][ranpcy]=='.' && map[ranpcx+1][ranpcy+1]=='.')
        {
            map[ranpcx][ranpcy]='C';
            map[ranpcx+1][ranpcy+1]='M';
        }
    }
    


   
    int aroundx[] = {0, 1, 0, -1, 1, -1, 1, -1};
    int aroundy[] = {1, 0, -1, 0, -1, 1, 1, -1};
    

    // FILLING MAP UP WITH DIFFERENT TERRAINS USING CUSTOM MADE QUEUE

    struct Queue *queue = createQueue();

    // FILLING UP TALL GRASS

    for (int i = 0; i < 3; i++)
    {
        int Ry = rand() % 80;
        int Rx = rand() % 21;

        while (map[Rx][Ry] != '.')
        {
            Ry = rand() % 80;
            Rx = rand() % 21;
        }
        struct CustomData data = {Rx, Ry, ':'};
        enqueue(queue, data);
    }

    // WATER IN THE QUEUE

    for (int i = 0; i < 2; i++)
    {
        int Ry = rand() % 80;
        int Rx = rand() % 21;

        while (map[Rx][Ry] != '.')
        {
            Ry = rand() % 80;
            Rx = rand() % 21;
        }
        struct CustomData data = {Rx, Ry, '~'};
        enqueue(queue, data);
    }

    // PUTTING SHORT GRASS IN THE QUEUE AT 2 LOCATIONS
    for (int i = 0; i < 3; i++)
    {
        int Ry = rand() % 80;
        int Rx = rand() % 21;

        while (map[Rx][Ry] != '.')
        {
            Ry = rand() % 80;
            Rx = rand() % 21;
        }
        struct CustomData data = {Rx, Ry, '^'};
        enqueue(queue, data);
    }

      while (!(isEmpty(queue)))
    {

        struct CustomData node = dequeue(queue);
        int x = node.x;
        int y = node.y;
        int terrain = node.value;
        for (int i = 0; i < 8; i++)
        {
            if (map[x + aroundx[i]][y + aroundy[i]] == '.')
            {
                map[x + aroundx[i]][y + aroundy[i]] = terrain;
                struct CustomData InputData = {x + aroundx[i], y + aroundy[i], terrain};
                enqueue(queue, InputData);
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {

        int row = rand() % 15 + 3;
        int col = rand() % 70 + 5;

        if (map[row][col] != '#' && map[row][col] != 'M' && map[row][col] != 'C')
        {
            map[row][col] = '%';
        }
    }



    
        if (mapx == 0)
        {
            for (int j = 0; j < 80; j++)
            {
                if (map[0][j] == '#')
                {
                    map[0][j] = '%';
                }
            }
        }
        else if (mapx == 400)
        {

            for (int j = 0; j < 80; j++)
            {
                if (map[20][j] == '#')
                {
                    map[20][j] = '%';
                }
            }
        }

        if (mapy == 0)
        {
            for (int i = 0; i < 21; i++)
            {
                if (map[i][0] == '#')
                {
                    map[i][0] = '%';
                }
            }
        }
        else if (mapy== 400)
        {
            for (int i = 0; i < 21; i++)
            {
                if (map[i][79] == '#')
                {
                    map[i][79] = '%';
                }
            }
        }


    

    int totalTrainers = 0;
    int count = 0;

    MyPriorityQueue *pq = createMyPriorityQueue(20);


    while(totalTrainers<numtrainers){
        count =count %6;
        // where to add the specific NPC
        if(count==0){

            int i = 3+ rand()%17;
            int j = 3+ rand()%76;
            while(!(map[i][j]!='@' && map[i][j]!='h' && map[i][j]!='r' && map[i][j]!='p' && map[i][j]!='w' && map[i][j]!='s' && map[i][j]!='e' && map[i][j]!='#' && getHikerWeight(map[i][j],i,j)!= INT_MAX)){
                i = 3+ rand()%17;
                j = 3+ rand()%76;
            }
            myEnqueue(pq,i,j,0,map[i][j]); // inserts where the character is and we have initialized the character to be at 0 distance from itself
            map[i][j] = 'h';
            
        }
        else if(count == 1){
            int i = 3+ rand()%17;
            int j = 3+ rand()%76;
            while(!(map[i][j]!='@' && map[i][j]!='h' && map[i][j]!='r' && map[i][j]!='p' && map[i][j]!='w' && map[i][j]!='s' && map[i][j]!='e' && map[i][j]!='#' && getRivalWeight(map[i][j],i,j)!= INT_MAX)){
                i = 3+ rand()%17;
                j = 3+ rand()%76;
            }
            myEnqueue(pq,i,j,0,map[i][j]); // inserts where the character is and we have initialized the character to be at 0 distance from itself
            map[i][j] = 'r';
        }
        else if(count ==2){
            // need to make cost method for each of these NPC types
            int i = 3+ rand()%17;
            int j = 3+ rand()%76;
            while(!(map[i][j]!='@' && map[i][j]!='h' && map[i][j]!='r' && map[i][j]!='p' && map[i][j]!='w' && map[i][j]!='s' && map[i][j]!='e' && map[i][j]!='#' && getOtherWeight(map[i][j],i,j)!= INT_MAX)){
                i = 3+ rand()%17;
                j = 3+ rand()%76;
            }
            myEnqueue(pq,i,j,0,map[i][j]); // inserts where the character is and we have initialized the character to be at 0 distance from itself
            map[i][j] = 'p';



        }else if(count ==3){

                // need to make cost method for each of these NPC types
            int i = 3+ rand()%17;
            int j = 3+ rand()%76;
            while(!(map[i][j]!='@' && map[i][j]!='h' && map[i][j]!='r' && map[i][j]!='p' && map[i][j]!='w' && map[i][j]!='s' && map[i][j]!='e' && map[i][j]!='#' && getOtherWeight(map[i][j],i,j)!= INT_MAX)){
                i = 3+ rand()%17;
                j = 3+ rand()%76;
            }
            myEnqueue(pq,i,j,0,map[i][j]); // inserts where the character is and we have initialized the character to be at 0 distance from itself
            map[i][j] = 'w';

        }else if(count ==4){

                // need to make cost method for each of these NPC types
            int i = 3+ rand()%17;
            int j = 3+ rand()%76;
            while(!(map[i][j]!='@' && map[i][j]!='h' && map[i][j]!='r' && map[i][j]!='p' && map[i][j]!='w' && map[i][j]!='s' && map[i][j]!='e' && map[i][j]!='#' && getOtherWeight(map[i][j],i,j)!= INT_MAX)){
                i = 3+ rand()%17;
                j = 3+ rand()%76;
            }
            myEnqueue(pq,i,j,0,map[i][j]); // inserts where the character is and we have initialized the character to be at 0 distance from itself

            map[i][j] = 's';

        }else if(count ==5 ){

                // need to make cost method for each of these NPC types
            int i = 3+ rand()%17;
            int j = 3+ rand()%76;
            while(!(map[i][j]!='@' && map[i][j]!='h' && map[i][j]!='r' && map[i][j]!='p' && map[i][j]!='w' && map[i][j]!='s' && map[i][j]!='e' && map[i][j]!='#' && (getOtherWeight(map[i][j],i,j)!= INT_MAX))){
                i = 3+ rand()%17;
                j = 3+ rand()%76;
            }
            myEnqueue(pq,i,j,0,map[i][j]); // inserts where the character is and we have initialized the character to be at 0 distance from itself
            map[i][j] = 'e';

        }



        totalTrainers++;
        count++;
    }
  
 signal(SIGINT, sigint_handler);
    int currentTime = 1;
    int pflag = 0;
    int wanderx[] = {-1,0,1,0};
    int wandery[] = {0,1,0,-1};
    int choosewander = rand()%4;
    int wx = wanderx[choosewander];
    int wy = wandery[choosewander];
    int expx[] = {-1,0,1,0};
    int expy[] = {0,1,0,-1};
    int chooseexp = rand()%4;
    int ex = expx[chooseexp];
    int ey = expy[chooseexp];
  

    // Game loop
    
 playerCharacter.x = 15;
    playerCharacter.y = 15;
             // Don't echo keystrokes
 initscr(); // Initialize the ncurses screen
cbreak(); // Line buffering disabled
noecho(); // Don't echo any keypresses
keypad(stdscr, TRUE); // Enable function and arrow keys
start_color(); // Enable color
 
while (!(isMyEmpty(pq)) && game_running) {
  
 char temp = map[playerCharacter.x][playerCharacter.y]; 
map[playerCharacter.x][playerCharacter.y]='@'; 



  

    
      

    
    
    

   
            printf("\n");
            // END TEST CODE
            PQElement minElement = myDequeue(pq);
            int x = minElement.x;
            int y = minElement.y;
            char oldc = minElement.c;
            int weight = minElement.weight;
            if(map[x][y]=='h'){
              
       
            int *val = printHmap(map,playerCharacter.x,playerCharacter.y,x,y);
            int newx = val[0];
            int newy = val[1];
            if(map[newx][newy]=='@') {
                weight+=currentTime;
                myEnqueue(pq,x,y,weight,map[x][y]);
                free(val);
                continue;
            }
            map[x][y] = oldc;
            weight = currentTime + getHikerWeight(map[newx][newy],newx,newy);
            myEnqueue(pq,newx,newy,weight,map[newx][newy]);
            map[newx][newy] = 'h';
            free(val); 
            

            }else if(map[x][y]=='r'){

                
       
            int *val = printRmap(map,playerCharacter.x,playerCharacter.y,x,y);
            int newx = val[0];
            int newy = val[1];
            if(map[newx][newy]=='@') {
                weight+=currentTime;
                myEnqueue(pq,x,y,weight,map[x][y]);
                free(val);
                continue;
            }
            map[x][y] = oldc;
            weight = currentTime + getRivalWeight(map[newx][newy],newx,newy);
            myEnqueue(pq,newx,newy,weight,map[newx][newy]);
            map[newx][newy] = 'r';
            free(val); 

        }else if(map[x][y]=='p'){
            
            if(pflag==0){
                if(getOtherWeight(map[x][y+1],x,y+1)!=INT_MAX){
                    map[x][y] = oldc;
                    weight = currentTime + getOtherWeight(map[x][y+1],x,y+1);
                    myEnqueue(pq,x,y+1,weight,map[x][y+1]);
                    map[x][y+1] = 'p';
                }else{
                    pflag = 1;
                    map[x][y] = oldc;
                    weight = currentTime + getOtherWeight(map[x][y-1],x,y-1);
                    myEnqueue(pq,x,y-1,weight,map[x][y-1]);
                    map[x][y-1] = 'p';
                }
            }else{
                if(getOtherWeight(map[x][y-1],x,y-1)!=INT_MAX){
                    map[x][y] = oldc;
                    weight = currentTime + getOtherWeight(map[x][y-1],x,y-1);
                    myEnqueue(pq,x,y-1,weight,map[x][y-1]);
                    map[x][y-1] = 'p';
                }else{
                    pflag = 0;
                    map[x][y] = oldc;
                    weight = currentTime + getOtherWeight(map[x][y+1],x,y+1);
                    myEnqueue(pq,x,y+1,weight,map[x][y+1]);
                    map[x][y+1] = 'p';
                }

            }
        }else if(map[x][y]=='w'){

            map[x][y] = oldc;
            if(map[x+wx][y+wy]==oldc){
                weight = currentTime + getOtherWeight(map[x+wx][y+wy],x+wx,y+wy);
                myEnqueue(pq,x+wx,y+wy,weight,map[x+wx][y+wy]);
                map[x+wx][y+wy] = 'w';
            }else{
                while(map[x+wx][y+wy]!=oldc){
                    choosewander = rand()%4;
                    wx = wanderx[choosewander];
                    wy = wandery[choosewander];
                }
                weight = currentTime + getOtherWeight(map[x+wx][y+wy],x+wx,y+wy);
                myEnqueue(pq,x+wx,y+wy,weight,map[x+wx][y+wy]);
                map[x+wx][y+wy] = 'w';

            }

        }else if(map[x][y]=='s'){
            // Wait for action to come to them and stay stationary
            // obviously we change the weight based on current time being updated 
            weight = currentTime + getOtherWeight(oldc,x,y);
            myEnqueue(pq,x,y,weight,oldc);
        }else if(map[x][y]=='e'){

            map[x][y] = oldc;
            if(getOtherWeight(map[x+ex][y+ey],x+ex,y+ey)!=INT_MAX){
                weight = currentTime + getOtherWeight(map[x+ex][y+ey],x+ex,y+ey);
                myEnqueue(pq,x+ex,y+ey,weight,map[x+ex][y+ey]);
                map[x+ex][y+ey] = 'e';
            }else{
                while(getOtherWeight(map[x+ex][y+ey],x+ex,y+ey)==INT_MAX){
                    chooseexp = rand()%4;
                    ex = expx[chooseexp];
                    ey = expy[chooseexp];
                }
                weight = currentTime + getOtherWeight(map[x+ex][y+ey],x+ex,y+ey);
                myEnqueue(pq,x+ex,y+ey,weight,map[x+ex][y+ey]);
                map[x+ex][y+ey] = 'e';

            }

        }
        currentTime++;   

 


    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color(); // Start color functionality

    // Initialize color pairs (foreground, background)
    init_pair(1, COLOR_CYAN, COLOR_BLACK); // Water
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Tree
    init_pair(3, COLOR_RED, COLOR_BLACK); // Pokémon Center
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Pokémon Mart
    init_pair(5, COLOR_WHITE, COLOR_BLACK); // Clearing/Road
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // Tall Grass
    // Add more color pairs as needed
    
    // PRINTING BOARD FORMULATED
 for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch(map[i][j]) {
                case '~': // Water
                    attron(COLOR_PAIR(1));
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(1));
                    break;
                case '^': // Tree
                    attron(COLOR_PAIR(2));
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(2));
                    break;
                case 'C': // Pokémon Center
                    attron(COLOR_PAIR(3));
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(3));
                    break;
                case 'M': // Pokémon Mart
                    attron(COLOR_PAIR(4));
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(4));
                    break;
                case '.': // Clearing
                case '#': // Road
                    attron(COLOR_PAIR(5));
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(5));
                    break;
                case ':': // Tall Grass
                    attron(COLOR_PAIR(6));
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(6));
                    break;
                default:
                    printw("%c", map[i][j]); // No specific color
            }
        }
        printw("\n");
    }
     // Refresh the screen to show changes
 

    
 
     
     
    
    
     

int ch = getch(); // Get character

       // Use getchar() to get a character without Enter key



switch(ch) {
    case '8': // Up
    case 'k': // Alternative for up
        switch (map[playerCharacter.x-1][playerCharacter.y] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            
            printw("Battle interface triggered!");
            
            refresh();
            getch();
        }
        if (map[playerCharacter.x-1][playerCharacter.y] == 'M') {
            printw("Shop encountered!");
            char g = getch(); // Get character
           if (g == '>' || g == '.') {
    


            map[playerCharacter.x][playerCharacter.y] = temp;
            
            printw("Entering shop!");
            
            refresh();
            getch();}

            else {
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
    }
        else if (map[playerCharacter.x-1][playerCharacter.y] == 'C') {
            printw("Shop encountered!");
            char g = getch(); // Get character
           if (g == '>' || g == '.') {
    


            map[playerCharacter.x][playerCharacter.y] = temp;
            
            printw("Entering shop!");
            
            refresh();
            getch();}

            else {
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
    }
        else if (map[playerCharacter.x-1][playerCharacter.y] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.x--;
        }
        break;
    case '7': // Up-left diagonal movement
    case 'y':
        switch (map[playerCharacter.x-1][playerCharacter.y-1] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
        if (map[playerCharacter.x-1][playerCharacter.y-1] == 'M'|| map[playerCharacter.x-1][playerCharacter.y-1] == 'C') {
            char g=getch();
            if((g=='>') || (g=='.')){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
       
        else if (map[playerCharacter.x-1][playerCharacter.y-1] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.x--;
            playerCharacter.y--;
        }
        break;
    case '9': // Up-right diagonal movement
    case 'u':
          switch (map[playerCharacter.x-1][playerCharacter.y+1] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
           
        if (map[playerCharacter.x-1][playerCharacter.y+1] == 'M') {
            printw("Shop encountered!");
            char g=getch();
            if ((g=='>') || (g=='.')){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                 map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
         else if (map[playerCharacter.x-1][playerCharacter.y+1] == 'C') {
            printw("Shop encountered!");
            char g=getch();
            if ((g=='>') || (g=='.')){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                 map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
        else if (map[playerCharacter.x-1][playerCharacter.y+1] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.x--;
            playerCharacter.y++;
        }
        break;
    case '6': // Right movement
    case 'l':
        switch (map[playerCharacter.x][playerCharacter.y+1] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
        if (map[playerCharacter.x][playerCharacter.y+1] == 'M') {
            printw("Shop encountered!");
            char g=getch();
            if ((g=='>') || (g=='.')){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
             map[playerCharacter.x][playerCharacter.y] = temp;

            }
        }
         else if (map[playerCharacter.x][playerCharacter.y+1] == 'C') {
            printw("Shop encountered!");
            char g=getch();
            if ((g=='>') || (g=='.')){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
             map[playerCharacter.x][playerCharacter.y] = temp;

            }
        }
        else if (map[playerCharacter.x][playerCharacter.y+1] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.y++;
        }
        break;
    case '3': // Down-right diagonal movement
    case 'n':
        switch (map[playerCharacter.x+1][playerCharacter.y+1] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
        if (map[playerCharacter.x+1][playerCharacter.y+1] == 'M' || map[playerCharacter.x+1][playerCharacter.y+1] == 'C') {
            printw("Shop encountered!");
            char g =getch();
            if (g=='>'|| g=='.'){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
        
        else if (map[playerCharacter.x+1][playerCharacter.y+1] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.x++;
            playerCharacter.y++;
        }
        break;
    case '2': // Down movement
    case 'j':
        switch (map[playerCharacter.x+1][playerCharacter.y] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
          if (map[playerCharacter.x+1][playerCharacter.y] == 'M' || map[playerCharacter.x+1][playerCharacter.y] == 'C') {
            printw("Shop encountered!");
            char g =getch();
            if (g=='>'|| g=='.'){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
        else if (map[playerCharacter.x+1][playerCharacter.y] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.x++;
        }
        break;
    case '1': // Down-left diagonal movement
    case 'b':
         switch (map[playerCharacter.x+1][playerCharacter.y-1] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
       if (map[playerCharacter.x+1][playerCharacter.y-1] == 'M' || map[playerCharacter.x+1][playerCharacter.y-1] == 'C') {
            printw("Shop encountered!");
            char g =getch();
            if (g=='>'|| g=='.'){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
        else if (map[playerCharacter.x+1][playerCharacter.y-1] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.x++;
            playerCharacter.y--;
        }
        break;
    case '4': // Left movement
    case 'h':
         switch (map[playerCharacter.x][playerCharacter.y-1] ) 
        {
            case 'r':
            case 's':
            case 'h':
            case 'p':
            case 'e':
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Battle interface triggered!");
            refresh();
            getch();
            break;
        }
        if (map[playerCharacter.x][playerCharacter.y-1] == 'M' || map[playerCharacter.x][playerCharacter.y-1] == 'C') {
            printw("Shop encountered!");
            char g =getch();
            if (g=='>'|| g=='.'){

            
            map[playerCharacter.x][playerCharacter.y] = temp;
            printw("Entering shop!");
            refresh();
            getch();}
            else{
                map[playerCharacter.x][playerCharacter.y] = temp;
            }
        }
        else if (map[playerCharacter.x][playerCharacter.y-1] == '%') {
            map[playerCharacter.x][playerCharacter.y] = temp;
        }
        else {
            map[playerCharacter.x][playerCharacter.y] = temp;
            playerCharacter.y--;
        }
        break;
    case '>':
        // Implement the action for '>' here if needed
        break;
    // Consider adding a default case if there's a possibility of 'ch' not matching any cases
    
    case 'Q':
         exit(0);
        break;
    case 't':
    
  printw("List of NPCs on the map:\n");
    for (int i = 0; i < 21; i++) { // Assuming your map size is 21x80
        for (int j = 0; j < 80; j++) {
            char cell = map[i][j];
            switch(cell) {
                case 'h':
                    printw("Hiker at (%d, %d)\n", i, j);
                    break;
                case 'r':
                    printw("Rival at (%d, %d)\n", i, j);
                    break;
                case 'p':
                    printw("Pacer at (%d, %d)\n", i, j);
                    break;
                case 'w':
                    printw("Wanderer at (%d, %d)\n", i, j);
                    break;
                case 's':
                    printw("Sentry at (%d, %d)\n", i, j);
                    break;
                case 'e':
                    printw("Explorer at (%d, %d)\n", i, j);
                    break;
                // Add cases for other NPCs as necessary
            }
        }
    }
    refresh(); // Update the screen with all printed messages
    getch(); // Wait for a key press before returning
    break;
    case 5:
    break;
    
}

refresh();
clear();
endwin();
}
// Additional logic and functions omitted for brevity

// Allocate memory for a 2D char array
char **mapArray = (char **)malloc(21 * sizeof(char *));
for (int i = 0; i < 21; i++) {
    mapArray[i] = (char *)malloc(80 * sizeof(char));
    for (int j = 0; j < 80; j++) {
        mapArray[i][j] = map[i][j];
    }
}

return mapArray;


   
}



/* void SwitchMap()
{

    char **mapArray[401][401];

    // Initialize each element to NULL
    for (int i = 0; i < 401; i++)
    {
        for (int j = 0; j < 401; j++)
        {
            if (i == 200 && j == 200)
                mapArray[i][j] = printmap(' ', -1,200,200,numtrainers); // 0, 0 in the map
            else
                mapArray[i][j] = NULL;
        }
    }

    printf("map x-coordinate : %d \n",0);
    printf("map y-coordinate : %d \n ",0);

   
    CurrMap currentMap;
    currentMap.x = 200;
    currentMap.y = 200;
    char userInput;

    do
    {
        printf("Enter a character (or 'q' to quit): ");
        scanf(" %c", &userInput);

        // Process the user input
        if (userInput == 'n')
        {
            char **map = mapArray[currentMap.x][currentMap.y];
            
            int gateIndex= -1;
            for (int j = 0; j < 80; j++)
            {
                if (map[0][j] == '#')
                {
                    gateIndex = j;
                    break;
                }
            }

            updateX(&currentMap, currentMap.x - 1);
            int x = currentMap.x;
            int y = currentMap.y;
            // gives the coordintes we want to store the pointer of
            if (mapArray[x][y] == NULL)
            {
                mapArray[x][y] = printmap('n', gateIndex , currentMap.x, currentMap.y,numtrainers); // need to modify this to take in specific gate posiions to generate
            }
            else
            {

                char **centralMap = mapArray[x][y];

                for (int i = 0; i < 21; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        if (centralMap[i][j] == '^')
                        {

                            printf(".");
                        }
                        else if (centralMap[i][j] == '.')
                        {

                            printf("^");
                        }
                        else
                        {
                            printf("%c", centralMap[i][j]);
                        }
                    }
                    printf("\n");
                }
            }

            printf("map x-coordinate : %d \n",currentMap.x - 200);
            printf("map y-coordinate : %d \n ",currentMap.y - 200);
        }
        else if (userInput == 's')
        {

            char **map = mapArray[currentMap.x][currentMap.y];
            
            int gateIndex = -1;
            for (int j = 0; j < 80; j++)
            {
                if (map[20][j] == '#')
                {
                    gateIndex = j;
                    break;
                }
            }

            // stores last row's gate index to pass on

            updateX(&currentMap, currentMap.x + 1);
            int x = currentMap.x;
            int y = currentMap.y;

            if (mapArray[x][y] == NULL)
            {
                mapArray[x][y] = printmap('s', gateIndex,currentMap.x,currentMap.y,numtrainers); // need to modify this to take in specific gate posiions to generate
            }
            else
            {

                char **centralMap = mapArray[x][y];

                for (int i = 0; i < 21; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        if (centralMap[i][j] == '^')
                        {

                            printf(".");
                        }
                        else if (centralMap[i][j] == '.')
                        {

                            printf("^");
                        }
                        else
                        {
                            printf("%c", centralMap[i][j]);
                        }
                    }
                    printf("\n");
                }
            }

            printf("map x-coordinate : %d \n",currentMap.x - 200);
            printf("map y-coordinate : %d \n ",currentMap.y - 200);
        }
        else if (userInput == 'e')
        {

            char **map = mapArray[currentMap.x][currentMap.y];
            
            int gateIndex = -1;
            for (int i = 0; i < 21; i++)
            {
                if (map[i][79] == '#')
                {
                    gateIndex = i;
                    break;
                }
            }

            // find appropriate row at which gate exists

            updateY(&currentMap, currentMap.y + 1);
            int x = currentMap.x;
            int y = currentMap.y;

            if (mapArray[x][y] == NULL)
            {
                mapArray[x][y] = printmap('e', gateIndex,currentMap.x,currentMap.y,numtrainers); // need to modify this to take in specific gate posiions to generate
            }
            else
            {

                char **centralMap = mapArray[x][y];

                for (int i = 0; i < 21; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        if (centralMap[i][j] == '^')
                        {

                            printf("^");
                        }
                        else if (centralMap[i][j] == '.')
                        {

                            printf(".");
                        }
                        else
                        {
                           printf("%c", centralMap[i][j]);
                        }
                    }
                    printf("\n");
                }
            }

           printf("map x-coordinate : %d \n",currentMap.x - 200);
            printf("map y-coordinate : %d \n ",currentMap.y - 200);
        }
        else if (userInput == 'w')
        {

            char **map = mapArray[currentMap.x][currentMap.y];
            
            int gateIndex = -1;
            for (int i = 0; i < 21; i++)
            {
                if (map[i][0] == '#')
                {
                    gateIndex = i;
                    break;
                }
            }

            updateY(&currentMap, currentMap.y - 1);
            int x = currentMap.x;
            int y = currentMap.y;

            if (mapArray[x][y] == NULL)
            {
                mapArray[x][y] = printmap('w', gateIndex,currentMap.x,currentMap.y,numtrainers); // need to modify this to take in specific gate posiions to generate
            }
            else
            {

                char **centralMap = mapArray[x][y];

                for (int i = 0; i < 21; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        if (centralMap[i][j] == '.')
                        {

                            printf(".");
                        }
                        else if (centralMap[i][j] == '^')
                        {

                            printf("^");
                        }
                        else
                        {
                            printf("%c", centralMap[i][j]);
                        }
                    }
                    printf("\n");
                }
            }

            printf("map x-coordinate : %d \n",currentMap.x - 200);
            printf("map y-coordinate : %d \n ",currentMap.y - 200);
        }
        else if (userInput == 'f')
        {
            int x;
            int y;
            printf("Enter x coordinate of map you want to go to ");
            scanf(" %d", &x); 
            printf("Enter y coordinate of map you want to go to ");
            scanf(" %d", &y); 
            
            x += 200;
            y += 200;
            updateX(&currentMap, x);
            updateY(&currentMap, y);
            x = currentMap.x;
            y = currentMap.y;

            if (mapArray[x][y] == NULL)
            {
                mapArray[x][y] = printmap(' ', -1,currentMap.x,currentMap.y,numtrainers); 
            }
            else
            {

                char **centralMap = mapArray[x][y];

                for (int i = 0; i < 21; i++)
                {
                    for (int j = 0; j < 80; j++)
                    {
                        if (centralMap[i][j] == '^')
                        {

                            printf("^");
                        }
                        else if (centralMap[i][j] == '.')
                        {

                            printf(".");
                        }
                        else
                        {
                            printf("%c", centralMap[i][j]);
                        }
                    }
                    printf("\n");
                }
            }

        printf("map x-coordinate : %d \n",currentMap.x - 200);
        printf("map y-coordinate : %d \n ",currentMap.y - 200);

        }
        else if (userInput == 'q')
        {
            printf("Quitting game \n");
        }
        else
        {
            printf("Please generate a valid input");
        }
// puts boulders to handle edge cases
        if (currentMap.x == 0)
        {
            for (int j = 0; j < 80; j++)
            {
                
                    mapArray[currentMap.x][currentMap.y][0][j] = '%';
                
            }
        }
        else if (currentMap.x == 400)
        {

            for (int j = 0; j < 80; j++)
            {
                
                    mapArray[currentMap.x][currentMap.y][20][j] = '%';
                }
            }
        
        if (currentMap.y == 0)
        {
            for (int i = 0; i < 21; i++)
            {
                
                    mapArray[currentMap.x][currentMap.y][i][0] = '%';
                
            }
        }
        else if (currentMap.y == 400)
        {
            for (int i = 0; i < 21; i++)
            {
                
                    mapArray[currentMap.x][currentMap.y][i][79] = '%';
                
            }
        }

    } while (userInput != 'q');
}
*/

