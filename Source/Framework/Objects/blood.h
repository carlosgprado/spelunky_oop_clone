

namespace game_framework {


	class Blood : public Object {
	public:
		virtual void initiate(void);
		virtual void onTurn(UI *);
	protected:
	private:
		CMovingBitmap pic1;
	};

}