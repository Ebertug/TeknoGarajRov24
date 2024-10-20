void PID_Ayarlama()//*
{
  //***HAREKET KOMUTLARI YORUMLARI***
  String gecici_veri = "";
  byte sayici_2 = 5;
  byte sayici_3 = 0;

  float PID_Veri[10];
  Serial.println("PID Test");

  while (true)
  {
    if (Gelen_Komutlar[sayici_2] == ' ')
    {
      PID_Veri[sayici_3] = gecici_veri.toFloat() / 10000; //10^4 HASSASİYETİNDE
      sayici_3++;

      if (sayici_3 == 3) break;
      // Serial.println(gecici_veri);

      gecici_veri = "";
    }
    else gecici_veri += Gelen_Komutlar[sayici_2];
    sayici_2++;
  }


  if (Gelen_Komutlar[3] == '1')
  {
    Kp_1 = PID_Veri[0];
    Ki_1 = PID_Veri[1];
    Kd_1 = PID_Veri[2];

  }
  else if (Gelen_Komutlar[3] == '2')
  {
    Kp_2 = PID_Veri[0];
    Ki_2 = PID_Veri[1];
    Kd_2 = PID_Veri[2];
  }
  else if (Gelen_Komutlar[3] == '3')
  {
    Kp_3 = PID_Veri[0];
    Ki_3 = PID_Veri[1];
    Kd_3 = PID_Veri[2];
  }
  else if (Gelen_Komutlar[3] == '4')
  {
    Kp_4 = PID_Veri[0];
    Ki_4 = PID_Veri[1];
    Kd_4 = PID_Veri[2];
  }

  Serial.println(PID_Veri[0] , 5); //ZAMANLAMA VERİSİ
  Serial.println(PID_Veri[1] , 5); //MOTOR GÜÇ VERİSİ
  Serial.println(PID_Veri[2] , 5); //MOTOR GÜÇ VERİSİ
}
