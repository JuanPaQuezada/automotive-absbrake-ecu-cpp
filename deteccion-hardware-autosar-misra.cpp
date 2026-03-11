#include <iostream>
#include <cstdint>
using namespace std;
//Simulamos la direccion de memoria fisica donde el hardware del sensor escribe su voltaje
constexpr uintptr_t SENSOR_HARDWARE_ADDR =0x40021000;

//1- Bufer circular (memoria estatica sin malloc ni memory leaks)
template <typename T, size_t N>
struct RingBuffer {
    T arr[N];
    size_t write_idx = 0;
    size_t count = 0;

    void push(T value){
        arr[write_idx] = value;
        write_idx = (write_idx+1)%N;
        if(count<N) count++;
    }
};

//RAII + PERIFERICOS MAPEADOS DE VOLATILE
class SensorLector{
    private:
        RingBuffer<uint32_t, 5> buffer; //buffer fijo de 5 lecturas
        bool activo;
    public:
        //Constructor adquisicion de recursos - RAII
        SensorLector(){
            activo=true;
            cout<<"[RAII] Hardware del sensor encendido y recursos listos"<<endl;
        }
        //Destructor liberacion automatica - RAII
        ~SensorLector() {
            activo=false;
            cout<<"[RAII] Hardware apagado de forma segura. Sin fugas de memoria."<<endl;
        }

        void read_hardware(){
            volatile uint32_t* ptr_sensor = reinterpret_cast<volatile uint32_t*>(SENSOR_HARDWARE_ADDR);
            buffer.push(rand()%100);
            cout<<"Lectura guardada en el Ring Buffer, el total de elementos es: "<<buffer.count<<endl;
        }
};

int main(){
    cout<<"---Inicio de operacion---"<<endl;
    {//al crear el objeto en este bloque , el constructor actua (RAII)
        SensorLector sensor_abs;
        sensor_abs.read_hardware();
        sensor_abs.read_hardware();
    }// Al salir de esta llave el destructor actua automaticamente (RAII)
    cout<<"---Fin de operacion---"<<endl;
    return 0;
}