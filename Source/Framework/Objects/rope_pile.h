#define ROPE_PER_PILE 3

namespace game_framework {


	class Rope_Pile : public Object {
	public:
		virtual void initiate(void);
		virtual void onTouch(UI *);
	protected:
	private:
		CMovingBitmap pic1;
	};

}