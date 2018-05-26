/***************************************************************************************************/
/*
  This is an Arduino library for Quadrature Rotary Encoder 

  written by : enjoyneering79
  sourse code: https://github.com/enjoyneering/

  This library uses interrupts, specials pins are required to interface
  Board:                                    int.0  int.1  int.2  int.3  int.4  int.5
  Uno, Mini, Pro, ATmega168, ATmega328..... 2      3      x       x      x     x
  Mega2560................................. 2      3      21      20     19    18
  Leonardo, Micro, ATmega32U4.............. 3      2      0       1      7     x
  Digistump, Trinket, ATtiny85............. 2/physical pin no.7
  Due...................................... all digital pins
  Zero..................................... all digital pins, except pin 4
  Arduino STM32,STM32F103xxxx boards....... all digital pins, maximun 16 pins at the same time
  ESP8266.................................. all digital pins, except gpio6 - gpio11 & gpio16

  Frameworks:
  ATtiny  Core - https://github.com/SpenceKonde/ATTinyCore 
  ESP8266 Core - https://github.com/esp8266/Arduino
  STM32   Core - https://github.com/rogerclarkmelbourne/Arduino_STM32

  NOTE: - Quadrature encoder makes two waveforms that are 90 deg. out of phase:
                           _______         _______         __
                  PinA ___|       |_______|       |_______|   PinA
          CCW <--              _______         _______
                  PinB _______|       |_______|       |______ PinB

                               _______         _______
                  PinA _______|       |_______|       |______ PinA
          CW  -->          _______         _______         __
                  PinB ___|       |_______|       |_______|   PinB


          The half of the pulses from top to bottom create full state array:  

          prev.A+B   cur.A+B   (prev.AB+cur.AB)  Array   Encoder State
          -------   ---------   --------------   -----   -------------
            00         00            0000          0     stop/idle
            00         01            0001          1     CW,  0x01
            00         10            0010         -1     CCW, 0x02
            00         11            0011          0     invalid state
            01         00            0100         -1     CCW, 0x04
            01         01            0101          0     stop/idle
            01         10            0110          0     invalid state
            01         11            0111          1     CW, 0x07
            10         00            1000          1     CW, 0x08
            10         01            1001          0     invalid state
            10         10            1010          0     stop/idle
            10         11            1011         -1     CCW, 0x0B
            11         00            1100          0     invalid state
            11         01            1101         -1     CCW, 0x0D 
            11         10            1110          1     CW,  0x0E
            11         11            1111          0     stop/idle

        - CW  states 0b0001, 0b0111, 0b1000, 0b1110
        - CСW states 0b0010, 0b0100, 0b1011, 0b1101

  GNU GPL license, all text above must be included in any redistribution, see link below for details:
  - https://www.gnu.org/licenses/licenses.html
*/
/***************************************************************************************************/
#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#if defined(ARDUINO) && ARDUINO >= 100 //arduino core v1.0 or later
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#if defined(__AVR__)
  #include <avr/pgmspace.h>            //use for PROGMEM Arduino AVR
#elif defined(ESP8266)
  #include <pgmspace.h>                //use for PROGMEM Arduino ESP8266
#elif defined(_VARIANT_ARDUINO_STM32_)
  #include <avr/pgmspace.h>            //use for PROGMEM Arduino STM32
#endif


class RotaryEncoder
{
  public:
    RotaryEncoder(uint8_t encoderA, uint8_t encoderB, uint8_t encoderButton);

    void     begin(void);
    void     readAB(void);
    void     readPushButton(void);

    int16_t  getPosition(void);
    bool     getPushButton(void);
  
    void     setPosition(int16_t position);
    void     setPushButton(bool state);

  private:
             uint8_t _encoderA;           //pin "A"
             uint8_t _encoderB;           //pin "B"
             uint8_t _encoderButton;      //pin "button"

    volatile uint8_t _prevValueAB = 0;    //previouse state of "A"+"B", "volatile" prevent compiler to make optimization/unnecessary changes in the code with the variable
    volatile uint8_t _currValueAB = 0;    //current   state of "A"+"B"
    volatile int16_t _counter     = 0;    //encoder click counter
    volatile bool    _buttonState = true; //encoder button idle status, "true" because internal pull-up resistor is enabled
};

#endif
