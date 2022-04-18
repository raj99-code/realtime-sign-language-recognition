#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

/**
   * @brief Driver class for the OLED display. 
   The recognition output is passed to the functions of this class to be emitted to the external display.
   * 
   *
   */
class LedDisplay {
	public:
		LedDisplay();
		~LedDisplay();
		/**
   * @brief Adds passed character to the current position in the display
   * 
   *
   */
		void addChar(char &curr_letter);
	private:
		ArduiPi_OLED display;
		int row;
		int col;
			/**
   * @brief Clear the OLED display when called
   * 
   *
   */
		void clearDisplay();

			/**
   * @brief Moves the display cursor to the next position
   * 
   *
   */
		void updateCursor();
};
