#include "led_display.hpp"

LedDisplay::LedDisplay() {
	if(!this->display.init(OLED_I2C_RESET, 6))
		exit(EXIT_FAILURE);
    this->display.begin();
    this->clearDisplay();
    this->row = 0;
    this->col = 0;
}

LedDisplay::~LedDisplay() {
	this->clearDisplay();
}

void LedDisplay::addChar(char &curr_letter) {
	// Add character to current position
	this->display.setTextSize(1);
	this->display.setTextColor(WHITE);
	this->display.setCursor(this->row, this->col);
	this->display.printf("%c", curr_letter);
	this->display.display();
	
	// Update character to next position
	this->updateCursor();
}

void LedDisplay::clearDisplay() {
	this->display.clearDisplay();
	this->display.display();
}

void LedDisplay::updateCursor() {
	this->row = (this->row + 8) % 128;		
	
	if (this->row == 0)
		this->col = (this->col + 32) % 64;
	
	if (this->row == 0 && this->col == 0) {
		this->display.clearDisplay();
	}
}
