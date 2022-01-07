#include "mpu.cpp"

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


}
