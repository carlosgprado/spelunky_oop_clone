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
#include "door.h"

namespace game_framework {
	void Door::initiate(void)
	{
		initiate(0);
	}
	void Door::initiate(int input) {
		Object::temp_x = Object::x = x - 47;
		Object::temp_y = Object::y = y - 17;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = FALSE;
		Object::isActive = TRUE;

		switch (input) {
		case 0:
			pic1.LoadBitmap("Pictures/Object/Item/exit_door_1.bmp", RGB(255, 255, 0));
			break;
		case 1:
			pic1.LoadBitmap("Pictures/Object/Item/enter_door_1.bmp", RGB(255, 255, 0));
			break;
		case 2:
			pic1.LoadBitmap("Pictures/Object/Item/exit_door_2.bmp", RGB(255, 255, 0));
			break;
		case 3:
			pic1.LoadBitmap("Pictures/Object/Item/enter_door_2.bmp", RGB(255, 255, 0));
			break;
		case 4:
			pic1.LoadBitmap("Pictures/Object/Item/exit_door_3.bmp", RGB(255, 255, 0));
			break;
		case 5:
			pic1.LoadBitmap("Pictures/Object/Item/enter_door_3.bmp", RGB(255, 255, 0));
			break;
		case 6:
			pic1.LoadBitmap("Pictures/Object/Item/exit_door_4.bmp", RGB(255, 255, 0));
			break;
		case 7:
			pic1.LoadBitmap("Pictures/Object/Item/enter_door_4.bmp", RGB(255, 255, 0));
			break;
		}
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();
		
		hit_zone.initiate();
		hit_zone.width = TILE_UNIT * 2;
		hit_zone.height = TILE_UNIT * 2;
		hit_zone.x = x + 47;
		hit_zone.y = y + 17;

	}

	void Door::explode(UI *)
	{
	}
	
	void Door::onTurn(UI *player)
	{
		if (player->character->hit_test_obj(&hit_zone) == TRUE && player->character->query("moveUp") == TRUE) {
			player->next_level();
		}
	}

}
