#include <Arduino.h>
#include <MQlab.h>
#include <string.h>

int MQ_PIN[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
float Lectura[10];
float Razon_medicion_MQ[10];
float MM_MQ[10];
float Razon_medicion_filtrada[10];
float MQ_L[10];
float f[] = {59.099, 25.1, 6.44, 3.635, 9.88, 59.099, 25.1, 6.44, 3.635, 9.88};
//float Ro[] = {40, 109, 784, 547, 177, 17.4, 49.28, 726.5, 627, 141};
float Ro[] = {60, 109, 784, 547, 177, 17.4, 49.28, 726.5, 627, 141};
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
bool door4=true;
bool door5=true;
bool door6=true;
bool door7=true;
bool door8=true; //se usa para hacer que el contador "tiempo=millis" se ejecute una sola vez cuando enre a modo manual 
bool door9=true;

unsigned long tiempo;
unsigned long tiempo2;
unsigned long tiempo3;

int range_time = 30;
int range_time1 = 30;
int range_time2 = 60;

byte sw=49;

Enose enose1(MQ_PIN, RL_MQ);




/*recepcion de datos*/

String str = "";
const char separator = ',';
const int dataLenght = 9;
String *buff;;
String data[dataLenght];

/* variables para la seleccion del modo de uso */

String use_mode="1"; //1 si es auto y 0 si es manual

/*variables para la seleccion de la secuencia*/
String t1="c";
String t2="a";
String t3="b";

/*variables para el control manual*/
String ch1="o";
String ch2="o";

/*variables para ajuste de tiempos*/
String tt1="30";
String tt2="30";
String tt3="60";

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

String* lectura_serial(const int l){
  str=Serial.readStringUntil('\n');
  for(int i=0; i<l; i++){
    int index=str.indexOf(separator);
    data[i]=str.substring(0,index);
    str=str.substring(index+1);
  }
  return data;
}

void comprobar_puerto(){
  if (Serial.available()){
    buff=lectura_serial(dataLenght);
    if(buff[0]=="1" || buff[0]=="0"){
      use_mode=buff[0];
    }
    if (buff[1]=="a" || buff[1]=="b" || buff[1]=="c" || buff[1]=="d"){
      t1=buff[1];
    }
    if (buff[2]=="a" || buff[2]=="b" || buff[2]=="c" || buff[2]=="d"){
      t2=buff[2];
    }
    if (buff[3]=="a" || buff[3]=="b" || buff[3]=="c" || buff[3]=="d"){
      t3=buff[3];
    }

    if(buff[4]=="o" || buff[4]=="i"){
      ch1=buff[4];
    }
    if(buff[5]=="o" || buff[5]=="i"){
      ch2=buff[5];
    }

    tt1=buff[6];
    tt2=buff[7];
    tt3=buff[8];

    if (tt1!="n"){
      range_time=tt1.toInt();
    }
    if (tt2!="n"){
      range_time1=tt2.toInt();
    }
    if (tt3!="n"){
      range_time2=tt3.toInt();
    }

    Serial.println(use_mode);
    Serial.println(t1);
    Serial.println(t2);
    Serial.println(t3);
    Serial.println(ch1);
    Serial.println(ch2);
    Serial.println(range_time);
    Serial.println(range_time1);
    Serial.println(range_time2);
  }
}
void seleccionar_proceso(String secuence){
  if (secuence=="a"){
    ledPannel(0,1,0,0);
    inyeccion_gases();
  }else if(secuence=="b"){
    ledPannel(0,1,0,1);
    limpieza_completa();
  }else if(secuence=="c"){
    ledPannel(1,0,0,0);
    detener_bombas();
  }else if(secuence=="d"){
    ledPannel(0,0,0,1);
    limpieza_parcial();
  }
}

void automatic_process(){
  while (!digitalRead(sw) && (use_mode)=="1")
  {
    comprobar_puerto();
    enose1.rs_filter_reseter();
    detener_bombas();
    ledPannel(0,0,0,0);
    door1=true;
    door0=false;
    door2=true;
    door4=true;
    door5=true;
    door6=true;
    door7=true;
    tiempo3=0;
    contador=0;
    tiempo=millis();
    activar_espera=false;
  }
  if(use_mode=="1"){
    if (contador<=range_time+range_time1+range_time2){ //contador>=range_time+1 && 
      enose1.pascalFilter();
    }

    tiempo2=millis()-tiempo;

    if (tiempo2-tiempo3 >= 1000){

      tiempo3=tiempo2;
      contador++;

      if (contador<=range_time){
        seleccionar_proceso(t1);
        enose1.HMIcomunication();
      }

      if (contador>=range_time+1 && contador<=range_time+range_time1)
      {
        if(door6){
          enose1.pascalFilter();
          door6=false;
        }
        seleccionar_proceso(t2);
        enose1.HMIcomunication(); 
      }
      if (contador>=((range_time+range_time1)+1)){
        activar_espera=true;
      }

      while(activar_espera && door1 && use_mode=="1"){
        comprobar_puerto();
        if(door4){
          enose1.HMIcomunication(true); //la sobrecarga permite indicar el final de la recoleccion de datos
          door4=false;
        }
        tiempo2 =millis()-tiempo;
        if (tiempo2-tiempo3 >= 1000){
          tiempo3=tiempo2;
        }
        ledPannel(0,0,1,0);
        detener_bombas();
        if (!digitalRead(sw)){
          if(door7){
            delay(500);
            door7=false;
          }
          door0=true;
        }
        if (door0){
          if (digitalRead(sw)){
            activar_espera=false;
            door1=false;
          }
        }
      }
      if(use_mode=="1"){
        if (contador>=((range_time+range_time1)+1) && contador<=(range_time+range_time1+range_time2)){
          seleccionar_proceso(t3);
          enose1.HMIcomunication();
          if(door2){
            delay(100);
            door2=false;
          }
        }

        if (contador>=((range_time+range_time1+range_time2)+1)){
          ledPannel(1,1,1,1);
          detener_bombas();
        }
      }
    }

    if(door5){
      delay(600);
      door5=false;
    }
  }
}

void loop()
{
  comprobar_puerto();
  if (use_mode=="1"){ 
    automatic_process();
    door9=true;
  }else if (use_mode=="0"){
    if(door9){
      ledPannel(1,0,0,1);
      door9=false;
    }
    enose1.pascalFilter();
    if (door8){
      tiempo=millis();
      door8=false;
    }
    tiempo2=millis()-tiempo;
    if (tiempo2-tiempo3 >= 1000){
      tiempo3=tiempo2;
      enose1.HMIcomunication();
    }
    if (ch1=="o"){
      if (ch2=="o"){
        ledPannel(1,0,0,1);
        detener_bombas();
      }else if(ch2=="i"){
        ledPannel(1,0,1,1);
        limpieza_parcial();
      }
    }else if (ch1=="i"){
      if (ch2=="o"){
        ledPannel(1,1,0,1);
        inyeccion_gases();
      }else if(ch2=="i"){
        ledPannel(1,1,1,1);
        limpieza_completa();
      }
    }
  }
}


