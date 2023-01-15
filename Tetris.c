#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#define MAP_SIZE_W 10
#define MAP_SIZE_H 20
#define HALF_W 20
#define HALF_H 10
#define EXIT 100
#define WALL 5
#define EMPTY 0
#define BLOCK 1
#define UP 72
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ESC 27
#define DOWN 80
typedef char MData ;
typedef struct currentlocation{
    int X;
    int Y;
} Location;
//move cursor.
void gotoxy(int x, int y){
    COORD P;//coordinates accesing by p
    P.X = 2*x;//Controlling Map X axis
    P.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), P);//Predefined Functionn to handle cursor position//
}
//conio.h library used//
//get keyboard input.
int getKeyDown(){
    if(kbhit())//check rather the key is pressed or not//
	{
	  return getch();//returns the ascii value of the pressed key//
    }
    else
	{
	  return -1;
   }
}
void drawWall(MData map[MAP_SIZE_H][MAP_SIZE_W]){
    int h, w;
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);//predefine function to handle colors
    SetConsoleTextAttribute(hand, 12);//predefine function ,numbers is used to control colors

    for(h=0; h<=MAP_SIZE_H +1; h++){
        for(w=0; w<=MAP_SIZE_W+1; w++){
            gotoxy(w+1,h+1);
            if(h==0 || w==0 || h==MAP_SIZE_H+1 || w==MAP_SIZE_W+1)
                printf("** ");//Making Of Borders//
//               printf(" %d,%d ",h,w);
        }
        printf("\n");
    }
    gotoxy(HALF_W, 3);
    printf("Best :");
    gotoxy(HALF_W, 4);
    printf("Score :");
    gotoxy(HALF_W, 7);
    printf("Exit : 't' / Pause : 'p'");
}
int drawFrontMenu(){
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    int keyInput;
    gotoxy(1,2);
    SetConsoleTextAttribute(hand, 3);
    printf("******");
    gotoxy(1,3);
    SetConsoleTextAttribute(hand, 4);
    printf("~~WeLcOmE To TeTrIs~~~");
    SetConsoleTextAttribute(hand, 3);
    gotoxy(1,4);
    printf("******\n");
    SetConsoleTextAttribute(hand, 4);
    gotoxy(2,6);
    printf("Instructions\n");
    SetConsoleTextAttribute(hand, 3);
    gotoxy(2,7);
    printf("Left :left arrow key\n");
    gotoxy(2,8);
    printf("Right :right arrow key \n");
    gotoxy(2,9);
    printf("Down:down arrow key \n");
    gotoxy(2,10);
    printf("Exit:'t'\n");
    while(1){
        keyInput = getKeyDown();
        if(keyInput == 's' || keyInput == 'S') break;
        if(keyInput == 't' || keyInput == 'T') break;
        gotoxy(7, 15);
        SetConsoleTextAttribute(hand, 4);
        printf("Press s To Start");
    }
    return keyInput;
}
void drawMap(MData map[MAP_SIZE_H][MAP_SIZE_W]){
    int h, w;
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    for(h=0; h<MAP_SIZE_H; h++){
        for(w=0; w<MAP_SIZE_W; w++){
            gotoxy(w+2,h+2);
            if(map[h][w] == EMPTY){
                printf("  ");
            }else if(map[h][w] == BLOCK){
                SetConsoleTextAttribute(hand, 14);
                printf("**");
            }
        }
        printf("\n");
    }
}
void drawSubMap(int best, int score){//best score and current score//
    HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hand, 14);
    gotoxy(HALF_W + 4, 3);
    printf("%d", best);
    gotoxy(HALF_W + 4, 4);
    printf("%d", score);
}
void drawShape(MData map[MAP_SIZE_H][MAP_SIZE_W],int shape[4][4], Location ptr){
    int h, w;
    for(h=0; h<4;h++){
        for(w=0; w<4;w++){
            if(shape[h][w] ==BLOCK){
                map[ptr.Y+ h][ptr.X +w]=BLOCK;
            }
        }
    }
}
void mapInit(MData map[MAP_SIZE_H][MAP_SIZE_W]){
    int i, j=0;
    for(i=0;i<MAP_SIZE_H;i++){
        for(j=0; j<MAP_SIZE_W; j++){
            map[i][j] = EMPTY;//use to clear the  internal map //
        }
    }
}
void locationInit(Location * ptr){
    ptr->X =3;//The coordinate from where the block falls//
    ptr->Y =0;
}
void copyBlock(int blockShape[4][4], int copy[4][4]){
    int i, j;
    for(i=0;i<4;i++){
        for(j=0; j<4;j++){
            blockShape[i][j] = copy[i][j];
        }
    }
}
void setBlock(int blockShape[4][4]){
    int shape[5][4][4] = {
            {{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0}},
            {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
            {{0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0}},
            {{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
            {{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
    };
   srand((unsigned int)(time(NULL)));
    switch(rand()%5) {
        case 0:
            copyBlock(blockShape, shape[0]);
            break;
        case 1:
            copyBlock(blockShape, shape[1]);
            break;
        case 2:
            copyBlock(blockShape, shape[2]);
            break;
        case 3:
            copyBlock(blockShape, shape[3]);
            break;
        case 4:
            copyBlock(blockShape, shape[4]);
            break;
        default :
            break;
    }
}
void removeShape(MData map[MAP_SIZE_H][MAP_SIZE_W], int blockShape[4][4], Location * ptr){
    int h, w;
    for(h=0; h<4;h++){
        for(w=0; w<4;w++){
            if(blockShape[h][w] ==BLOCK)
                map[ptr->Y + h][ptr->X + w]=EMPTY;
        }
    }
}
int getShapeLeftLoc(int blockShape[4][4]){
    int h, w, leftW=3;
    for(w=0; w<4;w++){
        for(h=0; h<4;h++){
            if(blockShape[h][w] == BLOCK){
                if(leftW > w)
                    leftW = w;
            }
        }
    }
    return leftW;
}
int getShapeRightLoc(int blockShape[4][4]){
    int h, w, rightW=0;
    for(w=3; w>=0;w--){
        for(h=3; h>=0;h--){
            if(blockShape[h][w] == BLOCK){
                if(rightW < w)
                    rightW = w;
            }
        }
    }
    return rightW+1;
}
int getShapeBottomLoc(int blockShape[4][4]){
    int h, w, bottomH=0;
    for(w=3; w>=0;w--){
        for(h=3; h>=0;h--){
            if(blockShape[h][w] == BLOCK){
                if(bottomH < h)
                    bottomH =h;
            }
        }
    }
    return bottomH+1;
}
int getEachBottomLoc(int blockShape[4][4], int w){
    int h, bottomH=-1;
    for(h=3; h>=0;h--){
        if(blockShape[h][w] == BLOCK){
            if(bottomH < h)
                bottomH =h;
        }
    }
    return bottomH;
}
int getEachLeftLoc(int blockShape[4][4], int h){
    int w, leftW= 5;
    for(w=0; w<4;w++){
        if(blockShape[h][w] == BLOCK){
            if(leftW > w)
                leftW = w;
        }
    }
    return leftW;
	}
int getEachRightLoc(int blockShape[4][4], int h){
    int w, rightW= -1;
    for(w=0; w<4;w++){
        if(blockShape[h][w] == BLOCK){
            if(rightW < w)
                rightW = w;
        }
    }
    return rightW;
}
void goLeft(MData map[MAP_SIZE_H][MAP_SIZE_W],int blockShape[4][4], Location *ptr){
    int leftW = getShapeLeftLoc(blockShape);
    int boundaryArr[4] ={0};
    int i;
    for(i=0; i<4;i++){
        boundaryArr[i] = getEachLeftLoc(blockShape, i);
}
    if((ptr->X) + leftW > 0){
        if(!((boundaryArr[0] != 5 && map[ptr->Y][ptr->X + boundaryArr[0] -1] != EMPTY)||(boundaryArr[1] != 5 && map[ptr->Y +1][ptr->X + boundaryArr[1] -1] != EMPTY)||(boundaryArr[2] != 5 && map[ptr->Y +2][ptr->X + boundaryArr[2] -1] != EMPTY)||(boundaryArr[3] != 5 && map[ptr->Y +3][ptr->X + boundaryArr[3] -1] != EMPTY))){
            removeShape(map, blockShape,ptr);
            (ptr->X)--;
        }
    }
}
void goRight(MData map[MAP_SIZE_H][MAP_SIZE_W],int blockShape[4][4], Location *ptr){
    int rightW = getShapeRightLoc(blockShape);
    int boundaryArr[4] ={0};
    int i;
    for(i=0; i<4;i++){
        boundaryArr[i] = getEachLeftLoc(blockShape, i);

    }
    if((ptr->X) + rightW < MAP_SIZE_W){
        if(!((boundaryArr[0] != 5 && map[ptr->Y][ptr->X + boundaryArr[0] +1] != EMPTY)
             ||(boundaryArr[1] != 5 && map[ptr->Y +1][ptr->X + boundaryArr[1] +1] != EMPTY)
             ||(boundaryArr[2] != 5 && map[ptr->Y +2][ptr->X + boundaryArr[2] +1] != EMPTY)
             ||(boundaryArr[3] != 5 && map[ptr->Y +3][ptr->X + boundaryArr[3] +1] != EMPTY))){
             removeShape(map, blockShape,ptr);
            (ptr->X)++;
        }
    }
}
int goDown(MData map[MAP_SIZE_H][MAP_SIZE_W], int blockShape[4][4], Location *ptr){
    int bottomH = getShapeBottomLoc(blockShape);
    int bottomArr[4] = {0};
    int i;
    for(i=0; i<4; i++){
        bottomArr[i] = getEachBottomLoc(blockShape, i);
    }
    if(ptr->Y + bottomH  == MAP_SIZE_H||(bottomArr[1] != -1 && map[ptr->Y + bottomArr[1] +1][ptr->X + 1] != EMPTY)||(bottomArr[0] != -1 && map[ptr->Y + bottomArr[0] +1][ptr->X + 0] != EMPTY)||(bottomArr[3] != -1 && map[ptr->Y + bottomArr[3] +1][ptr->X + 3] != EMPTY)||(bottomArr[2] != -1 && map[ptr->Y + bottomArr[2] +1][ptr->X + 2] != EMPTY)){
    	return TRUE;
    }
   if(ptr->Y + bottomH < MAP_SIZE_H){
        removeShape(map, blockShape, ptr);
        Sleep(1000/8);
        (ptr->Y)++;
    }
    return FALSE;
}
void deleteLine(MData map[MAP_SIZE_H][MAP_SIZE_W], int h){
    int w;
    for(w=0 ; w < MAP_SIZE_W ; w++){
        map[h][w] = EMPTY;
    }
}
void organizeLine(MData map[MAP_SIZE_H][MAP_SIZE_W], int h){
    int w;
    while(h > 1){
        for(w=0; w<MAP_SIZE_W;w++){
            map[h][w] = map[h-1][w];
        }
        h--;
    }
}
void checkLine(MData map[MAP_SIZE_H][MAP_SIZE_W], Location ptr, int * score){
    int h, w, full, count =0;
    for(h=MAP_SIZE_H ; h >= (ptr.Y -1); h--){
        full =0;
        for(w=0; w<MAP_SIZE_W  ;w++){
            if(map[h][w] == EMPTY){
                break;
            }else{
                full++;
            }
        }

        if(full == MAP_SIZE_W){
            (*score) += 10;
            deleteLine(map, h);
            organizeLine(map, h);
        }
    }
}
int GameOver(MData map[MAP_SIZE_H][MAP_SIZE_W],int score, int bestScore){
    FILE * fptr1;
    int w=0;
    for(w=0; w<MAP_SIZE_W; w++){
        if(map[0][w] == BLOCK){
            HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hand, 14);
            gotoxy(13, 8);
            printf("**GAME OVER**");
            gotoxy(14, 9);
            printf("Your Score : %4d\n", score);
            gotoxy(1, MAP_SIZE_H+3);
            if(score >= bestScore){
                fptr1= fopen("score.txt", "w");
                fprintf(fptr1, "%d", score);
                fclose(fptr1);
            }
            system("pause");
            return TRUE;
        }
    }
   return FALSE;
}
int GameStart(MData map[MAP_SIZE_H][MAP_SIZE_W]){
    int key;
    int reachBottom = 0,one = 1,score =0, bestScore =0, blockShape[4][4] ={0}, blockShapeSub[4][4] = {0};
    Location ptr={2,2};
    FILE * fptr;
    if((fptr = fopen("score.txt", "r")) == NULL){
        FILE * fptr;
        fptr= fopen("score.txt", "w");
        fprintf(fptr, "%d", score);
        fclose(fptr);
    }
    fscanf(fptr, "%d", &bestScore);
    mapInit(map);
    drawWall(map);
    drawMap(map);
    locationInit(&ptr);
    setBlock(blockShape);
    setBlock(blockShapeSub);
    while(1){
       if(reachBottom == TRUE){
            if(GameOver(map,score, bestScore)) return EXIT;
            checkLine(map, ptr, &score);
            checkLine(map, ptr, &score);
            locationInit(&ptr);
            copyBlock(blockShape, blockShapeSub);
            setBlock(blockShapeSub);
            reachBottom = FALSE;
        }
        drawSubMap(bestScore,score);
        drawShape(map,blockShape, ptr);
        drawMap(map);
        reachBottom = goDown(map, blockShape, &ptr);
        if(reachBottom == TRUE) continue;
        key = getKeyDown();
        if(key == 't' || key =='T') break;
        if(key == 'p' || key == 'P'){
	        system("pause"); 
			system("cls");
	        drawMap(map); 
			drawWall(map);
        }
        if(key==224 || key ==0){
            key = getch();
            if(key == UP){
            }else if(key == LEFT){
                goLeft(map, blockShape, &ptr);
            }else if(key == RIGHT){
                goRight(map, blockShape, &ptr);
            }
        }
    }
    return EXIT;
}
int main() {
    char map[MAP_SIZE_H][MAP_SIZE_W] ={0};   //map
    int key;
    while(1){
        key = drawFrontMenu();
        if(key == 't' || key == 'T') break;
        system("cls");
        GameStart(map);
    }
    return 0;
}