#include <SPI.h>

struct mpu6050_raw_data {
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t temp;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} __attribute__((packed));

void setup(void) {
    Serial.begin(115200);

    /* have to send on master in so it set as output */
    pinMode(MISO, OUTPUT);
    pinMode(MOSI, INPUT);

    /* turn on SPI in slave mode */
    SPCR |= _BV(SPE);

    /* turn on interrupt */
    //SPI.attachInterrupt(); 
}

/* SPI interrupt routine */
ISR(SPI_STC_vect) {
    /* read byte from SPI Data Register */
    c = SPDR;
    process++;
}

void loop(void) {
    struct mpu6050_raw_data data;
    size_t len = sizeof(data);
    uint8_t *p = (uint8_t *) &data;

    while (len--) {
        *(p++) = SPI.transfer(0);
    }

    Serial.print("\nAccX = ");
    Serial.print(data.acc_x);
    Serial.print("  AccY = ");
    Serial.print(data.acc_y);
    Serial.print("  AccZ = ");
    Serial.print(data.acc_z);
    Serial.print("  Temp = ");
    Serial.print(data.temp);
    Serial.print("  GyroX = ");
    Serial.print(data.gyro_x);
    Serial.print("  GyroY = ");
    Serial.print(data.gyro_y);
    Serial.print("  GyroZ = ");
    Serial.print(data.gyro_z);
}
