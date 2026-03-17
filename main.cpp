#include <iostream>
#include "./includes/sensor_abs.h"
#include <cstdlib>
#include <exception>
using namespace std;
//Implementar asserts ligeros
//Es un microcontrolador real, hal::get_pc() leeria el program counter mediante ensamblador
//Aqui lo simulamos devolviendo un 0 paraa compilar en pc
uint32_t get_pc_simulado(){return 0x08000F74;}

void log_pc_and_halt(std::uint32_t pc){
    cout<<"Fallo critico (assert) en direccion de memoria: 0x"<<hex<<pc<<endl;
    abort();//en el hardware real seria un bucle infinito while(true){}
}
//nuestro light_assert evalua la expresion. si es falsa, detiene el sistema sin guardar strings largos
#define light_assert(expr) (static_cast<bool>(expr)?void(0) : log_pc_and_halt(get_pc_simulado()))
int main(){
    //manejo seguro de terminacion
    //si desactivamos las excepciones cualquier error fatal del sistema llamara aqui
    set_terminate([](){
        cout<<"[EMERGENCIA] falla de sistema>>Desactivando hardware ABS y reiniciando ECU.."<<endl;
        abort();
    });
    cout<<"---INICIO DE OPERACIO DE LA ECU---"<<endl;
    {//iniciamos un bloque para probar el RAII
        SensorLector sensor_abs;//se llama al constructor y enciende el hardware
        //Comprobacion de seguridad en desarrollo usando nuestro assert ligero
        int estado_sistema=1;
        light_assert(estado_sistema == 1);//pasara limpiamente
        sensor_abs.read_hardware();
        sensor_abs.read_hardware();
        sensor_abs.read_hardware();
    }//el objeto sale de su ambito es decir de su scope, el destructor se llama automaticamente
    cout<<"---Fin de operacion de la ECU---"<<endl;
    return 0;
}