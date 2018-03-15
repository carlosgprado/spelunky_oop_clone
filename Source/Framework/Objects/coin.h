

namespace game_framework {


	class Coin : public Object {
	public:
		virtual void initiate(void); 
		virtual void initiate(int);

		virtual void onTouch(UI *);
	protected:
	private:
		CMovingBitmap pic1;
		int money;
	};

}