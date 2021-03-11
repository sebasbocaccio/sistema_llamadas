
#include <iostream>
#include "SistemaTelefonico.h"
#include <stdlib.h>
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
void SistemaTelefonico::agregarClienteAlSistema(tuple<string, int> usuario) {
    // Creo cliente y lo agrego a el diccionario de clientes y al vector de facturacion por dia del mes.
    vector<llamada> llamadasLocales;
    vector<llamada> llamadasNacionales;
    vector<llamada> llamadasInternaciones;
    cliente* nuevoCliente =  new cliente{
            std::get<1>(usuario), //Documento
            std::get<0>(usuario), // Apellido
            0,
            _numeroActual,
            llamadasLocales,
            llamadasNacionales,
            llamadasInternaciones,
            _cantClientesForNumero[_diaSemana]
    };
    _clientes.insert(std::pair<int,cliente*>(std::get<1>(usuario),nuevoCliente));
    _clientesFechaFacturacion[_numeroActual-1].push_back(nuevoCliente);
    _cantClientesForNumero[_numeroActual-1]++;
}

void SistemaTelefonico::agregarLlamadaAcliente(cliente* usuario, destino ubicacion, string lugar ,semana dia ,int hs, int min, int duracion){

    llamada nuevaLlamada {
            ubicacion,
            lugar,
            dia,
            hs,
            min,
            duracion
    };
    if(ubicacion == nacional){
        usuario->nacionales.push_back(nuevaLlamada);

    }
    else if(ubicacion == internacional){
        usuario->internaciones.push_back(nuevaLlamada);

    }
    else{
        usuario->locales.push_back(nuevaLlamada);
    }
}
void SistemaTelefonico::agregarNacion(string nombre, float costoMinuto){
    int index_letra = primera_letra(nombre);
    if( index_letra > 25 || index_letra < 0){ cerr << "Porfavor ingresar un pais valido. Recordar que tiene que comenzar con mayuscula y no aceptamos la ñ.\n";}
    else if(!estaEnSistema(internacional,nombre)){
        _precioInternacional[index_letra].push_back(std::pair<string,float>(nombre,costoMinuto));
    }
    else {cerr << "El pais ya se encuentra en la base de datos. Si desea actualizar el monto hagalo con la funcion adecuada.\n";}
}

void SistemaTelefonico::agregarRegion(string nombre, float costoMinuto){
    int index_letra = primera_letra(nombre);

    if( index_letra > 25 || index_letra < 0){ cerr << "Porfavor ingresar un lugar valido. Recordar que tiene que comenzar con mayuscula y no aceptamos la ñ.\n";}
    else{
        _preciolocalidades[index_letra].push_back( std::pair<string,int>(nombre,costoMinuto) );
    }
}
void SistemaTelefonico::borrarLlamadas(cliente* usuario){
    usuario->nacionales.clear();
    usuario->locales.clear();
    usuario->internaciones.clear();
}
void SistemaTelefonico::borrarUsuarios(){
    for (std::map<int,cliente*>::iterator it=_clientes.begin(); it!=_clientes.end(); ++it)
        delete it->second;

}
float SistemaTelefonico::calcularCosto(destino ubicacion, string lugar,semana dia,int hs, int min, int duracion){

    float costoMinuto;
    if(ubicacion == local){
        // * El calculo este es bastaaante engorroso. Tome una filosofia de 'keep it simple' para : 1) Saber que funciona 2) Sea entendible. Sin embargo, esta probablemente no sea la forma mas optima.
        // * Decidi hacer una funcion auxiliar para calcularlo para hacer mas legible esta funcion.
        float costo = calcularCostoLocal(dia,hs,min,duracion);
        cout << costo << "\n";
        return costo;
    }
    else if(ubicacion == nacional){
        for (int i = 0; i < _preciolocalidades[primera_letra(lugar)].size(); i++){
            if(std::get<0>(_preciolocalidades[primera_letra(lugar)][i]) == lugar){float costoMinuto = std::get<1>(_preciolocalidades[primera_letra(lugar)][i]);}
        }
        return costoMinuto*duracion;
    }
    else{
        for (int i = 0; i < _precioInternacional[primera_letra(lugar)].size(); i++){
            if(std::get<0>(_precioInternacional[primera_letra(lugar)][i]) == lugar){float costoMinuto = std::get<1>(_precioInternacional[primera_letra(lugar)][i]);}
        }
        return costoMinuto*duracion;
    }
}
float SistemaTelefonico::calcularCostoLocal(semana dia,int hs, int min, int duracion){
    float acumulado = 0;
    semana diaActual = dia;
    int tiempoRestante = duracion;

    // Aca si o si quiero que min se convierte en 0 para que todo sea mas facil.
    if((tiempoRestante >= (60-min))) {
        if( (diaActual == Sabado || diaActual == Domingo)){
            acumulado += (60-min)* _precioBarato;
        }
        else{
            if(hs >= COMIENZO_HORA_PICO && hs < FIN_HORA_PICO){acumulado+= (60-min) * _precioCaro;}
            else{acumulado+= (60-min) * _precioBarato;}  // No hora pico
        }
        tiempoRestante -= (60-min);
        hs++;
        min = 0;
    }
    else{ // No llego a cambiar de hora..
        if( (diaActual == Sabado || diaActual == Domingo)){
            acumulado += tiempoRestante* _precioBarato;
        }
        else{
            if(hs >= COMIENZO_HORA_PICO && hs < FIN_HORA_PICO){acumulado+= tiempoRestante * _precioCaro;}
            else{acumulado+= tiempoRestante * _precioBarato;}  // No hora pico
        }
        tiempoRestante = 0;
    }
    while(tiempoRestante >= 60){
        if(hs == 24 ){
            hs = 0;
            diaActual= static_cast<semana>((diaActual + 1) % 7);
        }
        if( (diaActual == Sabado || diaActual == Domingo)){
            acumulado += 60 * _precioBarato;
        }
        else{
            if( hs >= COMIENZO_HORA_PICO && hs < FIN_HORA_PICO){acumulado+= 60 * _precioCaro;}
            else{acumulado+= 60 * _precioBarato;}  // No hora pico
        }
        hs++ ;
        tiempoRestante-= 60;
    }
    // Necesarimente aca tiene menos de 60 mins restantes, pero puede o no los mins ser 0.

    // Note que puede pasar que el dia sea incorrecto porque no pregunto devuelta si cambio de dia. Sin embargo, esto no da un resultado incorrecto
    // ya que la hora 24 == 0 nunca es horario pico por lo que el precio es el mismo siempre.
    // Deberia cambiarse en el caso en que la tarifa modifique el monto segun dia de la semana estrictamente.

    if((diaActual == Sabado || diaActual == Domingo)){
        acumulado += tiempoRestante* _precioBarato;
    }
    else{
        if( hs >= COMIENZO_HORA_PICO && hs < FIN_HORA_PICO){acumulado+= tiempoRestante * _precioCaro;}
        else{acumulado+= tiempoRestante * _precioBarato;}  // No hora pico
    }
    return acumulado;
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
    if(!estaEnSistema(ubicacion,lugar)){return false;}
    if( (hs > 23 || hs < 0) || (min > 59 || hs < 0)){
        cerr << "En este sistema solo permitimos horarios entre 00:00 y 23:59. Porfavor ingrese un horario valido. \n";
        return false ;
    }
    if( duracion <= 0){ cerr << "En este sistema solo permitimos llamadas de tiempo positivo. \n";
        return false;
    }
    return true;

}
void SistemaTelefonico::cambiarDia(){
    _numeroActual = 1;
}
void SistemaTelefonico::darDeBajaCliente(int documento) {
    // Para agregar un cliente al sistema, no puede estar inscripto anteriormente en el sistema.
    if(!clienteEnSistema(documento)){
        { cerr << "La persona con documento " << documento << " no esta en el sistema. La operacion no fue realizada \n";}
    }
    else{
        auto it = _clientes.find(documento);
        _clientesFechaFacturacion[it->second->diaFacturacion-1].erase(_clientesFechaFacturacion[it->second->diaFacturacion-1].begin() +  (it->second->indiceVector) );
        int indice =it->second->indiceVector;

        _cantClientesForNumero[it->second->diaFacturacion-1]--;
        // Libero la memoria que pedi
        _clientes.erase(documento);
    }
    return;
}
bool SistemaTelefonico::estaEnSistema(destino locacion, string lugar){

    if(locacion == nacional){
        for (int i = 0; i < _preciolocalidades[primera_letra(lugar)].size(); i++){
            if(std::get<0>(_preciolocalidades[primera_letra(lugar)][i]) == lugar){return true;}
        }
        return false;
    }
    if(locacion == internacional){
        for (int i = 0; i < _precioInternacional[primera_letra(lugar)].size(); i++){
            if(std::get<0>(_precioInternacional[primera_letra(lugar)][i]) == lugar){return true;}
        }
        return false;
    }
    return true;
}
void SistemaTelefonico::mandarFactura( cliente* usuario){
   cout << usuario->apellido << " le mandamos su factura del mes " << _mesActual+1 << " del año "<< _anoActual << " \n" "\n";
   cout << "El importe a pagar es de " << _montoBasico << "(monto basico) + " << usuario->importe << " = " << _montoBasico+usuario->importe << "\n \n";
    cout << "A continuacion le detallamos las llamadas realizadas por categoria  \n\n ";
    cout << "Locales: \n";
    for(int i = 0 ; i < usuario->locales.size();i++){
        cout << "Llamada hecha a las  " << usuario->locales[i].hs<<":"<< usuario->locales[i].min << " de duracion " << usuario->locales[i].duracion << " minutos \n " ;
    }
    cout << "Nacionales: \n";
    for(int i = 0 ; i < usuario->nacionales.size();i++){
        cout << "Llamada hecha a las  " << usuario->locales[i].hs<<":"<< usuario->locales[i].min << " a la region " << usuario->locales[i].ubicacion  << " de duracion " << usuario->locales[i].duracion << " minutos \n " ;
    }
    cout << "Internacionales: \n";
    for(int i = 0 ; i < usuario->nacionales.size();i++){
        cout << "Llamada hecha a las  " << usuario->locales[i].hs<<":"<< usuario->locales[i].min << " al pais " << usuario->locales[i].ubicacion  << " de duracion " << usuario->locales[i].duracion << " minutos \n " ;
    }
}


void SistemaTelefonico::nuevoCliente(tuple<string , int> usuario) {

    // Para agregar un cliente al sistema, no puede estar inscripto anteriormente en el sistema.
    if( clienteEnSistema(std::get<1>(usuario) )){
        { cerr << "La persona " << std::get<0>(usuario) << " con documento " << std::get<1>(usuario) << " Ya esta en el sistema. La operacion no fue realizada \n";}
    }
    else{
        agregarClienteAlSistema(usuario);
    }
    return;

}

void SistemaTelefonico::nuevaLlamada(int documento,destino ubicacion, semana dia ,int hs, int min, int duracion){

    // Chequeo que todo los datos sean correctos.
    correctoDatosLlamada(documento,ubicacion, "Localidad",dia ,hs, min,duracion);
    // Calcular costo
    float costoLlamada = calcularCosto(ubicacion,"Localidad",dia,hs,min,duracion);

    // Actualizar saldo a pagar.
    auto it = _clientes.find(documento);
    it->second->importe += costoLlamada;

    // Le agrego la llamada.
    agregarLlamadaAcliente((it->second), ubicacion,"Localidad",dia,hs,min,duracion);

}
void SistemaTelefonico::nuevaLlamada(int documento,destino ubicacion, string lugar, semana dia  ,int hs, int min, int duracion) {

    // Chequeo que todo los datos sean correctos.
    correctoDatosLlamada(documento,ubicacion,lugar,dia ,hs, min,duracion);

    // Calcular costo
    float costoLlamada = calcularCosto(ubicacion,lugar,dia,hs,min,duracion);

    // Actualizar saldo a pagar.
    auto it = _clientes.find(documento);
    it->second->importe += costoLlamada;

    // Le agrego la llamada.
   agregarLlamadaAcliente((it->second), ubicacion,lugar,dia,hs,min,duracion);
}

void SistemaTelefonico::nuevoDia(){

    actualizarCalendario();

    for(int i = 0; i < _clientesFechaFacturacion[_numeroActual-1].size();i++){
        mandarFactura(_clientesFechaFacturacion[_numeroActual-1][i]);
        borrarLlamadas(_clientesFechaFacturacion[_numeroActual-1][i]);
    }

    //    cliente i. limpiarle llamadas
}

SistemaTelefonico::SistemaTelefonico(int ano, mes mes, int numero, semana dia ,int montoBasico){
    // Chequeo que me pasen bien el input
    int datosCorrectos = correctaInicializacion(mes,numero,dia);

    // Si esta todo bien, prosigo.
    if(datosCorrectos == 0){
        _diaSemana = dia;
        _numeroActual = numero;
        _mesActual = mes;
        _montoBasico = montoBasico;
        _anoActual = ano;
    }
        // No puedo permitir que me inicialicen mal la estructura.
    else{
        if(datosCorrectos == 1){ cerr << "Pasaste un mes incorrecto. Porfavor insertar un mes valido  .\n";}
        else if(datosCorrectos ==2){ cerr << "Pasaste un numero incorrecto. Porfavor insertar un dia valido. Recordar que en el sistema todos los dias tienen 30 dias.\n"; }
        else{ cerr << "Pasaste un dia de la semana incorrecto. Porfavor insertar un dia valido.\n";}

        exit (EXIT_FAILURE);
    }
}

SistemaTelefonico::~SistemaTelefonico(){
    borrarUsuarios();
}

int SistemaTelefonico::primera_letra(string lugar) {
    if(lugar.empty()){return -1;}

    return (int)lugar.at(0) - ASCII_OFFSET;
}
