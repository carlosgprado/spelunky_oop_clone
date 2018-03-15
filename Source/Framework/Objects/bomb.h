#define BOMB_COUNTDOWN_TURNS 4
#define BOMB_INITIAL_COOLDOWN 6
#define BOMB_COUNTDOWN_TIME 500 //ms (per turn) 1->wait->2->wait->3->wait->2->wait->1
#define EXPLOSION_ANIMATION_TIME 2 //game_cycle 
#define BOMB_NOT_AFFECTED 10

namespace game_framework {

	class Bomb_Explode : public Object {
	public:
		virtual void initiate(void);
		virtual void set(int);
		virtual void onTurn(UI *);

	protected:
	private:
		CMovingBitmap explosion[16];
		void destroy(UI *, int, int);
	};

	class Bomb : public Object {
	public:
		virtual void initiate(void);
		virtual void onTurn(UI *);
		virtual void explode(UI *);
	protected:
	private:
		CMovingBitmap pic1, pic2, pic3;
		int initial_cooldown;
	};

	

}