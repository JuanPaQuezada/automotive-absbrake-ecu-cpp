#pragma once
#include <cstdint>

//SensorLector real heredara de esta interfaz para usarse en el auto pero para pruebas usaremos una version falsa (un mock)
class isensor_velocidad {
    public:
    //convertimos la funcion leer_velocidad en virtual pura indicando que no tiene implementacion aqui
        virtual std::uint32_t leer_velocidad() = 0;
        virtual ~isensor_velocidad() = default;
};