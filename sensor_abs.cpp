#include "./includes/sensor_abs.h"
#include <iostream>
#include <cstdlib>
using namespace std;
//Direccion fisica para datos de telemetria oculta del resto del programa (solo lectura usando un template seguro)
constexpr uintptr_t SENSOR_HARDWARE_ADDR = 0x40021000;
using abs_data_reg = hw_register<SENSOR_HARDWARE_ADDR, read_access>;
//registro fisico simulado del DWT(Data Watchpoint and Trace) para medir los ciclos de reloj
#define DWT_CYCCNT (*(volatile uint32_t*)0xE0001004)

SensorLector::SensorLector(){
    active=true;
    cout<<"[RAII] Hardware del sensor encendido y recursos listos"<<endl;
}
SensorLector::~SensorLector(){
    active=false;
    cout<<"[RAII] Hardware apagado de forma segura. Sin fugas de memoria"<<endl;
}

void SensorLector::read_hardware(){
    //MEDICION DWT
    //capturamos el ciclo exactyo de CPU antes de ejecutar la logica
    uint32_t ciclos_inicio = DWT_CYCCNT;
    //En el hardware real usaria: uint32_t dato_crudo = abs_data_reg::read();
    uint32_t dato_crudo=85; //simulado para pc
    //puntero seguro y mapeado

    velocidad_rueda velocidad_actual(dato_crudo);//uso del tipo fuerte
    buffer.push(velocidad_actual);
    //calculamos cuanto tardo nuestra funcion en ejecutarse
    uint32_t ciclos_fin = DWT_CYCCNT;
    cout<<"Lectura guardada en el Ring Buffer en "<<(ciclos_fin - ciclos_inicio)<<" ciclos de CPU"<<endl;
}