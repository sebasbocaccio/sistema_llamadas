
#include <iostream>
#include "SistemaTelefonico.h"
using namespace  std;

void SistemaTelefonico::actualizarCalendario(){
    _numeroActual++;
    _diaSemana = static_cast<semana>(_diaSemana + 1 % 7);
    if(_numeroActual == 31){
        _numeroActual= 1;
        _mesActual = static_cast<mes>(_mesActual + 1 % 12);
        if(_mesActual == Enero) _anoActual++;
    }
}

void SistemaTelefonico::agregarLlamadaAcliente(cliente* usuario, destino ubicacion, double  costoxMinuto ,semana dia ,int hs, int min, int duracion){
    if(ubicacion == nacional){
        LlamadaNacional* nuevaLlamada = new LlamadaNacional(duracion, costoxMinuto, hs,min,dia);
        usuario->agregarLlamada(nuevaLlamada);
    }
    else if (ubicacion == internacional){
        LlamadaInternacional* nuevaLlamada = new LlamadaInternacional(duracion, costoxMinuto, hs,min,dia);
        usuario->agregarLlamada(nuevaLlamada);
    }
    else{
        LlamadaLocal* nuevaLlamada = new LlamadaLocal(duracion, hs,min,dia);
        usuario->agregarLlamada(nuevaLlamada);
    }
}

void SistemaTelefonico::agregarRegion(string nombre, double costoMinuto){
    int index_letra = primera_letra(nombre);

    if( index_letra > 25 || index_letra < 0){ cerr << "Porfavor ingresar un lugar valido. Recordar que tiene que comenzar con mayuscula y no aceptamos la ñ.\n";}
    else _preciolocalidades[index_letra].push_back( std::pair<string,int>(nombre,costoMinuto) );
}

void SistemaTelefonico::agregarNacion(string nombre, double costoMinuto){
    int index_letra = primera_letra(nombre);
    if( index_letra > 25 || index_letra < 0){ cerr << "Porfavor ingresar un pais valido. Recordar que tiene que comenzar con mayuscula y no aceptamos la ñ.\n";}
    else if(!estaEnSistema(internacional,nombre)){
        _precioInternacional[index_letra].push_back(std::pair<string,float>(nombre,costoMinuto));
    }
    else {cerr << "El pais ya se encuentra en la base de datos. Si desea actualizar el monto hagalo con la funcion adecuada.\n";}
}
void SistemaTelefonico::borrarLlamadas(cliente* usuario){
    usuario->vaciarlista();
}

void SistemaTelefonico::borrarUsuarios(){
    for (std::map<int,cliente*>::iterator it=_clientes.begin(); it!=_clientes.end(); ++it)
        it->second->vaciarlista();
        delete it->second;
}
bool SistemaTelefonico::clienteEnSistema(int documento){
    auto it = _clientes.find(documento) ;
    if( it == _clientes.end()){
        return false;
    }
    return true;
}



int SistemaTelefonico::correctaInicializacion( mes mes, int numero, semana dia){
    if ((mes > 12 || mes < 1)) {
        return 1;
    }
    if((numero > 30 || numero < 1) ){
        return 2;
    }
    if((dia > 6 || dia < 0) ){
        return 3;
    }
    return 0;
}


bool SistemaTelefonico::correctoDatosLlamada(int documento,destino ubicacion,string lugar, semana dia ,int hs, int min, int duracion){

    if (!clienteEnSistema(documento)) {
        cerr << "La persona con documento " << documento << " no esta en el sistema. La operacion no fue realizada. Agregar al sistema el usuario antes de adjudicarle una llamada \n";
        return false ;
    }
    if(!estaEnSistema(ubicacion,lugar)){
        cerr << "Ingreso un pais o region que no esta en el sistema; agregar al sistema antes porfavor. \n";
        return false;}
    if( (hs > 23 || hs < 0) || (min > 59 || hs < 0)){
        cerr << "En este sistema solo permitimos horarios entre 00:00 y 23:59. Porfavor ingrese un horario valido. \n";
        return false ;
    }
    if( duracion <= 0){ cerr << "En este sistema solo permitimos llamadas de tiempo positivo. \n";
        return false;
    }
    return true;

}
double SistemaTelefonico::costoMinuto(destino locacion, string lugar) {
    if (locacion == nacional) {
        for (int i = 0; i < _preciolocalidades[primera_letra(lugar)].size(); i++) {
            if (std::get<0>(_preciolocalidades[primera_letra(lugar)][i]) == lugar) {
                return std::get<1>(_preciolocalidades[primera_letra(lugar)][i]);
            }
        }
    }
    if (locacion == internacional) {
        for (int i = 0; i < _precioInternacional[primera_letra(lugar)].size(); i++) {
            if (std::get<0>(_precioInternacional[primera_letra(lugar)][i]) == lugar) { return std::get<1>(_precioInternacional[primera_letra(lugar)][i]);}
        }
    }
    else 0.10;
}

void SistemaTelefonico::darDeBajaCliente(int documento) {
    // Para dar de baja un cliente al sistema, debe estar inscripto anteriormente en el sistema.
    if(!clienteEnSistema(documento)){
        { cerr << "La persona con documento " << documento << " no esta en el sistema. La operacion no fue realizada \n";}
    }
    else{
        auto it = _clientes.find(documento);
        auto it2 = find(_clientesFechaFacturacion[it->second->diaIngreso()-1].begin(),_clientesFechaFacturacion[it->second->diaIngreso()-1].end(),it->second);
        _clientesFechaFacturacion[it->second->diaIngreso()-1].erase(it2);
        delete it->second;
        _clientes.erase(documento);
    }
    return;
}


bool SistemaTelefonico::estaEnSistema(destino locacion, string lugar) {
    if (locacion == nacional) {
        for (int i = 0; i < _preciolocalidades[primera_letra(lugar)].size(); i++) {
            if (std::get<0>(_preciolocalidades[primera_letra(lugar)][i]) == lugar) { return true; }
        }
        return false;
    }
    if (locacion == internacional) {
        for (int i = 0; i < _precioInternacional[primera_letra(lugar)].size(); i++) {
            if (std::get<0>(_precioInternacional[primera_letra(lugar)][i]) == lugar) { return true; }
        }
        return false;
    }
    return true;
}


void SistemaTelefonico::mandarFactura( cliente* usuario){
    cout << "Tenes que pagar " << usuario->montoAPagar()+_montoBasico;}


void SistemaTelefonico::nuevoCliente(tuple<string, int> usuario) {
    // Creo cliente y lo agrego a el diccionario de clientes y al vector de facturacion por dia del mes.
    cliente* nuevoCliente =  new cliente(std::get<1>(usuario),std::get<0>(usuario),_numeroActual);
    _clientes.insert(std::pair<int,cliente*>(std::get<1>(usuario),nuevoCliente));
    _clientesFechaFacturacion[_numeroActual-1].push_back(nuevoCliente);
}

void SistemaTelefonico::nuevoDia(){
    actualizarCalendario();
    for(int i = 0; i < _clientesFechaFacturacion[_numeroActual-1].size();i++){
        mandarFactura(_clientesFechaFacturacion[_numeroActual-1][i]);
        borrarLlamadas(_clientesFechaFacturacion[_numeroActual-1][i]);
    }
}


void SistemaTelefonico::nuevaLlamada(int documento,destino ubicacion, semana dia ,int hs, int min, int duracion){

    // Chequeo que todo los datos sean correctos.
    if(correctoDatosLlamada(documento,ubicacion, "Localidad",dia ,hs, min,duracion)){

        auto it = _clientes.find(documento);
        // Le agrego la llamada.

        agregarLlamadaAcliente((it->second), ubicacion,0.10,dia,hs,min,duracion);
    }
}
void SistemaTelefonico::nuevaLlamada(int documento,destino ubicacion, string lugar ,semana dia ,int hs, int min, int duracion){

    // Chequeo que todo los datos sean correctos.
    if(correctoDatosLlamada(documento,ubicacion,lugar,dia ,hs, min,duracion)){
        auto it = _clientes.find(documento);
        // Le agrego la llamada.
        agregarLlamadaAcliente((it->second), ubicacion,costoMinuto(ubicacion, lugar),dia,hs,min,duracion);
    }

}


int SistemaTelefonico::primera_letra(string lugar) {
    if(lugar.empty()){return -1;}
    return (int)lugar.at(0) - ASCII_OFFSET;
}


SistemaTelefonico::~SistemaTelefonico(){
    borrarUsuarios();
}


SistemaTelefonico::SistemaTelefonico(int ano, mes mes, int numero, semana dia, double montoBasico){
    int datosCorrectos = correctaInicializacion(mes,numero,dia);
    // Si esta todo bien, prosigo.
    if(datosCorrectos == 0){
        _diaSemana = dia;
        _numeroActual = numero;
        _mesActual = mes;
        _montoBasico = montoBasico;
        _anoActual = ano;
    }
    else{ throw std::invalid_argument("Argumentos invalidos");}
}


