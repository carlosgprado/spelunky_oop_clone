/*
* framework.cpp: 本檔案儲遊戲本身的Base Class的implementation
*/



#include "StdAfx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <cmath>
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
#include <fstream>


#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

//random
#include <cstdlib>
#include <ctime>



//Objects

#include "void.h"
#include "Character.h"
#include "Template.h"
#include "coin.h"
#include "crate.h"
#include "bomb_pack.h"
#include "rope_pile.h"
#include "bomb.h"
#include "Rope.h"
#include "pot.h"
#include "rock.h"
#include "blood.h"
#include "snake.h"
#include "arrow.h"
#include "door.h"

namespace game_framework {
	
	/////////////////////////////////////////////////////////////////////////////
	// My Own Classes - 遊戲基本設定 - Stage: 放置格子和物件的舞台 + 所有無物理性質之背景
	/////////////////////////////////////////////////////////////////////////////

	/*
	class Stage{
	public:
	Stage(void);
	~Stage(void);
	void Stage_Generator(void);                     // 建立舞台上面物品及格子
	void OnMove(void);								// 移動遊戲元素 (處理物理)
	void OnShow(void);								// 顯示Stage的遊戲畫面
	void background_move(int, int);                 // 移動Stage本身
	void key_detected(int, UINT);                   // 鍵盤偵測移動
	int obj_num;                                    // Stage上物件個數(Move/Show用)
	protected:
	private:
	int stage_x, stage_y;
	CMovingBitmap background[3][2];
	Object *obj[MAX_OBJ];                           //Pointer
	Tile tile[TILE_WIDTH][TILE_HEIGHT];              //Actual Class
	};
	*/


	//Constructor  舞台上面的物品及格子指標在這裡建立

	Stage::Stage(void) {
		obj_num = 0;
		stage_x = stage_y = 0;
		character = NULL;


	}


	//Destructor
	Stage::~Stage(void) {

	}


	//Stage_Generator 舞台上面的物品及格子本身在這裡建立 + Load Everything 
	void Stage::Stage_Generator(UI *temp) {


		stage_x = 0;
		stage_y = 0;

		
		if (temp->level == 0) {
			Build_Tutorial(temp);
		}
		else {
			Build_Level(temp);
		}

		int i, j;
		//Background直接Draw在全部後面
		for (i = 0; i <= total_x; i++) {
			for (j = 0; j <= total_y; j++) {
				if (j <= total_y / 4 && i <= total_x / 4) {
					background[i][j] = new CMovingBitmap;
					char * temp = (char *) bg_location.c_str();
					background[i][j]->LoadBitmapA(temp);
					background[i][j]->SetTopLeft(i * 256, j * 256);
				}
			}
		}

		stage_movement();

		//Load All Tiles
		player = temp;
		temp->character = character;
		temp->obj_stack = &obj_stack;
		temp->stage_x = &stage_x;
		temp->stage_y = &stage_y;
		if (player->level == 0) {
			player->lose_all_item();
		}

		for (i = 0; i < total_x; i++) {
			for (j = 0; j < total_y; j++) {
				temp->tile[i][j] = tile[i][j];
			}
		}

	}

	void Stage::Stage_Destructor(UI *temp) {
		int i, j;
		for (i = 0; i < total_x; i++) {
			for (j = 0; j < total_y; j++) {
				if (tile[i][j]->pic!=nullptr) {
					free(tile[i][j]->pic);
				}
				free(tile[i][j]);
				if (j <= total_y / 4 && i <= total_x / 4) {
					free(background[i][j]);
				}
			}
		}

		obj_stack.destroy();

		free(character);
	}

	//OnMove 移動遊戲元素 (處理物理)
	void Stage::OnMove(void) {

		int i, j;

		//
		//Character移動 (character)
		//

		//Check Action (move) Legit
		if (character != NULL && character->canMove) {

			character->rev_update();
	

			//movement During Turn
			character->move(player);

			if (character->test(player) == 0) {
				character->update();
			};

			character->rev_update();

			//Check Physics
			character->physics();

			if (character->test(player) == 0) {
				character->update();
			};



			//Show
			character->pic->SetTopLeft((int)(floor(character->x) - stage_x), (int)(floor(character->y) - stage_y));

			//Drop Bomb, Drop Rope, Throw/Attack
			character->onTurn(player);

		}



		//Move Stage According to Character's position
		stage_movement();

		//UI
		player->update();

		//物件碰撞處理
		for (i = 0; i < player->obj_stack->get_obj_num(); i++) {
			Object *temp = player->obj_stack->get_obj(i);

			//Cooldown
			if (temp->cool_down > 0) {
				temp->cool_down--;
			}

			if (temp->isActive == TRUE) {

				//有和玩家碰撞
				if (character->isActive == TRUE && temp->canTouch && temp->cool_down == 0 && character->hit_test_obj(temp)){
					temp->onTouch(player);
				}

			}
			
		}



		//TILE處理
		
		int start_X = (int)floor(stage_x / TILE_UNIT);
		int start_Y = (int)floor(stage_y / TILE_UNIT);
		int end_X = (int)ceil((stage_x + SIZE_X) / TILE_UNIT);
		int end_Y = (int)ceil((stage_y + SIZE_Y) / TILE_UNIT);

		for (i = start_X; i < end_X && i < total_x && i >= 0; i++) {
			for (j = start_Y; j < end_Y && j < total_y && j >= 0; j++) {
				tile[i][j]->onTurn(player);
				tile[i][j]->pic->SetTopLeft((int)(i*TILE_UNIT - stage_x), (int)(j*TILE_UNIT - stage_y));
			}
		}

		//每個物件檢查
		for (i = 0; i < obj_stack.get_obj_num(); i++) {
			Object * temp = obj_stack.get_obj(i);

			if (temp->isShowed) {
				
				temp->onTurn(player);

				if (temp->canMove) {

					temp->rev_update();

					//movement During Turn
					temp->move(player);

					if (temp->test(player) == 0) {
						temp->update();
					};

					temp->rev_update();

					//Check Physics
					temp->physics();

					if (temp->test(player) == 0) {
						temp->update();
					};
				}
			}

			//Show
			temp->pic->SetTopLeft((int)(floor(temp->x) - stage_x), (int)(floor(temp->y) - stage_y));
		}

		//Tutorial
		if (player->level == 0) {
			int i;
			for (i = 0; i < TUTORIAL_NUM; i++) {
				tutorial_pic[i].SetTopLeft((int)(floor(tutorial_position[0][i]) - stage_x), (int)(floor(tutorial_position[1][i]) - stage_y));
			}
		}
		
		//Holding Object Position
		if (character->query("isHolding") == TRUE) {
			Object * temp = obj_stack.get_obj(player->holdingNo);

			temp->x = character->x + character->width / 2 - temp->width / 2;
			temp->y = character->y + character->height - temp->height;
			temp->pic->SetTopLeft((int)(floor(temp->x) - stage_x), (int)(floor(temp->y) - stage_y));
		}

	}
	

	// 顯示Stage的遊戲畫面
	void Stage::OnShow() {
		//Show Background
		int i, j;
		for (i = 0; i <= SIZE_X/(TILE_UNIT*4); i++) {
			for (j = 0; j <= SIZE_Y/(TILE_UNIT*4); j++) {
				background[i][j]->ShowBitmap();
			}
		}

		//Tutorial
		if (player->level == 0) {
			int i;
			for (i = 0; i < TUTORIAL_NUM; i++) {
				tutorial_pic[i].ShowBitmap();
			}
		}

		//Show All Tiles
		int start_X = (int)floor(stage_x / TILE_UNIT);
		int start_Y = (int)floor(stage_y / TILE_UNIT);
		int end_X = (int)ceil((stage_x + SIZE_X) / TILE_UNIT);
		int end_Y = (int)ceil((stage_y + SIZE_Y) / TILE_UNIT);

		for (i = start_X; i < end_X && i < total_x && i >= 0; i++) {
			for (j = start_Y; j < end_Y && j < total_y && j >= 0; j++) {
				tile[i][j]->pic->ShowBitmap();
			}
		}


		for (i = 0; i < obj_stack.get_obj_num(); i++) {
			Object *temp = obj_stack.get_obj(i); 
			if (temp->isShowed == TRUE && temp->onlyShow == TRUE && (temp->x) <= (SIZE_X + stage_x) && (temp->x + temp->width) >= (stage_x) && (temp->y) <= (SIZE_Y + stage_y) && (temp->y + temp->height) >= (stage_y)) {
				temp->show(player);
				temp->pic->ShowBitmap();
			}
		}


		character->show(player);

		player->show();

		
		

	}


	// 移動Stage本身
	void Stage::stage_movement(void) {
		double a, b, c, d;

		//X-Movement_(TWO LINES VERSION)
		if ((character->x + character->width - stage_x) > SIZE_X - X_SAFE) {
			stage_x += (character->x + character->width - stage_x - (SIZE_X - X_SAFE));
			diff_stage_x_right = (total_x - x_addition) * TILE_UNIT - character->width;
			x_right_const = X_GENERAL;
		}
		else if (((character->x + character->width - stage_x) > SIZE_X - X_GENERAL) && character->query("moveLeft") != TRUE) {
			a = (total_x - x_addition) * TILE_UNIT - character->width;
			b = diff_stage_x_right;
			c = character->x;
			d = SIZE_X - X_SAFE - ((a - c)*(x_right_const - X_SAFE) / (a - b));

			if (character->x + character->width - stage_x  < d + character->query_int("speed") && a > b) {
				stage_x += (character->x + character->width - stage_x - d);
			}
			else {
				diff_stage_x_right = character->x;
				x_right_const = SIZE_X - (diff_stage_x_right + character->width - stage_x);
			}

			if (stage_x > (total_x - x_addition) * TILE_UNIT + X_SAFE - SIZE_X) {
				stage_x += (character->x - stage_x - (SIZE_X - X_SAFE));
			}

		}
		else {
			diff_stage_x_right = character->x;
			x_right_const = X_GENERAL;
		}


		if ((character->x - stage_x) <= X_SAFE) {

			stage_x += (character->x - stage_x - X_SAFE);
			diff_stage_x_left = 0;
			x_left_const = X_GENERAL;
		}
		else if (((character->x - stage_x) < X_GENERAL) && character->query("moveRight") != TRUE) {

			a = X_SAFE;
			b = diff_stage_x_left;
			c = character->x;
			d = X_SAFE + ((c - a)*(x_left_const - X_SAFE)) / (b - a);

			if (character->x - stage_x > d - character->query_int("speed") && b > a) {

				stage_x += (character->x - stage_x - d);
			}
			else {
				diff_stage_x_left = character->x;
				x_left_const = (diff_stage_x_left - stage_x);
			}

			if (stage_x < 0) {
				stage_x += (character->x - stage_x);
			}
		}
		else {
			diff_stage_x_left = character->x;
			x_left_const = X_GENERAL;
		}




		if ((character->y + character->height - stage_y) > SIZE_Y - Y_SAFE) {
			stage_y += (character->y + character->height - stage_y - (SIZE_Y - Y_SAFE));
		}
		else if ((character->y - stage_y) < Y_SAFE) {
			stage_y += (character->y - stage_y - Y_SAFE);
		}
	}

	void Stage::Build_Template(UI *temp)
	{
		int i, j;
		
		bg_location = "Pictures/UI/minebg.bmp";
		//Background直接Draw在全部後面
		for (i = 0; i <= total_x; i++) {
			for (j = 0; j <= total_y; j++) {
				tile[i][j] = new Tile;
				tile[i][j]->onInit();
			}
		}

		//Asssign Tiles

		for (i = 0; i < total_x; i++) {
			for (j = 0; j < y_addition; j++) {
				if (tile[i][j]->isSet == FALSE) {
					tile[i][j]->initiate(99, temp);
				}
				if (tile[i][total_y - 1 - j]->isSet == FALSE) {
					tile[i][total_y - 1 - j]->initiate(99, temp);
				}
			}
		}

		for (i = 0; i < total_y; i++) {
			for (j = 0; j < x_addition; j++) {
				if (tile[j][i]->isSet == FALSE) {
					tile[j][i]->initiate(99, temp);
				}
				if (tile[total_x - 1 - j][i]->isSet == FALSE) {
					tile[total_x - 1 - j][i]->initiate(99, temp);
				}
			}
		}

		tile[x_addition + 4][7]->initiate(3, temp);
		tile[x_addition + 4][8]->initiate(2, temp);
		tile[x_addition + 4][9]->initiate(2, temp);
		tile[x_addition + 4][10]->initiate(2, temp);
		tile[x_addition + 4][11]->initiate(2, temp);
		tile[x_addition + 4][12]->initiate(2, temp);
		tile[x_addition + 8][13]->initiate(4, temp);
		tile[x_addition + 8][14]->initiate(0, temp);

		tile[x_addition + 7][7]->initiate(6, temp);

		tile[x_addition + 3][7]->initiate(5, temp);
		tile[x_addition + 2][7]->initiate(5, temp);
		tile[x_addition + 1][8]->initiate(5, temp);
		tile[x_addition + 1][10]->initiate(5, temp);


		for (i = 0; i < total_x; i++) {
			for (j = 0; j < total_y; j++) {
				if (tile[i][j]->isSet == FALSE) {
					if ((j != 17 && j != 10 && j != 7 && j != 9 && j != 8 && j != 14) || i == 10 || i == 16) {
						tile[i][j]->initiate(-1, temp);
					}
					else if (i>9) {
						tile[i][j]->initiate(1, temp);
					}
					else {
						tile[i][j]->initiate(0, temp);
					}
				}
				tile[i][j]->x = i;
				tile[i][j]->y = j;
				tile[i][j]->pic->SetTopLeft(i*TILE_UNIT, j * TILE_UNIT);
			}
		}

		//Create Objects
		character = new Character;
		character->initiate();
		character->x = 600;
		character->y = Y_SAFE + 70;
		diff_stage_x_left = character->x;
		diff_stage_x_right = character->x;
		x_left_const = X_GENERAL;
		x_right_const = X_GENERAL;


		int now;

		obj_stack.intiate();
		now = obj_stack.new_obj(0);
		obj_stack.get_obj(now)->initiate();
		obj_stack.get_obj(now)->x = 600;
		obj_stack.get_obj(now)->y = 320;


		now = obj_stack.new_obj(0);
		obj_stack.get_obj(now)->initiate();
		obj_stack.get_obj(now)->x = 11 * TILE_UNIT + 15;
		obj_stack.get_obj(now)->y = 320;


		now = obj_stack.new_obj(11);
		obj_stack.get_obj(now)->initiate();
		obj_stack.get_obj(now)->x = (x_addition + 3)*TILE_UNIT;
		obj_stack.get_obj(now)->y = 6 * TILE_UNIT;

		now = obj_stack.new_obj(1);
		obj_stack.get_obj(now)->initiate();
		obj_stack.get_obj(now)->x = 250;
		obj_stack.get_obj(now)->y = 220;

		now = obj_stack.new_obj(2);
		obj_stack.get_obj(now)->initiate();
		obj_stack.get_obj(now)->x = 210;
		obj_stack.get_obj(now)->y = 360;

		now = obj_stack.new_obj(13);
		obj_stack.get_obj(now)->x = 8 * TILE_UNIT + 15;
		obj_stack.get_obj(now)->y = 360;
		obj_stack.get_obj(now)->initiate();


		now = obj_stack.new_obj(7);
		obj_stack.get_obj(now)->initiate();
		obj_stack.get_obj(now)->x = (int)(660 / TILE_UNIT)*TILE_UNIT + TILE_UNIT / 2 - obj_stack.get_obj(now)->width / 2;
		obj_stack.get_obj(now)->y = 420;


		for (i = obj_stack.get_obj_num(); i < 40; i++) {
			now = obj_stack.new_obj(1);
			obj_stack.get_obj(now)->initiate();

			int a, b;
			do {
				a = std::rand() % TILE_WIDTH + x_addition;
				b = std::rand() % TILE_HEIGHT + y_addition;
			} while (tile[a][b]->isSolid == TRUE);


			obj_stack.get_obj(now)->x = a*TILE_UNIT + std::rand() % (TILE_UNIT - obj_stack.get_obj(now)->width);
			obj_stack.get_obj(now)->y = b*TILE_UNIT + std::rand() % (TILE_UNIT - obj_stack.get_obj(now)->height);
			obj_stack.get_obj(now)->rev_update();
		}
	}

	void Stage::Build_Tutorial(UI *temp)
	{
		int i, j;


		if (tutorial_load == FALSE) {
			tutorial_load = TRUE;

			tutorial_pic[0].LoadBitmap("Pictures/UI/Tutorial_01.bmp", RGB(255, 0, 0));
			tutorial_position[0][0] = (4 + x_addition)*TILE_UNIT;
			tutorial_position[1][0] = (1 + y_addition)*TILE_UNIT;

			tutorial_pic[1].LoadBitmap("Pictures/UI/Tutorial_03.bmp", RGB(255, 0, 0));
			tutorial_position[0][1] = (22 + x_addition)*TILE_UNIT;
			tutorial_position[1][1] = (1 + y_addition)*TILE_UNIT - 30;

			tutorial_pic[2].LoadBitmap("Pictures/UI/Tutorial_04.bmp", RGB(255, 0, 0));
			tutorial_position[0][2] = (14 + x_addition)*TILE_UNIT;
			tutorial_position[1][2] = (1 + y_addition)*TILE_UNIT - 40;

			tutorial_pic[3].LoadBitmap("Pictures/UI/Tutorial_02.bmp", RGB(255, 0, 0));
			tutorial_position[0][3] = (24 + x_addition)*TILE_UNIT + 20;
			tutorial_position[1][3] = (9 + y_addition)*TILE_UNIT ;

			tutorial_pic[4].LoadBitmap("Pictures/UI/Tutorial_06.bmp", RGB(255, 0, 0));
			tutorial_position[0][4] = (19 + x_addition)*TILE_UNIT + 40;
			tutorial_position[1][4] = (9 + y_addition)*TILE_UNIT - 60;

			tutorial_pic[5].LoadBitmap("Pictures/UI/Tutorial_05.bmp", RGB(255, 0, 0));
			tutorial_position[0][5] = (13 + x_addition)*TILE_UNIT + 40;
			tutorial_position[1][5] = (11 + y_addition)*TILE_UNIT;

			tutorial_pic[6].LoadBitmap("Pictures/UI/Tutorial_07.bmp", RGB(255, 0, 0));
			tutorial_position[0][6] = (0 + x_addition)*TILE_UNIT + 40;
			tutorial_position[1][6] = (10 + y_addition)*TILE_UNIT;

			tutorial_pic[7].LoadBitmap("Pictures/UI/Tutorial_08.bmp", RGB(255, 0, 0));
			tutorial_position[0][7] = (4 + x_addition)*TILE_UNIT + 40;
			tutorial_position[1][7] = (10 + y_addition)*TILE_UNIT;

			tutorial_pic[8].LoadBitmap("Pictures/UI/Tutorial_09.bmp", RGB(255, 0, 0));
			tutorial_position[0][8] = (0 + x_addition)*TILE_UNIT + 40;
			tutorial_position[1][8] = (18 + y_addition)*TILE_UNIT - 20;
		}

		bg_location = "Pictures/UI/minebg.bmp";

		//Background直接Draw在全部後面
		for (i = 0; i <= total_x; i++) {
			for (j = 0; j <= total_y; j++) {
				tile[i][j] = new Tile;
				tile[i][j]->onInit();
			}
		}

		//Asssign Tiles

		for (i = 0; i < total_x; i++) {
			for (j = 0; j < y_addition; j++) {
				if (tile[i][j]->isSet == FALSE) {
					tile[i][j]->initiate(99, temp);
				}
				if (tile[i][total_y - 1 - j]->isSet == FALSE) {
					tile[i][total_y - 1 - j]->initiate(99, temp);
				}
			}
		}

		for (i = 0; i < total_y; i++) {
			for (j = 0; j < x_addition; j++) {
				if (tile[j][i]->isSet == FALSE) {
					tile[j][i]->initiate(99, temp);
				}
				if (tile[total_x - 1 - j][i]->isSet == FALSE) {
					tile[total_x - 1 - j][i]->initiate(99, temp);
				}
			}
		}

		//Tile Arrangements
		{
			//Read-File
			{
				ifstream myfile;
				char temp2;
				myfile.open("Source/Framework/Level/Test.txt");
				for (j = 0; j <= TILE_HEIGHT; j++) {
					for (i = 0; i <= TILE_WIDTH; i++) {
						myfile.get(temp2);
						DBOUT(temp2);
				
						int num;
						switch (temp2) {
						case '-':
							num = -1;
							break;
						case 'X':
							num = 0;
							break;
						case '+':
							num = 2;
							break; 
						case '^':
							num = 3;
							break; 
						case '|':
							num = 4;
							break;
						case 'T':
							num = 5;
							break;
						case '>':
							num = 6;
							break;
						case '<':
							num = 7;
							break;
						case '!':
							num = 8;
							break;
						default:
							num = -1;
						}
						if (tile[x_addition  + i][y_addition + j]->isSet == FALSE) {
							tile[x_addition  + i][y_addition  + j]->initiate(num, temp);
						}
					}
					DBOUT(endl);
					myfile.get(temp2);
				}
				myfile.close();
			}

			//Print_Map
			for (j = 0; j < total_y; j++) {
				for (i = 0; i < total_x; i++) {
					if (tile[i][j]->isSet != FALSE) {
						int num = tile[i][j]->type;
						switch (num) {
						case -1:
							DBOUT("-");
							break;
						case 0:
							DBOUT("X");
							break;
						case 2:
							DBOUT("+");
							break;
						case 3:
							DBOUT("^");
							break;
						case 4:
							DBOUT("|");
							break;
						default:
							DBOUT("O");
						}
					}
					else {
						DBOUT("?");
					}
				}
				DBOUT(endl);
			}
		




			/*

			//First Layer
			{
				tile[x_addition + 10][y_addition + 6]->initiate(0);
				tile[x_addition + 13][y_addition + 6]->initiate(0);
				tile[x_addition + 14][y_addition + 5]->initiate(0);
				tile[x_addition + 14][y_addition + 6]->initiate(0);
				tile[x_addition + 15][y_addition + 6]->initiate(0);
				tile[x_addition + 15][y_addition + 5]->initiate(0);
				tile[x_addition + 16][y_addition + 6]->initiate(0);


				tile[x_addition + 21][y_addition + 4]->initiate(0);
				tile[x_addition + 21][y_addition + 5]->initiate(0);
				tile[x_addition + 21][y_addition + 6]->initiate(0);
				tile[x_addition + 22][y_addition + 5]->initiate(0);
				tile[x_addition + 22][y_addition + 6]->initiate(0);
				tile[x_addition + 23][y_addition + 5]->initiate(0);
				tile[x_addition + 23][y_addition + 6]->initiate(0);
				tile[x_addition + 24][y_addition + 5]->initiate(0);
				tile[x_addition + 24][y_addition + 6]->initiate(0);
				tile[x_addition + 25][y_addition + 5]->initiate(0);
				tile[x_addition + 25][y_addition + 6]->initiate(0);

				tile[x_addition + 26][y_addition + 7]->initiate(4);
				tile[x_addition + 27][y_addition + 7]->initiate(4);
				tile[x_addition + 28][y_addition + 7]->initiate(-1);
				tile[x_addition + 29][y_addition + 7]->initiate(-1);
				tile[x_addition + 30][y_addition + 7]->initiate(-1);
				tile[x_addition + 28][y_addition + 4]->initiate(0);
				tile[x_addition + 29][y_addition + 4]->initiate(0);
				tile[x_addition + 30][y_addition + 4]->initiate(0);
				tile[x_addition + 31][y_addition + 4]->initiate(0);
				tile[x_addition + 29][y_addition + 4]->initiate(3);
				tile[x_addition + 32][y_addition + 4]->initiate(0);
				tile[x_addition + 29][y_addition + 5]->initiate(2);
				tile[x_addition + 29][y_addition + 6]->initiate(2);
				tile[x_addition + 29][y_addition + 7]->initiate(2);
				tile[x_addition + 32][y_addition + 7]->initiate(3);
				tile[x_addition + 32][y_addition + 8]->initiate(2);
			}

			//Second Layer
			{

				tile[x_addition + 32][y_addition + 9]->initiate(2);
				tile[x_addition + 32][y_addition + 10]->initiate(2);
				tile[x_addition + 32][y_addition + 11]->initiate(2);
				tile[x_addition + 32][y_addition + 12]->initiate(2);
				tile[x_addition + 32][y_addition + 14]->initiate(0);
				tile[x_addition + 31][y_addition + 14]->initiate(0);
				tile[x_addition + 30][y_addition + 14]->initiate(0);
				tile[x_addition + 29][y_addition + 14]->initiate(0);
			}


			for (i = x_addition; i < total_x; i++) {
			for (j = 0; j < total_y; j++) {
			if (tile[i][j]->isSet == FALSE) {
			if (j == (7 + y_addition) || j == (8 + y_addition) || j == 15 + y_addition || j == 16 + y_addition) {
			tile[i][j]->initiate(0);
			}
			}
			}
			}


			//Rest
			

			*/



		}

		for (i = 0; i < total_x; i++) {
			for (j = 0; j < total_y; j++) {
				if (tile[i][j]->isSet == FALSE) {
					tile[i][j]->initiate(-1, temp);
				}
				tile[i][j]->x = i;
				tile[i][j]->y = j;
				tile[i][j]->pic->SetTopLeft(i*TILE_UNIT, j * TILE_UNIT);
			}
		}



		//Create Objects
		character = new Character;
		character->initiate();
		character->x = X_SAFE + 200;
		character->y = Y_SAFE + 370;
		diff_stage_x_left = character->x;
		diff_stage_x_right = character->x;
		x_left_const = X_GENERAL;
		x_right_const = X_GENERAL;


		int now;
		obj_stack.intiate();

		/*
			now = obj_stack.new_obj(0);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = 11 * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = 12 * TILE_UNIT ;
		*/
		


		//First Layer
		{
			now = obj_stack.new_obj(9);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (24 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (3 + y_addition) * TILE_UNIT;


			now = obj_stack.new_obj(8);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (22 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (3 + y_addition) * TILE_UNIT;

			now = obj_stack.new_obj(8);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (32 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (3 + y_addition) * TILE_UNIT;

		}

		


		//Second Layer
		{
			now = obj_stack.new_obj(2);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (29 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (11 + y_addition) * TILE_UNIT;

			now = obj_stack.new_obj(2);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (26 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (11 + y_addition) * TILE_UNIT; 
			
			now = obj_stack.new_obj(2);
			obj_stack.get_obj(now)->initiate(13);
			obj_stack.get_obj(now)->x = (22 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (11 + y_addition) * TILE_UNIT;

			now = obj_stack.new_obj(2);
			obj_stack.get_obj(now)->initiate(14);
			obj_stack.get_obj(now)->x = (17 + x_addition) * TILE_UNIT + 45;
			obj_stack.get_obj(now)->y = (14 + y_addition) * TILE_UNIT;

			now = obj_stack.new_obj(9);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (9 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (11 + y_addition) * TILE_UNIT;


			
			now = obj_stack.new_obj(11);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = (27 + x_addition) * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = (12 + y_addition) * TILE_UNIT - 20;

			now = obj_stack.new_obj(13);
			obj_stack.get_obj(now)->x = (9 + x_addition) * TILE_UNIT;
			obj_stack.get_obj(now)->y = (20 + y_addition) * TILE_UNIT;
			obj_stack.get_obj(now)->initiate();
		}
		


		for (i = obj_stack.get_obj_num(); i < 40; i++) {
				now = obj_stack.new_obj(1);
				obj_stack.get_obj(now)->initiate();

				int a, b;
				do {
					a = std::rand() % TILE_WIDTH + x_addition;
					b = std::rand() % TILE_HEIGHT + y_addition;
				} while (tile[a][b]->isSolid == TRUE);


				obj_stack.get_obj(now)->x = a*TILE_UNIT + std::rand() % (TILE_UNIT - obj_stack.get_obj(now)->width);
				obj_stack.get_obj(now)->y = b*TILE_UNIT + std::rand() % (TILE_UNIT - obj_stack.get_obj(now)->height);
				obj_stack.get_obj(now)->rev_update();
			}
		
	}


	void Stage::Build_Level(UI *temp)
	{
		int i, j;
		


		//Background
		{
			if (temp->level < 5) {
				bg_location = "Pictures/UI/minebg.bmp";
			}
			else if (temp->level < 9) {
				bg_location = "Pictures/UI/forestbg.bmp";
			}
			else if (temp->level < 13) {
				bg_location = "Pictures/UI/icebg.bmp";
			}
			else if (temp->level < 17) {
				bg_location = "Pictures/UI/templebg.bmp";
			}
			else {
				bg_location = "Pictures/UI/minebg.bmp";
			}
		}

		//Tile Initiation
		for (i = 0; i <= total_x; i++) {
			for (j = 0; j <= total_y; j++) {
				tile[i][j] = new Tile;
				tile[i][j]->onInit();
			}
		}


		//Asssign Tiles

		for (i = 0; i < total_x; i++) {
			for (j = 0; j < y_addition; j++) {
				if (tile[i][j]->isSet == FALSE) {
					tile[i][j]->initiate(99,temp);
				}
				if (tile[i][total_y - 1 - j]->isSet == FALSE) {
					tile[i][total_y - 1 - j]->initiate(99, temp);
				}
			}
		}

		for (i = 0; i < total_y; i++) {
			for (j = 0; j < x_addition; j++) {
				if (tile[j][i]->isSet == FALSE) {
					tile[j][i]->initiate(99, temp);
				}
				if (tile[total_x - 1 - j][i]->isSet == FALSE) {
					tile[total_x - 1 - j][i]->initiate(99, temp);
				}
			}
			tile[total_x - j][i]->initiate(99, temp);
		}




		int room[4][3];
		int room_details[4][3][4]; //0 - up, 1 - down, 2 - left, 3 - right
				// Room Types: 0 - Random, 1 - Corridor, 2 - Falling, 3 - Landing, 4 - Starting Room, 5 - Ending Room

		//Random Generation
		{
			
			//Room Setting
			{
				int i, j;
				for (i = 0; i < 4; i++) {
					for (j = 0; j < 4; j++) {
						room[i][j] = -1;
					}
				}

				//Select Starting and Ending Room
				int starting = std::rand() % 4;
				int ending = std::rand() % 4;
				room[starting][0] = 4;
				room[ending][2] = 5;

				//Mid-land Room
				int mid_land;
				do {
					mid_land = std::rand() % 4;
				} while (mid_land == starting);

				int counter = 1;
				if (starting > mid_land) {
					counter *= -1;
				}
				starting += counter;
				while (starting != mid_land) {
					room[starting][0] = 1;
					starting += counter;
				}
				room[starting][0] = 2;
				room[starting][1] = 3;

				//Mid-drop Room
				int mid_drop;
				do {
					mid_drop = std::rand() % 4;
				} while (mid_drop == mid_land || mid_drop == ending);

				counter = 1;
				if (mid_land > mid_drop) {
					counter *= -1;
				}
				mid_land += counter;
				while (mid_land != mid_drop) {
					room[mid_land][1] = 1;
					mid_land += counter;
				}
				room[mid_land][1] = 2;
				room[mid_land][2] = 3;

				//Last Row
				counter = 1;
				if (mid_drop > ending) {
					counter *= -1;
				}
				mid_drop += counter;
				while (mid_drop != ending) {
					room[mid_drop][2] = 1;
					mid_drop += counter;
				}

				for (j = 0; j < 3; j++) {
					for (i = 0; i < 4; i++) {
						if (room[i][j] == -1) {
							room[i][j] = 0;
						}
						DBOUT(" " << room[i][j] << " ");
					}
					DBOUT(endl);
				}
			}


			//Room Details
			{
				int i, j, k;
				for (i = 0; i<4; i++) {
					for (j = 0; j<3; j++) {
						for (k = 0; k<4; k++) {
							room_details[i][j][k] = 0;
						}

						if (room[i][j] != 0) {
							if (i != 0) {
								if (room_details[i - 1][j][3] == 0) {
									room_details[i][j][2] = std::rand() % 4 + 1;
								}
								else {
									do {
										room_details[i][j][2] = room_details[i - 1][j][3] - 1 + std::rand() % 3;
									} while (room_details[i][j][2] <= 0 || room_details[i][j][2]>4);
								}
							}
							else {
								room_details[i][j][2] = std::rand() % 4 + 1;
							}

							room_details[i][j][3] = std::rand() % 4 + 1;
							
						}

						if (room[i][j] == 2) {
							room_details[i][j][1] = std::rand() % 2 + 2;
						}

						if (room[i][j] == 3) {
							room_details[i][j][0] = room_details[i][j - 1][1];
						}

					}
				}

				DBOUT(endl << endl);
				for (j = 0; j<3; j++) {
					for (i = 0; i<4; i++) {
						DBOUT(" " << room_details[i][j][0] << " ");
					}
					DBOUT(endl);
					for (i = 0; i<4; i++) {
						DBOUT(room_details[i][j][2] << " " << room_details[i][j][3]);
					}
					DBOUT(endl);
					for (i = 0; i<4; i++) {
						DBOUT(" " << room_details[i][j][1] << " ");
					}
					DBOUT(endl);
				}
				DBOUT(endl);
				DBOUT(endl);

			}

		}



		//Tile Arrangements
		{


			//Read-File
			{
				int temp_block[8][8], ran;
				ifstream myfile;
				char temp2;

				for (i = 0; i < 4; i++) {
					for (j = 0; j < 4; j++) {
						//Set temp
						{
							if (room[i][j] == 0) {
								//Full Random
								for (int k = 0; k < 2; k++) {
									for (int s = 0; s < 2; s++) {
										ran = std::rand() % 8;
										string loc;
										switch (ran) {
										case 0:
											loc = "Source/Framework/Level/mid/0.txt";
											break;
										case 1:
											loc = "Source/Framework/Level/mid/1.txt";
											break;
										case 2:
											loc = "Source/Framework/Level/mid/2.txt";
											break;
										case 3:
											loc = "Source/Framework/Level/mid/3.txt";
											break;
										case 4:
											loc = "Source/Framework/Level/mid/4.txt";
											break;
										case 5:
											loc = "Source/Framework/Level/mid/5.txt";
											break;
										case 6:
											loc = "Source/Framework/Level/mid/6.txt";
											break;
										case 7:
											loc = "Source/Framework/Level/mid/7.txt";
											break;
										}

										char * temp_3 = (char *)loc.c_str();
										myfile.open(temp_3);
										int num;

										int a, b;
										for (b = 0; b < 4; b++) {
											for (a = 0; a < 4; a++) {
												myfile.get(temp2);
												switch (temp2) {
												case '-':
													num = -1;
													break;
												case 'X':
													num = 0;
													break;
												case '+':
													num = 2;
													break;
												case '^':
													num = 3;
													break;
												case '|':
													num = 4;
													break;
												case 'T':
													num = 5;
													break;
												case '>':
													num = 6;
													break;
												case '<':
													num = 7;
													break;
												case '!':
													num = 8;
													break;
												case '1':
													num = 0;
													break;
												case '2':
													ran = std::rand() % 13;
													if (ran <= 9) {
														num = -1;
													}
													else {
														num = 0;
													}
													break;
												default:
													num = -1;
												}
												temp_block[k * 4 + a][s * 4 + b] = num;
											}
											myfile.get(temp2);

										}
										myfile.close();
									}



								}

							}
							else {
								//First Implement Corridors

								//left
								{
									int ran = std::rand() % 3;
									string loc;
									switch (room_details[i][j][2]) {
									case 1:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/1-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/1-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/1-2.txt";
										}
										break;
									case 2:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/2-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/2-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/2-2.txt";
										}
										break;
									case 3:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/3-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/3-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/3-2.txt";
										}
										break;
									case 4:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/4-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/4-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/4-2.txt";
										}
										break;
									}
									char * temp_3 = (char *)loc.c_str();
									myfile.open(temp_3);

									int a, b, num;
									for (b = 0; b < 8; b++) {
										for (a = 0; a < 4; a++) {
											myfile.get(temp2);
											switch (temp2) {
											case '-':
												num = -1;
												break;
											case 'X':
												num = 0;
												break;
											case '+':
												num = 2;
												break;
											case '^':
												num = 3;
												break;
											case '|':
												num = 4;
												break;
											case 'T':
												num = 5;
												break;
											case '>':
												num = 6;
												break;
											case '<':
												num = 7;
												break;
											case '!':
												num = 8;
												break;
											case '1':
												num = 2;
												break;
											case '2':
												ran = std::rand() % 13;
												if (ran <= 9) {
													num = -1;
												}
												else {
													num = 0;
												}
												break;
											default:
												num = -1;
											}
											temp_block[a][b] = num;
										}
										myfile.get(temp2);

									}
									myfile.close();
								}

								//right
								{
									int ran = std::rand() % 3;
									string loc;
									switch (room_details[i][j][3]) {
									case 1:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/1-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/1-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/1-2.txt";
										}
										break;
									case 2:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/2-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/2-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/2-2.txt";
										}
										break;
									case 3:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/3-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/3-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/3-2.txt";
										}
										break;
									case 4:
										if (ran == 0) {
											loc = "Source/Framework/Level/left/4-0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/left/4-1.txt";
										}
										else {
											loc = "Source/Framework/Level/left/4-2.txt";
										}
										break;
									}
									char * temp_3 = (char *)loc.c_str();
									myfile.open(temp_3);
									int a, b, num;
									for (b = 0; b < 8; b++) {
										for (a = 8; a > 4; a--) {
											myfile.get(temp2);
											switch (temp2) {
											case '-':
												num = -1;
												break;
											case 'X':
												num = 0;
												break;
											case '+':
												num = 2;
												break;
											case '^':
												num = 3;
												break;
											case '|':
												num = 4;
												break;
											case 'T':
												num = 5;
												break;
											case '>':
												num = 6;
												break;
											case '<':
												num = 7;
												break;
											case '!':
												num = 8;
												break;
											case '1':
												num = 0;
												break;
											case '2':
												ran = std::rand() % 13;
												if (ran <= 9) {
													num = -1;
												}
												else {
													num = 0;
												}
												break;
											default:
												num = -1;
											}
											temp_block[a][b] = num;
										}
										myfile.get(temp2);

									}
									myfile.close();
								}


								{
									
									if (room[i][j] == 2) {
										int ran = std::rand() % 4;
										string loc;
										if (ran == 0) {
											loc = "Source/Framework/Level/up/0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/up/1.txt";
										}
										else if (ran == 2) {
											loc = "Source/Framework/Level/up/2.txt";
										}
										else {
											loc = "Source/Framework/Level/up/3.txt";
										}
										
										char * temp_3 = (char *)loc.c_str();
										myfile.open(temp_3);
										for (int a = 2; a < 6; a++) {
											for (int b = 0; b < 2; b++){
												int num;
												myfile.get(temp2);
												switch (temp2) {
												case '-':
													num = -1;
													break;
												case 'X':
													num = 0;
													break;
												case '+':
													num = 2;
													break;
												case '^':
													num = 3;
													break;
												case '|':
													num = 4;
													break;
												case 'T':
													num = 5;
													break;
												case '>':
													num = 6;
													break;
												case '<':
													num = 7;
													break;
												case '!':
													num = 8;
													break;
												case '1':
													num = 0;
													break;
												case '2':
													ran = std::rand() % 13;
													if (ran <= 9) {
														num = -1;
													}
													else {
														num = 0;
													}
													break;
												default:
													num = -1;
												}
												temp_block[a][b] = num;

											}
											myfile.get(temp2);
										}
										myfile.close();
									}


									if (room[i][j] == 3) {
										int ran = std::rand() % 4;
										string loc;
										if (ran == 0) {
											loc = "Source/Framework/Level/up/0.txt";
										}
										else if (ran == 1) {
											loc = "Source/Framework/Level/up/1.txt";
										}
										else if (ran == 2) {
											loc = "Source/Framework/Level/up/2.txt";
										}
										else {
											loc = "Source/Framework/Level/up/3.txt";
										}

										char * temp_3 = (char *)loc.c_str();
										myfile.open(temp_3);
										for (int a = 2; a < 6; a++) {
											for (int b = 7; b > 5; b--) {
												int num;
												myfile.get(temp2);
												switch (temp2) {
												case '-':
													num = -1;
													break;
												case 'X':
													num = 0;
													break;
												case '+':
													num = 2;
													break;
												case '^':
													num = 3;
													break;
												case '|':
													num = 4;
													break;
												case 'T':
													num = 5;
													break;
												case '>':
													num = 6;
													break;
												case '<':
													num = 7;
													break;
												case '!':
													num = 8;
													break;
												case '1':
													num = 0;
													break;
												case '2':
													ran = std::rand() % 13;
													if (ran <= 9) {
														num = -1;
													}
													else {
														num = 0;
													}
													break;
												default:
													num = -1;
												}
												temp_block[a][b] = num;

											}
											myfile.get(temp2);
										}
										myfile.close();
									}
								}

							}
						}

						//Apply temp
						DBOUT("(" << i << "," << j << ") - "<< room[i][j] << endl);
						DBOUT(" " << room_details[i][j][0] << " " << endl << room_details[i][j][2] << " " << room_details[i][j][3] << endl << " " << room_details[i][j][1] << " "<<endl<<endl);
						for (int b = 0; b < 8; b++) {
							for (int a = 0; a < 8; a++) {
								if (tile[x_addition + i * 8 + a][y_addition + j * 8 + b]->isSet == FALSE) {
									tile[x_addition + i * 8 + a][y_addition + j * 8 + b]->initiate(temp_block[a][b], temp);

									switch (temp_block[a][b]) {
									case -1:
										DBOUT("-");
										break;
									case 0:
										DBOUT("X");
										break;
									case 2:
										DBOUT("+");
										break;
									case 3:
										DBOUT("^");
										break;
									case 4:
										DBOUT("|");
										break;
									case 5:
										DBOUT("T");
										break;
									case 6:
										DBOUT(">");
										break;
									case 7:
										DBOUT("<");
										break;
									case 8:
										DBOUT("!");
										break;
									default:
										DBOUT("O");
									}

								}
							}
							DBOUT(endl);
						}

						DBOUT(endl << endl);
					}

					/*
					int m, n;
					for (n = 0; n < total_y; n++) {
						for (m = 0; m < total_x; m++) {
							if (tile[m][n]->isSet != FALSE) {
								int num = tile[m][n]->type;
								switch (num) {
								case -1:
									DBOUT("-");
									break;
								case 0:
									DBOUT("X");
									break;
								case 2:
									DBOUT("+");
									break;
								case 3:
									DBOUT("^");
									break;
								case 4:
									DBOUT("|");
									break;
								case 5:
									DBOUT("T");
									break;
								case 6:
									DBOUT(">");
									break;
								case 7:
									DBOUT("<");
									break;
								case 8:
									DBOUT("!");
									break;
								default:
									DBOUT("O");
								}
							}
							else {
								DBOUT("?");
							}
						}
						DBOUT(endl);
					}
					*/

					DBOUT(endl << endl);
				}


				//Print_Map
				for (j = 0; j < total_y; j++) {
					for (i = 0; i < total_x; i++) {
						if (tile[i][j]->isSet != FALSE) {
							int num = tile[i][j]->type;
							switch (num) {
							case -1:
								DBOUT("-");
								break;
							case 0:
								DBOUT("X");
								break;
							case 2:
								DBOUT("+");
								break;
							case 3:
								DBOUT("^");
								break;
							case 4:
								DBOUT("|");
								break;
							case 5:
								DBOUT("T");
								break;
							case 6:
								DBOUT(">");
								break;
							case 7:
								DBOUT("<");
								break;
							case 8:
								DBOUT("!");
								break;
							default:
								DBOUT("O");
							}
						}
						else {
							DBOUT("?");
						}
					}
					DBOUT(endl);
				}


			}

			//Deal w/ rest of tiles
			for (i = 0; i < total_x; i++) {
				for (j = 0; j < total_y; j++) {
					if (tile[i][j]->isSet == FALSE) {
						tile[i][j]->initiate(-1, temp);
					}
					tile[i][j]->x = i;
					tile[i][j]->y = j;
					tile[i][j]->pic->SetTopLeft(i*TILE_UNIT, j * TILE_UNIT);
				}
				DBOUT("Hi");
			}



			//Create Objects
			character = new Character;
			character->initiate();
			character->x = X_SAFE + 200;
			character->y = Y_SAFE + 370;
			diff_stage_x_left = character->x;
			diff_stage_x_right = character->x;
			x_left_const = X_GENERAL;
			x_right_const = X_GENERAL;


			int now = 3;
			obj_stack.intiate();

			/*
			now = obj_stack.new_obj(0);
			obj_stack.get_obj(now)->initiate();
			obj_stack.get_obj(now)->x = 11 * TILE_UNIT + 15;
			obj_stack.get_obj(now)->y = 12 * TILE_UNIT ;
			*/



			//First Layer
		}
	}
	

	// 鍵盤偵測移動
	void Stage::key_detected(int state, UINT nChar) {
		//State - 0:down, 1:up
	    DBOUT(" " << nChar << " ");

		if (character->isActive == TRUE) {
			
			//M - 77
			if (nChar == 77 && state == 0) {
				player->map = !player->map;
			}


			//L-Shift - 16
			if (nChar == 16) {
				if (state == 1) {
					character->assign("moveLShift", FALSE);
				}
				else {
					character->assign("moveLShift", TRUE);
				}
			}
			
			
			
			
			//Right - 39
			if (nChar == 39) {
				if (state == 1) {
					character->assign("moveRight", FALSE);
				}
				else if (state == 0) {
					character->assign("moveRight", TRUE);
				}
			}

			//Left - 37
			if (nChar == 37) {
				if (state == 1) {
					character->assign("moveLeft", FALSE);
				}
				else if (state == 0) {
					character->assign("moveLeft", TRUE);
				}
			}

			//Down - 40
			if (nChar == 40) {
				if (state == 1) {
					character->assign("moveDown", FALSE);
				}
				else if (state == 0) {
					character->assign("moveDown", TRUE);
				}
			}

			//Up - 38
			if (nChar == 38) {
				if (state == 1) {
					character->assign("moveUp", FALSE);
				}
				else if (state == 0) {
					character->assign("moveUp", TRUE);
				}
			}


			//A - 65
			if (nChar == 65 && state == 0) {
				character->assign("throwBomb", TRUE);
			}

			//S - 83
			if (nChar == 83 && state == 0) {
				character->assign("throwRope", TRUE);
			}


			//X - 88
			if (nChar == 88 && state == 0) {
				character->assign("moveAction", TRUE);
			}


			//Z - 90
			if (nChar == 90 && state == 0) {
				if (character->query("jumping") == FALSE) {
					character->assign("moveJump", TRUE);
				}
			}
		}
		else {
			//Player Dead
		}
		
	}


	My_Stack::My_Stack(void) {
		
	}

	void My_Stack::intiate(void)
	{
		int i;
		for (i = 0; i < MAX_OBJ; i++) {
			obj[i] = NULL;
			exist[i] = 0;
		}
		obj_num = 0;
	}

	void My_Stack::destroy(void) {
		int i;
		for (i = 0; i < MAX_OBJ; i++) {
			if (check_exist(i) != FALSE) {
				delete obj[i];
			}
			obj_num = 0;
		}
	}

	boolean My_Stack::check_exist(int input) {
		if (input<obj_num && input >= 0 && exist[input] != 0) {
			return(TRUE);
		}
		else {
			return(FALSE);
		}
	}

	Object * My_Stack::get_obj(int input) {
		if (check_exist(input) == TRUE) {
			return(obj[input]);
		}
		else {
			return(empty);
		}
	}

	int My_Stack::new_obj(int type) {
		int num;
		boolean flag = FALSE;
		if (obj_num < MAX_OBJ) {
			num = obj_num;
			obj_num++;
			flag = TRUE;
		}
		else {
			int i;
			for (i = 0; i < obj_num; i++) {
				if (check_exist(i) == FALSE) {
					num = i;
					flag = TRUE;
				}
			}
		}

		if (flag == TRUE) {
			exist[num] = 1;
			Object *temp;
			switch (type) {
			case 0:
				temp = new Template;
				break;
			case 1:
				temp = new Coin;
				break;
			case 2:
				temp = new Crate;
				break;
			case 3: 
				temp = new Bomb_Pack;
				break;
			case 4:
				temp = new Rope_Pile;
				break;
			case 5:
				temp = new Bomb;
				break;
			case 6:
				temp = new Bomb_Explode;
				break;
			case 7:
				temp = new Rope;
				break;
			case 8:
				temp = new Pot;
				break;
			case 9:
				temp = new Rock;
				break;
			case 10:
				temp = new Blood;
				break;
			case 11:
				temp = new Snake;
				break;
			case 12:
				temp = new Arrow;
				break;
			case 13:
				temp = new Door;
				break;
			default:
				temp = new Void_Obj;
			}
			obj[num] = temp;
			obj[num]->no = num;
		}
		else {
			num = -1;
		}
		return(num);
	}

	void My_Stack::delete_obj(int input) {
		if (check_exist(input) != FALSE) {
			delete obj[input];
			exist[input] = 0;
		}
	}

	int My_Stack::get_obj_num(void) {
		return(obj_num);
	}
}