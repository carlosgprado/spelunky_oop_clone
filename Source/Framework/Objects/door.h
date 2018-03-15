

namespace game_framework {


	class Door : public Object {
	public:
		virtual void initiate(int);
		virtual void initiate(void); 
		virtual void explode(UI *);
		virtual void onTurn(UI *);
	protected:
	private:
		CMovingBitmap pic1;
		Void_Obj hit_zone;
	};

}