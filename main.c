//
//  main.c
//  HT
//
//  Created by Eric on 15-12-18.
//  Copyright (c) 2015年 Bakantu Eric. All rights reserved.
//

/*include*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "Keyboard.h"
/*define*/

//basic define
#define YMAX 16
#define XMAX 35
#define SNACK_MAX 1200
#define SLEEPTIME 500000
#define INITSNACK 3

#define NONE 0
#define WALL 1
#define SNACK 2
#define FOOD 3

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/*function*/
void init(void);
void game_engine(void);
void print_table(void);
void snack_food(void);
void snack_move(void);
void ifinput(void);
void if_eat(void);
int if_lose(void);
int if_win(void);
int safe_snack(int x, int y);

/*resource*/
int table[XMAX][YMAX];
struct p_snack{
    int x;
    int y;
    int dir;
};
struct s_food{
    int x;
    int y;
};
struct p_snack snacks[SNACK_MAX];
struct s_food s_foods;
int n_snacks = 0;
int w_ifnot  = 1;
int food = 0;
char user_control = '\0';
int score = 0;
int s_add = 0;


/*detail_function*/
int main(void){
    init();
    return 0;
}

void init(void){
    int x = 0,y = 0,i = 0;;
    /*init table*/
    for (x = 0; x < XMAX; x++)
        for (y = 0; y < YMAX; y++){
            table[x][y] = NONE;
        }
    /*print wall*/
    for (x = 0; x < XMAX; x++) {
        table[x][0] = WALL;
        table[x][YMAX-1] =  WALL;
    }
    for (y = 0; y < YMAX; y++){
        table[0][y] = WALL;
        table[XMAX-1][y] = WALL;
    }
    /*init snack*/
    n_snacks = INITSNACK;
    srand((unsigned int)time(0));
    snacks[0].x = (rand()%(XMAX-10))+5;
    snacks[0].y = (rand()%(YMAX-10))+5;
    snacks[0].dir = rand()%4;
    for (i = 1; i <= n_snacks; i++){
        if (snacks[0].dir == UP){
            snacks[i].x = snacks[0].x;
            snacks[i].y = snacks[0].y+i;
        }
        else if (snacks[0].dir == DOWN){
            snacks[i].x = snacks[0].x;
            snacks[i].y = snacks[0].y-i;
        }
        else if (snacks[0].dir == LEFT){
            snacks[i].x = snacks[0].x+i;
            snacks[i].y = snacks[0].y;
        }
        else{
            snacks[i].x = snacks[0].x-i;
            snacks[i].y = snacks[0].y;
        }
    }
    system("clear");
    game_engine();
    
}

void game_engine(void){
    while (w_ifnot){
        snack_food();
        ifinput();
        switch (user_control) {
            case 'w':
                if (safe_snack(snacks[0].x, snacks[0].y-1))
                snacks[0].dir = UP;
                user_control = '\0';
                break;
            case 's':
                if (safe_snack(snacks[0].x, snacks[0].y+1))
                snacks[0].dir = DOWN;
                user_control = '\0';
                break;
            case 'a':
                if (safe_snack(snacks[0].x-1, snacks[0].y))
                snacks[0].dir = LEFT;
                user_control = '\0';
                break;
            case 'd':
                if (safe_snack(snacks[0].x+1, snacks[0].y))
                snacks[0].dir = RIGHT;
                user_control = '\0';
                break;
                
            default:
                break;
        }
        snack_move();
        if_eat();
        /*refresh_print*/
        print_table();
        usleep(SLEEPTIME);
        /*win or lose*/
        if (if_lose()){
            printf("YOU LOSE!");
            w_ifnot = 0;
        }
        if (if_win()){
            printf("YOU WIN!");
            w_ifnot = 0;
        }
    }
}

void print_table(void){
    system("clear");
    int x = 0,y = 0,i = 0,s_ifnot = 0;
    for (y = 0; y < YMAX; y++){
        for (x = 0; x < XMAX; x++){
            /*print snack*/
            s_ifnot = 0;
            for (i = 0; i < n_snacks; i++)
                if ((snacks[i].x == x)&&(snacks[i].y == y)){
                    printf("●");
                    s_ifnot = 1;
                    break;
                }
            if (s_ifnot) continue;
            if (table[x][y] == NONE) printf(" ");
            else if (table[x][y] == WALL) printf("*");
            else if (table[x][y] == FOOD) printf("◆");
            else{
            }
        }
        printf("\n");
    }
    //printf("SNACK[0].dir = %d \n",snacks[0].dir);
    //printf("SNACK[0] = (%d,%d) \n",snacks[0].x,snacks[0].y);
    //printf("FOOD = (%d,%d) \n",s_foods.x,s_foods.y);
    printf("SCORE %d\n",score);
}

void snack_food(void){
    if (food == 0){
        s_foods.x = rand()%(XMAX-3)+1;
        s_foods.y = rand()%(YMAX-3)+1;
        table[s_foods.x][s_foods.y] = FOOD;
        food = 1;
    }
}

void snack_move(void){
    int i = 0;
    if (s_add){
        snacks[n_snacks].x = snacks[n_snacks-1].x;
        snacks[n_snacks].y = snacks[n_snacks-1].y;
        snacks[n_snacks].dir = snacks[n_snacks-1].dir;

    }
    for (i = n_snacks-1; i > 0; i--){
        snacks[i].x= snacks[i-1].x;
        snacks[i].y= snacks[i-1].y;
        snacks[i].dir= snacks[i-1].dir;
    }
    if (s_add){
        n_snacks += 1;
        s_add = 0;
    }

    if ((snacks[0].dir == UP)&&(snacks[0].y > 1)) snacks[0].y -= 1;
    else if((snacks[0].dir == UP)&&(snacks[0].y <= 1)) snacks[0].y = YMAX-2;
    
    if ((snacks[0].dir == DOWN)&&(snacks[0].y < (YMAX-2))) snacks[0].y +=1;
    else if((snacks[0].dir == DOWN)&&(snacks[0].y >= (YMAX-2))) snacks[0].y = 1;
    
    if ((snacks[0].dir == LEFT)&&(snacks[0].x > 1)) snacks[0].x -= 1;
    else if((snacks[0].dir == LEFT)&&(snacks[0].x <= 1)) snacks[0].x = XMAX-2;
    
    if ((snacks[0].dir == RIGHT)&&(snacks[0].x < (XMAX-2))) snacks[0].x += 1;
    else if((snacks[0].dir == RIGHT)&&(snacks[0].x >= (XMAX-2))) snacks[0].x = 1;

}

void ifinput(void){
    if ( kbhit() ) user_control = readch();
}

int safe_snack(int x, int y){
    int i = 0;
    for (i = 0; i < n_snacks; i++){
        if ((snacks[i].x == x)&&(snacks[i].y == y)) return 0;
    }
    return 1;
}

void if_eat(void){
    if ((snacks[0].x == s_foods.x)&&(snacks[0].y == s_foods.y)){
        food = 0;
        table[s_foods.x][s_foods.y] = NONE;
        score += 100;
        s_add = 1;
    }
}

int if_lose(void){
    int i = 0;
    for (i = 1; i < n_snacks; i++)
        if ((snacks[0].x == snacks[i].x)&&(snacks[0].y == snacks[i].y)) return 1;
    return 0;
}

int if_win(void){
    if (n_snacks == SNACK_MAX) return 1;
    return 0;
}
/*END*/
