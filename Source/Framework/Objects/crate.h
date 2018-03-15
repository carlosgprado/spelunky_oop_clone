#define CRATE_COIN_HOT_TIME 800
#define CRATE_HOT_TIME 8000
#define MAX_COIN 8
#define MIN_COIN 3

namespace game_framework {


	class Crate : public Object {
	public:
		virtual void initiate(void);
		virtual void initiate(int);
		virtual void move(UI *);
		virtual void obj_damage(UI *, Object *);
		virtual int query_int(char*);

	protected:
	private:
		CMovingBitmap pic1, pic2;
		boolean isOpen;
		int level; //Affect coin amount & quality
		int toughness; //crate_break needed strength (1/2)
		int special = 0; //obj-num
	};

}