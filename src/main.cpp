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
bool calibracion = true;
int prueba;

int led1=35;
int led2=34;
int led3=33;
int led4=32;

int inyeccion = 51;
int expulsion = 53;

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

  enose1.manualCalibration(Ro);
  while (!digitalRead(sw))
  {
  }
  enose1.excelInit();
}

void ledPannel(int ld1, int ld2, int ld3, int ld4){
  digitalWrite(led1, ld1);
  digitalWrite(led2, ld2);
  digitalWrite(led3, ld3);
  digitalWrite(led4, ld4);
}

void loop()
{
  while (!digitalRead(sw))
  {
    digitalWrite(expulsion, 0);
    digitalWrite(inyeccion, 0);
    ledPannel(0,0,0,0);
    contador = 0;
    calibracion = true;
  }
  while (contador < 301 && digitalRead(sw))     //301
  {
    contador++;

    digitalWrite(expulsion, 1);
    digitalWrite(inyeccion, 0);
    ledPannel(1,0,0,0);
    //enose1.ppmExcelWrite();
    //enose1.ppmSerialPlot();
    delay(1000);
  }
  while (contador > 300 && contador < 601 && digitalRead(sw)) //300   601
  {
    contador++;
    digitalWrite(expulsion, 0);
    digitalWrite(inyeccion, 1);
    ledPannel(0,1,0,0);
    enose1.ppmExcelWrite();
    //enose1.ppmSerialPlot();
    delay(1000);
  }
  if (contador == 601)   //601
  {
    while (digitalRead(sw))
    {
      digitalWrite(expulsion, 0);
      digitalWrite(inyeccion, 0);
      ledPannel(0,0,1,0);
      //enose1.ppmExcelWrite();
      //enose1.ppmSerialPlot();
      delay(1000);
    }
    while (!digitalRead(sw))
    {
    }
  }
  while (contador > 600 && contador < 901 && digitalRead(sw))   //600   901
  {
    contador++;
    digitalWrite(expulsion, 1);
    digitalWrite(inyeccion, 1);
    ledPannel(0,0,0,1);
    //enose1.ppmExcelWrite();
    //enose1.ppmSerialPlot();
    delay(1000);
  }
  while (contador > 900 && contador < 1201 && digitalRead(sw))   //600   901
  {
    contador++;
    digitalWrite(expulsion, 0);
    digitalWrite(inyeccion, 0);
    ledPannel(1,1,1,1);
    delay(1000);
    ledPannel(0,0,0,0);
  }
  contador = 0;
}

// Razon_medicion_filtrada[i] = (Razon_medicion_MQ[i]*alpha_MQ)+((1-alpha_MQ)*Razon_medicion_MQ[i]);
