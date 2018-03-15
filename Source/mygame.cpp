/*
* mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
* History:
*   2002-03-04 V3.1
*          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
*	 2004-03-02 V4.0
*      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
*         demonstrate the use of states.
*      2. Demo the use of CInteger in CGameStateRun.
*   2005-09-13
*      Rewrite the codes for CBall and CEraser.
*   2005-09-20 V4.2Beta1.
*   2005-09-29 V4.2Beta2.
*      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
*   2006-02-08 V4.2
*      1. Revise sample screens to display in English only.
*      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
*      3. Rename OnInitialUpdate() -> OnInit().
*      4. Fix the bug that OnBeginState() of GameStateInit is not called.
*      5. Replace AUDIO_CANYON as AUDIO_NTUT.
*      6. Add help bitmap to CGameStateRun.
*   2006-09-09 V4.3
*      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
*         event driven.
*   2006-12-30
*      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
*         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
*   2008-02-15 V4.4
*      1. Add namespace game_framework.
*      2. Replace the demonstration of animation as a new bouncing ball.
*      3. Use ShowInitProgress(percent) to display loading progress.
*   2010-03-23 V4.6
*      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/



#include "stdafx.h"
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
#include <iostream>
#include <string>
#include <cmath>
#include <Windows.h>
#include <sstream>
#include <ctime>


#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}


namespace game_framework {

	
	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	/////////////////////////////////////////////////////////////////////////////

	CGameStateInit::CGameStateInit(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateInit::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(0);	// 一開始的loading進度為0%
								//
								// 開始載入資料
								//
		start_bg.LoadBitmap("Pictures/UI/start_menu_bg.bmp",RGB(255,0,0));
		start_text.LoadBitmap("Pictures/UI/start_menu.bmp", RGB(255, 0, 0));
		level_bg.LoadBitmap("Pictures/UI/level_menu.bmp", RGB(255, 0, 0));
		level_text.LoadBitmap("Pictures/UI/level_menu_text.bmp", RGB(255, 0, 0));
		level[0].LoadBitmap("Pictures/UI/level_menu_0.bmp", RGB(255, 0, 0));
		level[1].LoadBitmap("Pictures/UI/level_menu_1.bmp", RGB(255, 0, 0));
		level[2].LoadBitmap("Pictures/UI/level_menu_2.bmp", RGB(255, 0, 0));
		level[3].LoadBitmap("Pictures/UI/level_menu_3.bmp", RGB(255, 0, 0));
		level[4].LoadBitmap("Pictures/UI/level_menu_4.bmp", RGB(255, 0, 0));
		level_pic = &level[0];
		cur_level = -1;
								//
								// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
								//
	}

	void CGameStateInit::OnBeginState()
	{
		level_pic = &level[0];
		if (cur_level != -1) {
			cur_level = 0;
		}
		count_down = 30;
	}

	void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		const char KEY_ESC = 27;
		//X - 88
		if (nChar == 88) {
			switch (cur_level) {
			case -1:
				break;
			case 0:
				cur_level = 1;
				level_pic = &level[1];
				break;
			case 1:
				cur_level = 5;
				level_pic = &level[2];
				break;
			case 5:
				cur_level = 9;
				level_pic = &level[3];
				break;
			case 9:
				cur_level = 13;
				level_pic = &level[4];
				break;
			case 13:
				cur_level = 0;
				level_pic = &level[0];
				break;
			default:
				cur_level = 0;
				level_pic = &level[0];
				break;
			}
		}
		
		//Z - 90
		if (nChar == 90) {
			if (cur_level == -1) {
				cur_level++;
				level_pic = &level[0];
			}
			else {
				GotoGameState(GAME_STATE_RUN,cur_level);
			}
		}
		
		if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
	}

	void CGameStateInit::OnShow()
	{
		//
		// Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
		//

		count_down--;
		if (cur_level == -1) {
			start_bg.ShowBitmap();
			if (count_down > 0) {
				start_text.ShowBitmap();
			}
			else if (count_down < -30) {
				count_down = 30;
			}
		}
		else {
			level_pic->ShowBitmap();
			level_bg.ShowBitmap();
			if (count_down > 0) {
				level_text.ShowBitmap();
			}
			else if (count_down < -25) {
				count_down = 45;
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	/////////////////////////////////////////////////////////////////////////////

	CGameStateOver::CGameStateOver(CGame *g)
		: CGameState(g)
	{
	}

	void CGameStateOver::OnMove()
	{
		if (player_p->isAlive == TRUE) {
			if (countdown != 0) {
				countdown--;
				if (countdown%animation_cycle == 0) {
					if (countdown == 0) {
						win_pic = &win_pics[0];
						win_menu = &win_menu_2;
						update_countdown = (int)(update_time / GAME_CYCLE_TIME);
						money.set_num(player_p->num_money() %update_countdown);
						money_update_amount = (int) floor(player_p->num_money() / update_countdown);

						time_update_amount = (int)(player_p->elapsed_time / update_countdown);
						cur_time = player_p->elapsed_time % update_countdown;
						int time = cur_time;
						second.set_num(time % 60);
						time /= 60;
						minute.set_num(time % 60);
						time /= 60;
						hour.set_num(time % 60);
						time /= 60;

					}
					else {
						win_pic = &win_pics[10 - countdown / animation_cycle];
					}
				}
			}
		}

		if (update_countdown != 0) {
			update_countdown--;
			money.set_num(money.get_current() + money_update_amount);
			cur_time += time_update_amount;
			int time = cur_time;
			second.set_num(time % 60);
			time /= 60;
			minute.set_num(time % 60);
			time /= 60;
			hour.set_num(time % 60);
			time /= 60;
		}


	}

	void CGameStateOver::OnBeginState()
	{
		money.set_location(400, 440, 150);
		hour.set_location(398, 492, 44);
		minute.set_location(456, 492, 44);
		second.set_location(517, 492, 44);

		if (player_p->isAlive != TRUE) {
			update_countdown = (int)(update_time / GAME_CYCLE_TIME);
			money.set_num(player_p->num_money() % update_countdown);
			money_update_amount = (int)floor(player_p->num_money() / update_countdown);

			time_update_amount = (int)(player_p->elapsed_time / update_countdown);
			cur_time = player_p->elapsed_time % update_countdown;
			int time = cur_time;
			second.set_num(time % 60);
			time /= 60;
			minute.set_num(time % 60);
			time /= 60;
			hour.set_num(time % 60);
			time /= 60;

			if (player_p->level == 0) {
				death_lv = &lv[0];
			}
			else {
				death_lv = &lv[((player_p->level - player_p->level%4)/4) +1];
			}
		}
		else {
			//Win

			countdown = animation_cycle * 9 - 1; 
			win_menu = &win_menu_1;
			win_pic = &win_pics[1];

		}

	}

	void CGameStateOver::OnInit()
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
								//
								// 開始載入資料
								//
		death_menu.LoadBitmap("Pictures/UI/death_menu.bmp", RGB(0, 0, 0));
		lv[0].LoadBitmap("Pictures/UI/level_0.bmp", RGB(255, 0, 0));
		lv[1].LoadBitmap("Pictures/UI/level_1.bmp", RGB(255, 0, 0));
		lv[2].LoadBitmap("Pictures/UI/level_2.bmp", RGB(255, 0, 0));
		lv[3].LoadBitmap("Pictures/UI/level_3.bmp", RGB(255, 0, 0));
		lv[4].LoadBitmap("Pictures/UI/level_4.bmp", RGB(255, 0, 0));

		win_menu_1.LoadBitmap("Pictures/UI/win_menu.bmp");
		win_menu_2.LoadBitmap("Pictures/UI/win_menu_2.bmp");

		win_pics[0].LoadBitmap("Pictures/Tile/default.bmp", RGB(0, 0, 0));
		win_pics[1].LoadBitmap("Pictures/UI/win_pic_1.bmp", RGB(0, 0, 255));
		win_pics[2].LoadBitmap("Pictures/UI/win_pic_2.bmp", RGB(0, 0, 255));
		win_pics[3].LoadBitmap("Pictures/UI/win_pic_3.bmp", RGB(0, 0, 255));
		win_pics[4].LoadBitmap("Pictures/UI/win_pic_4.bmp", RGB(0, 0, 255));
		win_pics[5].LoadBitmap("Pictures/UI/win_pic_5.bmp", RGB(0, 0, 255));
		win_pics[6].LoadBitmap("Pictures/UI/win_pic_6.bmp", RGB(0, 0, 255));
		win_pics[7].LoadBitmap("Pictures/UI/win_pic_7.bmp", RGB(0, 0, 255));
		win_pics[8].LoadBitmap("Pictures/UI/win_pic_8.bmp", RGB(0, 0, 255));
		win_pics[9].LoadBitmap("Pictures/UI/win_pic_9.bmp", RGB(0, 0, 255));

		win_pic = &win_pics[0];
		win_menu = &win_menu_1;

		death_menu.SetTopLeft(0, 0);
		lv[0].SetTopLeft(0, 0);
		lv[1].SetTopLeft(0, 0);
		lv[2].SetTopLeft(0, 0);
		lv[3].SetTopLeft(0, 0);
		lv[4].SetTopLeft(0, 0);

		death_lv = &lv[0];

		money.initiate();
		hour.initiate();
		minute.initiate();
		second.initiate();
		hour.isTime = TRUE;
		minute.isTime = TRUE;
		second.isTime = TRUE;
		money.set_max(999999999);
		hour.set_max(60);
		minute.set_max(60);
		second.set_max(60);
								// 最終進度為100%
								//
		ShowInitProgress(100);
	}

	void CGameStateOver::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 27) {				// Demo 關閉遊戲的方法
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
		}

		if (player_p->isAlive == FALSE){
			if (nChar == 90) {
				GotoGameState(GAME_STATE_RUN);
			}
			else if (nChar == 32) {
				GotoGameState(GAME_STATE_INIT);
			}
		}
		else {
			if (nChar == 32) {
				GotoGameState(GAME_STATE_INIT);
			}
		}
		
	}

	void CGameStateOver::OnShow()
	{
		if (player_p->isAlive != TRUE) {
			death_menu.ShowBitmap();
			death_lv->ShowBitmap();
			money.onShow();
			hour.onShow();
			minute.onShow();
			second.onShow();
		}
		else {
			win_menu->ShowBitmap();
			win_pic->ShowBitmap();
			if (countdown == 0) {
				money.onShow();
				hour.onShow();
				minute.onShow();
				second.onShow();
			}
		}



	}

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g)
		: CGameState(g)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
	}

	void CGameStateRun::OnBeginState(int level)
	{
		player.level = level;
		if (player.isAlive == FALSE) {
			player.reset();
			start_time = std::clock();
		}

		stage.Stage_Generator(&player);
	}

	void CGameStateRun::OnBeginState()
	{
		if (player.isAlive == FALSE) {
			player.reset();
			start_time = std::clock();
		}

		stage.Stage_Generator(&player);
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//
		// 如果希望修改cursor的樣式，則將下面程式的commment取消即可
		//
		// SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
		//
		if (player.isAlive == TRUE) {
			if (player.progress == TRUE) {
				//Move to Next Level
				player.progress = FALSE;
				stage.Stage_Destructor(&player);
				if (player.level >= 17) {
					end_time = std::clock();
					player.elapsed_time += (int)((double)(end_time - start_time) / CLOCKS_PER_SEC);

					GotoGameState(GAME_STATE_OVER, &player);
				}
				else{
					OnBeginState();
				}
			}
			else {
				stage.OnMove();
			}
		}
		else {
			end_time = std::clock();
			player.elapsed_time += (int)((double)(end_time - start_time) / CLOCKS_PER_SEC);

			stage.Stage_Destructor(&player);
			GotoGameState(GAME_STATE_OVER, &player);
		}
	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		//
		// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
		//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
		//
		ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
								//
								// 開始載入資料
								//
		player.onInit();
								//
								// 完成部分Loading動作，提高進度
								//
		ShowInitProgress(50);
					// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
					//
					// 繼續載入其他資料
					//
					//
					// 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
					//
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		stage.key_detected(0, nChar);
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 27) {				// Demo 關閉遊戲的方法
			stage.Stage_Destructor(&player);
			PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
		}

		if (nChar == 82 && player.isAlive == TRUE) {
			stage.Stage_Destructor(&player);
			player.isAlive = FALSE;
			OnBeginState();
		}

		stage.key_detected(1, nChar);
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		// 沒事。如果需要處理滑鼠移動的話，寫code在這裡
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnShow()
	{
		//
		//  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
		//        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
		//        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
		//
		//
		if(player.isAlive == TRUE)
			stage.OnShow();
	}

}