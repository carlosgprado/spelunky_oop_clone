#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "blood.h"

namespace game_framework {

	void Blood::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = Object::isActive = TRUE;


		pic1.LoadBitmap("Pictures/Object/Item/blood.bmp", RGB(0, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();

		Object::cool_down = std::rand() % 50 + 25;
		Object::elasticity = 2.5;
	}

	void Blood::onTurn(UI *)
	{
		if (Object::cool_down == 0) {
			Object::isActive = FALSE;
			Object::canMove = FALSE;
			Object::isShowed = FALSE;
		}
	}

}
