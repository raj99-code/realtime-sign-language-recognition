#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

class LedDisplay {
	public:
		LedDisplay();
		~LedDisplay();
		void addChar(char &curr_letter);
		void clearDisplay();
	private:
		ArduiPi_OLED display;
		int row;
		int col;
		void updateCursor();
};
