

namespace game_framework {


	class Rock : public Object {
	public:
		virtual void initiate(void);
		virtual void onTurn(UI *);
		virtual boolean query(char*);
		virtual void assign(char*, boolean);
	protected:
	private:
		CMovingBitmap pic1;
		boolean jumping;
	};

}