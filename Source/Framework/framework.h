/*
* framework.h: 本檔案儲遊戲本身的Base Class
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

#define TUTORIAL_NUM 9

namespace game_framework {


	class My_Stack {
	public:
		My_Stack(void);
		void intiate(void);
		void destroy(void);
		Object *get_obj(int);
		int new_obj(int);
		void delete_obj(int);
		int get_obj_num(void);
	protected:
	private:
		int obj_num;
		boolean check_exist(int);
		int exist[MAX_OBJ];
		Object *obj[MAX_OBJ];
		Object *empty;
	};

	class Stage {
	public:
		Stage(void);
		~Stage(void);
		void Stage_Generator(UI *);                     // 建立舞台上面物品及格子
		void Stage_Destructor(UI *);
		void OnMove(void);								// 移動遊戲元素 (處理物理)
		void OnShow(void);								// 顯示Stage的遊戲畫面
		void key_detected(int, UINT);                   // 鍵盤偵測移動
		int obj_num = 0;                                    // Stage上物件個數(Move/Show用)

	protected:
	private:
		int x_addition = (X_SAFE / TILE_UNIT) + 1;    //單邊需加的數量
		int y_addition = (Y_SAFE / TILE_UNIT) + 1;
		int total_x = (TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1;
		int total_y = (TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1;
		UI *player;
		double stage_x, stage_y;
		double diff_stage_x_left, diff_stage_x_right;
		double x_left_const, x_right_const;
		
		CMovingBitmap *background[(TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) / 4 + 1][(TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1];
		Object *obj[MAX_OBJ];                           //Pointer
		Object *character;                           //Player Pointer
		
		Tile *tile[(TILE_WIDTH + ((X_SAFE / TILE_UNIT) + 1) * 2) + 1][(TILE_HEIGHT + ((Y_SAFE / TILE_UNIT) + 1) * 2) + 1];
		My_Stack obj_stack;
		void stage_movement(void);

		CMovingBitmap tutorial_pic[TUTORIAL_NUM];
		double tutorial_position[2][TUTORIAL_NUM];
		boolean tutorial_load = FALSE;


		string bg_location;

		//Levels
		void Build_Template(UI *);
		void Build_Tutorial(UI *);
		void Build_Level(UI *);
	};


}