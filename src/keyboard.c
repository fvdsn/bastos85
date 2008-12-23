#include <stdio.h>
#include "keyboard.h"
#define KEY_COUNT 256
int pressed[KEY_COUNT];
#define UP 1
#define DOWN -1
#define DOWNUP 2
#define UNCHANGED 0
int changes[KEY_COUNT];
void key_down(unsigned char key){
	changes[key] = DOWN;
}
void key_up(unsigned char key){
	switch(changes[key]){
		case UP:
		case UNCHANGED:
			changes[key] = UP;
			break;
		case DOWN :
		case DOWNUP:
			changes[key] = DOWNUP;
			break;
	}
}
int key_pressed(unsigned char key){
	return pressed[key];
}
void keyboard_frame(){
	int k = KEY_COUNT;
	while(k--){
		switch(changes[k]){
			case UP:
				pressed[k] = 0;
				changes[k] = UNCHANGED;
				break;
			case DOWN:
				pressed[k] = 1;
				changes[k] = UNCHANGED;
				break;
			case DOWNUP:
				printf("downup:%c\n",k);
				pressed[k] = 1;
				changes[k] = UP;
				break;
		}
		
	}
}
