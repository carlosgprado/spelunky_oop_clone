#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "Rope.h"

namespace game_framework {
	void Rope::initiate(void)
	{
		initiate(1);
	}
	void Rope::initiate(int input) {
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 7;
		Object::canMove = Object::isShowed = Object::isActive = TRUE;


		rope[0].LoadBitmap("Pictures/Object/Item/Animation/rope_1_2.bmp", RGB(255, 0, 0));
		rope[1].LoadBitmap("Pictures/Object/Item/Animation/rope_2_2.bmp", RGB(255, 0, 0));
		rope[2].LoadBitmap("Pictures/Object/Item/Animation/rope_3_2.bmp", RGB(255, 0, 0));
		rope[3].LoadBitmap("Pictures/Object/Item/Animation/rope_4_2.bmp", RGB(255, 0, 0));
		rope[4].LoadBitmap("Pictures/Object/Item/Animation/rope_end_2.bmp", RGB(255, 0, 0));
		rope[5].LoadBitmap("Pictures/Object/Item/Animation/rope_fly.bmp", RGB(255, 0, 0));
		rope[6].LoadBitmap("Pictures/Object/Item/Animation/rope_hook.bmp", RGB(255, 0, 0));
		rope[7].LoadBitmap("Pictures/Object/Item/Animation/rope_mid_2.bmp", RGB(255, 0, 0));
		


		Object::elasticity = 1.5;
		level = 0;

		if (input == 1) { //Start
			Object::pic = &rope[5];
			Object::height = Object::pic->Height();
			Object::width = Object::pic->Width();
		

			Object::y_velocity = -std::ceil(std::sqrt((1*TILE_UNIT*ROPE_TILES) / GRAVITY_CONST)) * GRAVITY_CONST ;
			Object::state = 1; //1-Flying, 2-stop, 3 - background
			level = 6; //6-First, 7~12 -Mid, 4-End
		}
		else if (input == 0) { //Just one tile
			Object::pic = &rope[5];
			Object::height = Object::pic->Height();
			Object::width = Object::pic->Width();
			Object::canMove = FALSE;
			Object::state = 0;
		}
		else {
			Object::pic = &rope[0];
			Object::height = Object::pic->Height();
			Object::width = Object::pic->Width();
			Object::canMove = FALSE;
			Object::state = 1;
			level = input;
			Object::cool_down = ROPE_ANIMATION_TIME * 4 ;
		}
	}

	void Rope::move(UI *) {
		if (y_velocity > GRAVITY_CONST * (-2)) {
			Object::canMove = FALSE;
			Object::pic = &rope[6];
			Object::temp_y = Object::y = (std::floor(Object::y / TILE_UNIT) + 1)*TILE_UNIT - Object::height;
			Object::state = 2;
		}
	}


	
	void Rope::onTurn(UI *player) {
		if (Object::isActive) {
			if (Object::state == 2) {
				int temp_num = player->obj_stack->new_obj(7);
				Object *temp = player->obj_stack->get_obj(temp_num);
				temp->x = Object::x;
				if (level == 6) {
					temp->y = Object::y;
				}
				else {
					temp->y = Object::y + TILE_UNIT;
				}
				int b = (int)std::floor((temp->y) / TILE_UNIT) + 1;
				int a = (int)std::floor((temp->x) / TILE_UNIT);
				player->tile[a][b]->hasRope = Object::no;
				if (level != (6 + ROPE_TILES) && player->tile[a][b + 1]->isSolid == FALSE && player->tile[a][b + 2]->isSolid == FALSE) {
					temp->initiate(level + 1);
				}
				else {
					temp->initiate(4);
					player->tile[a][b + 1]->hasRope = Object::no;
				}

				Object::state = 3;
			}
		}

		switch (level) {
		case 6:

			break;
		case 0:
			break;
		default:
			if (Object::state == 1 && Object::cool_down%ROPE_ANIMATION_TIME == 0) {
				
				switch (Object::cool_down / ROPE_ANIMATION_TIME) {
				case 3:
					Object::pic = &rope[1];
					break;
				case 2:
					if (level != 4) {
						Object::pic = &rope[2];
					}
					else {
						Object::pic = &rope[4];
						Object::state = 3;
					}
					break;
				case 1:
					Object::pic = &rope[3];

					break;
				case 0:
					Object::pic = &rope[7];
					
					Object::state = 2;
					break;
				}
			}
		}

	}

	void Rope::explode(UI *player)
	{
		int b = (int)std::floor((Object::y) / TILE_UNIT) + 2;
		int a = (int)std::floor((Object::x) / TILE_UNIT);


		int now = player->tile[a][b]->hasRope;
		if (now != -1) {
			do {
				player->obj_stack->get_obj(player->tile[a][b]->hasRope)->isActive = FALSE;
				player->obj_stack->get_obj(player->tile[a][b]->hasRope)->isShowed = FALSE;
				player->tile[a][b]->hasRope = -1;
				now++;
				b++;
			} while (player->tile[a][b]->hasRope == now);
			player->obj_stack->get_obj(now)->isActive = FALSE;
			player->obj_stack->get_obj(now)->isShowed = FALSE;
		}
		else {
			Object::isActive = FALSE;
			Object::isShowed = FALSE;
		}

	}


}
