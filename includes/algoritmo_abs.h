#pragma once
#include <cstdint>
#include "isensor_velocidad.h" //La interfaz abstracta para el test
#include "estado_abs.h"
#include <array>

//implementadcion de estados concretos
class estado_activo : public estado_abs{
    public:
    //si estamos en activo y recuperamos la traccion, pasamos a monitoreo
    abs_state_enum handle_event(evento_recuperacion_traccion) override{
        apagar_valvulas();
        return abs_state_enum::monitoreo;
    }
    abs_state_enum get_state_enum() override {return abs_state_enum::activo;}
    private:
    void apagar_valvulas() {} //aqui pondremos la logica del hardware
};

class algoritmo_abs{
    //Hago una plantilla para la maquina finita de estados donde despacha cualquier evento al estado actual
    public:
        template<typename E>
        void handle_event(E evento){
            if(auto estado=obtener_puntero_estado(estado_actual_)){
                //el compilador sabe magicamente que sobrecarga llamar gracias al tag
                estado_actual_=estado->handle_event(evento);
            }
        }

        void add_state(estado_abs *estado){
            states_.push_back(estado);
        }
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
            abs_state_enum estado_actual_ = abs_state_enum::monitoreo;
            std::vector<estado_abs*> states_; //contenedor de punteros a la interfaz
            estado_abs* get_the_state(abs_state_enum state_enum){
                for (auto estado:states_){
                    if(estado->get_state_enum() == state_enum){
                        return estado;
                    }
                }
                return nullptr;
            }


            //Constante que se define que tan brusca debe ser la caida de velociad para activar el ABS
            static constexpr std::uint32_t UMBRAL_BLOQUEO=30;
            isensor_velocidad &sensor_; //referencia a nuestra interfaz
            std::uint32_t velocidad_previa_;
            bool primera_lectura_;

};