String Gorevler[100] = "";
byte gorev_sayisi = 0;
String gecici_gorev_veri = "";
byte G_Sayisi = 0;

void Gorev_Komutu()
{
  //Görev Komutu Adımları:
  /*
     1.Aracı Kab. et
     2.Komutları Yerine Getir
     3.Görev Bittiğinde Yüksel

     ÖRNEK GÖREV KOMUTU:
     G D60 G50 F3000 F3000 B3000 R2000 L3000 L3000 T B5000 GB GB GB
     G D60 G50 F5000 B5000 Q Q
     G D100 G50 F5000 B5000 L5000 R5000 +5000 -5000 Q Q Q

     GOREV KURGUSU:
     DERİNLİK: 60
     HIZ: 30
     3SN İLERİ

     G D70 G30 L1000 R1000 F3000 +500 T B3000 Q Q Q
     
  */
  gecici_gorev_veri = "";
  int sayici_2 = 2;
  int sayici_3 = 0;

  Serial.println("Gorev Atama Başladı:");

  for (int i = 0; i < veri_sayisi; i++) Serial.print(Gelen_Komutlar[i]);
  Serial.println();

  for (int i = 0; i < veri_sayisi; i++)
  {
    if (Gelen_Komutlar[sayici_2] == ' ')
    {
      Gorevler[sayici_3] = (String)gecici_gorev_veri; //10^4 HASSASİYETİNDE
      sayici_3++;
      gecici_gorev_veri = "";
    }
    else gecici_gorev_veri += (String)Gelen_Komutlar[sayici_2];

    sayici_2++;
    delayMicroseconds(100);
  }

  G_Sayisi = sayici_3; //Görev sayısı
  Serial.println("//************************");
  for (int i = 0; i < G_Sayisi; i++) Serial.println(Gorevler[i]);
  Serial.println("//**********************");

  Serial.println("Gorev Atama Tamamlandı...");
}
bool g_bitis = false;

void Gorev_Baslama()
{
  if (G_Baslama_Komut == true)
  {
    String g_veri = "";
    String g_veri_1 = "";
    //Referans derinlik işlemi başlatılır:
    g_veri = Gorevler[0];
    for (int i = 1; i < Gorevler[0].length(); i++) g_veri_1 += g_veri[i];
    Ref_Der = (int)g_veri_1.toInt();

    //Referans güç işlemi başlatılır:
    g_veri_1 = "";
    g_veri = Gorevler[1];
    for (int i = 1; i < Gorevler[1].length(); i++) g_veri_1 += g_veri[i];
    Ref_Guc = (int)g_veri_1.toInt();

    String G_ = Gorevler[Gorev_Sirasi];
    char Gorev_Tur = G_[0];

    if (yeni_gorev_atama == true)
    {
      switch (Gorev_Tur)
      {
        case 'F':
          Hareket_Durum = true;
          ileri_Komut = (Ref_Guc * 400) / 100; // %100 => 400ms
          ileri_Komut *= -1;
          h_zaman = millis();
          g_veri_1 = "";
          g_veri = Gorevler[Gorev_Sirasi];
          for (int i = 1; i < Gorevler[Gorev_Sirasi].length(); i++) g_veri_1 += g_veri[i];
          veri[0] = (int)g_veri_1.toInt();//zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case 'B':
          Hareket_Durum = true;
          geri_Komut = (Ref_Guc * 400) / 100; // %100 => 400ms
          h_zaman = millis();
          g_veri_1 = "";
          g_veri = Gorevler[Gorev_Sirasi];
          for (int i = 1; i < Gorevler[Gorev_Sirasi].length(); i++) g_veri_1 += g_veri[i];
          veri[0] = (int)g_veri_1.toInt();//zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case 'R':
          Hareket_Durum = true;
          sol_Komut1 = (Ref_Guc * 400) / 100; // %100 => 400ms
          sol_Komut2 = (Ref_Guc * 400) / 100; // %100 => 400ms
          sol_Komut1 *= -1;
          h_zaman = millis();
          g_veri_1 = "";
          g_veri = Gorevler[Gorev_Sirasi];
          for (int i = 1; i < Gorevler[Gorev_Sirasi].length(); i++) g_veri_1 += g_veri[i];
          veri[0] = (int)g_veri_1.toInt();//zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case 'L':
          Hareket_Durum = true;
          sag_Komut1 = (Ref_Guc * 400) / 100; // %100 => 400ms
          sag_Komut2 = (Ref_Guc * 400) / 100; // %100 => 400ms
          sag_Komut2 *= -1;
          h_zaman = millis();
          g_veri_1 = "";
          g_veri = Gorevler[Gorev_Sirasi];
          for (int i = 1; i < Gorevler[Gorev_Sirasi].length(); i++) g_veri_1 += g_veri[i];
          veri[0] = (int)g_veri_1.toInt();//zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case '+':
          Hareket_Durum = true;
          arti_Komut1 = (Ref_Guc * 400) / 100; // %100 => 400ms
          arti_Komut2 = (Ref_Guc * 400) / 100; // %100 => 400ms
          arti_Komut1 *= -1;
          h_zaman = millis();
          g_veri_1 = "";
          g_veri = Gorevler[Gorev_Sirasi];
          for (int i = 1; i < Gorevler[Gorev_Sirasi].length(); i++) g_veri_1 += g_veri[i];
          veri[0] = (int)g_veri_1.toInt();//zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case '-':
          Hareket_Durum = true;
          eksi_Komut1 = (Ref_Guc * 400) / 100; // %100 => 400ms
          eksi_Komut2 = (Ref_Guc * 400) / 100; // %100 => 400ms
          eksi_Komut2 *= -1;
          h_zaman = millis();
          g_veri_1 = "";
          g_veri = Gorevler[Gorev_Sirasi];
          for (int i = 1; i < Gorevler[Gorev_Sirasi].length(); i++) g_veri_1 += g_veri[i];
          veri[0] = (int)g_veri_1.toInt();//zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case 'T':
          Hareket_Durum = true;
          Ext_Servo_Kontrol.Ext_Servo(0, 2000);
          h_zaman = millis();
          veri[0] = 2000;//2sn bekleme verildi //zamanlama verisi
          Serial.print("Veri[0] = ");
          Serial.println(veri[0]);
          break;

        case 'Q': //Görev Bitme Komutu:
          g_bitis = true;
          Ref_Der = 0;
          G_Baslama_Komut = false;
          break;

        default:
          //h_zaman = millis();
          Serial.println("atlama yapıldı");
          break;
      }

      yeni_gorev_atama = false;

      Serial.println(Ref_Der);
      Serial.println(Gorevler[0].length());
      Serial.println(g_veri_1);
      Serial.println(Ref_Guc);
      Serial.println(Gorev_Tur);
    }

    //Serial.println(Ref_Der);
    //Serial.println(Gorevler[0].length());
    //Serial.println(g_veri_1);
    //Serial.println(Ref_Guc);
    //Serial.println(Gorev_Tur);
    //G_Baslama_Komut = false;
  }
}
