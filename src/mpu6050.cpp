
#include <mpu6050.h>

void mpu6050::setup() {
  //  serial for debugging
  //Serial.begin(115200);
  //Serial.println("*** started");
  //  i2c as a master
  Wire.begin(21,22); //デフォルト SDA:D21、SCL:D22

  //  wake it up
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();
}

void mpu6050::loop() {
  //  send start address
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_AX);
  Wire.endTransmission(); 

  //  request 14bytes (int16 x 7)
  Wire.requestFrom(MPU6050_ADDR, 14);
  //  get 14bytes

  AccX = Wire.read() << 8;  AccX |= Wire.read();
  AccY = Wire.read() << 8;  AccY |= Wire.read();
  AccZ = Wire.read() << 8;  AccZ |= Wire.read();
  Temp = Wire.read() << 8;  Temp |= Wire.read();  //  (Temp-12421)/340.0 [degC]
  GyroX = Wire.read() << 8; GyroX |= Wire.read();
  GyroY = Wire.read() << 8; GyroY |= Wire.read();
  GyroZ = Wire.read() << 8; GyroZ |= Wire.read();

  AccX_g = (double)AccX/16384;
  AccY_g = (double)AccY/16384;
  AccZ_g = (double)AccZ/16384;
  GyroX_dps = (double)GyroX/131;
  GyroY_dps = (double)GyroY/131;
  GyroZ_dps = (double)GyroZ/131;


}
