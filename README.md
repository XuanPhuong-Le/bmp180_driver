# BMP180 Driver for Raspberry Pi
---------------------------------
This repository contains a Linux kernel module for interfacing the BMP180 temperature and pressure sensor with a Raspberry Pi via the I2C bus. The BMP180 is a barometric pressure sensor that also provides temperature readings. This project allows you to build a working driver, insert it into the kernel, and test it with a simple user-space application.


## Project Structure
---------------------
- **bmp180_driver.c**: Main kernel driver that interfaces with the BMP180 sensor over I2C.
- **bmp180.h**: Header file with declarations and I2C register definitions.
- **Makefile**: Used to build the kernel module and demo application.
- **bmp180_demo.c**: A user-space C program that opens the driver and prints temperature and pressure.


## Requirements
----------------
- Raspberry Pi with Raspberry Pi OS.
- BMP180 sensor connected via I2C.
- Kernel headers installed for your Raspberry Pi’s current kernel version.


## Installation
-----------------
1. **Clone the Repository**
----------------------------
   ```
   git clone https://github.com/XuanPhuong-Le/bmp180_driver.git
   cd bmp180_driver
   ````
2. **Build the Module and Demo**
---------------------------------
   ```
   make
   ```

3. **Insert the Kernel Module**
--------------------------------
   ```
   sudo insmod bmp180_driver.ko
   ```

4. **Verify Module Loaded**
----------------------------
   ```
   dmesg | tail
   ```

5. **Run Demo Program**
------------------------
   ```
   ./bmp180_demo
   ```

6. **Remove the Module**
-------------------------
   ```
   sudo rmmod bmp180_driver
   ```


## Output Example
------------------
The demo program will output:
------------------------------
```
Raw Temperature: 30995
Raw Pressure: 40696
Temperature: 31.30 °C
Pressure: 1005.52 hPa

```


## File Description
--------------------
| File              | Description                                    |
| ----------------- | ---------------------------------------------- |
| `bmp180_driver.c` | Kernel driver for the BMP180 over I2C          |
| `bmp180.h`        | Sensor register definitions and declarations   |
| `Makefile`        | Build instructions for module and demo         |
| `bmp180_demo.c`   | Userspace program to test driver functionality |



## Important Functions in `bmp180_driver.c`
--------------------------------------------
### `s16 bmp180_read_raw_temp(void)`
-------------------------------------
* **Purpose**: Reads the raw temperature data from the BMP180 sensor.
* **Operation**:
  * Sends command `0x2E` to `0xF4` to initiate temperature reading.
  * Waits 5 ms.
  * Reads 2 bytes from `0xF6`.
* **Returns**: 16-bit raw temperature value.


### `s32 bmp180_read_raw_pressure(void)`
-----------------------------------------
* **Purpose**: Reads the raw pressure data.
* **Operation**:
  * Sends command `0x34` to `0xF4` to initiate pressure reading (OSS = 0).
  * Waits 5 ms.
  * Reads 3 bytes from `0xF6`.
* **Returns**: 32-bit raw pressure value (adjusted to OSS = 0).


### `void bmp180_calculate_temp_pressure(s16 ut, s32 up, long *temperature, long *pressure)`
---------------------------------------------------------------------------------------------
* **Purpose**: Converts raw sensor data into real-world values using Bosch's compensation formulas.
* **Parameters**:
  * `ut`: Uncompensated temperature.
  * `up`: Uncompensated pressure.
  * `temperature`: Output temperature in 0.1 °C.
  * `pressure`: Output pressure in Pascals.


### `static int bmp180_probe(struct i2c_client *client, const struct i2c_device_id *id)`
-----------------------------------------------------------------------------------------
* **Called When**: The I2C device is detected during module load.
* **Role**:
  * Allocates and initializes the device.
  * Reads calibration data from EEPROM (`0xAA` to `0xBF`).
  * Registers the misc device for user-space access.
  

### `static void bmp180_remove(struct i2c_client *client)`
-----------------------------------------------------------
* **Called When**: The driver is removed.
* **Role**: Frees resources and unregisters the device.


### `module_init(bmp180_init)` and `module_exit(bmp180_exit)`
--------------------------------------------------------------
* Standard kernel macros for registering and unregistering the driver.


## Author
----------
GitHub: [XuanPhuong-Le](https://github.com/XuanPhuong-Le)


To keep the repository clean, files like `*.o`, `*.ko`, `.cmd`, binaries, and other build artifacts are ignored using `.gitignore`.
