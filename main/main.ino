// Include the necessary header files
#include "BIT_MATH.h"
#include "LCD.h"
#include "DIO.h"
#include "ADC.h"
#include "FSC.h"


const int PUSH_BUTTON1 = 2;       // Define the first push button pin
const int PUSH_BUTTON2 = 3;       // Define the second push button pin
const int RED_LED = 4;            // Define the RED_LED pin
const int GREEN_LED = 5;          // Define the GREEN_LED pin


int main()
{
  // Initialize necessary Functions
  Adc_Init();                        /* Initialize ADC */
  LCD_Init();                        /* Initialize LCD */
  initIO();                          /* Initialize I/O */

  // Declare variables
  unsigned short adc_reading;
  unsigned char buffer[6];
  float MAX_Resistance = 10000.0;
  float Ratio = MAX_Resistance/5.0;
  float voltage;
  float Rx;
  float Limit = 5000.0;

  
  while (1)
  {
    _delay_ms(100);
    
    // Read ADC channel 1 and convert to voltage
    adc_reading = Adc_ReadChannel(1);
    if(adc_reading>0)
    {
      voltage = (adc_reading)*(5.0/1023);
    }
    else
    {
      voltage = (adc_reading)*(5.0/1023);
    }
    Rx = voltage*Ratio;     // Getting the Resistance value from the voltage


    LCD_Command (0x80);            // Set LCD cursor to the beginning of the first line
    LCD_String("R:");              // Display R: on LCD
    floatToString(Rx, buffer, 2);  // Convert the float to string with 2 decimal places and save it in the buffer array
    LCD_String(buffer);            // Display The value of resistance on LCD

    LCD_Command(0xC0);		            // Set LCD cursor to the beginning of the second line
	  LCD_String("Limit:");             // Display R: on LCD
    floatToString(Limit, buffer, 2);  // Convert the float to string with 2 decimal places and save it in the buffer array
    LCD_String(buffer);               // Display The value of Limit on LCD
    
    // Adjust Limit based on button presses
    if(DIO_GetPinValue(DIO_PORTD, PUSH_BUTTON1) == DIO_HIGH)
    {
      Limit=Limit+50;
      _delay_ms(50);
    }
    else if(DIO_GetPinValue(DIO_PORTD, PUSH_BUTTON2) == DIO_HIGH)
    {
      Limit=Limit-50;
      _delay_ms(50);
    }


    // Ensure Limit is within valid range
    if(Limit > 10000)
    {
      Limit = 10000;
    }
    else if(Limit < 0)
    {
      Limit = 0;
    }


    // Control LED based on comparison between Rx and Limit
    if(Rx>Limit)
    {
      DIO_SetPinValue(DIO_PORTB, RED_LED, DIO_HIGH);
      DIO_SetPinValue(DIO_PORTB, GREEN_LED, DIO_LOW);
    }
    else if(Limit>Rx)
    {
      DIO_SetPinValue(DIO_PORTB, RED_LED, DIO_LOW);
      DIO_SetPinValue(DIO_PORTB, GREEN_LED, DIO_HIGH);
    }
  }
  return 0;
}

