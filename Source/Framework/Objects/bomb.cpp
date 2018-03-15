#include "StdAfx.h"
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "Numbers.h"
#include "UI.h"
#include "Tile.h"
#include "Objects.h"
#include "framework.h"
#include "bomb.h"
#include "math.h"

namespace game_framework {

	void Bomb::initiate(void) {
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 5, Object::name = "bomb";
		Object::canMove = Object::isActive = TRUE;


		pic1.LoadBitmap("Pictures/Object/Item/bomb_1.bmp", RGB(0, 0, 255));
		pic2.LoadBitmap("Pictures/Object/Item/bomb_2.bmp", RGB(0, 0, 255));
		pic3.LoadBitmap("Pictures/Object/Item/bomb_3.bmp", RGB(0, 0, 255));





		Object::pic = &pic1;
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();

		Object::canPickup = TRUE;

		Object::elasticity = 1;

		Object::cool_down = (int)((BOMB_COUNTDOWN_TIME * (BOMB_COUNTDOWN_TURNS+BOMB_INITIAL_COOLDOWN) * 5 + 3) / (GAME_CYCLE_TIME * 5));
		initial_cooldown = (int)((BOMB_COUNTDOWN_TIME * (BOMB_COUNTDOWN_TURNS)* 5 + 3) / (GAME_CYCLE_TIME * 5));

	}


	void Bomb::onTurn(UI *player) {
		if (Object::cool_down <= initial_cooldown) {
			if (Object::cool_down % (BOMB_COUNTDOWN_TIME / GAME_CYCLE_TIME) == (int)(BOMB_COUNTDOWN_TIME * 3 / (4 * GAME_CYCLE_TIME))) {
				Object::pic = &pic2;
			}
			else if (Object::cool_down % (BOMB_COUNTDOWN_TIME / GAME_CYCLE_TIME) == (int)(BOMB_COUNTDOWN_TIME * 2 / (4 * GAME_CYCLE_TIME))) {
				Object::pic = &pic3;
			}
			else if (Object::cool_down % (BOMB_COUNTDOWN_TIME / GAME_CYCLE_TIME) == (int)(BOMB_COUNTDOWN_TIME / (4 * GAME_CYCLE_TIME))) {
				Object::pic = &pic2;
			}
			else if (Object::cool_down % (BOMB_COUNTDOWN_TIME / GAME_CYCLE_TIME) == 0) {
				Object::pic = &pic3;
			}

		}
		else {
			if (Object::cool_down % (BOMB_COUNTDOWN_TIME / GAME_CYCLE_TIME) == (int)(BOMB_COUNTDOWN_TIME * 3 / (4 * GAME_CYCLE_TIME))) {
				Object::pic = &pic2;
			}
			else if (Object::cool_down % (BOMB_COUNTDOWN_TIME / GAME_CYCLE_TIME) == (int)(BOMB_COUNTDOWN_TIME / (4 * GAME_CYCLE_TIME))) {
				Object::pic = &pic1;
			}
		}

		if (Object::cool_down == 0) {
			Object::isActive = FALSE;
			Object::isShowed = FALSE;
			
			int temp_num = player->obj_stack->new_obj(6);
			Object *temp = player->obj_stack->get_obj(temp_num);
			temp->initiate();
			temp->x = Object::x - (temp->width - pic->Width()) / 2;
			temp->y = Object::y - (temp->height - pic->Height()) / 2;
			temp->rev_update();
			Object::canPickup = FALSE;
		}
	}
	void Bomb::explode(UI *)
	{
		Object::cool_down = 0;
	}


	
	


	void Bomb_Explode::initiate(void)
	{
		Object::temp_x = Object::x = 0;
		Object::temp_y = Object::y = 0;
		Object::x_velocity = Object::y_velocity = 0;
		Object::type = 6, Object::name = "bomb_animation";
		Object::isActive = TRUE;
		Object::isShowed = TRUE;
		Object::canMove = FALSE;


		explosion[0].LoadBitmap("Pictures/Object/Item/Animation/explosion_1.bmp", RGB(0, 255, 0));
		explosion[1].LoadBitmap("Pictures/Object/Item/Animation/explosion_2.bmp", RGB(0, 255, 0));
		explosion[2].LoadBitmap("Pictures/Object/Item/Animation/explosion_3.bmp", RGB(0, 255, 0));
		explosion[3].LoadBitmap("Pictures/Object/Item/Animation/explosion_4.bmp", RGB(0, 255, 0));
		explosion[4].LoadBitmap("Pictures/Object/Item/Animation/explosion_5.bmp", RGB(0, 255, 0));
		explosion[5].LoadBitmap("Pictures/Object/Item/Animation/explosion_6.bmp", RGB(0, 255, 0));
		explosion[6].LoadBitmap("Pictures/Object/Item/Animation/explosion_7.bmp", RGB(0, 255, 0));
		explosion[7].LoadBitmap("Pictures/Object/Item/Animation/explosion_8.bmp", RGB(0, 255, 0));
		explosion[8].LoadBitmap("Pictures/Object/Item/Animation/explosion_9.bmp", RGB(0, 255, 0));
		explosion[9].LoadBitmap("Pictures/Object/Item/Animation/explosion_10.bmp", RGB(0, 255, 0));
		explosion[10].LoadBitmap("Pictures/Object/Item/Animation/explosion_11.bmp", RGB(0, 255, 0));
		explosion[11].LoadBitmap("Pictures/Object/Item/Animation/explosion_12.bmp", RGB(0, 255, 0));
		explosion[12].LoadBitmap("Pictures/Object/Item/Animation/explosion_13.bmp", RGB(0, 255, 0));
		explosion[13].LoadBitmap("Pictures/Object/Item/Animation/explosion_14.bmp", RGB(0, 255, 0));
		explosion[14].LoadBitmap("Pictures/Object/Item/Animation/explosion_15.bmp", RGB(0, 255, 0));
		explosion[15].LoadBitmap("Pictures/Object/Item/Animation/explosion_16.bmp", RGB(0, 255, 0));

		Object::pic = &explosion[0];
		Object::height = Object::pic->Height();
		Object::width = Object::pic->Width();




		Object::elasticity = 1;
		Object::cool_down = EXPLOSION_ANIMATION_TIME * 16;
	}
	

	void Bomb_Explode::onTurn(UI *player) {

		if (Object::cool_down == 0) {
			Object::isActive = FALSE;
			Object::isShowed = FALSE;
		}
		else if (Object::cool_down % EXPLOSION_ANIMATION_TIME == 0) {
			Object::pic = &explosion[(16 - (Object::cool_down / EXPLOSION_ANIMATION_TIME))];

			//Destroy
			if ((Object::cool_down / EXPLOSION_ANIMATION_TIME) == 11) {

				//Player
				if (Object::x + BOMB_NOT_AFFECTED <= player->character->x + player->character->width && Object::x + Object::width - BOMB_NOT_AFFECTED >= player->character->x && Object::y + BOMB_NOT_AFFECTED <= player->character->y + player->character->height && Object::y + Object::height - BOMB_NOT_AFFECTED >= player->character->y) {
					
					player->adj_life(-99);

					double mid_x = Object::x + Object::width / 2;
					double mid_y = Object::y + Object::height / 2;

					double mid_char_x = player->character->x + player->character->width/2;
					double mid_char_y = player->character->y + player->character->height / 2;

					int force = 30;

					player->character->x_velocity = force * std::cos(std::atan((mid_char_y - mid_y)/ (mid_char_x - mid_x)));
					player->character->y_velocity = force * std::sin(std::atan((mid_char_y - mid_y) / (mid_char_x - mid_x)));
				}

				//Tiles
				int start_X = (int)floor((Object::x + BOMB_NOT_AFFECTED) / TILE_UNIT);
				int start_Y = (int)floor((Object::y + BOMB_NOT_AFFECTED) / TILE_UNIT);
				int end_X = (int)floor((Object::x + Object::width - BOMB_NOT_AFFECTED) / TILE_UNIT);
				int end_Y = (int)floor((Object::y + Object::height - BOMB_NOT_AFFECTED) / TILE_UNIT);
				int a = (int)floor((Object::x + Object::width / 2) / TILE_UNIT + 0.5);
				int b = (int)floor((Object::y + Object::height / 2) / TILE_UNIT + 0.5);


				destroy(player, a - 2, b - 1);
				destroy(player, a - 2, b);
				destroy(player, a - 1, b - 2);
				destroy(player, a - 1, b - 1);
				destroy(player, a - 1, b);
				destroy(player, a - 1, b + 1);
				destroy(player, a, b - 2);
				destroy(player, a, b - 1);
				destroy(player, a, b);
				destroy(player, a, b + 1);
				destroy(player, a + 1, b - 1);
				destroy(player, a + 1, b);

				int j; 
				//Object
				for (j = 0; j < player->obj_stack->get_obj_num(); j++) {
					Object *temp = player->obj_stack->get_obj(j);
					if (temp->isShowed == TRUE && temp->hit_test_obj(this)==TRUE) {
						temp->explode(player);
					}
				}
			}


		}
	}
	
	void Bomb_Explode::set(int input)
	{
		Object::pic = &explosion[input];
	}
	
	void Bomb_Explode::destroy(UI *player, int a, int b)
	{
		if (a >= 0 && b >= 0 && a < (TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1 && b < (TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1) {
			player->tile[a][b]->destroy(player);
		}
	}
	

}
