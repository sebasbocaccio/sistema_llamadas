
* Para guardar la informacion de las personas, pude haberlo implementado con un trie. De hecho, esto genera beneficios con respecto a la complejidad temporal para la busqueda y agregado si hay muchos usuarios. Sin embargo, por temas del tiempo para realizar este trabajo decidi utilizar la estructura de datos map de std. Se puede cambiar la estructura de datos para generar un improvement sin tanta dificultad. 

* Asumo que todos los meses tienen 30 dias. Si se necesitase cambiar esto ya que no refleja el contexto de uso real no seria un gran problema ya que podria hacerse un array de 31 elementos y para los meses que no llegan a esa fecha se mandan las facturas de los dias faltantes en el ultimo dia del mes. Por ejemplo, si una persona deberia recibir su factura el dia 31 pero estamos en Abril, le llegaria el dia 30. Con respecto a febrero y cuando es bisiesto y cuando no, tampoco seria gran problema pues alcanzaria con un *if* para saber si tiene 28 o 29 dias ese mes. 

* Las llamadas **NO** pueden durar mas de 24 hs. Esta asuncion tiene que ver mas con poder justificar que la complejidad de calcular el costo de una llamada es O(1) que con la correctitud del programa. Si bien el sistema soporta sin problemas que las llamadas duren tiempo no acotado pero finito; me parece que esta asuncion puede ser razonable ya que creo que dificilmente una llamada dure mas de ese tiempo. En caso de que las llamadas duren tiempo no acotado, la funcion de costoLlamada es O(|cantidadHorasLlamada|)

* Asumo que los costos de llamadas Internacionales y Nacionales (no regionales) tienen un costo no dependiente de el dia y horario.

* Asumo que no existen paises ni regiones que empiecen con ñ. Si de verdad es necesario puede hacer un caso especial para saber si es o no pero no me parecio pertinente. Si de verdad se necesitase se hace un caso especial cuando el primer caracter vale 164 ( ñ en ASCII)

* Asumo que con los DNI me alcanza para distinguir univocamente a todos los usuarios. Podria fallar si tengo de varios paises. Podria solucionarse si ademas de dni tengo el campo Nacionalidad como clave del diccionario quedando como clave <Pais,Documento>. Nuevamente, creo que eso haria la resolucion mas engorrosa innecesariamente para este ejercicio.

* Asumo que la data de nuevaLlamada() puede haber llegado de forma tardia por lo que es necesaria la informacion de en que momento fue realizada. Por ejemplo, que un dia Miercoles me lleguen los datos de una llamada de un Lunes. Sin embargo, bajo estas hipotesis podria suceder que una llamada sea facturada en el mes siguiente en el que fue realizada. No se provee una solucion a este problema.

* Asumo que las llamadas duran una cantidad exactas de minutos y empiezan en el segundo 0. Osea, no puede durar 1.20 minutos una llamada y no puede una llamada empezar en el segundo 23 del minuto. 

* Asumo que el costo de las llamadas locales seran de .1 y .2 sin cambios. De todas formas, creando una variable de costo local usando get and set se soluciona rapido.
