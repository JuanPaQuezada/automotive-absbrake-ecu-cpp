#include "./includes/sensor_abs.h"
#include <iostream>
#include <cstdlib>
using namespace std;
//Direccion fisica para datos de telemetria oculta del resto del programa
constexpr uintptr_t SENSOR_HARDWARE_ADDR = 0x40021000;

SensorLector::SensorLector(){
    active=true;
    cout<<"[RAII] Hardware del sensor encendido y recursos listos"<<endl;
}
SensorLector::~SensorLector(){
    active=false;
    cout<<"[RAII] Hardware apagado de forma segura. Sin fugas de memoria"<<endl;
}

void SensorLector::read_hardware(){
    //puntero seguro y mapeado
    volatile uint32_t* ptr_sensor = reinterpret_cast<volatile uint32_t*>(SENSOR_HARDWARE_ADDR);
    /*EN UN HARDWARE REAL LEERIAMOS DIRECTAMENTE EL PERFERICO CON:
        buffer.push(*ptr_sensor);
    */

    //Para compilar en la pc sin segmentation Faul, simulamos la lectura con:
    uint32_t dato_simulado = rand()%100;
    buffer.push(dato_simulado);
    cout<<"Lectura guardada en el Ring Buffer"<<endl;
}