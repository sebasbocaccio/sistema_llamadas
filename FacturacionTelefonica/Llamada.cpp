#include "Llamada.h"
/// LLAMADA

llamada::~llamada(){}
bool llamada::inputValido(int duracion, double costoXMinuto, int hs , int min){
    if( (hs > 23 || hs < 0) || (min > 59 || hs < 0)){
        return false ;
    }
    if( duracion <= 0){
        return false;
    }
    if(costoXMinuto < 0)  return false;

    return true;

}
double llamada::costo(){
    return 0;
};
/// LLAMADA LOCAL
//LlamadaLocal::~LlamadaLocal(){}
LlamadaLocal::LlamadaLocal(int duracion, int hs, int min,semana dia): llamada(duracion,0.10,hs,min,dia){}
bool LlamadaLocal::HoraPico(int duracion, int hs, int min,semana dia){

    if( (dia == Sabado || dia == Domingo)){
        return false;
    }
    else {
        if (hs >= COMIENZO_HORA_PICO && hs < FIN_HORA_PICO) { return true; }
        else { return false; }
    }
}
double  LlamadaLocal::costo(){
    double acumulado = 0;
    semana diaActual = _dia;
    int tiempoRestante = _duracion;

    // Aca si o si quiero que min se convierte en 0 para que todo sea mas facil.
    if((tiempoRestante >= (60-_min))) {
        if( (diaActual == Sabado || diaActual == Domingo)){
            acumulado += (60-_min)* _costoXMinuto;
        }
        else{
            if(_hs >= COMIENZO_HORA_PICO && _hs < FIN_HORA_PICO){acumulado+= (60-_min) * _costoXMinuto * 2 ;}
            else{acumulado+= (60-_min) * +_costoXMinuto;}  // No hora pico
        }
        tiempoRestante -= (60-_min);
        _hs++;
        _min = 0;
    }
    else{ // No llego a cambiar de hora..
        if( (diaActual == Sabado || diaActual == Domingo)){
            acumulado += tiempoRestante* _costoXMinuto;
        }
        else{
            if(_hs >= COMIENZO_HORA_PICO && _hs < FIN_HORA_PICO){acumulado+= tiempoRestante * _costoXMinuto * 2 ;}
            else{acumulado+= tiempoRestante * _costoXMinuto;}  // No hora pico
        }
        tiempoRestante = 0;
    }
    while(tiempoRestante >= 60){
        if(_hs == 24 ){
            _hs = 0;
            diaActual= static_cast<semana>((diaActual + 1) % 7);
        }
        if( (diaActual == Sabado || diaActual == Domingo)){
            acumulado += 60 * _costoXMinuto;
        }
        else{
            if( _hs >= COMIENZO_HORA_PICO && _hs < FIN_HORA_PICO){acumulado+= 60 * _costoXMinuto * 2;}
            else{acumulado+= 60 * _costoXMinuto;}  // No hora pico
        }
        _hs++ ;
        tiempoRestante-= 60;
    }

    if((diaActual == Sabado || diaActual == Domingo)){
        acumulado += tiempoRestante* _costoXMinuto;
    }
    else{
        if( _hs >= COMIENZO_HORA_PICO && _hs < FIN_HORA_PICO){acumulado+= tiempoRestante * _costoXMinuto*2;}
        else{acumulado+= tiempoRestante * _costoXMinuto;}  // No hora pico
    }
    return acumulado;
}
/// LLAMADA NACIONAL

//LlamadaNacional::~LlamadaNacional(){}
LlamadaNacional::LlamadaNacional(int duracion, double costoxMinuto,int hs, int min,semana dia): llamada(duracion,costoxMinuto,hs,min,dia){}
double LlamadaNacional::costo(){
    _costoXMinuto*_duracion;
}

/// LLAMADA INTERNACIONAL
//LlamadaInternacional::~LlamadaInternacional(){}
LlamadaInternacional::LlamadaInternacional(int duracion, double costoxMinuto,int hs, int min,semana dia): llamada(duracion,costoxMinuto,hs,min,dia){}
double LlamadaInternacional::costo(){
    _costoXMinuto*_duracion;
}
