#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define BMP180_ADDRESS 0x77

#define REG_CALIB_START 0xAA
#define REG_CONTROL     0xF4
#define REG_RESULT      0xF6
#define CMD_TEMP        0x2E
#define CMD_PRESSURE    0x34  

int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

int read16(int fd, uint8_t reg) {
    uint8_t buf[2];
    buf[0] = reg;
    if (write(fd, buf, 1) != 1 || read(fd, buf, 2) != 2) {
        perror("Failed to read16");
        exit(1);
    }
    return (buf[0] << 8) | buf[1];
}

void read_calibration_data(int fd) {
    AC1 = (int16_t)read16(fd, 0xAA);
    AC2 = (int16_t)read16(fd, 0xAC);
    AC3 = (int16_t)read16(fd, 0xAE);
    AC4 = (uint16_t)read16(fd, 0xB0);
    AC5 = (uint16_t)read16(fd, 0xB2);
    AC6 = (uint16_t)read16(fd, 0xB4);
    B1  = (int16_t)read16(fd, 0xB6);
    B2  = (int16_t)read16(fd, 0xB8);
    MB  = (int16_t)read16(fd, 0xBA);
    MC  = (int16_t)read16(fd, 0xBC);
    MD  = (int16_t)read16(fd, 0xBE);
}

int read_raw_temp(int fd) {
    uint8_t buf[2];
    buf[0] = REG_CONTROL;
    buf[1] = CMD_TEMP;
    write(fd, buf, 2);
    usleep(4500);

    buf[0] = REG_RESULT;
    write(fd, buf, 1);
    read(fd, buf, 2);
    return (buf[0] << 8) | buf[1];
}

int read_raw_pressure(int fd) {
    uint8_t buf[2];
    buf[0] = REG_CONTROL;
    buf[1] = CMD_PRESSURE;
    write(fd, buf, 2);
    usleep(4500);

    buf[0] = REG_RESULT;
    write(fd, buf, 1);
    read(fd, buf, 2);
    return (buf[0] << 8) | buf[1]; 
}

int main() {
    int fd = open("/dev/i2c-1", O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, BMP180_ADDRESS) < 0) {
        perror("Failed to connect to BMP180");
        return 1;
    }

    read_calibration_data(fd);

    int UT = read_raw_temp(fd);
    int UP = read_raw_pressure(fd);
    

    int X1 = ((UT - AC6) * AC5) >> 15;
    int X2 = ((int32_t)MC << 11) / (X1 + MD);
    int B5 = X1 + X2;
    float temperature = ((B5 + 8) >> 4) / 10.0;
    

    int B6 = B5 - 4000;
    X1 = (B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = (AC2 * B6) >> 11;
    int X3 = X1 + X2;
    int B3 = (((int32_t)AC1 * 4 + X3) + 2) >> 2;

    X1 = (AC3 * B6) >> 13;
    X2 = (B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    uint32_t B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;
    uint32_t B7 = ((uint32_t)UP - B3) * 50000;

    int32_t pressure;
    if (B7 < 0x80000000)
        pressure = (B7 << 1) / B4;
    else
        pressure = (B7 / B4) << 1;

    X1 = (pressure >> 8) * (pressure >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * pressure) >> 16;
    pressure = pressure + ((X1 + X2 + 3791) >> 4);

    
    printf("Raw Temperature: %d\n", UT);
    printf("Raw Pressure: %d\n", UP);
    printf("Temperature: %.2f °C\n", temperature);
    printf("Pressure: %.2f hPa\n", pressure / 100.0);


    close(fd);
    return 0;
}

