
#ifndef FACTURACIONTELEFONICA_SISTEMATELEFONICO_H
#define FACTURACIONTELEFONICA_SISTEMATELEFONICO_H
#include "vector"
#include "array"
#include <map>
#include <iterator>
#include <algorithm>
#include "Llamada.h"
#include "cliente.h"
using namespace std;
const int ASCII_OFFSET = 65;


enum destino { local, nacional, internacional};
enum mes{Enero,Febrero,Marzo,Abril,Mayo,Junio,Julio,Agosto,Septiembre,Octubre,Noviembre,Diciembre};

class SistemaTelefonico {

    public:
    SistemaTelefonico(int ano, mes mes, int numero, semana dia, double montoBasico);
    ~SistemaTelefonico();

    void agregarLlamadaAcliente(cliente* emisor, destino ubicacion, double costoXMinuto,semana dia  ,int hs, int min, int duracion);
    void agregarNacion(string nombre, double costoMinuto);
    void agregarRegion(string nombre, double costoMinuto);
    void darDeBajaCliente(int documento);
    void mandarFactura( cliente* usuario);
    void nuevoCliente(tuple<string , int> usuario);
    void nuevaLlamada(int documento,destino ubicacion,string lugar, semana dia ,int hs, int min, int duracion);
    void nuevaLlamada(int documento,destino ubicacion, semana dia ,int hs, int min, int duracion);
    void nuevoDia();



    private:

    // Variables privadas
    int _anoActual;
    mes _mesActual;
    int _numeroActual;
    semana _diaSemana;
    double _montoBasico;


    // Guardo la informacion de los clientes en un diccionario usando como clave el documento.
    // (Usar un trie daria tiempo de acceso en O(1), tener en cuenta para futuras optimizaciones)
    map<int,cliente*> _clientes;

    // Guardo en una tabla de hash el nombre de la region junto al coste por minuto de llamada.
    vector<tuple<string,double>>  _preciolocalidades[25];
                                                                                                        // ( Supongo que no hay lugares con ñ por )
    // Guardo en una tabla de hash el nombre del pais junto al coste por minuto de llamada.
    vector<tuple<string,double>> _precioInternacional[25];

    // Aca voy a guardar punteros a la informacion de la gente. Esto sirve para que cuando para saber rapidamente
    // a que personas se les debe enviar factura cuando cambia de dia.
    vector<cliente*> _clientesFechaFacturacion[30];

    // Funciones privadas
    void actualizarCalendario();
    void agregarClienteAlSistema(tuple<string , int> usuario);
    void borrarUsuarios();
    void borrarLlamadas(cliente* usuario);
    bool clienteEnSistema(int dni);
    int correctaInicializacion( mes mes, int numero, semana dia); // Si no es correcta devuelve cual dato es incorrecto.
    bool correctoDatosLlamada(int documento,destino ubicacion,string lugar, semana dia ,int hs, int min, int duracion);
    double costoMinuto(destino locacion, string lugar);
    bool estaEnSistema(destino locacion,string lugar);
    int primera_letra(string lugar);
};


#endif //FACTURACIONTELEFONICA_SISTEMATELEFONICO_H
