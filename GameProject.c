#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
	
#define BLACK 0x0000 	
#define NAVY 0x000F 	
#define DGREEN 0x03E0 	
#define DCYAN 0x03EF 	
#define MAROON 0x7800 	
#define PURPLE 0x780F 	
#define OLIVE 0x7BE0 	
#define DGRAY 0x7BEF 	 
#define BLUE 0x001F 	
#define GREEN 0x07E0 	
#define CYAN 0x07FF 	 
#define RED 0xF800 	
#define MAGENTA 0xF81F 	
#define YELLOW 0xFFE0 	
#define WHITE 0xFFFF	
#define LGRAY 0xC618 	
	
void plot_pixel(int x, int y, short int line_color);
void draw_first_wall();
void draw_second_wall();
void draw_third_wall();
void draw_fourth_wall();
void draw_box(int x, int y, int width, int height, short int color);
void change_wall_color();
void clear_screen();
int rgb_to_pixel_color(int r, int g, int b);
void draw_line(int x0, int y0, int x1, int y1, short int color);
void swap(int *xp, int *yp);
void change_hole_numbers(int wall_id);
void score_counting();
void level_difficulty_increment();
void show_HEX();
void draw_helicopter();
void check_end_game();
void control_move();
void draw_helicopter_upgrade();
void draw_fighter();
void draw_coin(int dx, int dy);
void draw_plane();
void draw_end();
void change_speed_with_switch();

// global variable
char seg7[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x40};
int value1 = 0;
int value2 = 0;

volatile int pixel_buffer_start; 
// length of each plane
int plane_length = 30;

const int protect_value = 10;
// wall data combo
int move_step = 1;
//****************************//
// wall color
int color_r = 50;
int color_g = 50;
int color_b = 50;
int random_number1 = 53;
// codition for First wall
int x1 = 319;
bool dir1 = false;
int wall_1[8] = {2, 1, 0, 0, 1, 0, 1, 1};
// condition for Second wall
int x2 = 399;
bool start = false;
bool dir2 = false;
int wall_2[8] = {1, 1, 0, 1, 0, 1, 1, 0};
// condition for third wall
int x3 =479;
bool start3 = false;
bool dir3 = false;
int wall_3[8] = {1, 1, 1, 1, 0, 2, 0, 1};
// condition for fourth
int x4 =559;
bool start4 = false;
bool dir4 = false;
int wall_4[8] = {1, 2, 1, 0, 1, 0, 1, 1};
//****************************//

// change the shape of object
bool change_shape = false;
int change_object_count = 0;
// control moving

volatile int * SW_ptr         = 0xFF200040;     // SW slide switch address
volatile int * LEDR_ptr       = 0xFF200000;    // red LED address
volatile int * KEY_ptr       = 0xFF200050;    // red LED address
int switch_value = 0;
int key_value = 0;
bool move_up = false;
bool move_down = false;
bool choice_speed_up = false;
bool choice_speed_up2 = false;

// 
bool add_level_difficulty =false;
// score counting variables
int score_0 = 0;
int score_1 = 0;
int score_2 = 0;
int move_up_and_down = 0;

int main(void) {
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	// initial clear all scree into black
	clear_screen();
	// starting of moving map
	while(1) {
		switch_value = *SW_ptr;
		key_value = *KEY_ptr;
		
		// level speed change manually
		change_speed_with_switch();
		// check if end game
		check_end_game();
		check_collect_coin_event();
		// control moving
		control_move();
		// draw the object
		draw_plane();
		// draw wall 1, 2, 3, 4
		draw_first_wall();
		draw_second_wall();
		draw_third_wall();
		draw_fourth_wall();
		// change all wall color
		change_wall_color();
		
		// score counting
		score_counting();
		// level_difficulty
		level_difficulty_increment();
		// showing score and level into HEX
		show_HEX();
	}
}

void change_speed_with_switch(){
	// level 1
		if((switch_value == 1 || switch_value == 3) && choice_speed_up == false){
			move_step = move_step + 2;
			choice_speed_up = true;
		}
		else if((switch_value == 0 || switch_value == 2) && choice_speed_up == true){
			move_step = move_step - 2;
			choice_speed_up = false;
		}
	// level 2
		if((switch_value == 2 || switch_value == 3) && choice_speed_up2 == false){
			move_step = move_step + 2;
			choice_speed_up2 = true;
		}
		else if((switch_value == 0 || switch_value == 1) && choice_speed_up2 == true){
			move_step = move_step - 2;
			choice_speed_up2 = false;
		}
}


void control_move(){
	//KEY feature
	if(key_value == 1){
		while(key_value == 1){
			key_value = *KEY_ptr;
			if(choice_speed_up && choice_speed_up2){
				*LEDR_ptr = 515;
			}
			else if(choice_speed_up){
				*LEDR_ptr = 513;
			}
			else if(choice_speed_up2){
				*LEDR_ptr = 514;
			}
			
			else{
				*LEDR_ptr = 512;
			}

		}
		move_up = true;
	}
	if(key_value == 2){
		while(key_value == 2){
			key_value = *KEY_ptr;
			if(choice_speed_up && choice_speed_up2){
				*LEDR_ptr = 515;
			}
			else if(choice_speed_up){
				*LEDR_ptr = 513;
			}
			else if(choice_speed_up2){
				*LEDR_ptr = 514;
			}
			
			else{
				*LEDR_ptr = 512;
			}
		}
		move_down = true;
	}
	
	if(choice_speed_up && choice_speed_up2){
		*LEDR_ptr = 3;
	}
	else if(choice_speed_up){
		*LEDR_ptr = 1;
	}
	else if(choice_speed_up2){
		*LEDR_ptr = 2;
	}
	else{
		*LEDR_ptr = 0;
	}
		
	if(move_down && move_up_and_down < 210){
		draw_box(0,move_up_and_down, 52,30, LGRAY);
		move_up_and_down = move_up_and_down + 30;
		move_down = false;
	}

	else if(move_up && move_up_and_down > 29){
		draw_box(0,move_up_and_down, 52,30, LGRAY);
		move_up_and_down = move_up_and_down - 30;
		move_up = false;
	}
	move_down = false;
	move_up = false;
}

void draw_end(){
	/*letter E*/
	draw_box(124, 105, 18, 6, BLACK);
	draw_box(118, 105, 6, 30, BLACK);
	draw_box(124, 117, 12, 6, BLACK);
	draw_box(124, 129, 18, 6, BLACK);
	/*letter N*/
	draw_box(148, 105, 6, 30, BLACK);
	draw_box(154, 111, 6, 6, BLACK);
	draw_box(160, 117, 6, 6, BLACK);
	draw_box(166, 105, 6, 30, BLACK);
	/*letter D*/
	draw_box(178, 105, 6, 30, BLACK);
	draw_box(184, 105, 12, 6, BLACK);
	draw_box(184, 129, 12, 6, BLACK);
	draw_box(196, 111, 6, 18, BLACK);
	
}

void check_collect_coin_event(){
	if(wall_1[move_up_and_down/30] == 2 && x1 < (plane_length + move_step)){
		wall_1[move_up_and_down/30] = 0;
		draw_box(plane_length+1, move_up_and_down, 30, 30, LGRAY);
		change_shape =true;
	}
	else if(wall_2[move_up_and_down/30] == 2 && x2 <(plane_length + move_step)){
		wall_2[move_up_and_down/30] = 0;
		draw_box(plane_length+1, move_up_and_down, 30, 30, LGRAY);
		change_shape = true;
	}
	else if(wall_3[move_up_and_down/30] == 2 && x3 <(plane_length + move_step)){
		wall_3[move_up_and_down/30] = 0;
		draw_box(plane_length+1, move_up_and_down, 30, 30, LGRAY);
		change_shape = true;
	}
	else if(wall_4[move_up_and_down/30] == 2 && x4 <(plane_length + move_step)){
		wall_4[move_up_and_down/30] = 0;
		draw_box(plane_length+1, move_up_and_down, 30, 30, LGRAY);
		change_shape = true;
	}

}

void draw_plane(){
	if(change_shape == true){
		draw_box(0, move_up_and_down, plane_length+1, 30, LGRAY);
		change_object_count++;
		if(change_object_count == 3){
			change_object_count = 0;
		}
		change_shape = false;
	}
	if(change_object_count == 0){
		
		draw_helicopter();
	}
	else if(change_object_count == 1){
		draw_helicopter_upgrade();
	}
	else if(change_object_count == 2){
		draw_fighter();
	}
}

void check_end_game(){
	if((wall_1[move_up_and_down/30] == 1 && x1 <plane_length + move_step) || 
	   (wall_2[move_up_and_down/30] == 1 && x2 <plane_length + move_step) ||
	   (wall_3[move_up_and_down/30] == 1 && x3 <plane_length + move_step) ||
	   (wall_4[move_up_and_down/30] == 1 && x4 <plane_length + move_step) ){
		// end of game
		clear_screen();
		while(key_value == 0){
			draw_end();
			key_value = *KEY_ptr;
			*LEDR_ptr = key_value;	
			
		}
		clear_screen();
		change_object_count = 0;
		x1 = 319;
		x4 =559;
		x3 =479;
		x2 = 399;
		move_up_and_down = 0;
		dir1 = false;
		dir2 = false;
		dir3 = false;
		dir4 = false;
		move_step = 1;
		score_0 = 0;
		score_1 = 0;
		score_2 = 0;
		value1 = 0;
		value2 = 0;
	}
}

void draw_coin(int dx, int dy){
	// dx starting at left up corner
	draw_line(0+dx, 4+dy, 0+dx, 11+dy, BLACK);
	
	draw_line(1+dx, 2+dy, 1+dx, 4+dy, BLACK);
	draw_line(1+dx, 5+dy, 1+dx, 10+dy, WHITE);
	draw_line(1+dx, 11+dy, 1+dx, 13+dy, BLACK);
	
	plot_pixel(2+dx, 2+dy, BLACK);
	plot_pixel(2+dx, 13+dy, BLACK);
	plot_pixel(2+dx, 3+dy, WHITE);
	plot_pixel(2+dx, 4+dy, WHITE);
	plot_pixel(2+dx, 11+dy, WHITE);
	plot_pixel(2+dx, 12+dy, WHITE);
	draw_line(2+dx, 5+dy, 2+dx, 10+dy, YELLOW);
	draw_line(3+dx, 3+dy, 3+dx, 12+dy, YELLOW);
	draw_line(4+dx, 3+dy, 4+dx, 13+dy, YELLOW);
	plot_pixel(3+dx, 2+dy, WHITE);
	plot_pixel(4+dx, 2+dy, WHITE);
	plot_pixel(3+dx, 13+dy, WHITE);
	draw_line(2+dx, 1+dy, 4+dx, 1+dy, BLACK);
	draw_line(2+dx, 14+dy, 4+dx, 14+dy, BLACK);
	draw_line(4+dx, 0+dy, 9+dx, 0+dy, BLACK);
	draw_line(4+dx, 15+dy, 9+dx, 15+dy, BLACK);
	draw_line(8+dx, 1+dy, 11+dx, 1+dy, BLACK);
	draw_line(8+dx, 14+dy, 11+dx, 14+dy, BLACK);
	draw_line(11+dx, 2+dy, 11+dx, 4+dy, BLACK);
	draw_line(11+dx, 11+dy, 11+dx, 13+dy, BLACK);
	draw_line(12+dx, 3+dy, 12+dx, 12+dy, BLACK);
	draw_line(13+dx, 5+dy, 13+dx, 10+dy, BLACK);
	plot_pixel(10+dx, 2+dy, BLACK);
	plot_pixel(10+dx, 13+dy, BLACK);
	
	draw_line(8+dx, 3+dy, 8+dx, 12+dy, BLACK);
	draw_line(5+dx, 12+dy, 7+dx, 12+dy, BLACK);
	
	draw_line(5+dx, 1+dy, 7+dx, 1+dy, WHITE);
	draw_line(5+dx, 3+dy, 7+dx, 3+dy, WHITE);
	draw_line(5+dx, 4+dy, 5+dx, 11+dy, WHITE);
	
	
	draw_line(6+dx, 4+dy, 6+dx, 11+dy, YELLOW);
	draw_line(7+dx, 4+dy, 7+dx, 11+dy, YELLOW);
	draw_line(9+dx, 2+dy, 9+dx, 13+dy, YELLOW);
	draw_line(10+dx, 3+dy, 10+dx, 12+dy, YELLOW);
	draw_line(11+dx, 5+dy, 11+dx, 10+dy, YELLOW);
	draw_line(5+dx, 2+dy, 8+dx, 2+dy, YELLOW);
	draw_line(5+dx, 13+dy, 8+dx, 13+dy, YELLOW);
	draw_line(5+dx, 14+dy, 7+dx, 14+dy, YELLOW);
}

void draw_fighter(){
	plane_length = 52;
	
	draw_line(11, 6+move_up_and_down, 14, 6+move_up_and_down, BLACK);
	plot_pixel(11, 7+move_up_and_down, BLACK);
	draw_line(12, 7+move_up_and_down, 14, 7+move_up_and_down, RED);
	plot_pixel(15, 7+move_up_and_down, BLACK);
	
	plot_pixel(12, 8+move_up_and_down, BLACK);
	plot_pixel(15, 8+move_up_and_down, BLACK);
	plot_pixel(13, 8+move_up_and_down, DGRAY); 
	plot_pixel(14, 8+move_up_and_down, DGRAY); 
	
	plot_pixel(12, 9+move_up_and_down, BLACK);
	plot_pixel(16, 9+move_up_and_down, BLACK);
	draw_line(13, 9+move_up_and_down, 15, 9+move_up_and_down, DGRAY);
	
	plot_pixel(13, 10+move_up_and_down, BLACK);
	draw_line(14, 10+move_up_and_down, 16, 10+move_up_and_down, DGRAY);
	plot_pixel(17, 10+move_up_and_down, BLACK);
	draw_line(22, 10+move_up_and_down, 26, 10+move_up_and_down, BLACK);
	
	draw_line(8, 11+move_up_and_down, 11, 11+move_up_and_down, BLACK);
	plot_pixel(13, 11+move_up_and_down, BLACK);
	draw_line(14, 11+move_up_and_down, 16, 11+move_up_and_down, DGRAY);
	plot_pixel(17, 11+move_up_and_down, BLACK);
	plot_pixel(22, 11+move_up_and_down, BLACK);
	draw_line(23, 11+move_up_and_down, 25, 11+move_up_and_down, YELLOW);
	plot_pixel(27, 11+move_up_and_down, BLACK);
	
	plot_pixel(8, 12+move_up_and_down, BLACK);
	draw_line(9, 12+move_up_and_down, 11, 12+move_up_and_down, RED);
	plot_pixel(12, 12+move_up_and_down, BLACK);
	plot_pixel(13, 12+move_up_and_down, BLACK);
	draw_line(14, 11+move_up_and_down, 17, 12+move_up_and_down, DGRAY);
	plot_pixel(18, 12+move_up_and_down, BLACK);
	plot_pixel(22, 12+move_up_and_down, BLACK);
	draw_line(23, 12+move_up_and_down, 25, 12+move_up_and_down, DGRAY);
	plot_pixel(28, 12+move_up_and_down, BLACK);
	draw_line(31, 12+move_up_and_down, 38, 12+move_up_and_down, BLACK);
	
	plot_pixel(9, 13+move_up_and_down, BLACK);
	draw_line(10, 13+move_up_and_down, 13, 13+move_up_and_down, DGRAY);
	plot_pixel(14, 13+move_up_and_down, BLACK);
	draw_line(15, 13+move_up_and_down, 18, 13+move_up_and_down, DGRAY);
	plot_pixel(19, 13+move_up_and_down, BLACK);
	plot_pixel(21, 13+move_up_and_down, BLACK);
	draw_line(22, 13+move_up_and_down, 24, 13+move_up_and_down, YELLOW);
	draw_line(25, 13+move_up_and_down, 30, 13+move_up_and_down, BLACK);
	draw_line(31, 13+move_up_and_down, 34, 13+move_up_and_down, CYAN);
	plot_pixel(35, 13+move_up_and_down, BLACK);
	draw_line(36, 13+move_up_and_down, 38, 13+move_up_and_down, CYAN);
	plot_pixel(39, 13+move_up_and_down, BLACK);
	plot_pixel(40, 13+move_up_and_down, BLACK);
	
	plot_pixel(10, 14+move_up_and_down, BLACK);
	draw_line(11, 14+move_up_and_down, 14, 14+move_up_and_down, DGRAY);
	draw_line(15, 14+move_up_and_down, 24, 14+move_up_and_down, BLACK);
	draw_line(25, 14+move_up_and_down, 30, 14+move_up_and_down, DGRAY);
	plot_pixel(31, 14+move_up_and_down, BLACK);
	plot_pixel(32, 14+move_up_and_down, BLACK);
	plot_pixel(33, 14+move_up_and_down, CYAN);
	plot_pixel(34, 14+move_up_and_down, CYAN);
	plot_pixel(35, 14+move_up_and_down, BLACK);
	draw_line(36, 14+move_up_and_down, 40, 14+move_up_and_down, CYAN);
	plot_pixel(41, 14+move_up_and_down, BLACK);
	plot_pixel(42, 14+move_up_and_down, BLACK);
	
	draw_line(6, 15+move_up_and_down, 10, 15+move_up_and_down, BLACK);
	draw_line(11, 15+move_up_and_down, 15, 15+move_up_and_down, DGRAY);
	plot_pixel(16, 15+move_up_and_down, BLACK);
	plot_pixel(17, 15+move_up_and_down, BLACK);
	draw_line(18, 15+move_up_and_down, 32, 15+move_up_and_down, WHITE);
	plot_pixel(33, 15+move_up_and_down, BLACK);
	plot_pixel(34, 15+move_up_and_down, BLACK);
	plot_pixel(35, 15+move_up_and_down, DGRAY);
	draw_line(36, 15+move_up_and_down, 40, 15+move_up_and_down, BLACK);
	plot_pixel(41, 15+move_up_and_down, DGRAY);
	plot_pixel(42, 15+move_up_and_down, DGRAY);
	draw_line(43, 15+move_up_and_down, 46, 15+move_up_and_down, BLACK);
	
	plot_pixel(5, 16+move_up_and_down, BLACK);
	plot_pixel(6, 16+move_up_and_down, RED);
	draw_line(7, 16+move_up_and_down, 10, 16+move_up_and_down, DGRAY);
	plot_pixel(11, 16+move_up_and_down, BLACK);
	draw_line(12, 16+move_up_and_down, 17, 16+move_up_and_down, DGRAY);
	draw_line(18, 16+move_up_and_down, 25, 16+move_up_and_down, BLACK);
	draw_line(26, 16+move_up_and_down, 46, 16+move_up_and_down, WHITE);
	draw_line(47, 16+move_up_and_down, 49, 16+move_up_and_down, BLACK);
	
	plot_pixel(5, 17+move_up_and_down, BLACK);
	plot_pixel(6, 17+move_up_and_down, RED);
	draw_line(7, 17+move_up_and_down, 10, 17+move_up_and_down, BLACK);
	plot_pixel(11, 17+move_up_and_down, WHITE);
	draw_line(12, 17+move_up_and_down, 17, 17+move_up_and_down, BLACK);
	draw_line(18, 17+move_up_and_down, 25, 17+move_up_and_down, WHITE);
	draw_line(26, 17+move_up_and_down, 36, 17+move_up_and_down, BLACK);
	draw_line(37, 17+move_up_and_down, 48, 17+move_up_and_down, WHITE);
	plot_pixel(49, 17+move_up_and_down, RED);
	plot_pixel(50, 17+move_up_and_down, BLACK);
	
	plot_pixel(6, 18+move_up_and_down, BLACK);
	draw_line(7, 18+move_up_and_down, 10, 18+move_up_and_down, WHITE);
	plot_pixel(11, 18+move_up_and_down, BLACK);
	draw_line(12, 18+move_up_and_down, 16, 18+move_up_and_down, WHITE);
	draw_line(17, 18+move_up_and_down, 27, 18+move_up_and_down, BLACK);
	draw_line(28, 18+move_up_and_down, 37, 18+move_up_and_down, WHITE);
	plot_pixel(38, 18+move_up_and_down, WHITE);
	draw_line(39, 18+move_up_and_down, 48, 18+move_up_and_down, WHITE);
	plot_pixel(49, 18+move_up_and_down, RED);
	plot_pixel(50, 18+move_up_and_down, RED);
	plot_pixel(51, 18+move_up_and_down, BLACK);
	
	plot_pixel(5, 19+move_up_and_down, WHITE);
	draw_line(6, 19+move_up_and_down, 8, 19+move_up_and_down, WHITE);
	draw_line(9, 19+move_up_and_down, 13, 19+move_up_and_down, BLACK);
	plot_pixel(14, 19+move_up_and_down, WHITE);
	plot_pixel(15, 19+move_up_and_down, WHITE);
	plot_pixel(16, 19+move_up_and_down, WHITE);
	draw_line(17, 19+move_up_and_down, 24, 19+move_up_and_down, WHITE);
	draw_line(25, 19+move_up_and_down, 35, 19+move_up_and_down, BLACK);
	draw_line(36, 19+move_up_and_down, 40, 19+move_up_and_down, WHITE);
	draw_line(41, 19+move_up_and_down, 51, 19+move_up_and_down, BLACK);
	
	plot_pixel(4, 20+move_up_and_down, BLACK);
	plot_pixel(5, 20+move_up_and_down, WHITE);
	plot_pixel(6, 20+move_up_and_down, WHITE);
	plot_pixel(7, 20+move_up_and_down, BLACK);
	plot_pixel(8, 20+move_up_and_down, BLACK);
	plot_pixel(14, 20+move_up_and_down, BLACK);
	plot_pixel(15, 20+move_up_and_down, BLACK);
	draw_line(16, 20+move_up_and_down, 21, 20+move_up_and_down, WHITE);
	draw_line(22, 20+move_up_and_down, 24, 20+move_up_and_down, BLACK);
	draw_line(25, 20+move_up_and_down, 31, 20+move_up_and_down, DGRAY);
	plot_pixel(32, 20+move_up_and_down, RED);
	plot_pixel(33, 20+move_up_and_down, RED);
	draw_line(34, 20+move_up_and_down, 40, 20+move_up_and_down, BLACK);
	
	draw_line(4, 21+move_up_and_down, 6, 21+move_up_and_down, BLACK);
	plot_pixel(13, 21+move_up_and_down, BLACK);
	draw_line(14, 21+move_up_and_down, 19, 21+move_up_and_down, WHITE);
	plot_pixel(20, 21+move_up_and_down, BLACK);
	plot_pixel(21, 21+move_up_and_down, BLACK);
	draw_line(25, 21+move_up_and_down, 33, 21+move_up_and_down, BLACK);
	

	plot_pixel(12, 22+move_up_and_down, BLACK);
	draw_line(13, 22+move_up_and_down, 17, 22+move_up_and_down, WHITE);
	draw_line(18, 22+move_up_and_down, 23, 22+move_up_and_down, BLACK);
	
	plot_pixel(11, 23+move_up_and_down, BLACK);
	draw_line(12, 23+move_up_and_down, 16, 23+move_up_and_down, WHITE);
	plot_pixel(17, 23+move_up_and_down, BLACK);
	plot_pixel(18, 23+move_up_and_down, BLACK);
	draw_line(19, 23+move_up_and_down, 22, 23+move_up_and_down, DGRAY);
	plot_pixel(23, 23+move_up_and_down, RED);
	plot_pixel(24, 23+move_up_and_down, RED);
	plot_pixel(25, 23+move_up_and_down, BLACK);
	
	draw_line(11, 24+move_up_and_down, 15, 24+move_up_and_down, BLACK);
	draw_line(18, 24+move_up_and_down, 25, 24+move_up_and_down, BLACK);
}

void draw_helicopter_upgrade(){
	plane_length = 45;
	// draw wing
	draw_line(16, 8+move_up_and_down, 25, 8+move_up_and_down, BLACK);
	draw_line(33, 6+move_up_and_down, 40, 6+move_up_and_down, BLACK);
	draw_line(18, 7+move_up_and_down, 42, 7+move_up_and_down, BLACK);

	plot_pixel(29, 6+move_up_and_down, DGRAY); 
	plot_pixel(30, 6+move_up_and_down, DGRAY); 
	
	plot_pixel(29, 8+move_up_and_down, DGRAY); 
	plot_pixel(30, 8+move_up_and_down, DGRAY); 
	plot_pixel(29, 9+move_up_and_down, DGRAY); 
	plot_pixel(30, 9+move_up_and_down, DGRAY);
	
	// draw body
	draw_line(23, 10+move_up_and_down, 35, 10+move_up_and_down, YELLOW);
	draw_line(22, 11+move_up_and_down, 31, 11+move_up_and_down, YELLOW);
	draw_line(21, 12+move_up_and_down, 28, 12+move_up_and_down, YELLOW);
	draw_line(20, 13+move_up_and_down, 30, 13+move_up_and_down, YELLOW);
	draw_line(20, 14+move_up_and_down, 30, 14+move_up_and_down, YELLOW);
	draw_line(20, 15+move_up_and_down, 32, 15+move_up_and_down, YELLOW);
	draw_line(20, 16+move_up_and_down, 34, 16+move_up_and_down, YELLOW);
	
	//draw_line(22, 17, 31, 17, WHITE);
	//draw_line(22, 18, 31, 18, WHITE);
	// draw head
	
	
	draw_line(31, 11+move_up_and_down, 38, 11+move_up_and_down, BLUE);
	draw_line(29, 12+move_up_and_down, 39, 12+move_up_and_down, BLUE);
	draw_line(31, 13+move_up_and_down, 40, 13+move_up_and_down, BLUE);
	draw_line(31, 14+move_up_and_down, 40, 14+move_up_and_down, BLUE);
	draw_line(33, 15+move_up_and_down, 41, 15+move_up_and_down, BLUE);
	draw_line(35, 16+move_up_and_down, 39, 16+move_up_and_down, BLUE);
	
	
	// draw reflection windows
	plot_pixel(38, 12+move_up_and_down, WHITE); 
	plot_pixel(39, 13+move_up_and_down, WHITE); 
	plot_pixel(38, 13+move_up_and_down, WHITE); 
	
	// draw the base 
	draw_line(20, 17+move_up_and_down, 41, 17+move_up_and_down, YELLOW);
	draw_line(19, 18+move_up_and_down, 42, 18+move_up_and_down, YELLOW);
	draw_line(21, 19+move_up_and_down, 42, 19+move_up_and_down, YELLOW);
	draw_line(21, 20+move_up_and_down, 41, 20+move_up_and_down, YELLOW);
	draw_line(23, 21+move_up_and_down, 37, 21+move_up_and_down, YELLOW);
	
	//draw back wing
	// CYAN
	
	draw_line(8, 19+move_up_and_down, 20, 19+move_up_and_down, BLUE);
	draw_line(9, 20+move_up_and_down, 20, 20+move_up_and_down, BLUE);
	draw_line(9, 21+move_up_and_down, 22, 21+move_up_and_down, BLUE);
	draw_line(38, 21+move_up_and_down, 41, 21+move_up_and_down, BLUE);
	draw_line(24, 22+move_up_and_down, 38, 22+move_up_and_down, BLUE);
	
	draw_line(5, 18+move_up_and_down, 10, 18+move_up_and_down, BLUE);
	draw_line(5, 17+move_up_and_down, 9, 17+move_up_and_down, BLUE);
	draw_line(5, 16+move_up_and_down, 8, 16+move_up_and_down, BLUE);
	draw_line(5, 15+move_up_and_down, 7, 15+move_up_and_down, BLUE);
	
	plot_pixel(5, 14+move_up_and_down, BLACK);
	plot_pixel(6, 14+move_up_and_down, BLACK);
	plot_pixel(5, 13+move_up_and_down, BLACK); 
	
	
	draw_line(6, 17+move_up_and_down, 6, 22+move_up_and_down, BLACK);
	draw_line(4, 20+move_up_and_down, 8, 20+move_up_and_down, BLACK);
	
	//draw the foundation
	plot_pixel(26, 23+move_up_and_down, DGRAY);
	plot_pixel(27, 23+move_up_and_down, DGRAY);
	plot_pixel(34, 23+move_up_and_down, DGRAY);
	plot_pixel(35, 23+move_up_and_down, DGRAY);
	plot_pixel(26, 24+move_up_and_down, DGRAY);
	plot_pixel(27, 24+move_up_and_down, DGRAY);
	plot_pixel(34, 24+move_up_and_down, DGRAY);
	plot_pixel(35, 24+move_up_and_down, DGRAY);
	
	plot_pixel(38, 24+move_up_and_down, DGRAY);
	plot_pixel(39, 24+move_up_and_down, DGRAY);
	draw_line(21, 25+move_up_and_down, 38, 25+move_up_and_down, DGRAY);
	// decoration of 
	
}

void draw_helicopter(){	
	plane_length = 30;
	
	 draw_line(0, 6 + move_up_and_down, 18, 6+move_up_and_down, LGRAY); 
	 plot_pixel(19, 6+move_up_and_down, DGRAY); 
	 draw_line(20, 6+move_up_and_down, 21, 6+move_up_and_down, LGRAY);  
	 draw_line(22, 6+move_up_and_down, 27, 6+move_up_and_down, BLACK);  
	 draw_line(28, 6+move_up_and_down, 29, 6+move_up_and_down, LGRAY);  

	draw_line(0, 7+move_up_and_down, 10, 7+move_up_and_down, LGRAY);  
	draw_line(11, 7+move_up_and_down, 28, 7+move_up_and_down, BLACK);  
	plot_pixel(29, 7+move_up_and_down, DGRAY); 

	draw_line(0, 8+move_up_and_down, 9, 8+move_up_and_down, LGRAY);  
	draw_line(10, 8+move_up_and_down, 18, 8+move_up_and_down, BLACK);  
	plot_pixel(19, 8+move_up_and_down, LGRAY); 
	plot_pixel(20, 8+move_up_and_down, DGRAY); 
	draw_line(21, 8+move_up_and_down, 29, 8+move_up_and_down, LGRAY);  

	draw_line(0, 9+move_up_and_down, 16, 9+move_up_and_down, LGRAY);  
	draw_line(17, 9+move_up_and_down, 22, 9+move_up_and_down, WHITE);
	draw_line(23, 9+move_up_and_down, 29, 9+move_up_and_down, LGRAY); 

	draw_line(0, 10+move_up_and_down, 14, 10+move_up_and_down, LGRAY);  
	draw_line(15, 10+move_up_and_down, 20, 10+move_up_and_down, WHITE);
	draw_line(21, 10+move_up_and_down, 24, 10+move_up_and_down, BLUE); 
	draw_line(25, 10+move_up_and_down, 29, 10+move_up_and_down, LGRAY); 

	draw_line(0, 11+move_up_and_down, 12, 11+move_up_and_down, LGRAY); 
	draw_line(13, 11+move_up_and_down, 16, 11+move_up_and_down, WHITE);  
	plot_pixel(17, 11+move_up_and_down, YELLOW); 
	draw_line(18, 11+move_up_and_down, 19, 11+move_up_and_down, WHITE); 
	draw_line(20, 11+move_up_and_down, 25, 11+move_up_and_down, BLUE);  
	draw_line(26, 11+move_up_and_down, 29, 11+move_up_and_down, LGRAY);  

	draw_line(0, 12+move_up_and_down, 11, 12+move_up_and_down, LGRAY);  
	draw_line(12, 12+move_up_and_down, 15, 12+move_up_and_down, WHITE);  
	draw_line(16, 12+move_up_and_down, 18, 12+move_up_and_down, YELLOW); 
	plot_pixel(19, 12+move_up_and_down, WHITE); 
	draw_line(20, 12+move_up_and_down, 26, 12+move_up_and_down, BLUE);  
	draw_line(27, 12+move_up_and_down, 29, 12+move_up_and_down, LGRAY);  

	draw_line(0, 13+move_up_and_down, 10, 13+move_up_and_down, LGRAY); 
	draw_line(11, 13+move_up_and_down, 16, 13+move_up_and_down, WHITE);  
	plot_pixel(17, 13+move_up_and_down, YELLOW); 
	draw_line(18, 13+move_up_and_down, 19, 13+move_up_and_down, WHITE); 
	draw_line(20, 13+move_up_and_down, 26, 13+move_up_and_down, BLUE);  
	draw_line(27, 13+move_up_and_down, 29, 13+move_up_and_down, LGRAY); 

	plot_pixel(0, 14+move_up_and_down,LGRAY);
	plot_pixel(1, 14+move_up_and_down, BLACK); 
	plot_pixel(2, 14+move_up_and_down, BLACK); 
	draw_line(3, 14+move_up_and_down, 10, 14+move_up_and_down, LGRAY);  
	plot_pixel(11, 14+move_up_and_down, WHITE); 
	draw_line(12, 14+move_up_and_down, 16, 14+move_up_and_down, DGRAY);  
	draw_line(17, 14+move_up_and_down, 20, 14+move_up_and_down, WHITE); 
	draw_line(21, 14+move_up_and_down, 27, 14+move_up_and_down, BLUE); 
	draw_line(28, 14+move_up_and_down, 29, 14+move_up_and_down, LGRAY);  

	plot_pixel(0, 15+move_up_and_down, LGRAY); 
	plot_pixel(1, 15+move_up_and_down, BLACK); 
	plot_pixel(2, 15+move_up_and_down, BLACK); 
	draw_line(3, 15+move_up_and_down, 10, 15+move_up_and_down, LGRAY);  
	plot_pixel(11, 15+move_up_and_down, WHITE); 
	draw_line(12, 15+move_up_and_down, 16, 15+move_up_and_down, DGRAY);  
	draw_line(17, 15+move_up_and_down, 20, 15+move_up_and_down, WHITE);  
	draw_line(21, 15+move_up_and_down, 27, 15+move_up_and_down, BLUE);
	draw_line(28, 15+move_up_and_down, 29, 15+move_up_and_down, LGRAY);  

	plot_pixel(0, 16+move_up_and_down, LGRAY); 
	plot_pixel(2, 16+move_up_and_down, BLACK); 
	plot_pixel(3, 16+move_up_and_down, BLACK); 
	draw_line(4, 16+move_up_and_down, 9, 16+move_up_and_down, LGRAY);  
	plot_pixel(10, 16+move_up_and_down, BLACK); 
	draw_line(11, 16+move_up_and_down, 21, 16+move_up_and_down, WHITE);
	draw_line(22, 16+move_up_and_down, 25, 16+move_up_and_down, BLUE);  
	draw_line(26, 16+move_up_and_down, 28, 16+move_up_and_down, WHITE);
	plot_pixel(29, 16+move_up_and_down, LGRAY);

	draw_line(0, 17+move_up_and_down, 1, 17+move_up_and_down, LGRAY); 
	plot_pixel(2, 17+move_up_and_down, DGRAY);
	draw_line(3, 17+move_up_and_down, 10, 17+move_up_and_down, BLACK); 
	draw_line(11, 17+move_up_and_down, 29, 17+move_up_and_down, WHITE); 

    plot_pixel(2, 18+move_up_and_down, DGRAY); 
	plot_pixel(3, 18+move_up_and_down, DGRAY);
	draw_line(4, 18+move_up_and_down, 11, 18+move_up_and_down, BLACK);
	draw_line(12, 18+move_up_and_down, 29, 18+move_up_and_down, WHITE);

	draw_line(0, 19+move_up_and_down, 1, 19+move_up_and_down, LGRAY); 
	plot_pixel(2, 19+move_up_and_down, DGRAY); 
	draw_line(3, 19+move_up_and_down, 8, 19+move_up_and_down, LGRAY); 
	draw_line(9, 19+move_up_and_down, 13, 19+move_up_and_down, BLACK);  
	draw_line(14, 19+move_up_and_down, 27, 19+move_up_and_down, WHITE); 
	plot_pixel(28, 19+move_up_and_down, BLACK); 
	plot_pixel(29, 19+move_up_and_down, BLACK); 

	draw_line(0, 20+move_up_and_down, 10, 20+move_up_and_down, LGRAY); 
	draw_line(11, 20+move_up_and_down, 28, 20+move_up_and_down, BLACK);
	plot_pixel(29, 20+move_up_and_down, LGRAY); 

	draw_line(0, 21+move_up_and_down, 14, 21+move_up_and_down, LGRAY); 
	plot_pixel(15, 21+move_up_and_down, DGRAY); 
	draw_line(16, 21+move_up_and_down, 23, 21+move_up_and_down, LGRAY);  
	plot_pixel(24, 21+move_up_and_down, DGRAY); 
	draw_line(25, 21+move_up_and_down, 29, 21+move_up_and_down, LGRAY); 

	draw_line(0, 22+move_up_and_down, 14, 22+move_up_and_down, LGRAY);  
	plot_pixel(15, 22+move_up_and_down, DGRAY); 
	draw_line(16, 22+move_up_and_down, 23, 22+move_up_and_down, LGRAY); 
	plot_pixel(24, 22+move_up_and_down, DGRAY); 
	draw_line(25, 22+move_up_and_down, 26, 22+move_up_and_down, LGRAY);  
    plot_pixel(27, 22+move_up_and_down, DGRAY); 
	plot_pixel(28, 22+move_up_and_down, DGRAY); 
	plot_pixel(29, 22+move_up_and_down, LGRAY); 

	draw_line(0, 23+move_up_and_down, 10, 23+move_up_and_down, LGRAY); 
	draw_line(11, 23+move_up_and_down, 27, 23+move_up_and_down, DGRAY);  
	draw_line(28, 23+move_up_and_down, 29, 23+move_up_and_down, LGRAY);  
}	

void show_HEX(){
	volatile int * HEX3_0_ptr      = 0xFF200020;     // HEX3_HEX0 addressint value;
	volatile int * HEX5_4_ptr      = 0xFF200030;     // HEX5_HEX4 addressint value;
	*(HEX3_0_ptr) = seg7[value1 & 0xF] |  seg7[value2 & 0xF] << 8 | seg7[16] << 16 | seg7[score_0 & 0xF] << 24; 
	*(HEX5_4_ptr) = seg7[score_1 & 0xF] |  seg7[score_2 & 0xF] << 8 | seg7[16] << 16; 
}

// level difficulty, control the moving of wall
// value1 -> HEX0
// value2 -> HEX1
void level_difficulty_increment(){
	if(add_level_difficulty){
		if(value1 >= 9){
			value1=0;
			move_step++;
			if(value2 >= 9){
				value2 = 0;
			}
			else
				value2++;
		}
		else
			value1++;
		add_level_difficulty = false;
		move_step++;
	}
	
}

// counting the score on HEX3-5
// score_0 -> HEX3
// score_1 -> HEX4
// score_2 -> HEX5
void score_counting(){
	if(x1 <= 1+move_step || x2 <= 1+move_step || x3 <= 1+move_step || x4 <= 1+move_step){
		if(score_0 >= 9){
			score_0 = 0;
			add_level_difficulty = true;
			if(score_1 >= 9){
				score_1 = 0;
				if(score_2 >= 9){
					score_2 =0;
				}
				else
					score_2++;
			}
			else
				score_1++;

		}
		else 
			score_0++;
	}
}


//draw first wall
void draw_first_wall(){ 
	//void draw_coin(int dx, int dy){
	if (x1 <= 1+move_step){
		dir1 = true;	
		for(int j=0; j<8;j++){
			if(wall_1[j] == 1){
				for(int i=x1; i< x1+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30, LGRAY); 
			}
			else if(wall_1[j] == 2){
				draw_box(x1, j*30, 30+protect_value, 30+protect_value, LGRAY);
			}
		}
		
		change_hole_numbers(1);
	}

	if(!dir1){
		for(int j=0; j<8;j++){
			if(wall_1[j] == 1){
				for(int i=x1; i< x1+10; i++)
					draw_line(i, j*30, i, (j+1)*30,  rgb_to_pixel_color(color_r, color_g, color_b)); 	
				for(int i=x1+10; i<x1+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30, LGRAY); 
			}
			else if(wall_1[j] == 2){
				draw_box(x1, j*30, 30+protect_value, 30, LGRAY);
				draw_coin(x1, j*30+6);
			}
		}
	}	
	else{
		dir1 = false;
		x1 = 320;
	}
	x1=x1-move_step;
}

// draw second wall 
void draw_second_wall(){
	if (x2 <= 1+move_step){
		dir2 = true;	
		for(int j=0; j<8;j++){
			if(wall_2[j] == 1){
				for(int i=x2; i< x2+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30,  LGRAY); 
			}
			else if(wall_2[j] == 2){
				draw_box(x2, j*30, 30, 30, LGRAY);
			}
		}
		change_hole_numbers(2);
	}

	if(!dir2){
		for(int j=0; j<8;j++){
			if(wall_2[j] == 1){
				for(int i=x2; i< x2+10; i++)
					draw_line(i, j*30, i, (j+1)*30,  rgb_to_pixel_color(color_r, color_g, color_b)); 	
				for(int i=x2+10; i<x2+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30,  LGRAY); 
			}
			else if(wall_2[j] == 2){
				draw_box(x2, j*30, 30+protect_value, 30, LGRAY);
				draw_coin(x2, j*30+6);
			}
		}
	}	
	else{
		dir2 = false;
		x2 = 320;
	}
	x2=x2-move_step;
}

//draw third wall
void draw_third_wall(){
	if (x3 <= 1+move_step){
		dir3 = true;	
		for(int j=0; j<8;j++){
			if(wall_3[j] == 1){
				for(int i=x3; i< x3+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30,  LGRAY); 
			}
			else if(wall_3[j] == 2){
				draw_box(x3, j*30, 30+protect_value, 30+protect_value, LGRAY);
			}
		}
		change_hole_numbers(3);
	}
	
	if(!dir3){
		for(int j=0; j<8;j++){
			if(wall_3[j] == 1){
				for(int i=x3; i< x3+10; i++)
					draw_line(i, j*30, i, (j+1)*30,  rgb_to_pixel_color(color_r, color_g, color_b)); 	
				for(int i=x3+10; i<x3+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30,  LGRAY); 
			}
			else if(wall_3[j] == 2){
				draw_box(x3, j*30, 30+protect_value, 30, LGRAY);
				draw_coin(x3, j*30+6);
			}
		}
	}	
	else{
		dir3 = false;
		x3 = 320;
	}
	x3=x3-move_step;
}

void draw_fourth_wall(){
				// checkcolor condition
	if(x4 > 400 ){
		x4=x4-move_step;
		return;
	}
	
	if (x4 <= 1+move_step){
		dir4 = true;	
		for(int j=0; j<8;j++){
			if(wall_4[j] == 1){
				for(int i=x4; i< x4+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30,  LGRAY); 
			}
			else if(wall_4[j] == 2){
				draw_box(x4, j*30, 30+protect_value, 30, LGRAY);
			}
		}
		change_hole_numbers(4);
	}
	
	if(!dir4){
		for(int j=0; j<8;j++){
			if(wall_4[j] == 1){
				for(int i=x4; i< x4+10; i++)
					draw_line(i, j*30, i, (j+1)*30,  rgb_to_pixel_color(color_r, color_g, color_b)); 	
				for(int i=x4+10; i<x4+10+move_step+protect_value; i++)
					draw_line(i, j*30, i, (j+1)*30,  LGRAY); 
			}
			else if(wall_4[j] == 2){
				draw_box(x4, j*30, 30+protect_value, 30, LGRAY);
				draw_coin(x4, j*30+6);
			}
		}
	}	
	else{
		dir4 = false;
		x4 = 320;
	}
	x4=x4-move_step;
}

void change_wall_color(){
	if(score_0 == 9){
		color_r++;
		color_g = color_g + 1;
		color_b++;
		if(color_r == 0){
			color_r = 2;
		}
		if(color_g == 0){
			color_g = 2;
		}
		if(color_b == 0){
			color_b = 2;
		}
	}
}


// clear graph intio black
void clear_screen () {
    for (int x = 0; x < 320; x++) {
        for (int y = 0; y < 240; y++) {
            plot_pixel(x, y, LGRAY);
        }
    }
}

void plot_pixel(int x, int y, short int line_color) {
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

int rgb_to_pixel_color (int r, int g, int b) {
    return ((r<<8) + (g<<4) + b);
}

// draw line
void draw_line (int mx0, int my0, int mx1, int my1, short int color) {
    bool is_steep = abs(my1 - my0) > abs (mx1 -mx0);
    if (is_steep) {
        swap(&mx0, &my0);
        swap(&mx1, &my1);
    }
    if (mx0 > mx1) {
        swap(&mx0, &mx1);
        swap(&my0, &my1);
    }

    int deltax = mx1 - mx0;
    int deltay = abs(my1 - my0);
    int error = -(deltax / 2);
    int my = my0;
    int y_step;
    if (my0 < my1) {
        y_step = 1;
    } else {
        y_step = -1;
    }

    for (int mx = mx0; mx <= mx1; mx++) {
        if (is_steep) {
            plot_pixel(my, mx, color);
        } else {
            plot_pixel(mx, my, color);
        }
        error += deltay;
        if (error >= 0) {
            my += y_step;
            error -= deltax;
        }
    }
}

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void draw_box(int x, int y, int width, int height, short int color) {
	for (int x_increment = 0 ; x_increment < width; x_increment++) {
		for (int y_increment = 0 ; y_increment < height; y_increment++) {
			plot_pixel(x + x_increment, y + y_increment, color);
		}
	}
}

void change_hole_numbers(int wall_id){
	if(wall_id == 1){
		random_number1 = random_number1 + 7;
		if(random_number1 % 8 == 0)
			random_number1++;
		for(int i=0; i<8; i++)
			wall_1[i] = 1;
		wall_1[random_number1%7] = 0;
		random_number1 = random_number1 + 7;
		wall_1[random_number1%6] = 0;
		random_number1 = random_number1 + 3;
		wall_1[random_number1%5] = 0;
		
		if(score_0 != 0 && score_0%7 == 0){
			wall_1[random_number1%7] = 2;
		}
	}
	else if(wall_id == 2){
		random_number1 = random_number1 + 7;
		if(random_number1 % 8 == 0)
			random_number1++;
		for(int i=0; i<8; i++)
			wall_2[i] = 1;
		wall_2[random_number1%7] = 0;
		random_number1 = random_number1 + 7;
		wall_2[random_number1%6] = 0;
		random_number1 = random_number1 + 3;
		wall_2[random_number1%5] = 0;
		if(score_0 != 0 && score_0%3 == 0){
			wall_2[random_number1%7] = 2;
		}
	}
	else if(wall_id == 3){
		random_number1 = random_number1 + 7;
		if(random_number1 % 8 == 0)
			random_number1++;
		for(int i=0; i<8; i++)
			wall_3[i] = 1;
		wall_3[random_number1%7] = 0;
		random_number1 = random_number1 + 7;
		wall_3[random_number1%6] = 0;
		random_number1 = random_number1 + 3;
		wall_3[random_number1%5] = 0;
		
		if(score_0 != 0 && score_0%8 == 0){
			wall_3[random_number1%7] = 2;
		}
	}
	else if(wall_id == 4){
		random_number1 = random_number1 + 7;
		if(random_number1 % 8 == 0)
			random_number1++;
		for(int i=0; i<8; i++)
			wall_4[i] = 1;
		wall_4[random_number1%7] = 0;
		random_number1 = random_number1 + 7;
		wall_4[random_number1%6] = 0;
		random_number1 = random_number1 + 3;
		wall_4[random_number1%5] = 0;
	}
	
}
