void acelerometros(){
   //mpu(Acc, Gy, Angle,MPU1);
    mpu(Acc, Gy, Angle1,MPU1);
    Angle1[0] = Angle[0];
    Angle1[1] = Angle[1];
    
    mpu(Acc, Gy, Angle2,MPU2);
    Angle2[0] = Angle[0];
    Angle2[1] = Angle[1];
   //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
}
    /*
    int x =Angle[0];
    int y = Angle[1];
 Serial.print(x);
 Serial.println(y);
     //Mostrar los valores por consola
   Serial.print("Angle X: "); Serial.print(Angle[0]); Serial.print("\n");
   Serial.print("Angle Y: "); Serial.print(Angle[1]); Serial.println("\n------------\n");
  // Serial.print("Tmp = "); Serial.println(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  /// Serial.print(currentMillis); 
 */
void mpu(float Acc[2],float Gy[2],float Angle[2],int MPU){
Wire.begin();
Wire.beginTransmission(MPU);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);

  
 //Leer los valores del Acelerometro de la IMU
   Wire.beginTransmission(MPU);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true); //A partir del 0x3B, se piden 6 registros
   AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   AcY=Wire.read()<<8|Wire.read();
   AcZ=Wire.read()<<8|Wire.read();
 
    //A partir de los valores del acelerometro, se calculan los angulos Y, X
    //respectivamente, con la formula de la tangente.
   Acc[1] = atan(-1*(AcX/A_R)/sqrt(pow((AcY/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
   Acc[0] = atan((AcY/A_R)/sqrt(pow((AcX/A_R),2) + pow((AcZ/A_R),2)))*RAD_TO_DEG;
 
   //Leer los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   GyX=Wire.read()<<8|Wire.read();
   GyY=Wire.read()<<8|Wire.read();

   ///leer datos de termometro
      Wire.beginTransmission(MPU);
      Wire.write(0x41);
      Wire.endTransmission(false);
      Wire.requestFrom(MPU,7,true);
      Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 
   //Calculo del angulo del Giroscopio
   Gy[0] = GyX/G_R;
   Gy[1] = GyY/G_R;

   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*0.01) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*0.01) + 0.02*Acc[1];
}
