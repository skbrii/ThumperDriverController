#include <SoftwareSerial.h>

#define SoftSerial Serial1

#define RAZMER_STROKI 256
unsigned char stroka[RAZMER_STROKI];
int konets = 0;
byte est_nachalo = false;
unsigned char proverka[2];

struct dannye_ot_zhps {
  byte vremya_chasy;
  byte vremya_minuty;
  byte vremya_sekundy;
  long shirota;
  long dolgota;
  byte kolichestvo_sputnikov;
  long pogreshnost;
  long visota;
  long skorost;
  long napravlenie;
  int data_god;
  byte data_mesyats;
  byte data_chislo;  
} dannye;

void setup() {
  SoftSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if(SoftSerial.available()) {
    unsigned char bukva = SoftSerial.read();
    if(bukva == '$') {
      konets = 0;
      if(est_nachalo)
        oshibochka(1);
      est_nachalo = true;
    }

    stroka[konets ++] = bukva;
    
    if(konets > 3 && stroka[konets - 3] == '*') {
      if(est_nachalo) {
        proverka_stroki();
        if(proverka[0] == stroka[konets - 2] && proverka[1] == stroka[konets - 1])
          stroka_poluchena();
        else 
          oshibochka(3);
      } else
        oshibochka(2);
      est_nachalo = false;
    }
  }
}

void proverka_stroki() {
  unsigned char chislo = 0;
  for(int i = 1; i < konets - 3; i ++)
    chislo ^= stroka[i];
  proverka[0] = chislo_v_bukvu(chislo >> 4);
  proverka[1] = chislo_v_bukvu(chislo & 0x0f);
}

unsigned char chislo_v_bukvu(byte chislo) {
  return chislo < 10 ? '0' + chislo : 'A' + chislo - 10;
}

void stroka_poluchena() {  
  Serial.write(stroka, konets);
  Serial.print(" + \n");

  byte stroka_opoznana = true;/*
  switch(stroka[3]) {
    case 'G':
      switch(stroka[4]) {
        case 'G':
        break;
      }
      break;
    case 'R':
      break;
    case 'V':
      break;
    default:
      stroka_opoznana = false;
  }*/
  if(stroka[3] == 'G' && stroka[4] == 'G' && stroka[5] == 'A')
    razbor_gga();
  else
    if(stroka[3] == 'G' && stroka[4] == 'L' && stroka[5] == 'L')
      razbor_gll();
    else
      if(stroka[3] == 'G' && stroka[4] == 'S' && stroka[5] == 'A')
        razbor_gsa();
      else
        if(stroka[3] == 'G' && stroka[4] == 'S' && stroka[5] == 'V')
          razbor_gsv();
        else
          if(stroka[3] == 'R' && stroka[4] == 'M' && stroka[5] == 'C')
            razbor_rmc();
          else
            if(stroka[3] == 'V' && stroka[4] == 'T' && stroka[5] == 'G')
              razbor_vtg();
            else
              stroka_opoznana = false;
  if(! stroka_opoznana)
    Serial.print(" ! Neponyatnoe soobschenie\n");  
}

void razbor_gga() {
  Serial.print("gga\n");
}

void razbor_gll() {
  Serial.print("gll\n");
}

void razbor_gsa() {
  Serial.print("gsa\n");
}

void razbor_gsv() {
  Serial.print("gsv\n");
}

void razbor_rmc() {
  Serial.print("rmc\n");
}

void razbor_vtg() {
  Serial.print("vtg\n");
}

void oshibochka(byte kod_oshibki) {
  Serial.print(" ! Oshibochka :( ");
  switch(kod_oshibki) {
    case 1:
      Serial.print("net okonchaniya stroki");
      break;
    case 2:
      Serial.print("net nachala stroki");
      break;
    case 3:
      Serial.print("proverka ne udalas'");
      break;
  }
  Serial.print("\n");
}
