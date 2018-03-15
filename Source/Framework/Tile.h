#define MAX_OBJ 1000     //最大關卡上物件數
#define GRAVITY_CONST 0.5 //重力係數
#define AIR_FRICTION_CONST 0.03125 //空氣阻力係數
#define PI 3.14159265

namespace game_framework {

	class Tile {
	public:
		Tile(void);
		~Tile(void);
		void onInit(void);
		int type;            //0 = Solid, 1 = Not-Solid

		double elasticity;   //彈性
		CMovingBitmap *pic;
		boolean isSolid;     //是否固體
		boolean topSolid = FALSE;
		int hasRope;     
		boolean isSet;
		void initiate(int, UI*);
		void destroy(UI *);
		void onTurn(UI *);
		int x, y;
	protected:
	private:
		int level;
		boolean flag = FALSE;
	};

}