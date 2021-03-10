#include <iostream>
#include "SistemaTelefonico.h"
int main() {
    int ano = 1000;
    mes meso= static_cast<mes>(Agosto );
    int numero= 30;
    semana dia = static_cast<semana>(Domingo);
    int montoBasico = 1500;

    SistemaTelefonico sistem = SistemaTelefonico(ano, meso, numero, dia, montoBasico);
    std::tuple<string,int> foo ("Facu",99);
    sistem.nuevoCliente(foo);
    sistem.darDeBajaCliente(99);
    sistem.agregarNacion("Argentina",(float)0.3f);
    sistem.nuevoCliente(foo);
    sistem.nuevaLlamada(99,local,Martes,7,40,120.0);
    return 0;

}
