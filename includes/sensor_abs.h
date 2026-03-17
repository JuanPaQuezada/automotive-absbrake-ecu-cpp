#pragma once
#include <cstdint>
#include <type_traits>
#include "ring_buffer.h"
//using namespace std; NO PONER para evitar contamincacion del espacio de nombres o Namespace Pollution para evitar colisiones de nombres

//Implementamos Strong Types para evitar errores de presion hidraulica
//envolvemos el dato crudo en una clase para evitar conversiones implicitas
class velocidad_rueda {
    private:
        std::uint32_t valor_;
    public:
        //el explicit prohibe que un float o un int normal se convierta automaticamente aqui
        explicit velocidad_rueda(std::uint32_t v=0):valor_(v){}
        std::uint32_t  get() const{return valor_;}
};

//Implementamos Type-Safe HAL
//estructura de control de acceso en tiempo de compilacion
struct read_access{};
template<std::uintptr_t Address, typename Access=read_access, typename T=std::uint32_t>
struct hw_register{
    //SFINAE evaluara esto en tiempo de compilacion, solo compilara si el accesso es de lectura
    template <typename Access_ = Access>
    static std::enable_if_t<std::is_base_of_v<read_access, Access_>, T> read(){
        return *reinterpret_cast<volatile T*>(Address);
    }
};
class SensorLector{
    private:
        RingBuffer<velocidad_rueda, 5> buffer;
        bool active;
    public:
        //Constructores y destructores para RAII
        SensorLector();
        ~SensorLector();

        //Metodos del sensor
        void read_hardware();
};