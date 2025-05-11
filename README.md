# BMP180 Driver for Raspberry Pi
This repository contains a Linux kernel module for interfacing the BMP180 temperature and pressure sensor with a Raspberry Pi via the I2C bus.


## Project Structure
* **bmp180\_driver.c** – Core kernel module code that communicates with the BMP180 sensor via I2C.
* **bmp180.h** – Header file containing register definitions and calibration struct.
* **bmp180\_demo.c** – A simple user-space program to demonstrate how to read data from the BMP180 driver.
* **Makefile** – Script for compiling the kernel module and demo.


##  Installation & Usage

### 1. Clone and Build

```
git clone https://github.com/XuanPhuong-Le/bmp180_driver.git
cd bmp180_driver
make
```

### 2. Load the Module

```
sudo insmod bmp180_driver.ko
dmesg | tail
```

### 3. Run the Demo

```
./bmp180_demo
```

### 4. Unload the Module

```
sudo rmmod bmp180_driver
```

---

## How It Works: Driver Breakdown (`bmp180_driver.c`)

### **`bmp180_probe()`**

* Called when the I2C device is detected.
* Initializes the sensor, reads calibration data, and optionally tests reading temperature/pressure.
* Registers the misc device for user-space access via `/dev/bmp180`.

### **`bmp180_read_raw_temp()`**

* Sends command `0x2E` to register `0xF4` to start temperature conversion.
* Waits 5 ms and reads two bytes from `0xF6` and `0xF7` for raw temperature data.

### **`bmp180_read_raw_pressure()`**

* Sends command `0x34` to `0xF4` to start pressure conversion (OSS = 0).
* Waits 5 ms and reads three bytes from `0xF6`, `0xF7`, and `0xF8` for raw pressure data.

### **`bmp180_calculate_temp_pressure()`**

* Applies Bosch’s compensation algorithm to convert raw data into real-world values.
* Returns temperature in 0.1 °C and pressure in Pa.

### **`bmp180_read()`**

* Called when user-space reads from `/dev/bmp180`.
* Retrieves both temperature and pressure and copies them to user space.

### **`bmp180_remove()`**

* Called on driver removal; unregisters the device.

---

## Demo Program (`bmp180_demo.c`)

* Opens `/dev/bmp180`
* Reads 2 integers: temperature and pressure
* Displays the values on the terminal

Example output:

```
Temperature: 25.3 °C
Pressure: 100521 Pa
```

---

## Calibration Details (`bmp180.h`)

Defines sensor register addresses and a struct `bmp180_calib_data` that holds the calibration coefficients from the sensor’s EEPROM.

---

## .gitignore

To keep the repository clean, files like `*.o`, `*.ko`, `.cmd`, binaries, and other build artifacts are ignored using `.gitignore`.


