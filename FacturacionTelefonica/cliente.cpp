//
// Created by sebasbocaccio on 11/3/21.
//

#include "cliente.h"


double cliente::montoAPagar(){
	   double costo = 0;
        for( int i = 0; i < _llamadasCliente.size();i++){
            double valor = _llamadasCliente[i]->costo();
            costo += valor;
        }
        return costo;
    
}
void cliente::vaciarlista(){

    for( int i = 0; i < _llamadasCliente.size();i++){
        delete _llamadasCliente[i];
    }

    _llamadasCliente.clear();}