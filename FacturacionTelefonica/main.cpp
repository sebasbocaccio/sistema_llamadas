#include <iostream>
#include "SistemaTelefonico.h"
int main() {
    int ano = 1000;
    mes meso= static_cast<mes>(Agosto );
    int numero= 2;
    semana dia = static_cast<semana>(Domingo);
    int montoBasico = 1500;

    SistemaTelefonico sistem = SistemaTelefonico(ano, meso, numero, dia, montoBasico);
    std::tuple<string,int> foo ("Facu",99);
    sistem.nuevoCliente(foo);
    sistem.agregarNacion("Argentina",(float)0.3f);
    sistem.nuevaLlamada(99,local,Martes,17,30,230.0);
    sistem.nuevaLlamada(99,local,Martes,18,30,230.0);
    //sistem.darDeBajaCliente(99);
    sistem.cambiarDia();
    sistem.nuevoDia();
    return 0;

}
