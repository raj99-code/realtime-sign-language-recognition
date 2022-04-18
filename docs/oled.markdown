---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page
title: OLED Display
permalink: /journey/oled/

---

![OLED]({{site.url}}/images/oled.jpg)



## I<sup>2</sup>C Protocol
I<sup>2</sup>C (Inter-Integrated Circuit) is a serial communication Protocol used for short distance communication invented by Philips Semiconductor in 1982. It is alternatively known as I2C or IIC or Two Wired Interface (TWI). The transfer of data takes place bit by bit along the Serial Data (SDA) line and Serial Clock (SCL) line carries the clock signal that makes this protocol synchronous.Both od these lines are bi-directional open-drain lines.

There are 2 modes for this protocol: Master mode and Slave mode.
Master controls the clock signal and is shared between the master and slave. With each high to low pulse on the clock signal (SCL), the transfer of a data bit takes place on SDA line.
### Working of the Protocol

Start Condition: To generate this condition before the SCL changes from high voltage to low voltage, SDA is changed from high voltage to low voltage.

Stop Condition: To generate this condition after the SCL changes from low voltage to high voltage, SDA is changed from low voltage to high voltage.
So both of these conditions are implemented when the SCL is high.
Addressing: It is the 7 to 10 unique bits of each slave with which the master communicates when talking to a particular slave. The slave compares this address with itself and recognizes who the master is talk to and sends back the Acknowledge(ACK) bit.

ACK/NACK bit: Each data frame is followed by Acknwoledgement/no-acknowledgement bit. This bit is sent back by slave to the master when it receives a message successfully to acknowledge it.

Read/Write bit: When the master is sending data to the slave this bit will be high whereas when slave is sending data to the master this bit will be low.
![I2C]({{site.url}}/images/Itocpro.png)

## OLED Driver
OLED display driver integrated circuit(DDIC) controls the OLED display. Some of the commonly known OLED drivers are Adafruit, Seedstudio, Tindie, Generic. When a DDIC a driving signal is sent, the data is displayed onto the display panel like letters and images.
To get the Adafruit generic OLED display driver for Raspberry PI working, you need a Raspberry Pi, a I2C OLED display, wiring cable or ArduiPi board and the [ArduiPi OLED Library source code.](https://github.com/hallard/ArduiPi_OLED)Then you have to setup the I2C bus according to the Raspberry Pi. The wiring setup is as follows:

![oleddriver]({{site.url}}/images/oleddriver.png)
