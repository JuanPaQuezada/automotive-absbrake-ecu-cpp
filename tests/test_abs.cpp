#include <vector>
#include "gtest/gtest.h"
#include "../includes/algoritmo_abs.h"
#include "../includes/sensor_abs.h"
#include "../includes/isensor_velocidad.h"

//Nuestro Mock del harware
class fake_sensor_abs : public isensor_velocidad{
    public:
    //Al instanciarlo le pasamos un arreglo de velocidades simuladas
    fake_sensor_abs(const std::vector<uint32_t>& muestras) : muestras_(muestras),indice_(0){}
    //sobreescribimos la lectura de hardware para que devuelva nuestros datos falsos
    std::uint32_t leer_velocidad() override{
        if(indice_<muestras_.size()){
            return muestras_[indice_++];
        }
        return muestras_.back();//si se acaban, repetimos el ultimo
    }
    private:
        std::vector<uint32_t> muestras_;
        size_t indice_;
};
//Prueba verificar que el abs se activa ante una caida brusca de velocidad
TEST(AbsAlgoTest, DetectarFrenadoBrusco) {
    //1. Preparamos los datos falsos: el auto va a 100km/h y de repente cae a 20km/h (bloqueo de rueda)
    std::vector<uint32_t> velocidades_simuladas = {
        100, //lectura inicial normal
        100, //sigue normal->el abs no debe activarse
        20 //caida drastica -> abs debe ACTIVARSE
    };

    //2. instanciamos el sensor falso con nuestra simulacion
    fake_sensor_abs sensor_falso(velocidades_simuladas);
    //3.le inyectamos el sensor falso a tu algoritmo
    algoritmo_abs mi_abs(sensor_falso);
    //4. Validamos el comportamiento esperado paso a paso
    EXPECT_FALSE(mi_abs.evaluar_estado());//lee 100 ->abs apagado
    EXPECT_FALSE(mi_abs.evaluar_estado());//lee 100->abs apagado
    EXPECT_TRUE(mi_abs.evaluar_estado());//lee 20->peligro abs ENCENDIDO
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}