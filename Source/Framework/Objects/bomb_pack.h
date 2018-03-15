#define BOMB_PER_PACK 3

namespace game_framework {


	class Bomb_Pack : public Object {
	public:
		virtual void initiate(void);
		virtual void onTouch(UI *);
	protected:
	private:
		CMovingBitmap pic1;
	};

}