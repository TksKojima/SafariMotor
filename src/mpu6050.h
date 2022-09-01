
// 使い方 setupとloopを呼び出して、以下の変数を呼ぶ
// double AccX_g, AccY_g, AccZ_g;
// double Temp_deg;
// double GyroX_dps, GyroY_dps, GyroZ_dps;


#include <Arduino.h>
#include <Wire.h>

// https://www.ei.tohoku.ac.jp/xkozima/lab/espTutorial2.html
//  3V3:vcc, GND, IO4:SDA, IO5:SCL
//  MPU6050 on ESP8266


#define MPU6050_ADDR 0x68
#define MPU6050_AX  0x3B
#define MPU6050_AY  0x3D
#define MPU6050_AZ  0x3F
#define MPU6050_TP  0x41    //  data not used
#define MPU6050_GX  0x43
#define MPU6050_GY  0x45
#define MPU6050_GZ  0x47

// 属性を保持するための構造体の定義
typedef struct mpu6050
{
    short int AccX, AccY, AccZ;
    short int Temp;
    short int GyroX, GyroY, GyroZ;

    double AccX_g, AccY_g, AccZ_g;
    double Temp_deg;
    double GyroX_dps, GyroY_dps, GyroZ_dps;

    void setup();
    void loop();

}mpu6050;