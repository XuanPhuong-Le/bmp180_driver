# linux/drivers/Temperature/Pressure/bmp180-i2c.c
---------------------------------
The modules BMP180 is a barometric pressure sensor that also provides temperature readings. 

Author
-------
  - Lê Vũ Xuân Phương – 22146045
  - Hồ Phan Đức Minh  – 22146034
  - Phạm Văn Thịnh    – 22146056

📍 University: HCMC University of Technology and Education – HCMUTE

📅 Created: 12/05/2025


## Project Introduction
------------------------
This project aims to build a working driver for the BMP180 sensor on the Raspberry Pi, using I2C communication. The driver allows the sensor to read raw temperature and pressure values from the internal registers.
Afterward, the raw data will be processed and converted using precise calculation formulas (according to the BMP180 datasheet) to obtain :
   - Temperature (°C)
   - Atmospheric Pressure (Pa)

The final results will be displayed on the screen, allowing users to monitor and track environmental parameters directly.


## Technical Description of How the Driver Works
-------------------------------------------------
1. I2C Initialization and Driver Configuration:
- When the driver module is loaded into the Linux kernel, it initializes I2C communication with the BMP180 sensor through the SDA (data) and SCL (clock) pins of the Raspberry Pi.
- The BMP180 sensor is configured to read temperature and pressure values.

2. Reading Values from the Sensor:
- The driver reads raw values from the BMP180 sensor's registers. The sensor provides data in a 16-bit format, which includes:
  - Raw temperature value
  - Raw pressure value

3. Processing and Converting Data:
- These raw values are converted using the formulas provided in the BMP180 datasheet:
  - Temperature is converted from raw data to Celsius (°C).
  - Pressure is calculated and converted to Pascal (Pa).

4. Displaying Results:
- After conversion, the temperature and pressure values are returned to the user application or displayed on the console screen for easy monitoring.

5. Error Handling and Warnings:
- In case of any issues during I2C communication or if the sensor returns invalid data, the driver will report an error and prompt the user to retry.


## Project Structure
---------------------
 ```
bmp180-linux-driver/
├── src/
│   ├── bmp180_driver.c     # Main kernel driver that interfaces with the BMP180 sensor over I2C.
│   ├── bmp180_demo.c       # A user-space C program that opens the driver and prints temperature and pressure.
│   ├── bmp180.h            # Header file with declarations and I2C register definitions.
│   └── Makefile            # Used to build the kernel module and demo application.
└── README.md               # Project documentation
 ```

## Requirements
----------------
- Raspberry Pi with Raspberry Pi OS.
- BMP180 sensor connected via I2C.
- Kernel headers installed for your Raspberry Pi’s current kernel version.


## Hardware Connection
BMP180 → I2C Host (e.g., Raspberry Pi)
| **BMP180 Pin** | **Description** | **Connect to Host**               |
| -------------- | --------------- | --------------------------------- |
| VCC            | Power Supply    | 3.3V or 5V                        |
| GND            | Ground          | GND                               |
| SDA            | I2C Data Line   | SDA (e.g., GPIO2 on Raspberry Pi) |
| SCL            | I2C Clock Line  | SCL (e.g., GPIO3 on Raspberry Pi) |


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
Running the Test Application
------------------------------
```
gcc -o bmp180_demo bmp180_demo.c
sudo ./bmp180_demo
```
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


To keep the repository clean, files like `*.o`, `*.ko`, `.cmd`, binaries, and other build artifacts are ignored using `.gitignore`.
