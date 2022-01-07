#include "mpu.cpp"
#include "hc.cpp"
HC hc;
mpu MPU;
void setup() {
  Wire.begin();
  hc.begin();
  MPU.begin();
  Serial.begin(115200);
}

void loop() {

  delay(100);
  Data SensorData = MPU.getData();
  String Mes = "x=" + String(SensorData.roll) + " y=" + String(SensorData.yaw) + " z=" + String(SensorData.pitch);
  Serial.println(Mes);
  distance = hc.GetDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

}
