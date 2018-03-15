#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "arrow.h"

namespace game_framework {
	void Arrow::initiate(void)
	{
		initiate(0);
	}
	void Arrow::initiate(int dir) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = Object::isActive = TRUE;

		if (dir == 0) {
			pics[0].LoadBitmap("Pictures/Object/Item/arrow_right.bmp", RGB(0, 0, 255));
			pics[1].LoadBitmap("Pictures/Object/Item/arrow_right_broken.bmp", RGB(0, 0, 255));
		}
		else {
			pics[0].LoadBitmap("Pictures/Object/Item/arrow_left.bmp", RGB(0, 0, 255));
			pics[1].LoadBitmap("Pictures/Object/Item/arrow_left_broken.bmp", RGB(0, 0, 255));
		}
		Object::pic = &pics[0];
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();
		jumping = FALSE;

		Object::canDamage = TRUE;

		Object::canPickup = TRUE;

		Object::elasticity = 0.8;
	}

	void Arrow::onTurn(UI *player)
	{
		if (jumping) {
			int i;
			int obj_num = player->obj_stack->get_obj_num();
			for (i = 0; i < obj_num; i++) {
				Object *temp = player->obj_stack->get_obj(i);
				if (hit_test_obj(temp) == TRUE) {
					temp->onHit(player, this);
					pic = &pics[1];
					Object::canDamage = FALSE;
					Object::canPickup = FALSE;
					Object::elasticity = 0.2;
				}
			}


			if (hit_test_obj(player->character) == TRUE) {
				player->character->onHit(player, this);
				pic = &pics[1];
				Object::canDamage = FALSE;
				Object::canPickup = FALSE;
				Object::elasticity = 0.2;
			}
		}
	}

	boolean Arrow::query(char *statement)
	{
		if (statement == "jumping") {
			return(jumping);
		}
		return(0);
	}

	void Arrow::assign(char *statement, boolean ans)
	{
		if (statement == "jumping") {
			jumping = ans;
		}
		else {
			GAME_ASSERT(FALSE, "statement nonexistant");
		}

	}


}
