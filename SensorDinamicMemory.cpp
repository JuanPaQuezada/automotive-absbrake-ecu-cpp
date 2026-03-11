#include <iostream>
#include <stdlib.h>
using namespace std;

struct SensorNode {
    int sensor_id;
    float actual_metric;
    SensorNode* next;
};

void add_sensor_startup(SensorNode **head, int id){
    SensorNode *newSensor = (SensorNode*) malloc(sizeof(SensorNode));
    if (nullptr == newSensor){
        cout<<"Critical memory error"<<endl;
        exit(1);
    }
    newSensor->sensor_id = id;
    newSensor->next = nullptr;
    SensorNode *temp = *head;
    if(*head == nullptr){
        *head = newSensor;
        return;
    }
    while(temp->next != nullptr){
        temp = temp->next;
    }
    temp->next = newSensor;
}

bool read_sensors_safe(SensorNode *head){
    if(head == nullptr) return true;
    SensorNode *p1 = head;
    SensorNode *p2 = head;
    while(p2 != nullptr && p2->next!=nullptr){
        p1=p1->next;
        p2= p2->next->next;
        if(p1 == p2){
            cout<<"Corrupcion de memoria, cliclo detectado"<<endl;
            return false;
        }
    }
    cout<<"Lectura de sensores completada con seguridad"<<endl;
    return true;
}

void free_sensors(SensorNode **head){
    SensorNode *temp = *head;
    SensorNode *nextSensor;
    while(temp != nullptr){
        nextSensor = temp->next;
        free(temp);
        temp = nextSensor;
    }
    *head = nullptr;
}

int main(){
    SensorNode* sensors_network = nullptr;
    add_sensor_startup(&sensors_network, 101);
    add_sensor_startup(&sensors_network, 102);
    add_sensor_startup(&sensors_network,103);

    read_sensors_safe(sensors_network);
    
    //SIMULACION DE FALLO DE HARDWARE (CORRUPCION DE MEMORIA)
    //forzamos manualmente a que el ultimo nodo apunte al primero (ciclo infinito)
    if(sensors_network != nullptr && sensors_network->next != nullptr && sensors_network->next->next != nullptr){
        sensors_network->next->next->next = sensors_network;
    }
    cout<<"\nIntentando leer tras corrupcion de RAM..."<<endl;
    read_sensors_safe(sensors_network);

    sensors_network->next->next->next = nullptr;

    free_sensors(&sensors_network);

    return 0;
}