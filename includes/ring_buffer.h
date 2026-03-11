#pragma once //evita que el archivo se incluya multiples veces
#include <cstddef>

template <typename T, size_t N>
struct RingBuffer {
    T arr[N];
    size_t write_idx = 0;
    size_t count=0;

    void push(T value){
        arr[write_idx] = value;
        //avance circular (ring buffer)
        write_idx = (write_idx+1)%N;
        if (count<N) count++;
    }
};