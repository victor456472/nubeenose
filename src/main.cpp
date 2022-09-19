#include <Arduino.h>
#include <MQlab.h>

int MQ_PIN[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
float Lectura[10];
float Razon_medicion_MQ[10];
float MM_MQ[10];
float Razon_medicion_filtrada[10];
float MQ_L[10];
float f[] = {59.099, 25.1, 6.44, 3.635, 9.88, 59.099, 25.1, 6.44, 3.635, 9.88};
float Ro[] = {40, 109, 784, 547, 177, 17.4, 49.28, 726.5, 627, 141};
float *Ppm;; // ojo usar este tipo de variable para recibir arreglos de funciones

// MQ3 - ALCOHOL
float RL_MQ3_ALC = 991;
// MQ7 - CO
float RL_MQ7_CO = 950;
// MQ5 - DIHIDROGENO
float RL_MQ5_H2 = 985;
// MQ135 - ACETONA
float RL_MQ135_ACE = 905;
// MQ6 - CH4
float RL_MQ6_CH4 = 982;
// matriz de resistencias

float RL_MQ[] = {RL_MQ3_ALC, RL_MQ7_CO, RL_MQ5_H2, RL_MQ135_ACE, RL_MQ6_CH4, RL_MQ3_ALC, RL_MQ7_CO, RL_MQ5_H2, RL_MQ135_ACE, RL_MQ6_CH4};
float RL_L[10];
// Modelos matematicos mx^b
float Matriz_m[] = {297.57, 93.076, 1226.7, 35.012, 2205, 297.57, 93.076, 1226.7, 35.012, 2205};
float Matriz_b[] = {-1.481, -1.534, -4.257, -3.378, -2.507, -1.481, -1.534, -4.257, -3.378, -2.507};
// Filtrado de datos de datos
#define alpha_MQ 0.01
int contador = 0;
int contador2 = 0;
bool calibracion = true;
int prueba;

int led1=35;
int led2=34;
int led3=33;
int led4=32;

int inyeccion = 51;
int expulsion = 53;


bool activar_espera=false;
bool door0=false;
bool door1=true;
bool door2=true;
bool door3=true;

unsigned long tiempo;
unsigned long tiempo2;
unsigned long tiempo3;

int range_time = 300;

byte sw=49;

Enose enose1(MQ_PIN, RL_MQ);

void setup()
{
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(expulsion, OUTPUT);
  pinMode(inyeccion, OUTPUT);
  pinMode(sw, INPUT);
  pinMode(47, OUTPUT);
  digitalWrite(expulsion, LOW);
  digitalWrite(inyeccion, LOW);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(47, LOW);

  tiempo=0;
  tiempo2=0;
  tiempo3=0;

  enose1.manualCalibration(Ro);
  //while (!digitalRead(sw))
  //{
  //}
}

void ledPannel(int ld1, int ld2, int ld3, int ld4){
  digitalWrite(led1, ld1);
  digitalWrite(led2, ld2);
  digitalWrite(led3, ld3);
  digitalWrite(led4, ld4);
}

void limpieza_parcial(){
  digitalWrite(expulsion, 1);
  digitalWrite(inyeccion, 0);
}

void inyeccion_gases(){
  digitalWrite(expulsion, 0);
  digitalWrite(inyeccion, 1);
}

void detener_bombas(){
  digitalWrite(expulsion, 0);
  digitalWrite(inyeccion, 0);
}

void limpieza_completa(){
  digitalWrite(expulsion, 1);
  digitalWrite(inyeccion, 1);
}

void loop()
{
  while (!digitalRead(sw))
  {
    detener_bombas();
    ledPannel(0,0,0,0);
    door1=true;
    door0=false;
    door3=true;
    tiempo3=0;
    contador=0;
    tiempo=millis();
    activar_espera=false;
  }

  if(door3){
    enose1.excelInit();
    door3=false;
  }

  tiempo2=millis()-tiempo;

  if (tiempo2-tiempo3 >= 1000){

    tiempo3=tiempo2;
    contador++;

    if (contador<=range_time){
      limpieza_parcial();
      ledPannel(1,0,0,0);
      Serial.println(contador);
    }

    if (contador>=range_time+1 && contador<=range_time*2)
    {
      inyeccion_gases();
      ledPannel(0,1,0,0);
      Serial.println(contador);
      enose1.ppmExcelWrite();
    }
    if (contador>=((range_time*2)+1)){
      activar_espera=true;
    }

    while(activar_espera && door1){
      tiempo2 =millis()-tiempo;
      if (tiempo2-tiempo3 >= 1000){
        tiempo3=tiempo2;
        Serial.println(contador);
      }
      ledPannel(0,0,1,0);
      detener_bombas();
      if (!digitalRead(sw)){
        door0=true;
      }
      if (door0){
        if (digitalRead(sw)){
          activar_espera=false;
          door1=false;
        }
      }
    }

    if (contador>=((range_time*2)+1) && contador<=range_time*3){
      ledPannel(0,0,0,1);
      limpieza_completa();
      Serial.println(contador);
      if(door2){
        delay(100);
      }
    }

    if (contador>=((range_time*3)+1)){
      ledPannel(1,1,1,1);
      detener_bombas();
      Serial.println(contador);
    }
  }
}


