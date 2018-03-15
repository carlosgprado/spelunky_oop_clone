/*
* mygame.h: 本檔案儲遊戲本身的class的interface
* Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
*
* This file is part of game, a free game development framework for windows.
*
* game is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* game is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*	 2004-03-02 V4.0
*      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
*         demonstrate the use of states.
*   2005-09-13
*      Rewrite the codes for CBall and CEraser.
*   2005-09-20 V4.2Beta1.
*   2005-09-29 V4.2Beta2.
*   2006-02-08 V4.2
*      1. Rename OnInitialUpdate() -> OnInit().
*      2. Replace AUDIO_CANYON as AUDIO_NTUT.
*      3. Add help bitmap to CGameStateRun.
*   2006-09-09 V4.3
*      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
*         event driven.
*   2008-02-15 V4.4
*      1. Add namespace game_framework.
*      2. Replace the demonstration of animation as a new bouncing ball.
*      3. Use ShowInitProgress(percent) to display loading progress.
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////
/*
#define MAX_OBJ 300     //最大關卡上物件數
#define TILE_WIDTH 32   //X軸格數
#define TILE_HEIGHT 24  //Y軸格數
#define TILE_UNIT 64    //格單位
#define GRAVITY_CONST 0.5 //重力係數
*/

namespace game_framework {
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
		boolean moveLeft, moveRight, moveJump, jumping;
		virtual void initiate(void) = 0;
	protected:
		string name = "Name not 'virtual-ed'"; //物件名稱(偵錯用)
		double temp_x, temp_y;
		int mode;
		boolean bouncy; //會不會反彈(x,y)
	private:
	};

	

	class Stage {
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
		double stage_x, stage_y;
		CMovingBitmap background[3][2];
		Object *obj[MAX_OBJ];                           //Pointer
		Tile tile[TILE_WIDTH][TILE_HEIGHT];              //Actual Class
	};

	
	class Character : public Object {
	public:
		virtual void initiate(void);
		virtual void move(void);
		boolean moveLeft, moveRight, moveJump;
	protected:
	private:
		int speed;    //自我水平移動速度
		int strength; //起跳速度
	};
	*/

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyDown(UINT, UINT, UINT);
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap start_bg, start_text;
		CMovingBitmap level_bg, level[5], level_text;
		CMovingBitmap *level_pic;
		int count_down;
		int cur_level = -1;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState(int);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		UI player;
		Stage stage;
		clock_t start_time, end_time;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
		void OnKeyDown(UINT, UINT, UINT);
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap death_menu, lv[5], *death_lv;
		Numbers hour, minute, second, money;

		CMovingBitmap win_menu_1, win_menu_2;
		CMovingBitmap win_pics[10];
		CMovingBitmap *win_menu, *win_pic;


		int animation_cycle = 50;
		int countdown= 0;
		int update_countdown;
		int update_time = 1000;
		int money_update_amount;
		int cur_time = 0;
		int time_update_amount;

	};

}