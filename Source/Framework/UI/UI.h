
#define DEFAULT_LIFE 5
#define DEFAULT_BOMB 3
#define DEFAULT_ROPE 3


#define X_SAFE 120      //上下捲軸位置
#define Y_SAFE 200      //左右捲軸位置
#define X_GENERAL  600  //正常上下卷軸位置
#define TILE_UNIT 64    //格單位
#define TILE_WIDTH 32   //X軸格數
#define TILE_HEIGHT 24 //Y軸格數

#define MONEY_HOT_TIME 1000 //ms
#define MONEY_UPDATE_TIME 500 //ms
#define ITEM_UPDATE_TIME 200 //ms

namespace game_framework {

	class My_Stack;
	class Tile;
	class Object;

	class UI {
	public:
		void onInit(void);
		void reset(void);
		void update(void);
		void show(void);
		int num_bomb(void);
		int num_rope(void);
		int num_money(void);
		void gain_money(int);
		void gain_bomb(int);
		void gain_rope(int);
		void adj_life(int);
		void lose_all_item(void);
		void next_level(void);

		boolean useBomb, useRope, isAlive, progress; 
		int holdingNo;


		Tile *tile[(TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1][(TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1];
		My_Stack* obj_stack;
		Object* character;
		double *stage_x;
		double *stage_y;

		boolean map = TRUE;

		int elapsed_time;
		int level;
	protected:
	private:
		CMovingBitmap hud_death, hud_life, hud_money;
		int new_money_turns, update_amount, update_money_turns;
		int new_bomb_turns, new_rope_turns, new_death_turns;
		Numbers money, bomb, life, rope, new_money;

		//Header
		CMovingBitmap tutorial_header, world_header;
		CMovingBitmap *hud_world;
		int header_countdown;
		Numbers level_Tens, level_Ones;

		//Map
		CMovingBitmap hub_map;
		CMovingBitmap *hub_map_tiles[TILE_WIDTH + 1][TILE_HEIGHT + 1];
		CMovingBitmap hub_map_tiles_res[TILE_WIDTH + 1][TILE_HEIGHT + 1][3];
		int x_addition = (X_SAFE / TILE_UNIT) + 1;    //單邊需加的數量
		int y_addition = (Y_SAFE / TILE_UNIT) + 1;
	};

}