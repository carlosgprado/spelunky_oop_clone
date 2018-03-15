#include "StdAfx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "mygame.h"


namespace game_framework {
	Numbers::Numbers()
	{
	}
	void Numbers::initiate(void){
		current_value = 0;
		max_value = min_value = 0;
		x = y = 0;
		width = 300;

		//Load Bitmaps
		int i;
		for (i = 0; i <= MAX_DIGITS; i++) {
			pic[i][0].LoadBitmap("Pictures/UI/0.bmp", RGB(255, 0, 0));
			pic[i][1].LoadBitmap("Pictures/UI/1.bmp", RGB(255, 0, 0));
			pic[i][2].LoadBitmap("Pictures/UI/2.bmp", RGB(255, 0, 0));
			pic[i][3].LoadBitmap("Pictures/UI/3.bmp", RGB(255, 0, 0));
			pic[i][4].LoadBitmap("Pictures/UI/4.bmp", RGB(255, 0, 0));
			pic[i][5].LoadBitmap("Pictures/UI/5.bmp", RGB(255, 0, 0));
			pic[i][6].LoadBitmap("Pictures/UI/6.bmp", RGB(255, 0, 0));
			pic[i][7].LoadBitmap("Pictures/UI/7.bmp", RGB(255, 0, 0));
			pic[i][8].LoadBitmap("Pictures/UI/8.bmp", RGB(255, 0, 0));
			pic[i][9].LoadBitmap("Pictures/UI/9.bmp", RGB(255, 0, 0));
			pic[i][10].LoadBitmap("Pictures/UI/+.bmp", RGB(255, 0, 0));
			digits[i] = NULL;
		}
	}
	
	void Numbers::set_max(int input){
		max_value = input;
	}

	void Numbers::set_num(int input){
		if (input > max_value) {
			current_value = max_value;
		}
		else if (input < min_value) {
			current_value = min_value;
		}
		else {
			current_value = input;
		}
		update();
	}

	void Numbers::increase(void){
		if (current_value + 1 <= max_value) {
			current_value++;
		}
		update();
	}

	void Numbers::increase(int input){
		if (current_value + input <= max_value) {
			current_value += input;
		} else {
			current_value = max_value;
		}
		update();
	}

	void Numbers::decrease(void){
		if (current_value - 1 >= min_value) {
			current_value--;
		}
		update();
	}

	void Numbers::decrease(int input){
		if (current_value - input >= min_value) {
			current_value -= input;
		} else {
			current_value = min_value;
		}
		update();
	}

	int Numbers::get_current(void){
		return current_value;
	}
	
	void Numbers::update(void) {

		current_digits = 0;
		int i, temp = current_value;
		for (i = 0; i <= MAX_DIGITS; i++) {
			if (temp != 0) {
				current_digits++;
				digits[MAX_DIGITS - i] = &pic[MAX_DIGITS - i][temp % 10];
				temp = (temp - (temp % 10)) / 10;
			}
			else if (current_digits == 0) {
				current_digits++;
				digits[MAX_DIGITS] = &pic[MAX_DIGITS][0];
			}
			else {
				digits[MAX_DIGITS - i] = NULL;
			}
		}

		if (isAdd == TRUE) {
			digits[MAX_DIGITS - current_digits] = &pic[MAX_DIGITS - current_digits][10];
			current_digits++;
		}

		if (isTime == TRUE && current_digits == 1) {
			current_digits++;
			digits[MAX_DIGITS - 1] = &pic[MAX_DIGITS - 1][0];
		}

		//Update Location
		int each_width = pic[0][0].Width();
		int mid = x + (width / 2);
		int start;

		if (current_digits % 2 == 0) {
			start = mid - each_width * (current_digits / 2 + (MAX_DIGITS +1 - current_digits));
		}
		else {
			start = mid - each_width * ((current_digits + 1) / 2 + (MAX_DIGITS +1 - current_digits)) + (int)((double)each_width / 2);
		}


		for (i = 0; i <= MAX_DIGITS; i++) {
			if (digits[i] != NULL) {
				digits[i]->SetTopLeft(start + each_width*i, y);
			}
		}
	}



	void Numbers::set_location(int in_x, int in_y, int in_width){
		x = in_x;
		y = in_y;
		width = in_width;
		update();
	}

	void Numbers::onShow(void) {
		int i;
		for (i = MAX_DIGITS ; i > MAX_DIGITS - current_digits; i--) {
			digits[i]->ShowBitmap();
		}
	}

}
