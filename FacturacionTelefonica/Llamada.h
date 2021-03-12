//
// Created by sebasbocaccio on 11/3/21.
//

#ifndef FACTURACIONTELEFONICA_LLAMADA_H
#define FACTURACIONTELEFONICA_LLAMADA_H
#include <iostream>
const int COMIENZO_HORA_PICO = 8;
const int FIN_HORA_PICO = 20;
enum semana {Lunes,Martes,Miercoles,Jueves,Viernes,Sabado,Domingo};
class llamada {
public:
    virtual ~llamada();
    llamada(int duracion, double costoXMinuto, int hs , int min,semana dia){
       if(inputValido(duracion,costoXMinuto,hs,min)) {
           _duracion = duracion;
           _costoXMinuto = costoXMinuto;
           _hs = hs;
           _min = min;
           _dia = dia;
       }
       else{
           throw std::invalid_argument("Argumentos invalidos");
       }
    }

    virtual double costo() = 0;

protected:
    double _costoXMinuto;
    int _duracion;
    int _hs;
    int _min;
    semana _dia;
    bool inputValido(int duracion, double costoXMinuto, int hs , int min);

};

class LlamadaLocal : public llamada{

public:
    ~LlamadaLocal() override {}
    LlamadaLocal(int duracion, int hs , int min,semana dia);

    double costo();
private:
    bool HoraPico(int duracion, int hs, int min,semana dia);
};

class LlamadaNacional : public  llamada
{
public:
    ~LlamadaNacional() override{}
    LlamadaNacional(int duracion, double costoXMinuto, int hs , int min,semana dia);
    double costo();
};

class LlamadaInternacional : public llamada
{
public:
    ~LlamadaInternacional() override {}
    LlamadaInternacional(int duracion, double costoXMinuto, int hs , int min,semana dia );
    double costo();

};



#endif //FACTURACIONTELEFONICA_LLAMADA_H
