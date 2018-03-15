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


namespace game_framework {
	void UI::onInit(void)
	{
		hud_death.LoadBitmap("Pictures/UI/hud_death.bmp", RGB(0, 0, 255));
		hud_death.SetTopLeft(0, 3);
		hud_life.LoadBitmap("Pictures/UI/hud_life.bmp", RGB(0, 0, 255));
		hud_life.SetTopLeft(0, 3);
		hud_money.LoadBitmap("Pictures/UI/hud_money.bmp", RGB(0, 0, 255));
		hud_money.SetTopLeft(0, hud_death.Height() - 5);
		tutorial_header.LoadBitmap("Pictures/UI/Tutorial_header.bmp", RGB(255, 0, 0));
		tutorial_header.SetTopLeft(SIZE_X / 2 - tutorial_header.Height() / 2, SIZE_Y - 100);
		world_header.LoadBitmap("Pictures/UI/World_header.bmp", RGB(255, 0, 0));
		world_header.SetTopLeft(SIZE_X / 2 - world_header.Height() / 2, SIZE_Y - 100);
		level_Ones.initiate();
		level_Ones.set_location(SIZE_X / 2 - world_header.Height() / 2 + 175, SIZE_Y - 100 + 7, 17);
		level_Tens.initiate();
		level_Tens.set_location(SIZE_X / 2 - world_header.Height() / 2 + 140, SIZE_Y - 100 + 7, 17);

		new_money.initiate();
		money.initiate();
		bomb.initiate();
		life.initiate();
		rope.initiate();
		isAlive = FALSE;

		hub_map.LoadBitmap("Pictures/UI/map_base.bmp", RGB(255, 0, 0));
		hub_map.SetTopLeft(SIZE_X - hub_map.Width() + 5, -5);
		int i, j;
		for (i = 0; i <= TILE_WIDTH; i++) {
			for (j = 0; j <= TILE_HEIGHT; j++) {
				hub_map_tiles_res[i][j][0].LoadBitmap("Pictures/UI/map_empty.bmp", RGB(255, 0, 0));				
				hub_map_tiles_res[i][j][1].LoadBitmap("Pictures/UI/map_solid.bmp", RGB(255, 0, 0));
				hub_map_tiles_res[i][j][2].LoadBitmap("Pictures/UI/map_here.bmp", RGB(255, 0, 0)); 
				hub_map_tiles_res[i][j][0].SetTopLeft(SIZE_X + 1 - 288 + i*8, -5 + 70 + j * 8);
				hub_map_tiles_res[i][j][1].SetTopLeft(SIZE_X + 1 - 288 + i*8, -5 + 70 + j * 8);
				hub_map_tiles_res[i][j][2].SetTopLeft(SIZE_X + 1 - 288 + i*8, -5 + 70 + j * 8);
				hub_map_tiles[i][j] = &hub_map_tiles_res[i][j][0];
			}
		}
	}
	void UI::reset(void)
	{
		if (level == 0) {
			hud_world = &tutorial_header;
		}
		else {
			hud_world = &world_header;
			level_Ones.set_max(9);
			level_Ones.set_num(((level-1) % 4 + 1));
			level_Tens.set_max(9);
			level_Tens.set_num(((level - 1) / 4 + 1));
		}
		header_countdown = 200; 

		for (int i = 0; i <= TILE_WIDTH; i++) {
			for (int j = 0; j <= TILE_HEIGHT; j++) {
				hub_map_tiles[i][j] = &hub_map_tiles_res[i][j][0];
			}
		}
		
		new_money.set_location(46, 136, 174);
		new_money.set_max(99999999);
		new_money.set_num(0);
		new_money.isAdd = TRUE;
			
		money.set_location( 48, 91,174);
		money.set_max(99999999);
		money.set_num(0);
		
		bomb.set_location(109, 18, 38);
		bomb.set_max(99);
		bomb.set_num(DEFAULT_BOMB);
		useBomb = FALSE;

		life.set_location(19, 26,38);
		life.set_max(99);
		life.set_num(DEFAULT_LIFE);

		rope.set_location(192, 16, 38);
		rope.set_max(99);
		rope.set_num(DEFAULT_ROPE);
		useRope = FALSE;

		new_bomb_turns = 0;
		new_rope_turns = 0;
		new_death_turns = 0;

		holdingNo = -1;
		isAlive = TRUE;
		progress = FALSE;
		elapsed_time = 0;
	}

	void UI::update(void)
	{
		if (life.get_current()<=0 && new_death_turns==0) {
			new_death_turns = 15 * 7;
			character->die(this);
		}
		
		

		if (new_death_turns > 0) {
			new_death_turns--;
			if (new_death_turns == 0) {
				isAlive = FALSE;
			}
		}

		if (new_bomb_turns > 0) {
			new_bomb_turns--;
			if (new_bomb_turns % ((int)(ITEM_UPDATE_TIME / GAME_CYCLE_TIME))== 1) {
				bomb.increase();
			}
		}

		if (new_rope_turns > 0) {
			new_rope_turns--;
			if (new_rope_turns % ((int)(ITEM_UPDATE_TIME / GAME_CYCLE_TIME)) == 1) {
				rope.increase();
			}
		}

		if (useBomb != FALSE) {
			useBomb = FALSE;
			bomb.decrease();
		}
		
		if (useRope != FALSE) {
			useRope = FALSE;
			rope.decrease();
		}

		life.update();
		bomb.update();
		rope.update();
		money.update();


		//The Money Package

		if (new_money_turns > 0) {
			new_money_turns--;
			if (new_money_turns == 0) {
				update_money_turns = (int)(MONEY_UPDATE_TIME / GAME_CYCLE_TIME);
				money.increase(new_money.get_current() % update_money_turns);
				update_amount = (int)floor(new_money.get_current() / update_money_turns);
				new_money.set_num(0);
			}
		}

		if (update_money_turns > 0) {
			update_money_turns--;
			money.increase(update_amount);
			if (update_money_turns == 0) {
				update_amount = 0;
			}
		}

		new_money.update();


		
		//hub_map
		int i, j;
		for (i = 0; i <= TILE_WIDTH; i++) {
			for (j = 0; j <= TILE_HEIGHT; j++) {
				if (tile[i + x_addition][j + y_addition]->isSolid == TRUE) {
					double x = (i + x_addition)*TILE_UNIT + TILE_UNIT / 2, y = (j + y_addition)*TILE_UNIT + TILE_UNIT / 2;
					if (x > *stage_x && x < *stage_x + SIZE_X && y > *stage_y && y < *stage_y + SIZE_Y) {
						hub_map_tiles[i][j] = &hub_map_tiles_res[i][j][1];
					}
				}
				else {
					hub_map_tiles[i][j] = &hub_map_tiles_res[i][j][0];
				}
				
			}
		}
		int x = (int) ((character->x + character->width/2) / TILE_UNIT), y = (int)((character->y + character->height / 2) / TILE_UNIT);
		hub_map_tiles[x - x_addition][y - y_addition] = &hub_map_tiles_res[x - x_addition][y - y_addition][2];
		

	}

	void UI::show(void)
	{
		if (header_countdown > 0) {
			header_countdown--;
			hud_world->ShowBitmap();
			if (level != 0) {
				level_Ones.onShow();
				level_Tens.onShow();
			}
		}
		
		if (map == TRUE) {
			hub_map.ShowBitmap();
			int i, j;
			for (i = 0; i <= TILE_WIDTH; i++) {
				for (j = 0; j <= TILE_HEIGHT; j++) {
					hub_map_tiles[i][j]->ShowBitmap();
				}
			}
		}


		hud_money.ShowBitmap();
		if (life.get_current() > 0) {
			hud_life.ShowBitmap();
			life.onShow();
			bomb.onShow();
			rope.onShow();
		}
		else {
			hud_death.ShowBitmap();
		}
		money.onShow();

		if (new_money_turns > 0) {
			new_money.onShow();
		}
	}

	int UI::num_bomb(void)
	{
		return (bomb.get_current());
	}

	int UI::num_rope(void)
	{
		return (rope.get_current());
	}

	int UI::num_money(void)
	{
		return (money.get_current());
	}

	void UI::gain_money(int input) {
		
		new_money.set_num(new_money.get_current() + input);
		

		new_money_turns = (int)(MONEY_HOT_TIME / GAME_CYCLE_TIME);
		
	}

	void UI::gain_bomb(int input){
		new_bomb_turns += (int)(ITEM_UPDATE_TIME / GAME_CYCLE_TIME) * input + 1;
	}
	
	void UI::gain_rope(int input) {
		new_rope_turns += (int)(ITEM_UPDATE_TIME / GAME_CYCLE_TIME) * input + 1;
	}

	void UI::adj_life(int input)
	{
		if (input == -99) {
			life.set_num(0);
		}
		else if(input>=0){
			life.increase(input);
		}
		else {
			life.decrease(-input);
		}
	}
	void UI::lose_all_item(void)
	{
		money.set_num(0);
		bomb.set_num(0);
		rope.set_num(0);
	}
	void UI::next_level(void)
	{
		character->isActive = FALSE;
		character->isShowed = FALSE;
		progress = TRUE;
		level++; 
		if (level == 0) {
			hud_world = &tutorial_header;
		}
		else {
			hud_world = &world_header;
			level_Ones.set_max(9);
			level_Ones.set_num(((level - 1) % 4 + 1));
			level_Tens.set_max(9);
			level_Tens.set_num(((level - 1) / 4 + 1));
		}
		header_countdown = 200;
	}
}
