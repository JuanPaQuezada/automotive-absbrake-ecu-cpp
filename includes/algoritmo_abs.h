#pragma once
#include <cstdint>
#include "isensor_velocidad.h" //La interfaz abstracta para el test

class algoritmo_abs{
    public:
    //inyeccion de dependencias: Pasamos la interfaz abstracta (puede ser el hardware o el falso)
        algoritmo_abs(isensor_velocidad &sensor) : sensor_(sensor), primera_lectura_(true), velocidad_previa_(0){}
        // esta funcion es la que llama a un super-loop o tu secuenciador continuamente
        bool evaluar_estado(){
            //1. lee el dato (No sabes si viene de I2C, SPI O un Mock, solo sabemos que es un uint32_t)
            std::uint32_t velocidad_actual = sensor_.leer_velocidad();

            //2. si es la primera vez que corre, solo guardamos el dato y salimos
            if(primera_lectura_){
                velocidad_previa_ = velocidad_actual;
                primera_lectura_ = false;
                return false;
            }
            //3. Aplicamos las matematicas del abs, es decir, calculamos la desaceleracion
            bool activar_abs = false;
            //verificar si la velocidad cayo
            if(velocidad_previa_ > velocidad_actual){
                std::uint32_t desaceleracion = velocidad_previa_ - velocidad_actual;
                //si la caida de velocidad es mayor a nuestro umbral, la rueda se bloqueo
                if(desaceleracion>UMBRAL_BLOQUEO) {
                    activar_abs = true; // Peligro! debemos soltar el freno
                }
            }
            // 4. Guardamos la velocidad actual para el siguiente ciclo
            velocidad_previa_ = velocidad_actual;
            return activar_abs;
        }

        private:
            //Constante que se define que tan brusca debe ser la caida de velociad para activar el ABS
            static constexpr std::uint32_t UMBRAL_BLOQUEO=30;
            isensor_velocidad &sensor_; //referencia a nuestra interfaz
            std::uint32_t velocidad_previa_;
            bool primera_lectura_;

};