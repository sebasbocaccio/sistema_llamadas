
#ifndef FACTURACIONTELEFONICA_SISTEMATELEFONICO_H
#define FACTURACIONTELEFONICA_SISTEMATELEFONICO_H
#include "vector"
#include "array"
#include <map>
using namespace std;
const int ASCII_OFFSET = 65;
const int COMIENZO_HORA_PICO = 8;
const int FIN_HORA_PICO = 20;

enum destino { local, nacional, internacional};
enum semana {Lunes,Martes,Miercoles,Jueves,Viernes,Sabado,Domingo};
enum mes{Enero,Febrero,Marzo,Abril,Mayo,Junio,Julio,Agosto,Septiembre,Octubre,Noviembre,Diciembre};

struct llamada {
    destino ubicacion;
    string lugar;
    semana dia;
    int hs;// A que hora empezo
    int min;
    int duracion; // Cuanto duro en minutos
} ;


struct cliente {
    int documento;
    string apellido;
    float importe;
    int diaFacturacion;
    vector<llamada> locales;
    vector<llamada> nacionales;
    vector<llamada> internaciones;
    int indiceVector;
} ;



class SistemaTelefonico {

    public:
    SistemaTelefonico(int ano, mes mes, int numero, semana dia, int montoBasico);
    ~SistemaTelefonico();

    void agregarLlamadaAcliente(cliente* emisor, destino ubicacion, string lugar,semana dia  ,int hs, int min, int duracion);
    void agregarNacion(string nombre, float costoMinuto);
    void agregarRegion(string nombre, float costoMinuto);
    void darDeBajaCliente(int documento);
    void mandarFactura( cliente* usuario);
    void nuevoCliente(tuple<string , int> usuario);
    void nuevaLlamada(int documento,destino ubicacion,string lugar, semana dia ,int hs, int min, int duracion);
    void nuevaLlamada(int documento,destino ubicacion, semana dia ,int hs, int min, int duracion);
    void nuevoDia();
    void cambiarDia();


    private:

    // Variables privadas

    int _anoActual;
    mes _mesActual;
    int _numeroActual;
    semana _diaSemana;
    float _montoBasico;
    float _precioCaro = 0.2f;
    float _precioBarato = 0.1f;

    // Guardo la informacion de los clientes en un diccionario usando como clave el documento.
    // (Usar un trie daria tiempo de acceso en O(1), tener en cuenta para futuras optimizaciones)
    map<int,cliente*> _clientes;

    // Guardo en una tabla de hash el nombre de la region junto al coste por minuto de llamada.
    vector<tuple<string,float>>  _preciolocalidades[25];
                                                                                                        // ( Supongo que no hay lugares con ñ por )
    // Guardo en una tabla de hash el nombre del pais junto al coste por minuto de llamada.
    vector<tuple<string,float>> _precioInternacional[25];

    // Aca voy a guardar punteros a la informacion de la gente. Esto sirve para que cuando para saber rapidamente
    // a que personas se les debe enviar factura cuando cambia de dia.
    vector<cliente*> _clientesFechaFacturacion[30];

    // Guardo cuantas personas hay en cada fecha. Su funcion es que cuando elimino una persona puedo saber cual es su indice en _clientesFechaFacturacion[dia] y poder
    // eliminarlo sin buscarlo.
    int _cantClientesForNumero[30] = {0} ;


    // Funciones privadas
    void actualizarCalendario();
    void agregarClienteAlSistema(tuple<string , int> usuario);
    void borrarUsuarios();
    void borrarLlamadas(cliente* usuario);
    float calcularCosto(destino ubicacion, string lugar, semana dia,int hs, int min, int duracion);
    float calcularCostoLocal(semana dia,int hs, int min, int duracion);
    bool clienteEnSistema(int dni);
    int correctaInicializacion( mes mes, int numero, semana dia); // Si no es correcta devuelve cual dato es incorrecto.
    bool correctoDatosLlamada(int documento,destino ubicacion,string lugar, semana dia ,int hs, int min, int duracion);
    bool estaEnSistema(destino locacion,string lugar);
    int primera_letra(string lugar);

};


#endif //FACTURACIONTELEFONICA_SISTEMATELEFONICO_H
