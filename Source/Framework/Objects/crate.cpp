#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "coin.h"
#include "crate.h"

namespace game_framework {


	void Crate::initiate(void){
		initiate(1);
	}

	void Crate::initiate(int input) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::type = 2;
		if (input <= 10) {

			level = input;
		}
		else {
			special = input - 10;
		}
		Object::name = "crate";
		Object::x_velocity = Object::y_velocity = 0;
		Object::canMove = Object::isActive = TRUE;


		pic1.LoadBitmap("Pictures/Object/Item/crate_closed_2.bmp", RGB(255, 0, 0));
		pic2.LoadBitmap("Pictures/Object/Item/crate_open_2.bmp", RGB(255, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();

		isOpen = FALSE;
		Object::canPickup = TRUE;
		Object::canBreak = TRUE;

		Object::elasticity = 0.3;
		toughness = 2;
	}

	void Crate::move(UI *){
		if (isOpen == TRUE && Object::cool_down == 0) {
			Object::isShowed = FALSE;
		}
	}


	

	void Crate::obj_damage(UI *player, Object *)
	{
		Object::cool_down = (int)(CRATE_HOT_TIME / GAME_CYCLE_TIME);
		Object::pic = &pic2;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();
		Object::x += (pic1.Width() - pic2.Width());
		isOpen = TRUE;
		canPickup = FALSE;

		Object::isActive = FALSE;
		Object::canMove = TRUE;
		Object::canBreak = FALSE;

		if (special != 0) {
			int temp_num = player->obj_stack->new_obj(special);
			Object *temp = player->obj_stack->get_obj(temp_num);
			temp->initiate();
			temp->x = Object::x + (int)(Object::width ) - temp->width;
			temp->y = Object::y + (int)(Object::height ) - temp->height;
			temp->cool_down = (int)(CRATE_COIN_HOT_TIME / (GAME_CYCLE_TIME));
		}
		else {
			//Create Coins
			int num_coins = std::rand() % (MAX_COIN - MIN_COIN) + MIN_COIN;
			int i;
			for (i = 0; i < num_coins; i++) {
				int money_level;
				switch (level) {
				case 1:
					money_level = 75;
					break;
				case 2:
					money_level = 85;
					break;
				case 3:
					money_level = 92;
					break;
				case 4:
					money_level = 97;
					break;
				default:
					money_level = 75;
				}

				int temp_num = player->obj_stack->new_obj(1);
				Object *temp = player->obj_stack->get_obj(temp_num);
				temp->initiate(money_level);
				temp->x = Object::x + (int)(Object::width / 2);
				temp->y = Object::y + (int)(Object::height / 6);
				temp->y_velocity = rand() % 25 + 25;
				temp->x_velocity = rand() % 17 - 8;
				temp->cool_down = (int)(CRATE_COIN_HOT_TIME / (GAME_CYCLE_TIME));
			}
		}
	}
	int Crate::query_int(char * statement)
	{
		if (statement == "toughness") {
			return(toughness);
		}
		return 0;
	}
}
