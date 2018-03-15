

namespace game_framework {


	class Arrow : public Object {
	public:
		virtual void initiate(void);
		virtual void initiate(int);
		virtual void onTurn(UI *);
		virtual boolean query(char*);
		virtual void assign(char*, boolean);
	protected:
	private:
		CMovingBitmap pics[2];
		boolean jumping;
	};

}