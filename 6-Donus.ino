#include "ALACAKART.h"
ALACA_KART Veri_Kontrol;
#include "Servo_Ext_ALC.h"
Servo_Ext Ext_Servo_Kontrol;
#include "MS5837.h"
MS5837 sensor;

bool acilis_sayici = false;
bool kalibre_durum = false;

int tam_deger = 360,  sifirlama = 0, Z_Acisi;

String gecici_veri;
int veri_sayisi = 0;
int pus_sayici = 0;
int pus_sayici2 = 2;
bool G_Baslama_Komut = false;
int Gorev_Sirasi = 0; //Göreve başlama sırasını temsil eder
bool yeni_gorev_atama = false;

int Motor_Hiz = 1500; // Stop PWM
int Mak_deger = 1750; // Maksimum PWM
int Min_deger = 1250; // Minimum PWM

int Motor_0, Motor_1, Motor_2, Motor_3; // İtiş Motorları
int Motor_4, Motor_5, Motor_6, Motor_7; // Batış Motorları

float PID_Yun_Ref, PID_Yat_Ref, PID_Sap_Ref, PID_Der_Ref, PID_Pus_Ref;
float N_PID_Pus_Ref = 0;
float Ref_Yun, Ref_Yat, Ref_Sap, Ref_Der, Ref_Guc, Ref_Pus, Ref_Pus2;
float X_Acisi, Y_Acisi, Gyro_X, Gyro_Y, Gyro_Z, Gyro_Z2;
int itki, yunuslama, yatis, sapma, derinlik, pusula;
int kalan_aci = 0, PID_kalan_aci;
int Set_Pus = 0;
int veri[5];

//YUNUSLAMA / PİTCH PID KATSAYILARI: 100Hz
float Kp_1 = 3.0;
float Ki_1 = 0.000;
float Kd_1 = 0.5;

//YATIŞ PID / ROLL KATSAYILARI: 100Hz
float Kp_2 = 0.8;
float Ki_2 = 0.001;
float Kd_2 = 0.004;

//SAPMA PID / YAW KATSAYILARI: 100Hz
float Kp_3 = 0.0;
float Ki_3 = 0.000;
float Kd_3 = 0.000;

//DERİNLİK PID / YAW KATSAYILARI: 100Hz
float Kp_4 = 6D.0;
float Ki_4 = 0.000;
float Kd_4 = 0.000;

//PUSULA PID / YAW KATSAYILARI: 100Hz
float Kp_5 = 3;
float Ki_5 = 0.000;
float Kd_5 = 0.000;

float Kab_Dept = 0, Kalb_Deger_Dept;
float Arac_Alt = 0, Arac_Dept = 0;
bool Derinlik_Kabl = false;

unsigned long f_, zamanlama_1, h_zaman;
char Gelen_Komutlar[100];
bool Motor_Aktif = false;
bool Hareket_Durum = false;
String Ref_Pus_Deger;
int ileri_Komut = 0;
int ileri_Komut2 = 0;
int geri_Komut = 0;
int geri_Komut2 = 0;
int sol_Komut1 = 0;
int sol_Komut2 = 0;
int sag_Komut1 = 0;
int sag_Komut2 = 0;
int arti_Komut1 = 0;
int eksi_Komut1 = 0;
int arti_Komut2 = 0;
int eksi_Komut2 = 0;

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);

  Veri_Kontrol.Sensor_begin();
  Veri_Kontrol.LED_begin();

  Ext_Servo_Kontrol.begin(); //Harici Servo Kullanımını Aktifleştirme
  Ext_Servo_Kontrol.Ext_Servo_Fr(60); //Harici Servo için Çalışma Frekans Belirleme

  delay(3000);

  Ext_Servo_Kontrol.Ext_Servo(0, 0);
  Ext_Servo_Kontrol.Ext_Servo(1, 0);
  Ext_Servo_Kontrol.Ext_Servo(2, 0);
  Ext_Servo_Kontrol.Ext_Servo(3, 0);
  Ext_Servo_Kontrol.Ext_Servo(4, 0);
  Ext_Servo_Kontrol.Ext_Servo(5, 0);
  Ext_Servo_Kontrol.Ext_Servo(6, 0);
  Ext_Servo_Kontrol.Ext_Servo(7, 0);

  delay(3000);

  Ext_Servo_Kontrol.Ext_Servo(0, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(1, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(2, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(3, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(4, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(5, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(6, Motor_Hiz);
  Ext_Servo_Kontrol.Ext_Servo(7, Motor_Hiz);

  //PID değişkenleri:
  Veri_Kontrol.setup_PID_1();
  Veri_Kontrol.setup_PID_2();
  Veri_Kontrol.setup_PID_3();
  Veri_Kontrol.setup_PID_4();
  Veri_Kontrol.setup_PID_5();

  Dept_Setup();

  digitalWrite(LED_R, 0);
  digitalWrite(LED_G, 0);
  digitalWrite(LED_B, 1);
  //delay(3000);

  Serial.println("Sistem Başlatılıyor");
  Serial1.println("Sistem Başlatılıyor");
}

void loop() {
  if (!kalibre_durum) {
    Derinlik_Kabl = true;
    kalibre_durum = true;
  }
  /*
     21.07.2024
     Adımlamalar:
     1.Motor kontrol işlemlerine bakınız.
     ileri ve gitme : 8/9/10/11 1480 üstü: geri / 1480 altı: ileri

    yukarı / aşağı : 12/13/14/15 1480 üstü: yu
  */

  //**************************************************************
  Veri_isleme();
  Gorev_Baslama();
  Derinlik_Ayarlar();

  //Euler Verilerinin Alınması:***
  X_Acisi = Veri_Kontrol.Euler_X();  //Yunuslama Verisi - Pitch
  Y_Acisi = Veri_Kontrol.Euler_Y(); //Yatış Verisi - Roll
  Z_Acisi = Veri_Kontrol.Euler_Z(); //Pusula Verisi - Heading
  Z_Acisi -= sifirlama;
  Z_Acisi = (Z_Acisi % tam_deger + tam_deger) % tam_deger;

  //Gyro Verilerinin Alınması:***
  Gyro_X = Veri_Kontrol.GYRO_X(); //Yatış Verisi - Roll
  Gyro_Y = Veri_Kontrol.GYRO_Y() * -1; //Yunuslama Verisi - Pitch
  Gyro_Z = Veri_Kontrol.GYRO_Z() * -1; //Sapma Verisi - Yaw

  Ref_Yat = 0; //açı değerleri
  Ref_Yun = 0;
  Ref_Sap = 0; //map(Kanal_4, 1000, 2000, -165, 165);
  Ref_Pus = 0;

  PID_Yat_Ref = (Ref_Yat - Y_Acisi) * 8;
  PID_Yun_Ref = (Ref_Yun - X_Acisi) * 8;
  PID_Pus_Ref = (Ref_Pus - Z_Acisi) * -1;
  PID_Sap_Ref = Ref_Sap ;

  PID_Der_Ref = Ref_Der; //hedef derinlik verisi

  if (Ref_Pus2 > 360) {
    Ref_Pus2 -= 360;
  }
  else if (Ref_Pus2 < 0) {
    Ref_Pus2 += 360;
  }

  kalan_aci = Veri_Kontrol.Koord_A_Data(PID_Pus_Ref, Ref_Pus2);

  if (millis() - f_ >= 10) //100Hz
  {
    yunuslama = Veri_Kontrol.PID_1(PID_Yun_Ref - Gyro_Y, Kp_1, Ki_1, Kd_1, 50);
    yatis = Veri_Kontrol.PID_2(PID_Yat_Ref - Gyro_X, Kp_2, Ki_2, Kd_2, 50);
    sapma = Veri_Kontrol.PID_3(Ref_Sap - Gyro_Z, Kp_3, Ki_3, Kd_3, 50);
    derinlik = Veri_Kontrol.PID_4(PID_Der_Ref - Kab_Dept, Kp_4, Ki_4, Kd_4, 50);
    pusula = Veri_Kontrol.PID_5(kalan_aci, Kp_5, Ki_5, Kd_5, 50);
    f_ = millis();
  }

  //yatis = 0;
  //yunuslama = 0;

  Motor_0 = 1500  + ileri_Komut + geri_Komut + sol_Komut2 + sag_Komut2 + arti_Komut2 + eksi_Komut2 - pusula;
  if (Motor_0 >= Mak_deger) Motor_0 = Mak_deger;
  else if (Motor_0 <= Min_deger) Motor_0 = Min_deger;

  Motor_1 = 1500 + ileri_Komut2 + geri_Komut + sol_Komut2 + sag_Komut2 + arti_Komut2 + eksi_Komut2 - pusula;
  if (Motor_1 >= Mak_deger) Motor_1 = Mak_deger;
  else if (Motor_1 <= Min_deger) Motor_1 = Min_deger;

  Motor_2 = 1500 + ileri_Komut + geri_Komut + sol_Komut2 + sag_Komut2 + arti_Komut1 + eksi_Komut1 - pusula;
  if (Motor_2 >= Mak_deger) Motor_2 = Mak_deger;
  else if (Motor_2 <= Min_deger) Motor_2 = Min_deger;

  Motor_3 = 1500 + ileri_Komut2 + geri_Komut2 + sol_Komut2 + sag_Komut2 + arti_Komut1 + eksi_Komut1 - pusula;
  if (Motor_3 >= Mak_deger) Motor_3 = Mak_deger;
  else if (Motor_3 <= Min_deger) Motor_3 = Min_deger;
  //**************************************************************

  Motor_4 = 1500 - yunuslama - yatis;
  if (Motor_4 >= Mak_deger) Motor_4 = Mak_deger;
  else if (Motor_4 <= Min_deger) Motor_4 = Min_deger;

  Motor_5 = 1500 - yunuslama + yatis;
  if (Motor_5 >= Mak_deger) Motor_5 = Mak_deger;
  else if (Motor_5 <= Min_deger) Motor_5 = Min_deger;

  Motor_6 = 1500 - yunuslama + yatis;
  if (Motor_6 >= Mak_deger) Motor_6 = Mak_deger;
  else if (Motor_6 <= Min_deger) Motor_6 = Min_deger;

  Motor_7 = 1500 - yunuslama - yatis;
  if (Motor_7 >= Mak_deger) Motor_7 = Mak_deger;
  else if (Motor_7 <= Min_deger) Motor_7 = Min_deger;


  if (Motor_Aktif == true)
  {
    //***Tutum Kontrol***
    Ext_Servo_Kontrol.Ext_Servo(4, Motor_4 + 10);
    Ext_Servo_Kontrol.Ext_Servo(5, Motor_5 + 20);
    Ext_Servo_Kontrol.Ext_Servo(6, Motor_6);
    Ext_Servo_Kontrol.Ext_Servo(7, Motor_7 + 20);

    //***Sapma Kontrol***
    Ext_Servo_Kontrol.Ext_Servo(0, Motor_0);
    Ext_Servo_Kontrol.Ext_Servo(1, Motor_1);
    Ext_Servo_Kontrol.Ext_Servo(2, Motor_2 + 20);
    Ext_Servo_Kontrol.Ext_Servo(3, Motor_3);
  }
  else
  {
    //***Tutum Kontrol***
    Ext_Servo_Kontrol.Ext_Servo(4, Motor_Hiz + 10);
    Ext_Servo_Kontrol.Ext_Servo(5, Motor_Hiz + 20);
    Ext_Servo_Kontrol.Ext_Servo(6, Motor_Hiz ); //OYNAYAN MOTOR
    Ext_Servo_Kontrol.Ext_Servo(7, Motor_Hiz + 20);

    //***Sapma Kontrol***
    Ext_Servo_Kontrol.Ext_Servo(0, Motor_Hiz);
    Ext_Servo_Kontrol.Ext_Servo(1, Motor_Hiz);
    Ext_Servo_Kontrol.Ext_Servo(2, Motor_Hiz + 20);
    Ext_Servo_Kontrol.Ext_Servo(3, Motor_Hiz);
  }


  //Ekranda_Goster();
}

void Ekranda_Goster()
{

  //Serial.print("Motor_0 > "); Serial.println(Motor_0);
  //Serial.print("Motor_1 > "); Serial.println(Motor_1);
  //Serial.print("Motor_2 > "); Serial.println(Motor_2);
  //Serial.print("Motor_3 > "); Serial.println(Motor_3);
  //Serial.print("Motor_4 > "); Serial.println(Motor_4);
  //Serial.print("Motor_5 > "); Serial.println(Motor_5);
  //Serial.print("Motor_6 > "); Serial.println(Motor_6);
  //Serial.print("Motor_7 > "); Serial.println(Motor_7);

  //Serial.print("PID_Yat_Ref > "); Serial.println(PID_Yat_Ref);
  //Serial.print("PID_Yun_Ref > "); Serial.println(PID_Yun_Ref);
  //Serial.print("PID_Sap_Ref > "); Serial.println(PID_Sap_Ref);
  //Serial.print("PID_Der_Ref > "); Serial.println(PID_Der_Ref);
  //Serial.print("PID_Pus_Ref > "); Serial.println(PID_Pus_Ref);
  //Serial.print("Ref_Pus > "); Serial.println(Ref_Pus);
  //Serial.print("Ref_Pus2 > "); Serial.println(Ref_Pus2);

  //Serial.print("Gyro_X > "); Serial.println(Gyro_X);
  //Serial.print("Gyro_Y > "); Serial.println(Gyro_Y);
  //Serial.print("Gyro_Z > "); Serial.println(Gyro_Z);

  //Serial.print("X_Acisi > "); Serial.println(X_Acisi);
  //Serial.print("Y_Acisi > "); Serial.println(Y_Acisi);
  //Serial.print("Z_Acisi > "); Serial.println(Z_Acisi);

  //Serial.print("Yunuslama > "); Serial.println(yunuslama);
  //Serial.print("Yatış > "); Serial.println(yatis);
  //Serial.print("Sapma > "); Serial.println(sapma);
  //Serial.print("Derinlik > "); Serial.println(derinlik);
  //Serial.print("Pusula > "); Serial.println(pusula);

  //Serial.print("Kalan Açı > "); Serial.println(kalan_aci);
  //Serial.print("N_PID_Pus_Ref > "); Serial.println(N_PID_Pus_Ref);
  //Serial.print("PID_kalan_aci > "); Serial.println(PID_kalan_aci);
  delay(50);
}
