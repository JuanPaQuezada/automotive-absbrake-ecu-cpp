#include <iostream>
#include "./includes/sensor_abs.h"
using namespace std;
int main(){
    cout<<"---INICIO DE OPERACIO DE LA ECU---"<<endl;
    {//iniciamos un bloque para probar el RAII
        SensorLector sensor_abs;//se llama al constructor y enciende el hardware
        sensor_abs.read_hardware();
        sensor_abs.read_hardware();
        sensor_abs.read_hardware();
    }//el objeto sale de su ambito es decir de su scope, el destructor se llama automaticamente
    cout<<"---Fin de operacion de la ECU---"<<endl;
    system("pause");
    return 0;
}