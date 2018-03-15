#include "StdAfx.h"
#include "Resource.h"
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
#include "void.h"

//random
#include <cstdlib>
#include <ctime>

namespace game_framework {
	
	/////////////////////////////////////////////////////////////////////////////
	// My Own Classes - 遊戲基本設定 - Iile: 所有格子(具有物理性質)
	/////////////////////////////////////////////////////////////////////////////

	/*
	class Tile {
	public:
	Tile(void);
	~Tile(void);
	int type;            //0 = Solid, 1 = Not-Solid
	CMovingBitmap pic;
	protected:
	private:
	};
	*/
	Tile::Tile(void) {
	}

	Tile::~Tile(void) {
		free(pic);
	}

	void Tile::onInit(void)
	{
		isSet = false;
	}

	//With Level Choices
	void Tile::initiate(int input, UI* player) {
		type = input;
		isSet = TRUE;
		hasRope = -1;

		if (player != nullptr) {
			if (player->level == 0) {
				level = 1;
			}
			else if (player->level < 5) {
				level = 1;
			}
			else if (player->level < 9) {
				level = 2;
			}
			else if (player->level < 13) {
				level = 3;
			}
			else if (player->level < 17) {
				level = 4;
			}
			else {
				level = 2;
			}

		}
		else {
			level = 1;
		}
		
		int temp;

		pic = new CMovingBitmap;
		switch (type) {
		case 0:
			switch (level) {
			case 2:
				temp = std::rand() % 100;
				if (temp < 2) {
					pic->LoadBitmap("Pictures/Tile/level_2/dirt_2.bmp");
				}
				else if (temp < 13) {
					pic->LoadBitmap("Pictures/Tile/level_2/dirt_4.bmp");
				}
				else if (temp < 25) {
					pic->LoadBitmap("Pictures/Tile/level_2/dirt_3.bmp");
				}
				else if (temp < 40) {
					pic->LoadBitmap("Pictures/Tile/level_2/dirt_6.bmp");
				}
				else if (temp < 65) {
					pic->LoadBitmap("Pictures/Tile/level_2/dirt_5.bmp");
				}
				else {
					pic->LoadBitmap("Pictures/Tile/level_2/dirt.bmp");
				}
				break;
			default:
				temp = std::rand() % 100;
				if (temp < 2) {
					pic->LoadBitmap("Pictures/Tile/level_1/dirt_2.bmp");
				}
				else if (temp < 13) {
					pic->LoadBitmap("Pictures/Tile/level_1/dirt_4.bmp");
				}
				else if (temp < 25) {
					pic->LoadBitmap("Pictures/Tile/level_1/dirt_3.bmp");
				}
				else if (temp < 40) {
					pic->LoadBitmap("Pictures/Tile/level_1/dirt_6.bmp");
				}
				else if (temp < 65) {
					pic->LoadBitmap("Pictures/Tile/level_1/dirt_5.bmp");
				}
				else {
					pic->LoadBitmap("Pictures/Tile/level_1/dirt.bmp");
				}
			}
			elasticity = 0.5;
			isSolid = true;
			topSolid = TRUE;
			break;
		case 1:
			pic->LoadBitmap("Pictures/Tile/bouncy.bmp");
			elasticity = 0.85;
			isSolid = true;
			topSolid = TRUE;
			break;
		case 2:
			//ladder-normal
			switch (level) {
			case 2:
				pic->LoadBitmap("Pictures/Tile/level_2/ladder_middle.bmp", RGB(255, 0, 0));
				break;
			default:
				pic->LoadBitmap("Pictures/Tile/level_1/ladder_middle.bmp", RGB(0, 0, 0));
			}
			elasticity = 0.85;
			isSolid = false;
			break;
		case 3:
			//ladder-top
			switch (level) {
			case 2:
				pic->LoadBitmap("Pictures/Tile/level_2/ladder_top.bmp", RGB(255, 0, 0));
				break;
			default:
				pic->LoadBitmap("Pictures/Tile/level_1/ladder_top.bmp", RGB(0, 0, 0));
			}
			elasticity = 0.85;
			isSolid = false;
			topSolid = TRUE;
			break;
		case 4:
			//spike
			switch (level) {
			case 2:
				temp = std::rand() % 3;
				switch (temp) {
				case 1:
					pic->LoadBitmap("Pictures/Tile/level_2/spike_2.bmp", RGB(0, 0, 0));
					break;
				case 2:
					pic->LoadBitmap("Pictures/Tile/level_2/spike_3.bmp", RGB(0, 0, 0));
					break;
				default:
					pic->LoadBitmap("Pictures/Tile/level_2/spike.bmp", RGB(0, 0, 0));
				}
				break;
			default:
				temp = std::rand() % 3;
				switch (temp) {
				case 1:
					pic->LoadBitmap("Pictures/Tile/level_1/spike_2.bmp", RGB(0, 0, 0));
					break;
				case 2:
					pic->LoadBitmap("Pictures/Tile/level_1/spike_3.bmp", RGB(0, 0, 0));
					break;
				default:
					pic->LoadBitmap("Pictures/Tile/level_1/spike.bmp", RGB(0, 0, 0));
				}
			}
			elasticity = 0.85;
			isSolid = false;
			break;
		case 5:
			//DNT
			pic->LoadBitmap("Pictures/Tile/dynamite.bmp", RGB(0, 0, 0));
			elasticity = 0.85;
			isSolid = true;
			topSolid = TRUE;
			break;
		case 6:
			//Shooter_right
			pic->LoadBitmap("Pictures/Tile/trap_right.bmp", RGB(0, 0, 0));
			elasticity = 0.85;
			isSolid = true;
			topSolid = TRUE;
			break;
		case 7:
			//Shooter_left
			pic->LoadBitmap("Pictures/Tile/trap_left.bmp", RGB(0, 0, 0));
			elasticity = 0.85;
			isSolid = true;
			topSolid = TRUE;
			break;
		case 8:
			//ladder-normal-bottom
			switch (level) {
			case 2:
				pic->LoadBitmap("Pictures/Tile/level_2/ladder_bottom.bmp", RGB(255, 0, 0));
				break;
			default:
				pic->LoadBitmap("Pictures/Tile/level_1/ladder_middle.bmp", RGB(0, 0, 0));
			}
			elasticity = 0.85;
			isSolid = false;
			break;
		case 99:
			temp = std::rand() % 4;
			switch (temp) {
			case 1:
				pic->LoadBitmap("Pictures/Tile/side_wall_2.bmp");
				break;
			case 2:
				pic->LoadBitmap("Pictures/Tile/side_wall_3.bmp");
				break;
			case 3:
				pic->LoadBitmap("Pictures/Tile/side_wall_4.bmp");
				break;
			default:
				pic->LoadBitmap("Pictures/Tile/side_wall.bmp");
				break;
			}
			elasticity = 0.3;
			isSolid = true;
			topSolid = TRUE;
			break;
		default:
			type = -1;
			pic->LoadBitmap("Pictures/Tile/default.bmp", RGB(0, 0, 0));
			elasticity = 0;
			isSolid = false;
			topSolid = FALSE;
		}
	}

	

	void Tile::destroy(UI *player) {
		if (type != -1 && type != 99) {
			delete(pic);
			initiate(-1, player);
		}
	}

	void Tile::onTurn(UI *player)
	{
		switch (type) {
		case 4:
			//spike
			{
				Void_Obj hit_area;
				hit_area.initiate();
				hit_area.width = 46;
				hit_area.height = 14;
				hit_area.x = x * TILE_UNIT + 8;
				hit_area.y = y * TILE_UNIT + 20;

				int i;
				for (i = 0; i < player->obj_stack->get_obj_num(); i++) {
					Object *source = player->obj_stack->get_obj(i);

					Void_Obj test;
					test.initiate();
					test.width = source->width;
					test.height = source->height / 8;
					test.x = source->x;
					test.y = source->y + source->height * 7 / 8;

					if (test.hit_test_obj(&hit_area) == TRUE) {
						source->onHit(player, &test);
						source->die(player);
					}
				}

				Void_Obj test;
				test.initiate();
				test.width = player->character->width;
				test.height = player->character->height / 4;
				test.x = player->character->x;
				test.y = player->character->y + player->character->height * 3 / 4;

				if (test.hit_test_obj(&hit_area) == TRUE && player->character->y_velocity > 0) {
					player->character->onHit(player, &test);
					player->character->die(player);
				}
			}
			break;
		case 5:
			//DNT
			if (!flag) {
				flag = TRUE;
				int temp_no = player->obj_stack->new_obj(5);
				Object *temp = player->obj_stack->get_obj(temp_no);
				temp->initiate();
				temp->cool_down = -1;
				temp->onlyShow = FALSE;
				temp->canMove = FALSE;
				temp->width = TILE_UNIT;
				temp->height = TILE_HEIGHT;
				temp->x = x * TILE_UNIT;
				temp->y = y * TILE_UNIT;
			}
			break;
		case 6:
			//Shooter_right
			if (!flag) {
				//Find Length
				int i;
				boolean flag1 = FALSE;
				int line_of_sight = 7;
				int arrow_speed = 50;

				for (i = 0; !flag1 && i < line_of_sight; i++) {
					if (x > 0 && x < (TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1) {
						if (player->tile[x + 1 + i][y]->isSolid) {
							flag1 = TRUE;
						}
					}
				}

				Void_Obj hit_area;
				hit_area.initiate();
				hit_area.width = TILE_UNIT * i;
				hit_area.height = TILE_UNIT/2;
				hit_area.x = x * TILE_UNIT;
				hit_area.y = y * TILE_UNIT + TILE_UNIT / 4;

				if (hit_area.hit_test_obj(player->character) == TRUE) {
					flag = TRUE;

					//Create New Arrow
					int temp_no = player->obj_stack->new_obj(12);
					Object *temp = player->obj_stack->get_obj(temp_no);
					temp->initiate(0);
					temp->x = x * TILE_UNIT + TILE_UNIT / 2 - temp->width / 2;
					temp->y = y * TILE_UNIT + TILE_UNIT / 2 - temp->height / 2;
					temp->y_velocity = -GRAVITY_CONST;
					temp->x_velocity = arrow_speed;
				}

				for (i = 0; i < player->obj_stack->get_obj_num(); i++) {
					Object *source = player->obj_stack->get_obj(i);
					if (source->canHurt || source->canPickup) {
						if (hit_area.hit_test_obj(source) == TRUE) {
							flag = TRUE;

							//Create New Arrow
							int temp_no = player->obj_stack->new_obj(12);
							Object *temp = player->obj_stack->get_obj(temp_no);
							temp->initiate(0);
							temp->x = x * TILE_UNIT + TILE_UNIT / 2 - temp->width / 2;
							temp->y = y * TILE_UNIT + TILE_UNIT / 2 - temp->height / 2;
							temp->y_velocity = -GRAVITY_CONST;
							temp->x_velocity = arrow_speed;
						}
					}
				}
			}
			break;
		case 7:
			//Shooter_left
			if (!flag) {
				//Find Length
				int i;
				boolean flag1 = FALSE;
				int line_of_sight = 7;
				int arrow_speed = 50;

				for (i = 0; !flag1 && i < line_of_sight; i++) {
					if (x > 0 && x < (TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1) {
						if (player->tile[x - 1 - i][y]->isSolid) {
							flag1 = TRUE;
						}
					}
				}

				Void_Obj hit_area;
				hit_area.initiate();
				hit_area.width = TILE_UNIT * i;
				hit_area.height = TILE_UNIT / 2;
				hit_area.x = (x - i) * TILE_UNIT ;
				hit_area.y = y * TILE_UNIT + TILE_UNIT / 4;

				if (hit_area.hit_test_obj(player->character) == TRUE) {
					flag = TRUE;

					//Create New Arrow
					int temp_no = player->obj_stack->new_obj(12);
					Object *temp = player->obj_stack->get_obj(temp_no);
					temp->initiate(1);
					temp->x = x * TILE_UNIT + TILE_UNIT / 2 - temp->width / 2;
					temp->y = y * TILE_UNIT + TILE_UNIT / 2 - temp->height / 2;
					temp->y_velocity = -GRAVITY_CONST;
					temp->x_velocity = -arrow_speed;
				}

				for (i = 0; i < player->obj_stack->get_obj_num(); i++) {
					Object *source = player->obj_stack->get_obj(i);
					if (source->canHurt || source->canPickup) {
						if (hit_area.hit_test_obj(source) == TRUE) {
							flag = TRUE;

							//Create New Arrow
							int temp_no = player->obj_stack->new_obj(12);
							Object *temp = player->obj_stack->get_obj(temp_no);
							temp->initiate(1);
							temp->x = x * TILE_UNIT + TILE_UNIT / 2 - temp->width / 2;
							temp->y = y * TILE_UNIT + TILE_UNIT / 2 - temp->height / 2;
							temp->y_velocity = -GRAVITY_CONST;
							temp->x_velocity = -arrow_speed;
						}
					}
				}
			}
			break;
		}
	}
}
