#define ROPE_SPEED 0.75
#define HOLD_ITEM_SUPER_TIME 10
#define WEAPON_ANIMATION_TIME 2

namespace game_framework {

	class Void_Obj; 

	class Character : public Object {
	public:
		virtual void initiate(void);
		virtual void move(UI *);
		virtual void onTurn(UI *);
		virtual void show(UI *);
		virtual void obj_damage(UI *, Object *);


		virtual boolean query(char*);
		virtual int query_int(char*);
		virtual void assign(char*, boolean);
	protected:
	private:
		boolean onRope;
		boolean onHang;
		boolean onLadder;
		double speed;    //自我水平移動速度
		int strength; //起跳速度
		CMovingBitmap pic[15];
		CMovingBitmap whip_pic[2][11];
		CMovingBitmap *weapon_pic;

		Void_Obj weapon_shadow;

		boolean moveLeft, moveRight, moveJump, jumping, moveAction, moveDown, moveUp, moveLShift, isFast;
		boolean throwBomb, throwRope, canHold, isHolding = FALSE;
		boolean usingWeapon;

		int holding_countdown; 
		int hanging_countdown;
		int weapon_countdown;
		int super_countdown;
		int super_direction;




		void die(UI *);
		void throw_rope(UI *);
		void throw_bomb(UI *);
		void press_action(UI *);


	};

}