//
// Created by sebasbocaccio on 11/3/21.
//

#ifndef FACTURACIONTELEFONICA_CLIENTE_H
#define FACTURACIONTELEFONICA_CLIENTE_H
#include "Llamada.h"
#include "vector"
#include "string"

class cliente {
public:
    cliente(int documento, std::string apellido,int dia): _documento(documento),_apellido(apellido),_diaIngreso(dia){}
    std::vector<llamada> llamadas(){ return _llamadasCliente;}
    void agregarLlamada(llamada llamada){_llamadasCliente.push_back(llamada);}
    void vaciarlista(){_llamadasCliente.clear();}
    double montoAPagar(){
        double costo = 0;
        for( int i = 0; i < _llamadasCliente.size();i++){
            costo += _llamadasCliente[i].costo();
        }
    }
    int diaIngreso(){return _diaIngreso;}
private:
    int _documento;
    std::string _apellido;
    std::vector<llamada> _llamadasCliente;
    int _diaIngreso;
};


#endif //FACTURACIONTELEFONICA_CLIENTE_H
