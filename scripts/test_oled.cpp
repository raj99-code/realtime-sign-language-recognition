#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <termios.h>
#include <netinet/in.h>
#include <getopt.h>
#include <iostream>

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

ArduiPi_OLED display;

int main() {
    
    // I2C Parameters for SH1106
    if ( !display.init(OLED_I2C_RESET, 6) )
			exit(EXIT_FAILURE);
    
    display.begin(); // Initiate display
    // init done
    
    display.clearDisplay();
    display.display();
    
    int i = 0;
    int j = 0;
    char letter = 'a';
    
    // Program to take character input and print it to LED
    while(1) {
        std::cout << "Enter character: ";
        std::cin >> letter;
        
        if(letter == '/')
            break;
        else if((letter == '*')) {
            display.clearDisplay();
            display.display();}
        else {        
            display.setTextSize(1);
            display.setTextColor(WHITE);
            display.setCursor(i, j);
            display.print(&letter);
            display.display();

            i = (i + 8) % 128;
            
            if (i == 0)
                j = (j + 32) % 64;
            
            if (i == 0 && j == 0) {
                display.clearDisplay();
                display.display();
            }
        }
    }
    
    display.clearDisplay(); // Clear display buffer
    display.display(); // display buffer on screen
}
