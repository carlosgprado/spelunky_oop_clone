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

//random
#include <cstdlib>
#include <ctime>



namespace game_framework {
	
	void Coin::initiate(void) {
		initiate(0);
	}

	void Coin::initiate(int input) {
		Object::temp_x = Object::x = 0; 
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 1;
		Object::name = "coin";
		Object::canMove = canTouch = TRUE;
		Object::isActive = TRUE;


		if (input > 100) {
			input = 100;
		}

		int ran = std::rand() % (100-input) + input;
		if (ran >= 97) {
			pic1.LoadBitmap("Pictures/Object/Item/coin_6_2.bmp", RGB(0, 0, 255));
			money = ((std::rand() % 9) + 1) * 10000;
		}
		else if (ran >= 92) {
			pic1.LoadBitmap("Pictures/Object/Item/coin_5_2.bmp", RGB(0, 0, 255));
			money = ((std::rand() % 5) + 1) * 10000;
		}
		else if (ran >= 85) {
			pic1.LoadBitmap("Pictures/Object/Item/coin_4_2.bmp", RGB(0, 0, 255));
			money = ((std::rand() % 9) + 1) * 1000;

		}
		else if (ran >= 75) {
			pic1.LoadBitmap("Pictures/Object/Item/coin_3_2.bmp", RGB(0, 0, 255));
			money = ((std::rand() % 5) + 1) * 1000;
		}
		else if (ran >= 45) {
			pic1.LoadBitmap("Pictures/Object/Item/coin_2_2.bmp", RGB(0, 0, 255));
			money = ((std::rand() % 5) + 1) * 100;
		}
		else {
			pic1.LoadBitmap("Pictures/Object/Item/coin_1_2.bmp", RGB(0, 0, 255));
			money = ((std::rand() % 5) + 1) * 10;
		}

		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();


		Object::elasticity = 0.5;
	}



	void Coin::onTouch(UI *player)
	{

		Object::isActive = FALSE;
		Object::isShowed = FALSE;
		player->gain_money(money);
	}
}
