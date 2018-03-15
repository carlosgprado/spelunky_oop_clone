#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "snake.h"
#include "void.h"

namespace game_framework {

	void Snake::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 0;
		Object::canMove = Object::isActive = TRUE;


		pic1[0][0].LoadBitmap("Pictures/Object/Enemy/snake_right.bmp", RGB(255, 0, 0));
		pic1[0][1].LoadBitmap("Pictures/Object/Enemy/Animation/snake_right_2.bmp", RGB(255, 0, 0));
		pic1[0][2].LoadBitmap("Pictures/Object/Enemy/Animation/snake_right_3.bmp", RGB(255, 0, 0));
		pic1[0][3].LoadBitmap("Pictures/Object/Enemy/Animation/snake_right_4.bmp", RGB(255, 0, 0));
		pic1[0][4].LoadBitmap("Pictures/Object/Enemy/Animation/snake_right_5.bmp", RGB(255, 0, 0));
		pic1[0][5].LoadBitmap("Pictures/Object/Enemy/Animation/snake_right_6.bmp", RGB(255, 0, 0));
		pic1[1][0].LoadBitmap("Pictures/Object/Enemy/snake_left.bmp", RGB(255, 0, 0));
		pic1[1][1].LoadBitmap("Pictures/Object/Enemy/Animation/snake_left_2.bmp", RGB(255, 0, 0));
		pic1[1][2].LoadBitmap("Pictures/Object/Enemy/Animation/snake_left_3.bmp", RGB(255, 0, 0));
		pic1[1][3].LoadBitmap("Pictures/Object/Enemy/Animation/snake_left_4.bmp", RGB(255, 0, 0));
		pic1[1][4].LoadBitmap("Pictures/Object/Enemy/Animation/snake_left_5.bmp", RGB(255, 0, 0));
		pic1[1][5].LoadBitmap("Pictures/Object/Enemy/Animation/snake_left_6.bmp", RGB(255, 0, 0));
		Object::pic = &pic1[0][0];
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();

		Object::canHurt = TRUE;

		brain.pushState(onIdle);
		direction.pushState(1);

		//Direction - 1 = Right, 2 = Left

		life = 1;
		speed = 2.5;

		Object::elasticity = 0.5;
	}

	void Snake::move(UI *player)
	{
		switch (brain.getState()) {
		case onIdle:
			onIdle_Action(player);
			break;
		case onHunt:
			onHunt_Action(player);
			break;
		case onAttack:
			onAttack_Action(player);
			break;
		case onSuper:
			onSuper_Action(player);
			break;
		case onDeath:
			onDeath_Action(player);
			break;
		}


		if (pic_cool_down != 0 ) {
			
			temp_y = temp_y + pic->Height();

			if (direction.getState() == 2) {
				temp_x = temp_x + pic->Width();
			}

			if (pic_cool_down > 13) {
				pic = &pic1[direction.getState()-1][0];
			}
			else if (pic_cool_down > 12) {
				pic = &pic1[direction.getState() - 1][1];

			}
			else if (pic_cool_down >9) {
				pic = &pic1[direction.getState() - 1][2];

			}
			else if (pic_cool_down >6) {
				pic = &pic1[direction.getState() - 1][3];

			}
			else if (pic_cool_down >3) {
				pic = &pic1[direction.getState() - 1][4];

			}
			else if (pic_cool_down >1) {
				pic = &pic1[direction.getState() - 1][5];

			}
			else {
				pic = &pic1[direction.getState() - 1][0];
			}
			
			temp_y = temp_y - pic->Height();

			if (direction.getState() == 2) {
				temp_x = temp_x - pic->Width();
			}

			pic_cool_down--;

		}else if (brain.getState() != onSuper) {
			pic = &pic1[direction.getState() - 1][0];
		}
	}

	void Snake::obj_damage(UI *player, Object *source)
	{
		life--;
		if (life > 0) {
			brain.pushState(onSuper);
			Object::cool_down = ENEMY_SUPER_TIME;
			if (source->x + source->width / 2 > Object::x + Object::width / 2) {
				direction.pushState(2);
			}
			else {
				direction.pushState(1);
			}
		}
		else {
			brain.pushState(onDeath);
		}
	}

	void Snake::onIdle_Action(UI *player)
	{
		Void_Obj temp;
		temp.initiate();
		temp.width = 320;
		temp.height = 80;
		temp.x = x + width/2 - 160;
		temp.y = y + height / 2 - 40;
		if (player->character->hit_test_obj(&temp)!=TRUE || cool_down > 0) {
			if (cool_down > 0) {
				cool_down--;
				if (cool_down == 0) {
					brain.popState();
				}
			}
			if (direction.getState() == 1) {
				//right
				int a = (int)std::floor((x + width + speed) / TILE_UNIT);
				int b = (int)std::floor((y + height - 1) / TILE_UNIT);
				if (player->tile[a][b]->isSolid != TRUE && player->tile[a][b + 1]->topSolid == TRUE) {
					temp_x += speed;
				}
				else {
					direction.popState();
					direction.pushState(2);
				}
			}
			else {
				//left
				int a = (int)std::floor((x - speed - 1) / TILE_UNIT);
				int b = (int)std::floor((y + height - 1) / TILE_UNIT);
				if (player->tile[a][b]->isSolid != TRUE && player->tile[a][b + 1]->topSolid == TRUE) {
					temp_x -= speed;
				}
				else {
					direction.popState();
					direction.pushState(1);
				}
			}
		}
		else {
			brain.pushState(onHunt);
			if (Object::x + Object::width / 2 > player->character->x + player->character->width / 2) {
				direction.pushState(2);
			}
			else {
				direction.pushState(1);
			}
			
		}
	}

	void Snake::onHunt_Action(UI *player)
	{
		Void_Obj temp;
		temp.initiate();
		temp.width = 320;
		temp.height = 80;
		temp.x = x + width / 2 - 160;
		temp.y = y + height / 2 - 40;
		if (player->character->hit_test_obj(&temp) == FALSE) {
			brain.popState();
			direction.popState();
		}
		else {
			temp.width = 120;
			temp.height = 60;
			temp.x = x + width / 2 - 60;
			temp.y = y + height / 2 - 30;
			if (player->character->hit_test_obj(&temp) == TRUE) {
				Object::cool_down = 14;
				pic_cool_down = 14;
				brain.pushState(onAttack);
			}
			else {
				if (Object::x + Object::width / 2 > player->character->x + player->character->width / 2) {
					direction.popState();
					direction.pushState(2);
				}
				else {
					direction.popState();
					direction.pushState(1);
				}


				if (direction.getState() == 1) {
					temp_x += speed;
				}
				else {
					temp_x -= speed;
				}
			}
		}
	}

	void Snake::onAttack_Action(UI *player)
	{
		

		width = pic->Width();
		height = pic->Height();
		
		if (player->character->canHurt == TRUE && Object::hit_test_obj(player->character) == TRUE) {
			player->character->onHit(player, this);
		}

		width = pic1[0][0].Width();
		height = pic1[0][0].Height();


		if (Object::cool_down != 0) {			
			
			if (direction.getState() == 1) {
				temp_x += speed + 5;
			}
			else {
				temp_x -= (speed + 5);
			}
		}
		else {
			brain.popState();
			Object::cool_down = 55;
			brain.pushState(onIdle);
		}
		
	}

	void Snake::onSuper_Action(UI *player)
	{
		Object::canHurt = FALSE;
		
		if (direction.getState() == 1) {
			//Right
			temp_x += 4;
		}
		else {
			//Left
			temp_x -= 4;
		}

		if (cool_down == 0) {
			Object::canHurt = TRUE;
			brain.popState();
			direction.popState();
		}
	}

	void Snake::onDeath_Action(UI *player)
	{
		Object::canHurt = FALSE;
		Object::isShowed = FALSE;
		Object::canMove = FALSE;
	}

}
