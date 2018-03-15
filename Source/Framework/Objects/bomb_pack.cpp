#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "bomb_pack.h"

namespace game_framework {

	void Bomb_Pack::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 3;
		Object::canMove = canTouch =  Object::isActive = true;


		pic1.LoadBitmap("Pictures/Object/Item/bomb_pack.bmp", RGB(255, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();


		Object::elasticity = 0;
	}

	void Bomb_Pack::onTouch(UI *player)
	{

		Object::isActive = FALSE;
		Object::isShowed = FALSE;
		player->gain_bomb(BOMB_PER_PACK);
	}
}
