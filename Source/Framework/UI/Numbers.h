#define MAX_DIGITS 8

namespace game_framework {

	class Numbers {
	public:
		Numbers();
		void initiate(void);
		void set_max(int);
		void set_num(int);
		void increase(void);
		void increase(int);
		void decrease(void);
		void decrease(int);
		int get_current(void);
		void update(void);
		void set_location(int, int, int);
		void onShow(void);
		boolean isAdd = FALSE;
		boolean isTime = FALSE;
	protected:
	private:
		int current_value;
		int max_value, min_value, current_digits;
		CMovingBitmap pic[MAX_DIGITS+1][11];
		CMovingBitmap *digits[MAX_DIGITS+1];
		int x, y, width;
	};

}