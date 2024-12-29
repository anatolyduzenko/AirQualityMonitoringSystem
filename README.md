# Air Quality and Meteorological Monitoring System

## Overview

This project is an Arduino-based system that combines a **2.0-inch ST7789 IPS display**, **MQ135 air quality sensor**, **MQ2 gas sensor**, and a **DHT11 temperature and humidity sensor** to monitor environmental parameters. The device displays real-time information about air quality, temperature, and humidity.

## Features

- **Air Quality Monitoring**:
  - **MQ135**: Measures CO2 and other pollutants.
  - **MQ2**: Detects LPG, CO, and alcohol.
- **Meteorological Data**:
  - Temperature and humidity readings via the DHT11 sensor.
- **Graphical Output**:
  - Data visualization on an ST7789 240x320 IPS display using the `Adafruit_GFX` library.
- **Calibration**:
  - Automatic calibration of the MQ2 sensor for accurate readings.

---

## Hardware Requirements

### Components
- **Arduino UNO** (or compatible board)
- **ST7789 IPS Display** (240x320 resolution)
- **MQ135 Sensor**
- **MQ2 Gas Sensor**
- **DHT11 Temperature and Humidity Sensor**
- Jumper wires and power source

### Wiring
| Pin Name | Connection       |
|----------|------------------|
| GND      | GND              |
| VCC      | VCC (3.3V only!) |
| SCL      | D13 (SCK)        |
| SDA      | D11 (MOSI)       |
| RES      | Optional         |
| DC       | D10 (or digital) |
| CS       | D9 (or digital)  |

**Sensor Pins:**
- **MQ135**: Analog output to A0
- **MQ2**: Analog output to A1
- **DHT11**: Digital output to D7

---

## Software Requirements

### Libraries
Ensure the following libraries are installed in your Arduino IDE:
1. [MQ135](https://github.com/GeorgK/MQ135)
2. [DHT11](https://github.com/adafruit/DHT-sensor-library)
3. [Adafruit_GFX](https://github.com/adafruit/Adafruit-GFX-Library)
4. [ST7789_AVR](https://github.com/DatanoiseTV/ST7789_AVR)
5. [MQUnifiedSensor](https://github.com/miguel5612/MQSensorsLib)

### Configuration
- **Voltage Resolution**: 5V
- **ADC Bit Resolution**: 10 bits
- **Clean Air Ratio (MQ2)**: 9.83

---

## Setup Instructions

1. **Connect the Hardware**:
   - Follow the wiring table for correct connections.
2. **Install Libraries**:
   - Use the Arduino Library Manager to install the required libraries.
3. **Upload the Code**:
   - Open the provided `.ino` file in the Arduino IDE and upload it to your board.

---

## Usage

1. **Calibration**:
   - Upon powering the system, the MQ2 sensor will automatically calibrate in clean air. This process takes approximately 10 cycles.
2. **Display**:
   - The display will show temperature, humidity, air quality (PPM), and gas levels.
   - Gas levels will be highlighted in red if they exceed a threshold.

---

## Functionality

### Sensors
- **DHT11**:
  - Measures temperature and humidity.
- **MQ135**:
  - Reads air resistance, RZero, and calculates CO2 levels.
- **MQ2**:
  - Detects LPG, CO, and alcohol concentrations using regression models.

### Display
- **Static Text**: Displays parameter labels.
- **Dynamic Data**: Updates readings in real time with color-coded alerts.

---

## Customization

- **Thresholds**: Adjust gas detection thresholds in the `MQ2.setA()` and `MQ2.setB()` functions.
- **Display Layout**: Modify `displayStatic()` and `displayMeteo()` to change the visualization.
- **Additional Sensors**: Extend functionality by adding more sensors or modifying the analog input pins.

---

## Troubleshooting

- **Display Issues**:
  - Ensure correct pin assignments for the ST7789 display.
  - Verify that the power supply is 3.3V for the display.
- **Sensor Calibration**:
  - Ensure MQ sensors are exposed to clean air during calibration.
- **DHT11 Read Failures**:
  - Check sensor connection and use a pull-up resistor if required.

---

## License

This project is open-source and distributed under the [MIT License](LICENSE).

---

Happy monitoring! 🌍
