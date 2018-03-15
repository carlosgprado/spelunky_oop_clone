#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "rock.h"

namespace game_framework {

	void Rock::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = Object::isActive = TRUE;


		pic1.LoadBitmap("Pictures/Object/Item/rock.bmp", RGB(255, 0, 0));
		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();
		jumping = FALSE;

		Object::canDamage = TRUE;

		Object::canPickup = TRUE;

		Object::elasticity = 0.8;
	}

	void Rock::onTurn(UI *player)
	{
		if (jumping) {
			int i;
			int obj_num = player->obj_stack->get_obj_num();
			for (i = 0; i < obj_num; i++) {
				Object *temp = player->obj_stack->get_obj(i);
				if (hit_test_obj(temp) == TRUE) {
					temp->onHit(player, this);
				}
			}

			if (hit_test_obj(player->character) == TRUE) {
				player->character->onHit(player, this);
			}
		}
	}

	boolean Rock::query(char *statement)
	{
		if (statement == "jumping") {
			return(jumping);
		}
		return(0);
	}

	void Rock::assign(char *statement, boolean ans)
	{
		if (statement == "jumping") {
			jumping = ans;
		}
		else {
			GAME_ASSERT(FALSE, "statement nonexistant");
		}

	}


}
