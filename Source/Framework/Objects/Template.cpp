#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "Template.h"

namespace game_framework {

	void Template::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = Object::isActive = TRUE;


		pic1.LoadBitmap("Pictures/Object/Item/ball.bmp", RGB(0, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();


		Object::elasticity = 1.5;
	}

}
