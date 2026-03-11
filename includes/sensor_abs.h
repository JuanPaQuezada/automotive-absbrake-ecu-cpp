#pragma once
#include <cstdint>
#include "ring_buffer.h"

class SensorLector{
    private:
        RingBuffer<uint32_t, 5> buffer;
        bool active;
    public:
        //Constructores y destructores para RAII
        SensorLector();
        ~SensorLector();

        //Metodos del sensor
        void read_hardware();
};