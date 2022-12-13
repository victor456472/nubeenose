#ifndef _ENOSE_H
#define _ENOSE_H

#include <Arduino.h>


class Enose {
    private:
        int pinA;
        int pinB;
        int pinC;
        int pinD;
        int pinE;
        int pinF;
        int pinG;
        int pinH;
        int pinI;
        int pinJ;
        int RLA;
        int RLB;
        int RLC;
        int RLD;
        int RLE;
        int RLF;
        int RLG;
        int RLH;
        int RLI;
        int RLJ;
        int puerto_lectura[10];
        float factor_aire_limpio[10] = {59.099, 25.1, 6.44, 3.635, 9.88, 59.099, 25.1, 6.44, 3.635, 9.88};
        float RL[10];
        float RO[10] = {0,0,0,0,0,0,0,0,0,0};
        float RS[10];
        float RS_filter_input[5];
        float RS_sum[10] = {0,0,0,0,0,0,0,0,0,0};
        float RS_internal_counter=0;
        float ppm[10];
        float ratio[10];
        float Matriz_m[10] = {297.57,93.076,1226.7,35.012,2205,297.57,93.076,1226.7,35.012,2205};
        float Matriz_b[10] = {-1.481,-1.534,-4.257,-3.378,-2.507,-1.481,-1.534,-4.257,-3.378,-2.507};
        void assignArray();
        void rsGet();
        void ratioGet();

    public:
        int variable=3;
        float* ppmGet();
        Enose(int puerto_lectura[10], float RL[10]);
        //calibra de manera automatica el valor de Ro de los sensores MQ y los imprime en el monitor serie. 
        void serialWriteAutoCalibration(int muestras);
        //asigna los valores de Ro manualmente
        void manualCalibration(float Ro[10]);
        //Lee Rs de los sensores y los imprime en el puerto serie
        void rsSerialWrite();
        //Lee Rs de los sensores y lo imprime en el puerto serie separado por coma para ser graficado
        void rsSerialPlot();
        //calcula el ratio de cada sensor y lo imprime en el puerto serie
        void ratioSerialWrite();
        //calcula el ratio de cada sensor y lo imprime en el puerto serie separado por coma para ser graficado
        void ratioSerialPlot();
        //mide el valor instantaneo de Ro y lo imprime en el puerto serie separado por coma para ser graficado
        void roSerialPlot();
        //inicializa las etiquetas DATA, TIME y los titulos requeridos para que excel muestre los datos de manera organizada
        void excelInit();
        //imprime los datos leidos la Enose (ppm) en excel
        void ppmExcelWrite();
        //grafica en el Serial plotter los datos leidos por la Enose
        void ppmSerialPlot();
        //envia los datos por el puerto serial al HMI
        float* HMIcomunication();
        float* HMIcomunication(bool finalizar_recoleccion);
        //filtrado
        void pascalFilter();
        void rs_filter_reseter();
};


/*//devuelve el valor de concentracion de un gas leido por un sensor MQ. tiene como parametros la base de y el exponente del modelo matematico y la razon Rs/Ro que arroja el sensor
float* ppmReadBox(float Rs_Ro[10], float base_function[10], float exp_function[]);

//imprime en el puerto serie un arreglo de 10 posiciones separados por coma; de manera que pueda ser graficado por el serial plotter
void tenArraySerialPrintln(float array[10]);

//agrega indices para cada columna de excel
void mqExcelIndex();

//envia los datos de un array de 10 posicion a excel
void mqExcelPrintln(float array[10]); */

#endif