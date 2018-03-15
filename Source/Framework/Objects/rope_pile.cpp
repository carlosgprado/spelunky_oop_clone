#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "rope_pile.h"

namespace game_framework {

	void Rope_Pile::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 4;
		Object::canMove = Object::isActive = canTouch = true;

		pic1.LoadBitmap("Pictures/Object/Item/rope_pack.bmp", RGB(255, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();

		Object::elasticity = 0;
	}


	void Rope_Pile::onTouch(UI *player)
	{
		
		Object::isActive = FALSE;
		Object::isShowed = FALSE;
		player->gain_rope(ROPE_PER_PILE);
	}
}
