#include <SPI.h>

volatile uint8_t process = 0;
byte i = 0, variable = 0;
int8_t c;
int16_t x;

void setup(void) {
    Serial.begin(115200);

    /* have to send on master in so it set as output */
    pinMode(MISO, OUTPUT);
    pinMode(MOSI, INPUT);

    /* turn on SPI in slave mode */
    SPCR |= _BV(SPE);

    process = 0;

    /* turn on interrupt */
    SPI.attachInterrupt(); 
}

/* SPI interrupt routine */
ISR(SPI_STC_vect) {
    /* read byte from SPI Data Register */
    c = SPDR;
    process++;
}

void loop(void) {
    if (process == (uint8_t) 1U) {
       x = c;
       x <<= 8;
    } else if (process == (uint8_t) 2U) {
        process = 0;
        x = x | c;
        
        switch (variable) {
        case 0:
            variable++;
            Serial.print("\nAccX = ");
            Serial.print(x);
            break;
        case 1:
            variable++;
            Serial.print("  AccY = ");
            Serial.print(x);
            break;
        case 2:
            variable++;
            Serial.print("  AccZ = ");
            Serial.print(x);
            break;
        case 3:
            variable++;
            Serial.print("  Temp = ");
            Serial.print(x);
            break;
        case 4:
            variable++;
            Serial.print("  GyroX = ");
            Serial.print(x);
            break;
        case 5:
            variable++;
            Serial.print("  GyroY = ");
            Serial.print(x);
            break;
        case 6:
            Serial.print("  GyroZ = ");
            Serial.print(x);
            variable = 0;
            break;
        }
    }
}
