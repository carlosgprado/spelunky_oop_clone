#define BREAK_CONST 3
#define ENEMY_SUPER_TIME 20

namespace game_framework {

	class Object {
	public:
		int no;
		Object(void);
		~Object(void);
		double x, y;
		int width, height;
		double elasticity; //彈性
		double x_velocity; //垂直被動移動速度
		double y_velocity; //水平被動移動速度
		boolean canMove, isActive, onAir, isShowed;
		boolean canPickup, canBreak, canDamage, canHurt, canTouch;

		CMovingBitmap *pic;
		void physics(void);                    //物理
		virtual void move(UI *);               //Inherted Classes移動用 (改變temp_x, temp_y)
		virtual int test(UI *);                //測試是否允許移動
		void update(void);                     //暫定座標-->確定座標
		void rev_update(void);                 //確定座標-->暫定座標


		virtual void initiate(void) = 0;
		virtual void initiate(int);

		boolean onlyShow = TRUE; 

		int cool_down = 0; //Can not be touched
		

		virtual void die(UI *);
		virtual void onTurn(UI *);

		virtual void explode(UI *);


		virtual void show(UI *);
		virtual void obj_damage(UI *, Object *);

		virtual boolean query(char*);
		virtual int query_int(char*);
		virtual void assign(char*, boolean);

		virtual void onTouch(UI *);
		virtual void onHit(UI *, Object *);
		boolean hit_test_obj(Object *);
	protected:
		string name = "Name not 'virtual-ed'"; //物件名稱(偵錯用)
		double temp_x, temp_y;
		int type = 0; //0-character 1-coin, 2-crate, 3-bomb_pack, 4-rope_pile, 5-bomb, 6-bomb_animation,
		// 7 - Rope, 8 - Pot, 9 - Rock, 10 - Blood, 11 - Snake, 12 - Arrow, 13 - Door,

		int state;
	private:
	};

	class FSM {
	public:
		void popState(void);
		void pushState(int);
		int getState(void);
	private:
		int stack[10];
		int stack_pointer = -1;
	};
}