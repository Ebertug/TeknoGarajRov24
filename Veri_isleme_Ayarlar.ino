void Veri_isleme()
{
  veri_sayisi = 0;
  bool Veri_Gelme_Durumu = false;
  //***************************************************************
  //Gelen veri sıfırlama işlemi:
  for (int i = 0; i < 30; i++) Gelen_Komutlar[i] = ' ';

  while (Serial.available())
  {
    Gelen_Komutlar[veri_sayisi] = (char)Serial.read();
    veri_sayisi++;
    delayMicroseconds(200);
    Veri_Gelme_Durumu = true;

    if (veri_sayisi >= 100) break;
  }
  //******************************************************************
  while (Serial1.available())
  {
    Gelen_Komutlar[veri_sayisi] = (char)Serial1.read();
    veri_sayisi++;
    delayMicroseconds(200);
    Veri_Gelme_Durumu = true;

    if (veri_sayisi >= 100) break;
  }
  //*****************************************************************

  if (Veri_Gelme_Durumu == true)
  {
    for (int i = 0; i < veri_sayisi; i++) Serial.print(Gelen_Komutlar[i]);
    Serial.println();

    //İŞLEM YORUMLAMA:
    //***MOTOR AÇMA - KAPATMA DURUMLARI***
    if (Gelen_Komutlar[0] == 'A' && Gelen_Komutlar[1] == 'B') {
      Motor_Aktif = true;
      sifirlama = Veri_Kontrol.Euler_Z();
    }
    else if (Gelen_Komutlar[0] == 'A' && Gelen_Komutlar[1] == 'K') Motor_Aktif = false;

    //***HAREKET KOMUTLARI YORUMLARI***
    String gecici_veri = "";
    byte sayici_2 = 3;
    byte sayici_3 = 0;

    if (Gelen_Komutlar[0] == 'P')
    {
      PID_Ayarlama();
    }
    else if (Gelen_Komutlar[0] == 'D')
    {
      if (Gelen_Komutlar[1] == 'K') Derinlik_Kabl = true;
      else
      {
        Derinlik_Ayarlama();
      }
    }
    else if (Gelen_Komutlar[0] == 'T') //Görev atama komutu ayıklama işlemi
    {
      Ref_Pus2 += (String(Gelen_Komutlar).substring(String(Gelen_Komutlar).indexOf(' ') + 1)).toInt();

      if (Ref_Pus2 > 360) {
        Ref_Pus2 -= 360;
      }
      else if (Ref_Pus2 < 0) {
        Ref_Pus2 += 360;
      }
      Serial.print("Ref_Pus2 > "); Serial.println(Ref_Pus2);

    }

    else if (Gelen_Komutlar[0] == 'G') //Görev atama komutu ayıklama işlemi
    {
      Gorev_Komutu();
    }
    else if (Gelen_Komutlar[0] == 'B') //Görev Başlama Komutu:
    {
      //Görev başlatılır...
      Gorev_Sirasi = 2;
      yeni_gorev_atama = true;
      G_Baslama_Komut = true;
    }
    else
    {
      while (true)
      {
        if (Gelen_Komutlar[sayici_2] == ' ')
        {
          veri[sayici_3] = gecici_veri.toInt();
          sayici_3++;

          if (sayici_3 == 2) break;
          //Serial.println(gecici_veri);

          gecici_veri = "";
        }
        else gecici_veri += Gelen_Komutlar[sayici_2];
        sayici_2++;
      }

      Serial.print("Motor Güc");
      Serial.println(veri[0]); //MOTOR GÜÇ VERİSİ
      Serial.print("Zaman");
      Serial.println(veri[1]); //ZAMANLAMA VERİSİ


      if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == 'F')
      {
        //ileri ve gitme : 8/9/10/11 1500 üstü: geri / 1500 altı: ileri
        ileri_Komut = (veri[0] * 300) / 100; // %100 => 300ms
        Serial.print("İleri Komut");
        Serial.println(ileri_Komut);
        ileri_Komut *= -1;
        ileri_Komut2 = -1 * ileri_Komut;
        h_zaman = millis();
        Hareket_Durum = true;
      }
      else if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == 'B') {

        //ileri ve gitme : 8/9/10/11 1500 üstü: geri / 1500 altı: ileri
        geri_Komut = (veri[0] * 300) / 100; // %100 => 300ms
        geri_Komut2 = geri_Komut * -1;
        h_zaman = millis();
        Hareket_Durum = true;
      }
      else if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == 'L') {

        // 8 - 11 > + | 9 - 10 > -
        sol_Komut1 = (veri[0] * 300) / 100; // %100 => 300ms
        sol_Komut2 = (veri[0] * 300) / 100; // %100 => 300ms
        sol_Komut1 *= -1;

        h_zaman = millis();
        Hareket_Durum = true;
      }
      else if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == 'R') {

        // 8 - 11 > - | 9 - 10 > +
        sag_Komut1 = (veri[0] * 300) / 100; // %100 => 300ms
        sag_Komut2 = (veri[0] * 300) / 100; // %100 => 300ms
        sag_Komut2 *= -1;

        h_zaman = millis();
        Hareket_Durum = true;
      }

      else if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == '-') {

        // 8 - 11 > + | 9 - 10 > -
        arti_Komut1 = (veri[0] * 300) / 100; // %100 => 300ms
        arti_Komut2 = (veri[0] * 300) / 100; // %100 => 300ms
        arti_Komut1 *= -1;

        h_zaman = millis();
        Hareket_Durum = true;
      }
      else if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == '+') {

        // 8 - 11 > - | 9 - 10 > +
        eksi_Komut1 = (veri[0] * 300) / 100; // %100 => 300ms
        eksi_Komut2 = (veri[0] * 300) / 100; // %100 => 300ms
        eksi_Komut2 *= -1;

        h_zaman = millis();
        Hareket_Durum = true;
      }
      else if (Gelen_Komutlar[0] == 'H' && Gelen_Komutlar[1] == 'T') {

        Ext_Servo_Kontrol.Ext_Servo(15, 2000);
        h_zaman = millis();
        Hareket_Durum = true;
      }
    }

    Veri_Gelme_Durumu = false;
  }

  //***zamanlaması**********
  if (Hareket_Durum == true)
  {

    if (Gelen_Komutlar[0] == 'A' && Gelen_Komutlar[1] == 'S')
    {
      ileri_Komut = 0;
      ileri_Komut2 = 0;
      geri_Komut = 0;
      geri_Komut2 = 0;

      sag_Komut1 = 0;
      sag_Komut2 = 0;

      sol_Komut1 = 0;
      sol_Komut2 = 0;

      arti_Komut1 = 0;
      eksi_Komut1 = 0;

      arti_Komut2 = 0;
      eksi_Komut2 = 0;

      Ext_Servo_Kontrol.Ext_Servo(0, 1000);

      Gorev_Sirasi++;
      yeni_gorev_atama = true;
      Hareket_Durum = false;
    }
    //Serial.println(veri[0]);
    if (millis() - h_zaman >= veri[1])
    {
      ileri_Komut = 0;
      ileri_Komut2 = 0;
      geri_Komut = 0;
      geri_Komut2 = 0;

      sag_Komut1 = 0;
      sag_Komut2 = 0;

      sol_Komut1 = 0;
      sol_Komut2 = 0;

      arti_Komut1 = 0;
      eksi_Komut1 = 0;

      arti_Komut2 = 0;
      eksi_Komut2 = 0;

      Ext_Servo_Kontrol.Ext_Servo(0, 1000);

      Gorev_Sirasi++;
      yeni_gorev_atama = true;
      Hareket_Durum = false;
    }
  }
}
