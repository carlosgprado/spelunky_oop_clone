#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "void.h"

namespace game_framework {

	void Void_Obj::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = Object::isActive = Object::isShowed = Object::canPickup = Object::canBreak = FALSE;

		Object::height = 0;
		Object::width = 0;

		Object::elasticity = 1.5;
	}

}
