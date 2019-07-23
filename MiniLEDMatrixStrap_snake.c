/*
 * MiniLEDMatrixStrap2_snake.c
 *
 * Created: 2015/07/23 19:41:12
 *  Author: R
 */ 


#define F_CPU   1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define SNAKE_LENDGH 6

// 8ビット・ランダム作成(ビルトイン関数rand()でも可)
unsigned int _rand( uint16_t init )
{
	static uint16_t seed;
	
	if ( init != 0 ){
		seed ^= init;
	}
	seed *= 12345;
	seed += 35;
	return (uint8_t)((seed >> 8) & 0xFF);
}

int8_t getMoveWidth(){
	switch(_rand(562) % 4){
		case 0 :
			return 1;
		case 1 :
			return -1;
		case 2 :
			return 4;
		case 3 :
			return -4;
		default:
			return 0;
	}
}
	
//LEDを点灯するときHIGHにするピン(左右反転)
const uint8_t Anode[5][4]={

	{2,0,1,0},
	{4,0,3,0},
	{3,1,2,1},
	{3,2,4,1},
	{4,3,4,2},
	
};

//LEDを点灯するときLOWにするピン(左右反転)
const uint8_t Cathode[5][4]={
	
	{0,2,0,1},
	{0,4,0,3},
	{1,3,1,2},
	{2,3,1,4},
	{3,4,2,4},
	
};

//Snakeの位置情報
uint8_t Snake[SNAKE_LENDGH];
uint8_t SnakeMoveCount;

//LEDをすべて消灯
void clsdisp(){
	PORTB = 0x00;
	DDRB = 0x00;
}

//(x,y)のLEDを点灯
void onLED(uint8_t x ,uint8_t y){
	DDRB |= ( 0x01 << Anode[y][x] );
	DDRB |= ( 0x01 << Cathode[y][x] );
	PORTB |= ( 0x01 << Anode[y][x] );
	PORTB &= ~(0x01 << Cathode[y][x]);
}

void moveSnake(){
	SnakeMoveCount++;
	Snake[6] = Snake[5];
	Snake[5] = Snake[4];
	Snake[4] = Snake[3];
	Snake[3] = Snake[2];
	Snake[2] = Snake[1];
	Snake[1] = Snake[0];
	if(SnakeMoveCount == 2){
		while(Snake[1] == Snake[0]){
			int q = ((int)Snake[0] + getMoveWidth() );
			if( q < 0 )q = q +20;
			Snake[0] = q % 20;
		}
		SnakeMoveCount = 0;
	}
}

//メイン関数
int main(void){
	
	uint8_t i;
	
	for(i=0 ; i < SNAKE_LENDGH; i++){
		Snake[i]= 15;
		SnakeMoveCount = 0;
	}
	
	while(1){
		for(uint8_t j = 0; j < 60; j++){
			for(uint8_t y = 0; y < 5 ; y++){
				for(uint8_t x = 0; x<4; x++){
					clsdisp();
					for(i = 0; i < SNAKE_LENDGH; i++){
						if(Snake[i] == (y*4) + x){
							onLED(x, y);
						}
					}
				}
			}
		}
		
		moveSnake();
		
	}
}
