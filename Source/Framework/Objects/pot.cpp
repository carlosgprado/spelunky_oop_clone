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
#include "pot.h"

namespace game_framework {


	void Pot::initiate(void){
		initiate(1);
	}

	void Pot::initiate(int input) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::type = 2;
		level = input;
		Object::name = "pot";
		Object::x_velocity = Object::y_velocity = 0;
		Object::canMove = Object::isActive = TRUE;


		pic1.LoadBitmap("Pictures/Object/Item/pot.bmp", RGB(255, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();

		Object::canPickup = TRUE;
		Object::canBreak = TRUE;

		Object::elasticity = 0.3;
		toughness = 1;
	}



	

	void Pot::obj_damage(UI *player, Object *)
	{
		
		Object::isShowed = FALSE;
		Object::isActive = FALSE;
		Object::canMove = FALSE;
		Object::canBreak = FALSE;

		//Create Coins
		int num_coins = std::rand() % (max_coins - min_coins) + min_coins;
		int i;
		for (i = 0; i < num_coins; i++) {
			int money_level;
			switch (level) {
			case 1:
				money_level = 10;
				break;
			case 2:
				money_level = 20;
				break;
			case 3:
				money_level = 30;
				break;
			case 4:
				money_level = 40;
				break;
			default:
				money_level = 10;
			}

			int temp_num = player->obj_stack->new_obj(1);
			Object *temp = player->obj_stack->get_obj(temp_num);
			temp->initiate(money_level);
			temp->x = Object::x + (int)(Object::width / 2);
			temp->y = Object::y + (int)(Object::height / 6);
			temp->y_velocity = rand() % 15 + 15;
			temp->x_velocity = rand() % 11 - 5;
			temp->cool_down = (int)(CRATE_COIN_HOT_TIME / (GAME_CYCLE_TIME));
		}
	}

	int Pot::query_int(char * statement)
	{
		if (statement == "toughness") {
			return(toughness);
		}
		return 0;
	}
}
