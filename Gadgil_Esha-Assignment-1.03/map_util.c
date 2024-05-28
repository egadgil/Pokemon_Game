#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include "queue.h"

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

void printRmap(char map[21][80] ,int row , int col){

    printf("RIVAL MAP \n");

    int hiker[21][80];
    PriorityQueue *pq = createPriorityQueue(20000); 
    
    
    int visited[21][80];
    
    for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            visited[i][j] = 0;
        }
    }

      for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            hiker[i][j] = INT_MAX; 
        }
    }

    insert(pq,row,col,0);

    hiker[row][col] = 0; 

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

            int newWeight = (weight + getRivalWeight(map[x+aroundx[i]][y+aroundy[i]], x + aroundx[i], y + aroundy[i])) % 100;
            
            if(visited[x+aroundx[i]][y+aroundy[i]]== 0 && hiker[x+aroundx[i]][y+aroundy[i]] > newWeight) {
                
                hiker[x+aroundx[i]][y+aroundy[i]] = (newWeight < 0) ? INT_MAX : newWeight;
                insert(pq,x+aroundx[i],y+aroundy[i],hiker[x+aroundx[i]][y+aroundy[i]]);
            }
            
        }
    }

  

    for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            if(hiker[i][j] == INT_MAX) printf("   ");
            else if(hiker[i][j]==0) printf("00 ");
            else printf("%2d ",hiker[i][j]);
        }
        printf("\n");
    }

}


void printHmap(char map[21][80] ,int row , int col){

    printf("HIKER MAP \n");
    
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
            hiker[i][j] = INT_MAX; 
        }
    }

    insert(pq,row,col,0);

    hiker[row][col] = 0; 

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

            int newWeight = (weight + getHikerWeight(map[x+aroundx[i]][y+aroundy[i]], x + aroundx[i], y + aroundy[i]));
            
            if(visited[x+aroundx[i]][y+aroundy[i]]== 0 && hiker[x+aroundx[i]][y+aroundy[i]] > newWeight) {
               //checks to see if the new weight is one of the unrecongnized symbols and initializes it to infinitiy if true
                hiker[x+aroundx[i]][y+aroundy[i]] = (newWeight < 0) ? INT_MAX : newWeight;
                insert(pq,x+aroundx[i],y+aroundy[i],hiker[x+aroundx[i]][y+aroundy[i]]);
            }
            
        }
    }


    for(int i = 0 ; i < 21 ; i++){
        for(int j = 0 ; j < 80 ; j++){
            if(hiker[i][j] == INT_MAX) printf("   ");
            else if(hiker[i][j]==0) printf("00 ");
            else printf("%2d ",hiker[i][j]%100);
        }
        printf("\n");
    }

    printf("\n");
    printf("\n");




 }

// Function to create an empty queue
 

char **printmap(char gate, int index , int mapx , int mapy )
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
         
        int count = rand()%10;
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
                if(var == count){
                    map[rowx][rowy] = '@'; 
                    playerCharacter.x = rowx;  
                    playerCharacter.y = rowy;  // Set y-coordinate
                }
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

    // PRINTING BOARD FORMULATED

    for (int i = 0; i < rows; i++)
    {

        for (int j = 0; j < cols; j++)
        {
            
            if (map[i][j] == '^')
            {

                printf(".");
            }
            else if (map[i][j] == '.')
            {

                printf("^");
            }
            else
            {
                printf("%c", map[i][j]);
            }
        }
        printf("\n");
    }

    
    printHmap(map, playerCharacter.x, playerCharacter.y);

    printRmap(map, playerCharacter.x, playerCharacter.y);
    

    // Allocate memory for a 2D char array
    char **mapArray = (char **)malloc(21 * sizeof(char *));
    for (int i = 0; i < 21; i++)
    {
        mapArray[i] = (char *)malloc(80 * sizeof(char));
        for (int j = 0; j < 80; j++)
        {
            mapArray[i][j] = map[i][j];
        }
    }

    return mapArray;
}



void SwitchMap()
{

    char **mapArray[401][401];

    // Initialize each element to NULL
    for (int i = 0; i < 401; i++)
    {
        for (int j = 0; j < 401; j++)
        {
            if (i == 200 && j == 200)
                mapArray[i][j] = printmap(' ', -1,200,200); // 0, 0 in the map
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
                mapArray[x][y] = printmap('n', gateIndex , currentMap.x, currentMap.y); // need to modify this to take in specific gate posiions to generate
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
                mapArray[x][y] = printmap('s', gateIndex,currentMap.x,currentMap.y); // need to modify this to take in specific gate posiions to generate
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
                mapArray[x][y] = printmap('e', gateIndex,currentMap.x,currentMap.y); // need to modify this to take in specific gate posiions to generate
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
                mapArray[x][y] = printmap('w', gateIndex,currentMap.x,currentMap.y); // need to modify this to take in specific gate posiions to generate
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
                mapArray[x][y] = printmap(' ', -1,currentMap.x,currentMap.y); 
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



