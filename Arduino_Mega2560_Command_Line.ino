// Arduino_Command_Line_Example
// Development board: Arduino Mega 2560
// Connections:  none - USB Serial only
// Use Arduino's "Serial Monitor" or Teraterm with local echo
//
// Notes:
// The serial input appears slow.  The may be due to Serial.readString() waiting for timeout.
// Try setting a timeout first
#include <Wire.h>
#include "command_line.h"

// Create a file stream with name, f_out
FILE f_out;
int sput(char c, __attribute__((unused)) FILE* f) {return !Serial.write(c);}

void setup() {
  Wire.begin();
  // Using Serial1, open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial.setTimeout(100); // try 100ms timeout

  // Assign f_out for _FDEV_SETUP_WRITE
  fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE); // cf https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
  stdout = &f_out;

  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  cl_setup();
}

// lame dump routine
void lame_dump(uint8_t *p, int len)
{
  int i=0;
  while (len > 0) {
    printf("%02X ",*p);
    p++; i++; len--;
    if(i>=16) {
      printf("\n");
      i=0;
    }
  }
}


void loop() {
  // Wait for a string from the Arduino "Serial Monitor"
  while(!Serial.available()){}; // spin until more serial data is available

  String ser_data = Serial.readString(); // read in an entire string (or timeout) into String object..
  char * buffer = ser_data.c_str();

  //printf("String: \"%s\"\n",buffer);
  cl_process_buffer(buffer);

  printf(">"); // command prompt

  // Although "flush" is used in all other programming languages as clearing buffers, Arduino doesn't clear the buffers.
  // Arduino's "flush" waits for input data to time-out
  Serial.flush();
  Serial.readString(); // read in the string, thus emptying the buffer

}

#define I2C_ADDRESS_MIN	0x03
#define I2C_ADDRESS_MAX 0x77

int cl_i2c_scan(void)
{
  printf("I2C Scan - scanning I2C addresses 0x%02X - 0x%02X\n",I2C_ADDRESS_MIN,I2C_ADDRESS_MAX);
  // Display Hex Header
  printf("    "); for(int i=0;i<=0x0F;i++) printf(" %0X ",i);
  // Walk through address range 0x00 - 0x77, but only test 0x03 - 0x77
  for(uint8_t addr=0;addr<=I2C_ADDRESS_MAX;addr++) {
    // If address defines the beginning of a row, start a new row and display row text
    if(!(addr%16)) printf("\n%02X: ",addr);
    // Check I2C addresses in the range 0x03-0x7F
    if(addr < I2C_ADDRESS_MIN || addr > I2C_ADDRESS_MAX) {
      printf("   "); // out of range
      continue;
    }
    // Perform I2C device detection
    // The i2c_scanner uses the return value of the Wire.endTransmission to see if
    // a device acknowledged the address.
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();

    if(0 == error)
      printf("%02X ",addr);
    else
      printf("-- ");
  } // for-loop
  printf("\n");
  return 0;
}


