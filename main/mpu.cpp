

#include <Wire.h>
#include "Arduino.h"
class Data {
public:
  float AccX, AccY, AccZ;
  float GyroX, GyroY, GyroZ;
  float roll, pitch, yaw;
};
class mpu {
public:
  const int MPU = 0x68;
  float AccX, AccY, AccZ;
  float GyroX, GyroY, GyroZ;
  float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
  float roll, pitch, yaw;
  float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
  float elapsedTime, currentTime, previousTime;
  int c = 0;

  void begin() {
    Wire.beginTransmission(MPU);       
    Wire.write(0x6B);                 
    Wire.write(0x00);                  
    Wire.endTransmission(true);        


    Wire.beginTransmission(MPU);
    Wire.write(0x1C);                  
    Wire.write(0x10);                  
    Wire.endTransmission(true);

    Wire.beginTransmission(MPU);
    Wire.write(0x1B);                   
    Wire.write(0x10);                   
    Wire.endTransmission(true);
    delay(20);
    calculate_IMU_error();
  }
  void calculate_IMU_error() {
    //200 kere oku ortalamasını al
    while (c < 200) {
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU, 6, true);
      AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
      AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
      AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

      AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
      AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
      c++;
    }

    AccErrorX = AccErrorX / 200;
    AccErrorY = AccErrorY / 200;
    c = 0;

    while (c < 200) {
      Wire.beginTransmission(MPU);
      Wire.write(0x43);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU, 6, true);
      GyroX = Wire.read() << 8 | Wire.read();
      GyroY = Wire.read() << 8 | Wire.read();
      GyroZ = Wire.read() << 8 | Wire.read();

      GyroErrorX = GyroErrorX + (GyroX / 131.0);
      GyroErrorY = GyroErrorY + (GyroY / 131.0);
      GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
      c++;
    }

    GyroErrorX = GyroErrorX / 200;
    GyroErrorY = GyroErrorY / 200;
    GyroErrorZ = GyroErrorZ / 200;
/*
    Serial.print("AccErrorX: ");
    Serial.println(AccErrorX);
    Serial.print("AccErrorY: ");
    Serial.println(AccErrorY);
    Serial.print("GyroErrorX: ");
    Serial.println(GyroErrorX);
    Serial.print("GyroErrorY: ");
    Serial.println(GyroErrorY);
    Serial.print("GyroErrorZ: ");
    Serial.println(GyroErrorZ);*/
  }
  void ReadAccel() {
    // === accel data=== //
    Wire.beginTransmission(MPU);
    Wire.write(0x3B); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true); 
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis 
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis 
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis 

    accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; 
    accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; 

  }

  void ReadGyro() {
    // === gyro data=== //
    previousTime = currentTime;        
    currentTime = millis();            
    elapsedTime = (currentTime - previousTime) / 1000; 
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; //250deg/s ayarlı datasheete göre 131e bölüncek
    GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
    GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

    GyroX = GyroX + 0.96; 
    GyroY = GyroY - 499; 
    GyroZ = GyroZ + 0.26;


    if (GyroX < 7) {
      GyroX = 0;
    }
    else {
      GyroX -= 7;
    }

    if (GyroY < 5) {
      GyroY = 0;
    }
    else {
      GyroY -= 5;
    }

    if (GyroZ < 3) {
      GyroZ = 0;
    }
    else {
      GyroZ -= 3;
    }


    // açısal iveyi veriyor süreyle çarpınca açıyı buluyoruz
    gyroAngleX = gyroAngleX + GyroX * elapsedTime; 
    gyroAngleY = gyroAngleY + GyroY * elapsedTime;
    yaw = yaw + GyroZ * elapsedTime;

    // ivmeyi ekleyip filtreliyoruz
    roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
    pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;

    if (abs(roll) > 360) {
      roll = 0;
    }if (abs(pitch) > 360) {
      pitch = 0;
    }if (abs(yaw) > 360) {
      yaw = 0;
    }

  }

  Data getData() {
    ReadAccel();
    ReadGyro();
    Data newData;
    newData.AccX = AccX;
    newData.AccY = AccY;
    newData.AccZ = AccZ;
    newData.GyroX = GyroX;
    newData.GyroY = GyroY;
    newData.GyroZ = GyroZ;
    newData.roll = roll;
    newData.pitch = pitch;
    newData.yaw = yaw;

    return newData;
  }
};

/*
mpu MPU;
void setup() {
  Wire.begin();

  MPU.begin();
  Serial.begin(115200);
}

void loop() {

  delay(100);
  Data SensorData = MPU.getData();
  String Mes = "x=" + String(SensorData.roll) + " y=" + String(SensorData.yaw) + " z=" + String(SensorData.pitch);
  Serial.println(Mes);


}*/
