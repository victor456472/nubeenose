#include <Arduino.h>
#include "MQLab.h"

Enose::Enose(int puerto_analogico[10], float Rl[10]){
    //pines analogicos
    this->pinA = puerto_analogico[0];
    this->pinB = puerto_analogico[1];
    this->pinC = puerto_analogico[2];
    this->pinD = puerto_analogico[3];
    this->pinE = puerto_analogico[4];
    this->pinF = puerto_analogico[5];
    this->pinG = puerto_analogico[6];
    this->pinH = puerto_analogico[7];
    this->pinI = puerto_analogico[8];
    this->pinJ = puerto_analogico[9];
    //resistencias de carga
    this->RLA = Rl[0];
    this->RLB = Rl[1];
    this->RLC = Rl[2];
    this->RLD = Rl[3];
    this->RLE = Rl[4];
    this->RLF = Rl[5];
    this->RLG = Rl[6];
    this->RLH = Rl[7];
    this->RLI = Rl[8];
    this->RLJ = Rl[9];
    assignArray();
}

void Enose::assignArray(){
    //asignando puertos de lectura
    puerto_lectura[0]=pinA;
    puerto_lectura[1]=pinB;
    puerto_lectura[2]=pinC;
    puerto_lectura[3]=pinD;
    puerto_lectura[4]=pinE;
    puerto_lectura[5]=pinF;
    puerto_lectura[6]=pinG;
    puerto_lectura[7]=pinH;
    puerto_lectura[8]=pinI;
    puerto_lectura[9]=pinJ;
    //asignando resistencias de carga
    RL[0]=RLA;
    RL[1]=RLB;
    RL[2]=RLC;
    RL[3]=RLD;
    RL[4]=RLE;
    RL[5]=RLF;
    RL[6]=RLG;
    RL[7]=RLH;
    RL[8]=RLI;
    RL[9]=RLJ;

}

void Enose::serialWriteAutoCalibration(int muestras){
    float suma[] = {0,0,0,0,0,0,0,0,0,0};
    float promedio[] = {0,0,0,0,0,0,0,0,0,0};
    for(int i=0; i<=muestras; i++){
        for(int j=0; j<=9; j++){
            suma[j]+=((1023-(float)analogRead(puerto_lectura[j]))/(float)analogRead(puerto_lectura[j]))*RL[j];
            
            Serial.print("suma[");
            Serial.print(j);
            Serial.print("]: ");
            Serial.print(suma[j]);
            Serial.print("  ");
        }
        Serial.print("\n");
        delay(500);
    }
    for (int j = 0; j <= 9; j++)
    {
        promedio[j]=suma[j]/muestras;
        
        Serial.print("promedio[");
        Serial.print(j);
        Serial.print("]: ");
        Serial.print(promedio[j]);
        Serial.print("  ");
    }
    Serial.print("\n");
    for (int j= 0; j <= 9; j++){
        RO[j] = promedio[j]/factor_aire_limpio[j];
        Serial.print("Ro[");
        Serial.print(j);
        Serial.print("]: ");
        Serial.print(RO[j]);
        Serial.print(" ");
    }
    Serial.print("\n");
}

void Enose::manualCalibration(float Ro[10]){
    for(int i = 0; i < 10; i++){
        RO[i]=Ro[i];
    }
}

void Enose::rsGet(){
    for (int i = 0; i < 10; i++)
    {
        RS[i]= ((1023-(float)analogRead(puerto_lectura[i]))/(float)analogRead(puerto_lectura[i]))*RL[i];     
    }
    
}

void Enose::rsSerialWrite(){
    rsGet();
    for(int i = 0; i < 10; i++){  
        Serial.print("Rs");
        Serial.print("[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(RS[i]);
        Serial.print(" ");
    }
    Serial.print("\n");
}

void Enose::rsSerialPlot(){
    rsGet();
    for(int i = 0; i < 9; i++){  
        Serial.print(RS[i]);
        Serial.print(",");
    }
    Serial.println(RS[9]);
}

void Enose::ratioGet(){
    rsGet();
    for(int i = 0; i < 10; i++){
        ratio[i]=RS[i]/RO[i];
    }
}

void Enose::ratioSerialWrite(){
    ratioGet();
    for (int i = 0; i < 10; i++)
    {
        Serial.print("Rs/Ro");
        Serial.print("[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.print(ratio[i]);
        Serial.print(" ");
    }
    Serial.print("\n");
}

void Enose::ratioSerialPlot(){
    ratioGet();
    for (int i = 0; i < 9; i++)
    {
        Serial.print(ratio[i]);
        Serial.print(",");
    }
    Serial.println(ratio[9]);
}

void Enose::roSerialPlot(){
    rsGet();
    for (int i = 0; i < 9; i++){
        Serial.print(RS[i]/factor_aire_limpio[i]);
        Serial.print(",");
    }
    Serial.println(RS[9]/factor_aire_limpio[9]);
}

void Enose::ppmGet(){
    ratioGet();
    for (int i = 0; i < 10; i++)
    {
        ppm[i]=Matriz_m[i]*pow(ratio[i],Matriz_b[i]);
    }
}

void Enose::ppmSerialPlot(){
    ppmGet();
    for (int i = 0; i < 9; i++){
        Serial.print(ppm[i]);
        Serial.print(",");
    }
    Serial.println(ppm[9]);
}

void Enose::excelInit(){
    Serial.println("LABEL,hora,ALCOHOL_s1[PPM],MONOXIDO DE CARBONO_S1[PPM],DIHIDROGENO_s1[PPM],ACETONA_s1[PPM],METANO_s1[PPM], ALCOHOL_s2[PPM],MONOXIDO DE CARBONO_S2[PPM],DIHIDROGENO_s2[PPM],ACETONA_s2[PPM],METANO_s2[PPM]");
}

void Enose::ppmExcelWrite(){
    ppmGet();
    Serial.print("DATA,TIME,");
    for(int i=0; i<9; i++){
        Serial.print(ppm[i]);
        Serial.print(",");
    }
    Serial.println(ppm[9]);

}

void Enose::HMIcomunication(){
      
}