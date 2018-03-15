#include "StdAfx.h"
#include "resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "mygame.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <sstream>


#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

namespace game_framework {
	

	/////////////////////////////////////////////////////////////////////////////
	// My Own Classes - 遊戲基本設定 - Object: 所有物件(包括遊戲腳色,敵人,實體物件)
	/////////////////////////////////////////////////////////////////////////////

	/*
	class Object {
	public:
	Object(void);
	~Object(void);
	double x, y;
	int width, height;
	double x_velocity; //垂直被動移動速度
	double y_velocity; //水平被動移動速度
	boolean canMove, isActive, onAir;
	CMovingBitmap pic;
	void physics(void);                    //物理
	virtual void move(void);               //Inherted Classes移動用 (改變temp_x, temp_y)
	int test(Tile tile[][TILE_HEIGHT]);   //測試是否允許移動
	void update(void);                     //暫定座標-->確定座標
	boolean moveLeft, moveRight, moveJump;
	protected:
	string name = "Name not 'virtual-ed'"; //物件名稱(偵錯用)
	double temp_x, temp_y;
	int mode;
	private:
	};
	*/

	Object::Object(void) {
		isShowed = TRUE;
		canPickup = FALSE;
		canBreak = FALSE;
		canDamage = FALSE;
		canHurt = FALSE;
		canTouch = FALSE;
	}

	Object::~Object(void) {

	}

	void Object::move(UI *) {

	}

	void Object::physics(void) {

		temp_y = y + y_velocity;
		temp_x = x + x_velocity;

		y_velocity += GRAVITY_CONST;

		if (x_velocity > 0) {
			x_velocity -= AIR_FRICTION_CONST;

		}
		else if (x_velocity < 0) {
			x_velocity += AIR_FRICTION_CONST;
		}

		if (y_velocity >= 45) {
			y_velocity = 45;
		}
	}

	void Object::update(void) {
		x = temp_x;
		y = temp_y;
	}

	void Object::rev_update(void) {
		temp_x = x;
		temp_y = y;
	}

	void Object::initiate(int)
	{
	}

	void Object::die(UI * player)
	{
		if (canHurt) {
			
			Object::canHurt = FALSE;
			Object::isShowed = FALSE;
			Object::canMove = FALSE;
		}
	}

	//特殊show
	void Object::show(UI *)
	{
		pic->ShowBitmap();
	}

	//物件破裂後行為
	void Object::obj_damage(UI *, Object *)
	{
	}

	//Query for private variable
	boolean Object::query(char *)
	{
		return boolean();
	}

	//Query for private variable
	int Object::query_int(char *)
	{
		return 0;
	}

	//Assign Boolean to string-ed variable
	void Object::assign(char *, boolean)
	{
	}

	//被玩家碰到後行為
	void Object::onTouch(UI *)
	{
	}

	//被打到後行為
	void Object::onHit(UI *player, Object *obj)
	{
		if (canHurt) {
			obj_damage(player, obj);
			int i;
			int num = std::rand() % 2 + 2;
			for (i = 0; i < num; i++) {
				int temp_num = player->obj_stack->new_obj(10);
				Object *temp = player->obj_stack->get_obj(temp_num);
				temp->initiate();
				temp->x = x + width / 2 - temp->width / 2;
				temp->y = y + height / 2 - temp->height / 2;
				do {
					temp->x_velocity = std::rand() % 31 - 15;
				} while (std::abs(temp->x_velocity)>4);
				temp->y_velocity = std::rand() % 10 + 5;
			}
		}

		if (canBreak) {
			obj_damage(player, NULL);
		}
	}

	//測試物件A和B是否碰撞
	boolean Object::hit_test_obj(Object *temp)
	{
		if (x <= temp->x + temp->width && x + width >= temp->x && y <= temp->y + temp->height && height + y >= temp->y) {
			return(TRUE);
		}
		else {
			return(FALSE);
		}
	}
	
	//每回合行為
	void Object::onTurn(UI *)
	{
	}

	//物件被炸到後之行為
	void Object::explode(UI *)
	{
		isActive = FALSE;
		isShowed = FALSE;
		canPickup = FALSE;
	}
	
	//拿來測試是否移動後在Tile之外
	int Object::test(UI *player) {

		boolean not_stuck = TRUE;
		int start_X = (int)floor(temp_x / TILE_UNIT);
		int start_Y = (int)floor(temp_y / TILE_UNIT);
		int end_X = (int)floor((temp_x + width) / TILE_UNIT);
		int end_Y = (int)floor((temp_y + height) / TILE_UNIT);
		int i, j;
		int test = 0;

		for (i = start_X; not_stuck && i <= end_X && i >= 0; i++) {
			for (j = start_Y; not_stuck && j <= end_Y && j >= 0; j++) {
				if (i<(TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1 && j < (TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1 && player->tile[i][j]->isSolid == TRUE) {
					not_stuck = FALSE;


					//太左邊
					if (i == start_X && floor(x / TILE_UNIT) > start_X) {
						test = 1;
						x = (i + 1)*TILE_UNIT + 1;

						if (canBreak == TRUE && abs(x_velocity) >= GRAVITY_CONST * 8 * query_int("toughness")) {
							obj_damage(player, NULL);
						}

						x_velocity *= (player->tile[i][j]->elasticity * elasticity * -1);

						

						if (abs(x_velocity) <= 2.3) {
							x_velocity = 0;
							if (canDamage == TRUE) {
								assign("jumping", FALSE);
							}
						}
						
						
					}

					//太右邊
					if (i == end_X && floor((x + width) / TILE_UNIT) < end_X) {
						test = 3;
						x = i * TILE_UNIT - width - 1;

						if (canBreak == TRUE && abs(x_velocity) >= GRAVITY_CONST * 8 * query_int("toughness")) {
							obj_damage(player, NULL);
						}

						x_velocity *= (player->tile[i][j]->elasticity * elasticity * -1);

						if (abs(x_velocity) <= 2.3) {
							x_velocity = 0;
							if (canDamage == TRUE) {
								assign("jumping", FALSE);
							}
						}
					}

					//太上面
					if (j == start_Y && floor(y / TILE_UNIT) > start_Y) {
						test = 2;
						y = (j + 1) * TILE_UNIT + 1;

						y_velocity *= (player->tile[i][j]->elasticity * elasticity * -1);

					}

					//太下面
					if (j == end_Y && floor((y + height) / TILE_UNIT) < end_Y) {
						test = 4;
						y = j * (TILE_UNIT)-height - 1;
					
						if (type == 0 || canDamage == TRUE) {
							assign("jumping", false);
						}

						if (canBreak == TRUE && abs(y_velocity) >= GRAVITY_CONST * 17 * query_int("toughness")) {
							obj_damage(player, NULL);
						}

						y_velocity *= (player->tile[i][j]->elasticity * elasticity * -1);

							
						if (abs(y_velocity) <= GRAVITY_CONST * 7) {
							y_velocity = 0;
						}
						else {
							if (type == 0 || canDamage == TRUE) {
								assign("jumping", TRUE);
							}
						}
					}

					

				}
				if (i<(TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1 && j < (TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1 && player->tile[i][j]->isSolid == FALSE && player->tile[i][j]->topSolid == TRUE) {
					not_stuck = FALSE;


					//太下面
					if (j == end_Y && floor((y + height) / TILE_UNIT) < end_Y) {
						test = 4;
						y = j * (TILE_UNIT)-height - 1;

						if (type == 0 || canDamage == TRUE) {
							assign("jumping", false);
						}

						if (canBreak == TRUE && abs(y_velocity) >= GRAVITY_CONST * 17 * query_int("toughness")) {
							obj_damage(player, NULL);
						}

						y_velocity *= (player->tile[i][j]->elasticity * elasticity * -1);


						if (abs(y_velocity) <= GRAVITY_CONST * 7) {
							y_velocity = 0;
						}
						else {
							if (type == 0 || canDamage == TRUE) {
								assign("jumping", TRUE);
							}
						}
					}



				}
			}
		}
		
		

		if (not_stuck) {
			return(0);
		}
		else {
			return(test);
		}
	}
 

	void FSM::popState(void)
	{
		if (stack_pointer >= 0) {
			stack_pointer--;
		}
	}

	void FSM::pushState(int input)
	{
		if (stack_pointer < 0) {
			stack_pointer = 0;
			stack[stack_pointer] = input;
		}
		else {
			stack_pointer++;
			stack[stack_pointer] = input;
		}
		
	}

	int FSM::getState(void)
	{
		if (stack_pointer == -1) {
			return(-1);
		}
		else {
			return(stack[stack_pointer]);
		}
	}

}
