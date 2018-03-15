

namespace game_framework {


	class Snake : public Object {
	public:
		virtual void initiate(void);
		virtual void move(UI *);
		virtual void obj_damage(UI *, Object *);

	protected:
	private:
		CMovingBitmap pic1[2][6];
		
		int life;
		double speed; 
		int pic_cool_down;

		void onIdle_Action(UI *);
		void onHunt_Action(UI *);
		void onAttack_Action(UI *);
		void onSuper_Action(UI *);
		void onDeath_Action(UI *);

		FSM brain;
		FSM direction;

		enum state {
			onIdle,
			onHunt,
			onAttack,
			onSuper,
			onDeath
		};
	};

}