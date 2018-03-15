#include "stdafx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "void.h"
#include "Character.h"



#include <iostream>
#include <string>
#include <cmath>
#include <Windows.h>
#include <sstream>

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

namespace game_framework {

	void Character::initiate(void) {
		Object::temp_x = Object::x = 250;
		Object::temp_y = Object::y = 130;
		Object::x_velocity = Object::y_velocity = 0;
		Object::canMove = Object::isActive =TRUE;
		onAir = moveLeft = moveRight = moveJump = moveAction = moveDown = moveUp = FALSE;
		jumping = usingWeapon = moveLShift = isFast = false;
		canHold = canHurt = TRUE;
		Object::elasticity = 0.6;
		
		pic[0].LoadBitmap("Pictures/Object/Character/Character_right.bmp", RGB(255, 0, 0));
		pic[1].LoadBitmap("Pictures/Object/Character/Character_left.bmp", RGB(255, 0, 0));
		pic[2].LoadBitmap("Pictures/Object/Character/Character_down_left.bmp", RGB(255, 0, 0));
		pic[3].LoadBitmap("Pictures/Object/Character/Character_down_right.bmp", RGB(255, 0, 0));
		pic[4].LoadBitmap("Pictures/Object/Character/Character_up_left.bmp", RGB(255, 0, 0));
		pic[5].LoadBitmap("Pictures/Object/Character/Character_up_right.bmp", RGB(255, 0, 0));
		pic[6].LoadBitmap("Pictures/Object/Character/Character_rope.bmp", RGB(255, 0, 0));
		pic[7].LoadBitmap("Pictures/Object/Character/Character_wj_right.bmp", RGB(255, 0, 0));
		pic[8].LoadBitmap("Pictures/Object/Character/Character_wj_left.bmp", RGB(255, 0, 0));
		pic[9].LoadBitmap("Pictures/Object/Character/Character_die_right_1.bmp", RGB(255, 0, 0));
		pic[10].LoadBitmap("Pictures/Object/Character/Character_die_right_2.bmp", RGB(255, 0, 0));
		pic[11].LoadBitmap("Pictures/Object/Character/Character_die_right_3.bmp", RGB(255, 0, 0));
		pic[12].LoadBitmap("Pictures/Object/Character/Character_die_left_1.bmp", RGB(255, 0, 0));
		pic[13].LoadBitmap("Pictures/Object/Character/Character_die_left_2.bmp", RGB(255, 0, 0));
		pic[14].LoadBitmap("Pictures/Object/Character/Character_die_left_3.bmp", RGB(255, 0, 0));

		whip_pic[0][0].LoadBitmap("Pictures/Object/Item/Animation/whip_1_2.bmp", RGB(255, 0, 0));
		whip_pic[0][1].LoadBitmap("Pictures/Object/Item/Animation/whip_2_2.bmp", RGB(255, 0, 0));
		whip_pic[0][2].LoadBitmap("Pictures/Object/Item/Animation/whip_3_2.bmp", RGB(255, 0, 0));
		whip_pic[0][3].LoadBitmap("Pictures/Object/Item/Animation/whip_4_2.bmp", RGB(255, 0, 0));
		whip_pic[0][4].LoadBitmap("Pictures/Object/Item/Animation/whip_5_2.bmp", RGB(255, 0, 0));
		whip_pic[0][5].LoadBitmap("Pictures/Object/Item/Animation/whip_6_2.bmp", RGB(255, 0, 0));
		whip_pic[0][6].LoadBitmap("Pictures/Object/Item/Animation/whip_7_2.bmp", RGB(255, 0, 0));
		whip_pic[0][7].LoadBitmap("Pictures/Object/Item/Animation/whip_8_2.bmp", RGB(255, 0, 0));
		whip_pic[0][8].LoadBitmap("Pictures/Object/Item/Animation/whip_9_2.bmp", RGB(255, 0, 0));
		whip_pic[0][9].LoadBitmap("Pictures/Object/Item/Animation/whip_10_2.bmp", RGB(255, 0, 0));
		whip_pic[0][10].LoadBitmap("Pictures/Object/Item/Animation/whip_11_2.bmp", RGB(255, 0, 0));
		whip_pic[1][0].LoadBitmap("Pictures/Object/Item/Animation/whip_1.bmp", RGB(255, 0, 0));
		whip_pic[1][1].LoadBitmap("Pictures/Object/Item/Animation/whip_2.bmp", RGB(255, 0, 0));
		whip_pic[1][2].LoadBitmap("Pictures/Object/Item/Animation/whip_3.bmp", RGB(255, 0, 0));
		whip_pic[1][3].LoadBitmap("Pictures/Object/Item/Animation/whip_4.bmp", RGB(255, 0, 0));
		whip_pic[1][4].LoadBitmap("Pictures/Object/Item/Animation/whip_5.bmp", RGB(255, 0, 0));
		whip_pic[1][5].LoadBitmap("Pictures/Object/Item/Animation/whip_6.bmp", RGB(255, 0, 0));
		whip_pic[1][6].LoadBitmap("Pictures/Object/Item/Animation/whip_7.bmp", RGB(255, 0, 0));
		whip_pic[1][7].LoadBitmap("Pictures/Object/Item/Animation/whip_8.bmp", RGB(255, 0, 0));
		whip_pic[1][8].LoadBitmap("Pictures/Object/Item/Animation/whip_9.bmp", RGB(255, 0, 0));
		whip_pic[1][9].LoadBitmap("Pictures/Object/Item/Animation/whip_10.bmp", RGB(255, 0, 0));
		whip_pic[1][10].LoadBitmap("Pictures/Object/Item/Animation/whip_11.bmp", RGB(255, 0, 0));


		Object::height = pic[0].Height();
		Object::width = pic[0].Width();
		speed = 5;
		strength = -11;
		onRope = FALSE;
		onHang = FALSE; 
		onLadder = FALSE;
		Object::pic = &pic[0];
		Object::state = 1; //0: Left. 1: Right


		holding_countdown = 0;
		super_countdown = 0;
		hanging_countdown = 0;
	}

	void Character::move(UI *player) {

		if (super_countdown != 0) {
			super_countdown--;

			if (super_direction == 1) {
				temp_x -= 4;
			}
			else {
				temp_x += 4;
			}

			if (super_countdown == 0) {
				canHurt = TRUE;
			}
		}

		hanging_countdown--;

		int start_X = (int)floor((Object::temp_x) / TILE_UNIT);
		int start_Y = (int)floor((Object::temp_y + Object::height * 2 / 5) / TILE_UNIT);
		int start_Y_2 = (int)floor((Object::temp_y + Object::height - 2) / TILE_UNIT);
		int end_X = (int)floor((Object::temp_x + Object::width) / TILE_UNIT);
		int end_Y = (int)floor((Object::temp_y + Object::height * 6 / 5) / TILE_UNIT);
		int end_Y_2 = (int)floor((Object::temp_y + Object::height - 2) / TILE_UNIT);

		if (moveLShift && !isFast && !jumping) {
			isFast = TRUE;
			speed *= 2;
		}

		if (!moveLShift && isFast && !jumping) {
			isFast = FALSE;
			speed /= 2;
		}

		if (moveUp) {
			if (onRope) {
				double temp = Object::y - speed;
				if (player->tile[start_X][(int)floor((temp + Object::height / 5) / TILE_UNIT)]->hasRope != -1) {
					Object::temp_y = Object::y - speed * ROPE_SPEED;
				}
			}
			else if (onLadder) {
				Object::temp_y = Object::y = Object::y - speed * ROPE_SPEED;
				if (onLadder && (player->tile[end_X][end_Y_2]->type != 2 && player->tile[end_X][end_Y_2]->type != 3 && player->tile[end_X][end_Y_2]->type != 8)) {
					onLadder = FALSE;
				}
			}
			else if (player->tile[start_X][start_Y]->hasRope != -1 && player->tile[end_X][end_Y]->hasRope != -1 && canHold == TRUE) {
				onRope = TRUE;
				jumping = FALSE;
			}
			else if (player->tile[start_X][start_Y_2]->type == 2 || player->tile[start_X][start_Y_2]->type == 3 || player->tile[start_X][start_Y_2]->type == 8) {
				onLadder = TRUE;
				jumping = FALSE;
			}

		}

		if (moveDown) {
			if (onRope || onLadder) {
				Object::temp_y = Object::y = Object::y + speed * ROPE_SPEED;
			}
			else if (start_X == end_X && (player->tile[start_X][end_Y_2 + 1]->type == 2 || player->tile[start_X][end_Y_2 + 1]->type == 3 || player->tile[start_X][end_Y_2 + 1]->type == 8)) {
				if (((int)(temp_y + height) % TILE_UNIT > 50 || (int)(temp_y + height) % TILE_UNIT == 0)) {
					onLadder = TRUE;
					Object::temp_y = Object::y = Object::y + speed * ROPE_SPEED;
				}

			}
		}

		if (onRope || onLadder) {
			if (moveJump) {
				if (moveLeft) {
					Object::temp_x = Object::x - (speed);
					Object::state = 0;

					moveJump = false;
					Object::y_velocity = strength;
					jumping = true;
					onRope = onLadder = FALSE;
				}
				else if (moveRight) {
					Object::temp_x = Object::x + (speed);
					Object::state = 1;

					moveJump = false;
					Object::y_velocity = strength;
					jumping = true;
					onRope = onLadder = FALSE;
				}
				else {
					Object::y_velocity = 0;
					Object::temp_x = start_X * TILE_UNIT + TILE_UNIT / 2 - 20;
				}
			}
			else {
				Object::y_velocity = 0;
				Object::temp_x = start_X * TILE_UNIT + TILE_UNIT / 2 - 20;

				if (moveLeft) {
					Object::state = 0;
				}
				else if (moveRight) {
					Object::state = 1;
				}
			}
		}
		else {
			if (moveLeft) {
				Object::temp_x = Object::x - (speed);
				Object::state = 0;
			}
			if (moveRight) {
				Object::temp_x = Object::x + (speed);
				Object::state = 1;
			}
			if (moveJump) {
				moveJump = false;
				Object::y_velocity = strength;
				jumping = true;
			}
		}



		//decide pic
		if (Object::isActive) {
			if (onRope || onLadder) {
				Object::pic = &pic[6];
			}
			else if (moveDown && !jumping) {
				if (Object::state == 0) {
					Object::pic = &pic[2];
				}
				else {
					Object::pic = &pic[3];
				}
			}
			else if (moveUp) {
				if (Object::state == 0) {
					Object::pic = &pic[4];
				}
				else {
					Object::pic = &pic[5];
				}
			}
			else {
				if (Object::state == 0) {
					Object::pic = &pic[1];
				}
				else {
					Object::pic = &pic[0];
				}
			}
		}
		else if (cool_down != 0) {
			cool_down--;
			if (cool_down == 10) {
				if (Object::state == 0) {
					Object::pic = &pic[13];
				}
				else {
					Object::pic = &pic[10];
				}
			}
			else if (cool_down == 1) {
				if (Object::state == 0) {
					Object::pic = &pic[14];
				}
				else {
					Object::pic = &pic[11];
				}
			}
		}

		if (onLadder) {
			if (player->tile[end_X][start_Y_2+1]->type != 2 && player->tile[end_X][start_Y_2+1]->type != 3 && player->tile[end_X][start_Y_2 + 1]->type != 8 &&(player->tile[end_X][start_Y_2]->type != 2 && player->tile[end_X][start_Y_2]->type != 3 && player->tile[end_X][start_Y_2]->type != 8)) {
				onLadder = FALSE;
			}
			else if ((int)temp_y % TILE_UNIT > 50) {
				if (player->tile[end_X][start_Y_2+1]->isSolid) {
					onLadder = FALSE;
				}
			}
		}

		if (player->tile[start_X][start_Y]->hasRope == -1 || player->tile[end_X][end_Y]->hasRope == -1) {
			onRope = FALSE;
		}
	}

	void Character::onTurn(UI *player)
	{

		if (throwBomb == TRUE) {
			throw_bomb(player);
		}
		
		if (throwRope == TRUE) {
			throw_rope(player);
		}
		
		//玩家按動作鍵
		if (holding_countdown == 0) {
			press_action(player);
		}
		else if (holding_countdown == 1) {
			holding_countdown = 0;
			player->holdingNo = -1;
			canHold = TRUE;
		}
		else {
			holding_countdown--;
		}

		//Using Whip
		if (usingWeapon) {
			weapon_countdown--;
			if(weapon_countdown == 0) {
				usingWeapon = FALSE;
			}
			else if (weapon_countdown % WEAPON_ANIMATION_TIME == 0) {
				int temp_no = (int)floor((11 * WEAPON_ANIMATION_TIME - weapon_countdown) / WEAPON_ANIMATION_TIME);
				weapon_pic = &whip_pic[Object::state][temp_no];
				int a = (int)(floor(Object::x - 42) - *(player->stage_x));
				int b = (int)(floor(Object::y - 11) - *(player->stage_y));
				weapon_pic->SetTopLeft(a, b);

				if (temp_no >= 5) {
					if (temp_no >= 8) {
						weapon_shadow.width = 76;
						weapon_shadow.height = 6;
						if (Object::state == 1) {
							weapon_shadow.x = Object::x - 42 + 55;
							weapon_shadow.y = Object::y - 11 + 54;
						}
						else {
							weapon_shadow.x = Object::x - 42;
							weapon_shadow.y = Object::y - 11 + 54;
						}
					}
					else {
						weapon_shadow.width = 58;
						weapon_shadow.height = 16;
						if (Object::state == 1) {
							weapon_shadow.x = Object::x - 42 + 45;
							weapon_shadow.y = Object::y - 11 + 35;
						}
						else {
							weapon_shadow.x = Object::x - 42 + 26;
							weapon_shadow.y = Object::y - 11 + 35;
						}
					}

					int i;
					int obj_num = player->obj_stack->get_obj_num();
					for (i = 0; i < obj_num; i++) {
						Object *temp = player->obj_stack->get_obj(i);
						if (temp->hit_test_obj(&weapon_shadow) == TRUE) {
							temp->onHit(player, this);
						}
					}
				}
			}


		}
		

		//Bump-On-Head
		if (y_velocity > 0 && !onRope && !onLadder) {
			Void_Obj hit_area;
			hit_area.initiate();
			hit_area.width = 20;
			hit_area.height = 10;
			hit_area.x = x + width/2 - hit_area.width/2;
			hit_area.y = y + height + 2;
			int i;
			for (i = 0; i < player->obj_stack->get_obj_num(); i++) {
				Object *temp = player->obj_stack->get_obj(i);
				if (temp->canHurt && temp->isShowed) {
					Void_Obj hit_area2;
					hit_area2.initiate();
					hit_area2.width = temp->width/2;
					hit_area2.height = temp->height/6;
					hit_area2.x = temp->x + temp->width/6;
					hit_area2.y = temp->y;
					if (hit_area.hit_test_obj(&hit_area2) == TRUE) {
						temp->onHit(player, this);
					}
				}
			}
		}


		//Forced Drop
		if (isHolding == TRUE && player->obj_stack->get_obj(player->holdingNo)->canPickup == FALSE) {
			holding_countdown = 0;
			player->holdingNo = -1;
			isHolding = FALSE;
			canHold = TRUE;
		}
		
	}

	void Character::die(UI *player) {
		Object::isActive = FALSE;
		player->adj_life(-99);
		if (Object::state == 0) {
			Object::pic = &pic[12];
		}
		else {
			Object::x += Object::width;
			Object::pic = &pic[9];
			Object::x -= Object::pic->Width();
			Object::temp_x = Object::x;
		}
		Object::cool_down = 20;
	}

	void Character::show(UI *player)
	{
		if (super_countdown % 6 != 5 && super_countdown % 6 != 4) {
			if (usingWeapon == TRUE) {
				weapon_pic->ShowBitmap();
			}

			Object::pic->ShowBitmap();

			if (isHolding == TRUE) {
				player->obj_stack->get_obj(player->holdingNo)->show(player);
			}
		}
	}

	void Character::obj_damage(UI *player, Object *source)
	{
		player->adj_life(-1);
		super_countdown = ENEMY_SUPER_TIME;
		canHurt = FALSE;

		if (source->x + source->width / 2 > Object::x + Object::width / 2) {
			super_direction = 1;
		}
		else {
			super_direction = 0;
		}
	}

	void Character::throw_rope(UI *player)
	{
		throwRope = FALSE;

		if (player->num_rope() > 0 && canHold == TRUE) {
			player->useRope = TRUE;
			int temp_num = player->obj_stack->new_obj(7);
			Object *temp = player->obj_stack->get_obj(temp_num);


			int a = (int)((x + width / 2) / TILE_UNIT);
			int b = (int)((y + height * 3 / 4) / TILE_UNIT);
			boolean stop = FALSE;


			int width = 40;



			if (moveDown == TRUE) {
				if (Object::state == 1) {
					//moveRight
					if (player->tile[a + 1][b]->isSolid != TRUE) {
						a++;
					}
				}
				else {
					//moveLeft
					if (player->tile[a - 1][b]->isSolid != TRUE) {
						a--;
					}
				}
				stop = TRUE;
				b++;
			}

			temp->x = (a)*TILE_UNIT + TILE_UNIT / 2 - width / 2;
			temp->y = (b)*TILE_UNIT - TILE_UNIT / 2 - 5;

			if (player->tile[a][b]->isSolid == TRUE) {
				temp->y = (b - 1)*TILE_UNIT;
				temp->initiate(0);
			}
			else {
				temp->initiate(1);
			}

			if (stop == TRUE) {
				temp->y_velocity = 0;
			}
			temp->x_velocity = 0;


			temp->rev_update();
		}
	}

	void Character::throw_bomb(UI *player)
	{
		throwBomb = FALSE;

		if (player->num_bomb() > 0 && canHold == TRUE) {
			player->useBomb = TRUE;
			int temp_num = player->obj_stack->new_obj(5);
			Object* temp = player->obj_stack->get_obj(temp_num);
			temp->initiate();
			if (moveDown == TRUE) {
				temp->y = Object::y + Object::height / 2 - temp->height - 1;
				temp->x_velocity = temp->y_velocity = 0;
				if (Object::state == 1) {
					//moveRight
					temp->x = Object::x + Object::width - temp->width / 2;
				}
				else {
					//moveLeft
					temp->x = Object::x + temp->width / 2;
				}


			}
			else {
				temp->x = Object::x + Object::width / 2 - temp->width / 2;
				temp->y = Object::y + Object::height / 2 - temp->height;
				temp->y_velocity = strength;
				if (Object::state == 1) {
					//moveRight
					temp->x_velocity = strength * (-1);
				}
				else {
					//moveLeft
					temp->x_velocity = strength * 1;
				}
			}

		}
	}

	void Character::press_action(UI *player)
	{
		if (moveAction == TRUE) {
			if (canHold == TRUE) {
				if (moveDown == TRUE) {
					//pick_up
					moveAction = FALSE;
					boolean flag = FALSE;
					int i;
					for (i = 0; flag != TRUE && i < player->obj_stack->get_obj_num(); i++) {
						Object *temp = player->obj_stack->get_obj(i);
						if (temp->canPickup == TRUE && Object::hit_test_obj(temp) == TRUE) {
							if (temp->canDamage) {
								temp->assign("jumping", FALSE);
							}
							isHolding = TRUE;
							canHold = FALSE;
							player->holdingNo = i;
							flag = TRUE;
							temp->canMove = FALSE;
						}
					}

				}
				else {
					//attack
					moveAction = FALSE;
					if (!usingWeapon) {
						usingWeapon = TRUE;
						weapon_countdown = 11 * WEAPON_ANIMATION_TIME +1;
						weapon_pic = &whip_pic[Object::state][0];
					}

				}
			}
			else {
				//Player is Holding ==> Throw
				moveAction = FALSE;
				isHolding = FALSE;
				Object *temp = player->obj_stack->get_obj(player->holdingNo);
				temp->canMove = TRUE;

				if (temp->canDamage == TRUE) {
					assign("jumping", TRUE);
				}

				if (state == 1) {
					temp->x_velocity = -strength;
				}
				else {
					temp->x_velocity = strength;
				}

				temp->y_velocity = strength;

				if (moveUp == TRUE) {
					temp->y_velocity += strength;
				}

				if (moveDown == TRUE) {
					if (jumping == TRUE) {
						temp->y_velocity -= 2 * strength;
					}
					else {
						temp->y_velocity = 0;
						temp->x_velocity = 0;
					}
				}
				temp->physics();
				temp->update();
				holding_countdown = HOLD_ITEM_SUPER_TIME;
			}
		}
	}

	boolean Character::query(char *statement)
	{
		if (statement == "moveRight") {
			return(moveRight);
		}
		else if (statement == "moveLeft") {
			return(moveLeft);
		}
		else if (statement == "moveUp") {
			return(moveUp);
		}
		else if (statement == "moveDown") {
			return(moveDown);
		}
		else if (statement == "moveAction") {
			return(moveAction);
		}
		else if (statement == "moveJump") {
			return(moveJump);
		}
		else if (statement == "jumping") {
			return(jumping);
		}
		else if (statement == "throwBomb") {
			return(throwBomb);
		}
		else if (statement == "throwRope") {
			return(throwRope);
		}
		else if (statement == "isHolding") {
			return(isHolding);
		}
		else if (statement == "canHold") {
			return(canHold);
		}
		else if (statement == "moveLShift") {
			return(moveLShift);
		}
		else if (statement == "onLadder") {
			return(onLadder);
		}
		else {
			GAME_ASSERT(FALSE, "statement nonexistant");
			return(FALSE);
		}
		
	}

	int Character::query_int(char *statement)
	{
		if (statement == "speed") {
			return((int)speed);
		}
		return(0);
	}

	void Character::assign(char *statement, boolean ans)
	{
		if (statement == "moveRight") {
			moveRight = ans;
		}
		else if (statement == "moveLeft") {
			moveLeft = ans;
		}
		else if (statement == "moveUp") {
			moveUp = ans;
		}
		else if (statement == "moveDown") {
			moveDown = ans;
		}
		else if (statement == "moveAction") {
			moveAction = ans;
		}
		else if (statement == "moveJump") {
			moveJump = ans;
		}
		else if (statement == "jumping") {
			jumping = ans;
		}
		else if (statement == "throwBomb") {
			throwBomb = ans;
		}
		else if (statement == "throwRope") {
			throwRope = ans;
		}
		else if(statement == "isHolding") {
			isHolding = ans;
		}
		else if (statement == "canHold") {
			canHold = ans;
		}
		else if (statement == "moveLShift") {
			moveLShift = ans;
		}
		else if (statement == "onLadder") {
			onLadder = ans;
		}
		else {
			GAME_ASSERT(FALSE, "statement nonexistant");
		}
		
	}


}
