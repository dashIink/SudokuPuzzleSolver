#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/wait.h>
#include <pthread.h>

typedef struct 
{ 
    int row; 
    int column; 
} parameters; 

int board[9][9] = {
	{5,3,4,6,7,8,9,1,2},
	{6,7,2,1,9,5,3,4,8},
	{1,9,8,3,4,2,5,6,7},
	{8,5,9,7,6,1,4,2,3},
	{4,2,6,8,5,3,7,9,1},
	{7,1,3,9,2,4,8,5,6},
	{9,6,1,5,3,7,2,8,4},
	{2,8,7,4,1,9,6,3,5},
	{3,4,5,2,8,6,1,7,9}};

bool columnCheck = true;
bool rowCheck = true;
bool squareCheck = true;


void *checkRows(void *){
    int check[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i++){
        for(int x = 0; x < 9; x++){
            
            check[board[i][x]-1] += 1;
            
            if (check[board[i][x]-1] > 1){
                rowCheck = false;
            }
        }
        for(int x = 0; x < 9; x++){ 
            check[x] = 0;
        }
    }
    
}
void *checkColumns(void *){
    int check[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i++){
        for(int x = 0; x < 9; x++){
            
            check[board[x][i]-1] += 1;
            
            if (check[board[i][x]-1] > 1){
                columnCheck = false;
            }
        }
        for(int x = 0; x < 9; x++){ 
            check[x] = 0;
        }
    }
}

void *checkSquares(void *){
    int check[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < 9; i+=3){
        for(int x = 0; x < 9; x+=3){
            for(int j = i; j < i+3; j++){
                for (int h = x; h < x+3; h++){
                    check[board[j][h]-1] += 1;
                    if (check[board[j][h]-1] > 1){
                        squareCheck = false;
                    }
                }
            }
        for(int x = 0; x < 9; x++){ 
            check[x] = 0;
        }
        }
        
    }
}

void *checkSquare(void *data){
    //printf("%d", ((parameters*)data)->row);
    int j = ((parameters*)data)->row;
    int h = ((parameters*)data)->column;
    int check[9] = {0,0,0,0,0,0,0,0,0};
    for(int i = j; i < j+3; i++){
        for(int x = h; x < h+3; x++){
            check[board[i][x]-1] += 1;
            if (check[board[i][x]-1] > 1){
                squareCheck = false;
            }
        }
    }
    for(int x = 0; x < 9; x++){ 
        check[x] = 0;
    }
        
        
    
}

int main(){
		pthread_t id[11];
		//int arg = 1;
		pthread_create(&id[0], NULL, *checkRows, NULL);
		pthread_create(&id[1], NULL, *checkColumns, NULL);
		//pthread_create(&id[2], NULL, *checkSquares, NULL);
		
		
		int i = 2;
		for(int x = 0; x < 9; x+=3){
		    for(int y = 0; y < 9; y+=3){
    		    parameters *data = (parameters *) malloc(sizeof(parameters)); 
                data->row = x;
                data->column = y; 
    		    pthread_create(&id[i], NULL, *checkSquare, (void *)data);
    		    i++;
		    }
		}
		
		pthread_join(id[0], NULL);
		pthread_join(id[1], NULL);
		for(int h = 2; h < 11; h++){
		    pthread_join(id[h], NULL);
		}
		
		if (rowCheck && columnCheck && squareCheck){
		    printf("It is a correct sudoku Puzzle!");
		}
		else{
		    printf("This puzzle has errors!");
		}
		
}
