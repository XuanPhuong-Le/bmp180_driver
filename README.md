# BMP180 Driver for Raspberry Pi

This repository contains the driver for the BMP180 sensor to be used with Raspberry Pi. The BMP180 is a barometer sensor that can measure temperature and pressure. The driver provides an interface for reading raw temperature and pressure data, as well as for calculating real-world values based on sensor data.

## Project Overview

The BMP180 driver is implemented as a Linux kernel module for easy integration with the Raspberry Pi. The driver interacts with the I2C interface on the Raspberry Pi to communicate with the BMP180 sensor and retrieve data such as raw temperature and pressure readings.

## Files

- **bmp180_driver.c**: The main source code file for the BMP180 driver.
- **bmp180.h**: The header file for the BMP180 driver.
- **Makefile**: Build instructions for compiling the BMP180 driver.
- **bmp180_demo.c**: A simple demonstration program to interact with the BMP180 driver.


## Installation

1. Clone the repository:
   
   git clone https://github.com/XuanPhuong-Le/bmp180_driver.git
   cd bmp180_driver

3. Build the kernel module:

   make

4. Insert the module:
   
   sudo insmod bmp180_driver.ko

6. Check the kernel logs to verify that the driver was loaded successfully:

   dmesg | tail

7. To remove the module:

   sudo rmmod bmp180_driver

## Usage

You can use the `bmp180_demo` program to test the driver and see the raw data as well as the converted temperature and pressure values.

./bmp180_demo

This will display the following values:

* **Raw Temperature**: The raw temperature value from the BMP180 sensor.
* **Raw Pressure**: The raw pressure value from the BMP180 sensor.
* **Calculated Temperature**: The temperature in degrees Celsius.
* **Calculated Pressure**: The pressure in Pascals.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

