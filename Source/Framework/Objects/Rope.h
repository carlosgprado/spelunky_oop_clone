#define ROPE_TILES 6
#define ROPE_ANIMATION_TIME 1 //Game Cycles


namespace game_framework {


	class Rope : public Object {
	public:
		virtual void initiate(void);
		virtual void initiate(int);
		virtual void move(UI *);
		virtual void onTurn(UI *);
		virtual void explode(UI *);
	protected:
	private:
		CMovingBitmap rope[8];
		int level;
	};

}