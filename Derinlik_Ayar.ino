
/*
   DERİNLİK KOMUT İŞLEMLERİ:
   DK : Derinlik Kalibrasyon başlatır
   D 100 : cm cinsinden hedef derinlik
*/


void Dept_Setup()
{
  while (!sensor.init()) {
    Serial.println("Sensör Hatası!");
    Serial.println("Bağlantıları Kontrol Ediniz...");
    delay(2000);
  }

  //Sensörün 30Bar olduğunu net şekilde açıklanır, açık olması zorunlu değildir.
  sensor.setModel(MS5837::MS5837_30BA);

  sensor.setFluidDensity(997);
  /*
     Sıvı Yoğunluk Ayarı:
     Tatlı su (freshwater) : 997 kg/m^3
     Deniz suyu (seawater) : 1029 kg/m^3
  */
}

void Derinlik_Ayarlar()
{
  // Sensörün sıcaklık ve basınç verileri güncellenir:
  sensor.read();
  Arac_Dept = sensor.depth();

  //KALİBRASYON İŞLEMİ:
  if (millis() <= 5000 || Derinlik_Kabl == true)
  {
    Kalb_Deger_Dept = Arac_Dept;
    Derinlik_Kabl = false;
  }
  else
  {
    Kab_Dept = Arac_Dept - Kalb_Deger_Dept;
    Kab_Dept *= 100; //CM ÇEVİRME
  }

  /*
    Serial.print("Pressure:");
    Serial.print(sensor.pressure());
    Serial.print(" mbar ");

    Serial.print("Temp:");
    Serial.print(sensor.temperature());
    Serial.print(" deg C  ");
  */
  /*
    Serial.print("Depth:");
    Serial.print(Kab_Dept);
    Serial.print(" cm   ");
    Serial.print(Ref_Der);
    Serial.print(" cm  ");
    Serial.println();

    Serial1.print("Depth:");
    Serial1.print(Kab_Dept);
    Serial1.print(" cm  ");
    Serial1.print(Ref_Der);
    Serial1.print(" cm  ");
    Serial1.println();
  */
}

void Derinlik_Ayarlama()
{
  String gecici_veri = "";
  byte sayici_2 = 2;

  Serial.println("PID Derinlik Test");

  while (true)
  {
    if (Gelen_Komutlar[sayici_2] == ' ')
    {
      Ref_Der = gecici_veri.toInt();
      break;
      // Serial.println(gecici_veri);
    }
    else gecici_veri += Gelen_Komutlar[sayici_2];
    sayici_2++;
  }

  Serial.println(Ref_Der); //DERİNLİK VERİSİ
}
