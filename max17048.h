#include "esphome.h"
#include "Wire.h"

#define MAX17048_I2C_ADDRESS 0x36

class MAX17048 : public PollingComponent {
 public:
  Sensor *voltage_sensor = new Sensor();
  Sensor *percentage_sensor = new Sensor();

  MAX17048() : PollingComponent(60000) {}

  void setup() override {
    Wire.begin(3, 2);  // SDA, SCL
  }

  void update() override {
    uint16_t voltage = read_register(0x09);  // Voltage register
    uint16_t percentage = read_register(0x06);  // Percentage register

    float voltage_value = voltage * 0.00125;  // Convert to volts
    float percentage_value = percentage / 256.0;  // Convert to percentage

    voltage_sensor->publish_state(voltage_value);
    percentage_sensor->publish_state(percentage_value);
  }

  uint16_t read_register(uint8_t reg) {
    Wire.beginTransmission(MAX17048_I2C_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MAX17048_I2C_ADDRESS, (uint8_t)2);

    uint16_t value = Wire.read() << 8 | Wire.read();
    return value;
  }
};
