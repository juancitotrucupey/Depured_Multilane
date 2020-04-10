//Tabla completa de velocidades y gaps
/*
  velocity 0:gapfrenado=0;gapaceleracion=3;retardo=1;break;
  velocity 1:gapfrenado=3;gapaceleracion=4;retardo=1;break;
  velocity 2:gapfrenado=3;gapaceleracion=5;retardo=1;break;
  velocity 3:gapfrenado=4;gapaceleracion=5;retardo=1;break;
  velocity 4:gapfrenado=5;gapaceleracion=6;retardo=2;break;
  velocity 5:gapfrenado=6;gapaceleracion=7;retardo=2;break;
  velocity 6:gapfrenado=6;gapaceleracion=8;retardo=2;break;
  velocity 7:gapfrenado=7;gapaceleracion=9;retardo=2;break;
  velocity 8:gapfrenado=8;gapaceleracion=10;retardo=3;break;
  velocity 9:gapfrenado=8;gapaceleracion=11;retardo=3;break;
  velocity 10:gapfrenado=10;gapaceleracion=13;retardo=3;break;
  velocity 11:gapfrenado=10;gapaceleracion=13;retardo=3;break;
  velocity 12:gapfrenado=11;gapaceleracion=13;retardo=3;break;
  velocity 13:gapfrenado=11;gapaceleracion=13;retardo=3;break;
  velocity 14:gapfrenado=12;gapaceleracion=13;retardo=3;break;
  velocity 15:gapfrenado=13;gapaceleracion=14;retardo=3;break;
*/
/*
  Convenciones:
  1. Las variables internas de las clases empiezan con mayusculas
  2. Las variables mudas i.e. varibles en las declaraciones de funciones son palabras en minusculas
*/  
// g++ List_Multilane.h List_Carril.cpp List_Avenida.cpp List_Carro.cpp Dinamica.cpp List_Implementation.cpp -g -o 3.x 

#ifndef __TRANSPORTE_H__
#define __TRANSPORTE_H__

#include <iostream>
#include <cmath>
#include <list>  //std::list
#include <vector>  //std::vector
#include <iterator>  //std::iterator
#include <algorithm> //std::sort

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>



enum Tipo_Carril{Simulacion,Frontera_Entrada,Frontera_Salida};
enum Posicion_Carril{Sin_Vecinos,Izquierdo,Central,Derecho};

//Declaracion de las clases
class Crandom;
class Carro;
class Carro_Sencillo;
class Carril;
class Carril_Generador;
class Avenida;
class Dinamica;

//____________________________Implementación de las clases_________________________________________

//_________________________Generador de numeros aletorios_________________________________________
class Crandom{
  unsigned long long u,v,w;
 public:
  Crandom(unsigned long long j);
  unsigned long long int64();
  double r() {return 5.42101086242752217E-20 * int64();}
  unsigned int int32(){return (unsigned int) int64();};
  double exponencial(float tau);
  double gauss(float mu,float sigma);
};

//____________________________________Clase Carro__________________________________________________

class Carro{
 private:
  //Iteradores a los primeros vecinos
  std::list<Carro>::iterator Carro_Adelante;
  std::list<Carro>::iterator Carro_Atras;
  std::list<Carro>::iterator Carro_Der_Adelante;
  std::list<Carro>::iterator Carro_Izq_Adelante;
  std::list<Carro>::iterator Carro_Der_Atras;
  std::list<Carro>::iterator Carro_Izq_Atras;

  //Iteradores a los carros a los cuales les da la via
  std::list<Carro>::iterator Carro_Ceder_El_Paso_Derecha;
  std::list<Carro>::iterator Carro_Ceder_El_Paso_Izquierda;

  //Variables que determinan el estado del carro
  int VELMAX;
  int Velocity,Delay,Position;
  bool Brake;
  bool Estacionarias;//Determina si el carro está detenido
  int Tiempo_Estacionarias;//Determina por cuánto tiempo va a estár detenido el carro
  int Id;//Número de identificación del carro
  int Tiempo_Simulacion;//Contador del tiempo en la zona de simulacion
  int Inset_Lane;//Carril de entrada
  int Ouset_Lane;//Carril de salida
  bool Stoping_Safely;//Determina si el carro se va a detener

  //Varibles que determinan el estado de los primeros vecinos
  int Estacionarias_Ahead;
  bool Brake_Ahead;
  bool Brake_Back;
  bool Delay_Back;
  bool Brake_Izq_Ahead;
  bool Brake_Izq_Back;
  bool Brake_Der_Ahead;
  bool Brake_Der_Back;

  //Gaps con los primeros vecinos
  int Gap_Ahead,Gap_Position_Ahead,Gap_Velocity_Ahead;//Carril donde se mueve, carro adelante
  int Gap_Back,Gap_Position_Back,Gap_Velocity_Back;//Carril donde se mueve, carro atras
  int Gap_Izq_Ahead,Gap_Position_Izq_Ahead,Gap_Velocity_Izq_Ahead;//Carril izquierdo, carro adelante
  int Gap_Izq_Back,Gap_Position_Izq_Back,Gap_Velocity_Izq_Back;//Carril izquierdo, carro atras
  int Gap_Der_Ahead,Gap_Position_Der_Ahead,Gap_Velocity_Der_Ahead;//Carril derecho, carro adelante
  int Gap_Der_Back,Gap_Position_Der_Back,Gap_Velocity_Der_Back;//Carril derecho, carro atras
        
  //Indicadores de la interaccion multicarril
  bool Change_Lines_Izq;//Booleano que indica si el carro cambiará al carril izquierdo
  bool Change_Lines_Der;//Booleano que indica si el carro cambiará al carril derecho 
  bool Changed_Lines_Izq;//Booleano que indica si el carro cambió al carril izquierdo
  bool Changed_Lines_Der;//Booleano que indica si el carro cambió al carril derecho
  bool Direccional_Derecha;//Booleano que indica si el carro prendió las direccionales derechas
  bool Direccional_Izquierda;//Booleano que indica si el carro prendió las direccionales izquierdas
  bool Ceder_Paso_Derecha;//Booleano que indica si el carro cede el paso al carro en el carril derecho
  bool Ceder_Paso_Izquierda;//Booleano que indica si el carro cede el paso al carro en el carril izquierdo

  //Pointers al carril al que pertenece y a la avenida a la que pertenece
  Carril * Line;
  Avenida * Carrera;

 public:
  //Constructor of the class
  Carro(void){
    VELMAX=7;
    Line=NULL;
    Carrera=NULL;
  }
  //_________________________________Funciones carga___________________________________________________
  //Variables de estado del carro
  void Set_Vel_Max(int vel_max){VELMAX=vel_max;};
  void Set_Id(int id_1){Id=id_1;};
  void Set_Velocity(int vel){Velocity=vel;};
  void Set_Brake(bool bra){Brake=bra;};
  void Set_Delay(int del){Delay=del;};
  void Set_Position(int pos){Position=pos;};
  void Set_Estacionarias(bool sta){Estacionarias=sta;};
  void Set_Tiempo_Estacionarias(int time_sta){Tiempo_Estacionarias=time_sta;};
  void Set_Tiempo_Simulacion(int time_sim){Tiempo_Simulacion=time_sim;};
  void Set_Inset_Lane(int lane){Inset_Lane=lane;};
  void Set_Outset_Lane(int lane){Ouset_Lane=lane;};
  void Set_Stoping_Safely(bool stop){Stoping_Safely=stop;};
  //Varibles de estado de los primeros vecinos
  void Set_Estacionarias_Ahead(int sta){Estacionarias_Ahead=sta;};
  void Set_Brake_Ahead(bool brake){Brake_Ahead=brake;};
  void Set_Brake_Back(bool brake){Brake_Back=brake;};
  void Set_Delay_Back(int delay){Delay_Back=delay;};
  void Set_Brake_Izq_Ahead(bool brake){Brake_Izq_Ahead=brake;};
  void Set_Brake_Izq_Back(bool brake){Brake_Izq_Back=brake;};
  void Set_Brake_Der_Ahead(bool brake){Brake_Der_Ahead=brake;};
  void Set_Brake_Der_Back(bool brake){Brake_Der_Back=brake;};
  //Interaccion multicarril
  void Set_Change_Lines_Izq(bool c_l){Change_Lines_Izq=c_l;};
  void Set_Change_Lines_Der(bool c_l){Change_Lines_Der=c_l;};
  void Set_Changed_Lines_Der(bool c_l){Changed_Lines_Der=c_l;};
  void Set_Changed_Lines_Izq(bool c_l){Changed_Lines_Izq=c_l;};
  void Set_Direccional_Derecha(bool dir_der){Direccional_Derecha=dir_der;};
  void Set_Direccional_Izquierda(bool dir_izq){Direccional_Izquierda=dir_izq;};
  void Set_Ceder_Paso_Derecha(bool ceder_paso){Ceder_Paso_Derecha=ceder_paso;};
  void Set_Ceder_Paso_Izquierda(bool ceder_paso){Ceder_Paso_Izquierda=ceder_paso;};
  //Iteradores a los primeros vecinos
  void Set_Carro_Adelante(std::list<Carro>::iterator car){Carro_Adelante=car;};
  void Set_Carro_Atras(std::list<Carro>::iterator car){Carro_Atras=car;};
  void Set_Carro_Der_Adelante(std::list<Carro>::iterator car){Carro_Der_Adelante=car;};
  void Set_Carro_Izq_Adelante(std::list<Carro>::iterator car){Carro_Izq_Adelante=car;};
  void Set_Carro_Der_Atras(std::list<Carro>::iterator car){Carro_Der_Atras=car;};
  void Set_Carro_Izq_Atras(std::list<Carro>::iterator car){Carro_Izq_Atras=car;};
  //Iteradores a los carros a los cuales les da la via
  void Set_Carro_Ceder_El_Paso_Derecha(std::list<Carro>::iterator car);
  void Set_Carro_Ceder_El_Paso_Izquierda(std::list<Carro>::iterator car);
  //Set pointers al carril al que pertenece y a la avenida a la que pertenece
  void Set_Pointer_Carril(Carril * line){Line=line;};
  void Set_Pointer_Avenida(Avenida * carrera){Carrera=carrera;};
        
  //_____________________________Imprimir informacion_________________________________________________
  //Variables de estado del carro
  int Get_Velocity(void) const {return Velocity;};
  bool Get_Brake(void) const {return Brake;};
  int Get_Delay(void) const {return Delay;};
  int Get_Position(void) const {return Position;};
  bool Get_Estacionarias(void) const {return Estacionarias;};
  int Get_Tiempo_Estacionarias(void) const {return Tiempo_Estacionarias;};
  int Get_Vel_Max(void) const {return VELMAX;};
  int Get_Id(void) const {return Id;};
  int Get_Tiempo_Simulacion(void) const {return Tiempo_Simulacion;};
  int Get_Inset_Lane(void) const {return Inset_Lane;};
  int Get_Outset_Lane(void) const {return Ouset_Lane;};
  bool Get_Stoping_Safely(void) const {return Stoping_Safely;};
  //Interaccion multicarril
  bool Get_Change_Lines_Izq(void) const {return Change_Lines_Izq;};
  bool Get_Change_Lines_Der(void) const {return Change_Lines_Der;};
  bool Get_Changed_Lines_Izq(void) const {return Changed_Lines_Izq;};
  bool Get_Changed_Lines_Der(void) const {return Changed_Lines_Der;};
  bool Get_Direccional_Derecha(void) const {return Direccional_Derecha;};
  bool Get_Direccional_Izquierda(void) const {return Direccional_Izquierda;};
  bool Get_Ceder_Paso_Derecha(void) const {return Ceder_Paso_Derecha;};
  bool Get_Ceder_Paso_Izquierda(void) const {return Ceder_Paso_Izquierda;};
  //Gaps con los primeros vecinos
  int Get_Gap_Posicion_Adelante(void) const {return Gap_Position_Ahead;};
  int Get_Gap_Velocidad_Adelante(void) const {return Gap_Velocity_Ahead;};
  int Get_Gap_Total_Adelante(void) const {return Gap_Ahead;};
  bool Get_Brake_Ahead(void) const {return Brake_Ahead;};
  int Get_Gap_Posicion_Atras(void) const {return Gap_Position_Back;};
  int Get_Gap_Velocidad_Atras(void) const {return Gap_Velocity_Back;};
  int Get_Gap_Total_Atras(void) const {return Gap_Back;};
  int Get_Gap_Posicion_Der_Adelante(void) const {return Gap_Position_Der_Ahead;};
  int Get_Gap_Velocidad_Der_Adelante(void) const {return Gap_Velocity_Der_Ahead;};
  int Get_Gap_Total_Der_Adelante(void) const {return Gap_Der_Ahead;};
  int Get_Gap_Posicion_Der_Atras(void) const {return Gap_Position_Der_Back;};
  int Get_Gap_Velocidad_Der_Atras(void) const {return Gap_Velocity_Der_Back;};
  int Get_Gap_Total_Der_Atras(void) const {return Gap_Der_Back;};
  int Get_Gap_Posicion_Izq_Adelante(void) const {return Gap_Position_Izq_Ahead;};
  int Get_Gap_Velocidad_Izq_Adelante(void) const {return Gap_Velocity_Izq_Ahead;};
  int Get_Gap_Total_Izq_Adelante(void) const {return Gap_Izq_Ahead;};
  int Get_Gap_Posicion_Izq_Atras(void) const {return Gap_Position_Izq_Back;};
  int Get_Gap_Velocidad_Izq_Atras(void) const {return Gap_Velocity_Izq_Back;};
  int Get_Gap_Total_Izq_Atras(void) const {return Gap_Izq_Back;};
  //Iteradores a los primeros vecinos
  std::list<Carro>::iterator Get_Carro_Adelante(void){return Carro_Adelante;};
  std::list<Carro>::iterator Get_Carro_Atras(void){return Carro_Atras;};
  std::list<Carro>::iterator Get_Carro_Der_Adelante(void){return Carro_Der_Adelante;};
  std::list<Carro>::iterator Get_Carro_Izq_Adelante(void){return Carro_Izq_Adelante;};
  std::list<Carro>::iterator Get_Carro_Der_Atras(void){return Carro_Der_Atras;};
  std::list<Carro>::iterator Get_Carro_Izq_Atras(void){return Carro_Izq_Atras;};
  //Iteradores a los carros a los cuales les da la via
  std::list<Carro>::iterator Set_Carro_Ceder_El_Paso_Derecha(void){return Carro_Ceder_El_Paso_Derecha;};
  std::list<Carro>::iterator Set_Carro_Ceder_El_Paso_Izquierda(void){return Carro_Ceder_El_Paso_Izquierda;};
  //Get pointers al carril al que pertenece y a la avenida a la que pertenece
  Carril * Get_Pointer_Carril(void){return Line;};
  Avenida * Get_Pointer_Avenida(void){return Carrera;};

  //____________________Funciones para la evolucion de las variables internas_____________________________
  void Calcule_Gap_Adelante(int position_adelante, int velocity_adelante);
  void Calcule_Gap_Atras(int position_atras, int velocity_atras);
  void Calcule_Gap_Der_Adelante(int position_der_adelante, int velocity_der_adelante);
  void Calcule_Gap_Der_Atras(int position_der_atras, int velocity_der_atras);
  void Calcule_Gap_Izq_Adelante(int position_izq_adelante, int velocity_izq_adelante);
  void Calcule_Gap_Izq_Atras(int position_izq_atras, int velocity_izq_atras);
  bool Is_Safe_To_Change_Lines_Izq_Ahead();
  bool Is_Safe_To_Change_Lines_Izq_Back();
  bool Is_Safe_To_Change_Lines_Der_Ahead();
  bool Is_Safe_To_Change_Lines_Der_Back();
  bool Is_Worth_To_Ask_For_Way_Izquierda();
  bool Is_Worth_To_Ask_For_Way_Derecha();
  void Decide_Change_Lines_And_Direccionales(int tipo,bool direccionales,Crandom & ran2);
  bool Is_Safe_To_Give_Way_Right(int & vel_after_giving_way);
  bool Is_Safe_To_Give_Way_Left(int & vel_after_giving_way);
  void Decide_Ceder_El_Paso(Crandom & ran2, double amabilidad);
  void Safe_Stop(int& vel, int& delay, bool& brake, bool& stoping_safely);
  void Evolucion_Olmos_Munoz(int& vel, int& delay, bool& brake, int gap, bool brake_adelante);
  void Calcular_Nuevo_Estado(int& vel, int& delay, bool& brake, bool& estacionarias, int& tiempo_estacionarias, bool& stoping_safely);
  void Evolucionar_Variables_Internas(void);
  void Reinicie_Interaccion_Multicarril(void);
  void Modificar_Tiempo_Estacionarias(int tiempo_est){Tiempo_Estacionarias+=tiempo_est;};
  void Modificar_Tiempo_Simulacion(int tiempo_sim){Tiempo_Simulacion+=tiempo_sim;};
};


//___________________________________________Carro_Sencillo______________________________________________
//_____Clase carro creada para dinamica de un solo carril y ser usado en las zonas generadoras________________

class Carro_Sencillo{
 private:
  //Iteradores a los primeros vecinos
  std::list<Carro_Sencillo>::iterator Carro_Adelante;
  std::list<Carro_Sencillo>::iterator Carro_Atras;
        
  //Variables de estado    
  int VELMAX;
  int Velocity,Delay,Position;
  bool Brake;
  int Id;

  //Variables que determinan el estado de los primeros vecinos
  bool Brake_Ahead;
  bool Brake_Back;
  //Gaps con los primeros vecinos
  int Gap_Ahead,Gap_Position_Ahead,Gap_Velocity_Ahead;
  int Gap_Back,Gap_Position_Back,Gap_Velocity_Back;

  //Pointers al carril al que pertenece y a la avenida a la que pertenece
  Carril_Generador * Line;
  Avenida * Carrera;
     
 public:
  //Constructor for the class
  Carro_Sencillo(void){
    VELMAX=7;
    Line=NULL;
    Carrera=NULL;
  }
  //________________________________Funciones carga______________________________________________
  //Variables de estado
  void Set_Vel_Max(int vel_max){VELMAX=vel_max;};
  void Set_Id(int id_1){Id=id_1;};
  void Set_Velocity(int vel){Velocity=vel;};
  void Set_Brake(bool bra){Brake=bra;};
  void Set_Delay(int del){Delay=del;};
  void Set_Position(int pos){Position=pos;};
  //Variables de estado de los primeros vecinos
  void Set_Brake_Ahead(bool brake){Brake_Ahead=brake;};
  void Set_Brake_Back(bool brake){Brake_Back=brake;};
  //Iteradores a los primeros vecinos
  void Set_Carro_Adelante(std::list<Carro_Sencillo>::iterator car){Carro_Adelante=car;};
  void Set_Carro_Atras(std::list<Carro_Sencillo>::iterator car){Carro_Atras=car;};
  //Set pointers al carril al que pertenece y a la avenida a la que pertenece
  void Set_Pointer_Carril(Carril_Generador * line){Line=line;};
  void Set_Pointer_Avenida(Avenida * carrera){Carrera=carrera;};
        
  //________________________Imprimir informacion_________________________________________________
  //Variables de estado
  int Get_Velocity(void) const {return Velocity;};
  bool Get_Brake(void) const {return Brake;};
  int Get_Delay(void) const {return Delay;};
  int Get_Position(void) const {return Position;};
  int Get_Vel_Max(void) const {return VELMAX;};
  int Get_Id(void) const {return Id;};
  //Gaps con los primeros vecinos
  int Get_Gap_Posicion_Adelante(void) const {return Gap_Position_Ahead;};
  int Get_Gap_Velocidad_Adelante(void) const {return Gap_Velocity_Ahead;};
  int Get_Gap_Total_Adelante(void) const {return Gap_Ahead;};
  bool Get_Brake_Ahead(void) const {return Brake_Ahead;};
  int Get_Gap_Posicion_Atras(void) const {return Gap_Position_Back;};
  int Get_Gap_Velocidad_Atras(void) const {return Gap_Velocity_Back;};
  int Get_Gap_Total_Atras(void) const {return Gap_Back;};
  bool Get_Brake_Back(void) const {return Brake_Back;};
  //Iteradores a los primeros vecinos
  std::list<Carro_Sencillo>::iterator Get_Carro_Adelante(void){return Carro_Adelante;};
  std::list<Carro_Sencillo>::iterator Get_Carro_Atras(void){return Carro_Atras;};
  //Get pointers al carril al que pertenece y a la avenida a la que pertenece
  Carril_Generador * Get_Pointer_Carril(void){return Line;};
  Avenida * Get_Pointer_Avenida(void){return Carrera;};
        
  //____________________Funciones para la evolucion de las variables internas____________________________
  void Calcule_Gap_Adelante(int position_adelante, int velocity_adelante);
  void Calcule_Gap_Atras(int position_atras, int velocity_atras);
  void Evolucion_Olmos_Munoz(int& vel, int& delay, bool& brake, int gap, bool brake_adelante, bool tipo_juan );
  void Calcular_Nuevo_Estado(int& vel, int& delay, bool& brake);
  void Evolucionar_Variables_Internas(void);
};


//_________________________________________________________________________________________
//______________________________________________________Carril_____________________________

class Carril{
  friend class Avenida;
 private:
  //Contenedor para los carros
  std::list<Carro> Carril_1;
  //Contenedor de los carros que salen de la via
  std::list<Carro> Carros_Que_Salen;
  //Variables de estado
  int Largo;
  int Largo_Entrada;
  int Largo_Salida;
  int Solapamiento_Con_Frontera_Entrada;
  int VELMAX;
  int Cantidad_Maxima_Carros;//=(int)(Largo/3);
  bool Fronteras_Periodicas;//true==fronteras periodicas, false==fronteras abiertas
  int Numero_De_Carril;
  Posicion_Carril Posicion;
  std::list<Carro>::iterator Primer_Carro;
  std::list<Carro>::iterator Ultimo_Carro;

  //Iterador a los carriles vecinos
  std::vector<Carril>::iterator Carril_Vecino_Derecho;
  std::vector<Carril>::iterator Carril_Vecino_Izquierdo;

  //Iterador a los carriles que determinan las condiciones de frontera
  std::vector<Carril_Generador>::iterator Carril_Frontera_Entrada;
  std::vector<Carril_Generador>::iterator Carril_Frontera_Salida;

  //Pointer a la instancia de la clase
  Carril * Line;
  //Pointer a la avenida a la cual pertenece
  Avenida * Carrera;
        
 public:
  //Constructor of the class
  Carril(void){
    Largo_Entrada=20;
    Largo_Salida=20;
    Solapamiento_Con_Frontera_Entrada=20;
    VELMAX=7;
    Fronteras_Periodicas=true;
    Line=NULL;
    Carrera=NULL;
    Posicion=Sin_Vecinos;
  }
  //Tablas de evolución
  //Velunity=10km/h, tstep=0.99s  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
  const int Gap_Frenado_Carro[16]={0,3,3,4,5,6,6,7,8,8,10,10,11,11,12,13};
  const int Gap_Aceleracion_Carro[16]={2,4,5,5,6,7,8,9,10,11,13,13,13,13,13,14};
  const int Retardo_Carro[16]={1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3};

  //Safe brake
  const int Safe_Exceding_Distance[16]={0,0,0,0,0,0,1,1,1,2,2,2,2,3,3,3};
  const int Safe_Braking_Distance[16]={0,1,2,5,8,12,18,24,31,39,48,58,69,81,94,108};
  const int Safe_Minimum_Next_Velocity[16]={0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
        
  //int * Gap_Frenado_Carro;
  //Elementos de los casos especiales
  std::list<Carro> Carril_Lateral_Extremo;
  std::list<Carro> Carril_Vacio;
  std::list<Carro> Condiciones_De_Frontera_Entrada;
  std::list<Carro> Condiciones_De_Frontera_Salida;
  std::list<Carro>::iterator No_Existe_Carril_Vecino; 
  std::list<Carro>::iterator Carril_Vecino_Vacio; 
  std::list<Carro>::iterator Carro_Frontera_Entrada;
  std::list<Carro>::iterator Carro_Frontera_Salida;

  //__________________Fijar propiedades__________________________________________________
  void Set_Largo(int largo){Largo=largo; Cantidad_Maxima_Carros=(int)(Largo/3.0);}; 
  void Set_Vel_Max(int velmax){VELMAX=velmax;};
  void Set_Fronteras_Periodicas(bool fronteras){Fronteras_Periodicas=fronteras;};
  void Set_Numero_De_Carril(int num){Numero_De_Carril=num;};
  void Set_Posicion(Posicion_Carril pos){Posicion=pos;};
  void Set_Self_Pointer(Carril * carril){Line=carril;};
  void Set_Pointer_Avenida(Avenida * carrera){Carrera=carrera;};
  //Set iterators a los casos especiales    
  void Set_Iterators_To_Special_Cases();
  //Fijar pointers de los carros 
  void Set_Pointer_Carril_Carros();
  void Set_Pointer_Avenida_Carros();
  //__________________Organizar los carros del carril__________________________________
  static bool Comparar_Posiciones(const Carro & carro1, const Carro & carro2){return (carro1.Get_Position()<carro2.Get_Position());};
  void Organizar_Carril(void);
  void Rearrange_Positions(void);
  //____________________Posiciones iniciales_____________________________________________
  void Posiciones_Iniciales(Carro * carros, int cantidad_carros, int& contador_id);
  void Posiciones_Iniciales_Aleatorias(int cantidad_carros, Crandom & ran2, int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(int cantidad_carros, Crandom & ran2, int& contador_id, Carro * carros_estaticos, int cantidad_carros_estaticos);
  //___________________Dinamica del sistema______________________________________________
  void Fijar_Carros_Condiciones_De_Frontera(void);
  void Intercambiar_Informacion_Y_Calcular_Gaps(void);
  void Decide_Change_Lines_And_Direccionales(int tipo,bool direccionales,Crandom & ran2);
  void Decide_Ceder_El_Paso(Crandom & ran2,double amabilidad);
  void Evolucionar_Variables_Internas_Carros(void);
  void Mover_Carros(void);
  void Interaccion_Con_Carriles_Frontera(void);
  //__________________Funciones para modificar el sistema________________________________
  //Globales
  void Borrar_Carros(void);
  void Borrar_Carros_Que_Salen(void);
  void Set_Cars_Time_Stationary(int time_sta);
  void Set_Cars_Inset_Lane(int inset_lane);
  void Set_Cars_Outset_Lane(int outset_lane);
  void Set_Cars_Estacionarias(bool estacionarias);
  //Locales
  std::list<Carro>::iterator Agregar_Carro(std::list<Carro>::iterator lugar, Carro carro_agregado);
  std::list<Carro>::iterator Eliminar_Carro(std::list<Carro>::iterator lugar);
  bool Stop_Car(int position,int time_estacionarias);
  //________________Imprimir informacion del sistema_____________________________________
  //Locales
  bool Verificar_Casilla_Vacia(int casilla,std::list<Carro>::iterator & lugar_carro);
  std::list<Carro>::iterator Ultimo_Carro_Antes_De(int posicion, std::list<Carro>::iterator carro_inicial);
  int Distancia_Entre_Carros(int posicion1, int posicion2);
  int Posicion_Real_En_Carril(int posicion);
  bool Carros_Se_Chocan(int pos_1_before, int pos_1_after, int pos_2_before, int pos_2_after);
  //Globales
  double Get_Flujo_Espacial(void);
  double Get_Rho(bool simulacion);
  int Get_Cantidad_Carros(bool simulacion);
  int Get_Vel_Max(void){return VELMAX;};
  int Get_Largo(void){return Largo;};
  int Get_Max_Carros(void){return Cantidad_Maxima_Carros;};
  Posicion_Carril Get_Posicion_Carril(void){return Posicion;};
  bool Get_Fronteras_Periodicas(void){return Fronteras_Periodicas;};
  int Get_Numero_De_Carril(void){return Numero_De_Carril;};
  std::list<Carro> Get_Info_Carros_Que_Salen(void);
  bool Cars_Have_Stoped(int punto_inicial,int punto_final);
  //Pointers
  Avenida * Get_Pointer_Avenida(void){return Carrera;};
  Carril * Get_Self_Pointer(void){return Line;};
};


//_______________________________________________________________________________________________________________
//____________________________________________Carril_Generador_______________________________________

class Carril_Generador{
  friend class Avenida;
  friend class Carril;
 private:
  //Contenedor para los carros
  std::list<Carro_Sencillo> Carril_1;
        
  //Varibles de estado
  int Largo;
  int VELMAX;
  int Cantidad_Maxima_Carros;//=(int)(Largo/3);
  std::list<Carro_Sencillo>::iterator Primer_Carro;
  std::list<Carro_Sencillo>::iterator Ultimo_Carro;
  std::list<Carro_Sencillo>::iterator Carro_Condiciones_De_Frontera_Salida;

  //Pointer a la instancia de la clase
  Carril_Generador * Line;
  //Pointer a la avenida a la que pertenece
  Avenida * Carrera;
        
 public:
  Carril_Generador(void){
    VELMAX=7;
    Line=NULL;
    Carrera=NULL;
  }
  //Tablas de evolución
  //Velunity=10km/h, tstep=0.99s
  const int Gap_Frenado_Carro[16]={0,3,3,4,5,6,6,7,8,8,10,10,11,11,12,13};
  const int Gap_Aceleracion_Carro[16]={3,4,5,5,6,7,8,9,10,11,13,13,13,13,13,14};
  const int Retardo_Carro[16]={1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3};

  //Elementos de los casos especiales
  std::list<Carro_Sencillo> Carril_Sin_Carros;
  std::list<Carro_Sencillo>::iterator Carril_Generador_Vacio;
        
  //__________________Fijar propiedades________________________________________________
  void Set_Largo(int largo){Largo=largo; Cantidad_Maxima_Carros=(int)(Largo/3);}; 
  void Set_Vel_Max(int velmax){VELMAX=velmax;};
  void Set_Self_Pointer(Carril_Generador * carril){Line=carril;};
  void Set_Pointer_Avenida(Avenida * carrera){Carrera=carrera;};
  //Set iterators a los casos especiales
  void Set_Iterators_To_Special_Cases();
  //Fijar pointers de los carros 
  void Set_Pointer_Carril_Carros();
  void Set_Pointer_Avenida_Carros();
  //_________________Organizar los carros del carril___________________________________
  static bool Comparar_Posiciones(const Carro_Sencillo & carro1, const Carro_Sencillo & carro2){return (carro1.Get_Position()<carro2.Get_Position());};
  void Organizar_Carril(void);

  //________________Posiciones inciales_________________________________________________
  void Posiciones_Iniciales(Carro_Sencillo * carros, int cantidad_carros, int& contador_id);
  void Posiciones_Iniciales_Aleatorias(int cantidad_carros, Crandom & ran2, int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(int cantidad_carros, Crandom & ran2, int& contador_id, Carro_Sencillo * carros_estaticos, int cantidad_carros_estaticos);

  //_______________Dinamica del sistema__________________________________________________
  void Intercambiar_Informacion_Y_Calcular_Gaps(void);
  void Evolucionar_Variables_Internas_Carros(void);
  void Mover_Carros(void);

  //______________Funciones para modificar el sistema____________________________________
  //Globales
  void Borrar_Carros(void);
        
  //Locales
  std::list<Carro_Sencillo>::iterator Agregar_Carro(std::list<Carro_Sencillo>::iterator lugar, Carro_Sencillo carro_agregado);
  std::list<Carro_Sencillo>::iterator Eliminar_Carro(std::list<Carro_Sencillo>::iterator lugar);

  //______________Imprimir informacion del sistema_______________________________________
  //Locales
  int Distancia_Entre_Carros(int posicion1, int posicion2);
  bool Verificar_Casilla_Vacia(int casilla,std::list<Carro_Sencillo>::iterator & anterior_carro);
  std::list<Carro_Sencillo>::iterator Ultimo_Carro_Antes_De(int posicion, std::list<Carro_Sencillo>::iterator carro_inicial);
  int Posicion_Real_En_Carril(int posicion);
  bool Carros_Se_Chocan(int pos_1_before, int pos_1_after, int pos_2_before, int pos_2_after);
  //Globales
  double Get_Flujo_Espacial(void);
  double Get_Rho(bool simulacion); 
  int Get_Cantidad_Carros(void){return Carril_1.size();};
  int Get_Vel_Max(void){return VELMAX;};
  int Get_Largo(void){return Largo;};
  int Get_Max_Carros(void){return Cantidad_Maxima_Carros;};
  //Pointers
  Avenida * Get_Pointer_Avenida(void){return Carrera;};
  Carril_Generador * Get_Self_Pointer(void){return Line;};
};


//______________________________Interaccion entre carriles______________________________________________
//______________________________________________________________________________________________________

class Avenida{
 private:
  //Contenedores de los carriles 
  std::vector<Carril> Avenida_1;//Zona de simulacion
  std::vector<Carril_Generador> Carriles_Frontera_Entrada;//Fronteras abiertas, carriles de entrada
  std::vector<Carril_Generador> Carriles_Frontera_Salida;//Fronteras abiertas, carriles de salida

  //Variables de estado
  int VELMAX;
  bool Fronteras_Periodicas;//true==fronteras periodicas, false==fronteras abiertas

  //Lista de los Id disponibles
  std::vector<int> Id_Disponibles; 

  //Pointer a la instancia de la clase
  Avenida * Carrera;
    
 public:
  Avenida(void){
    VELMAX=7;
    Fronteras_Periodicas=true;
    Carrera=NULL;
  }
  //Elementos de los casos especiales
  std::vector<Carril> None_Carril_Vecino;
  std::vector<Carril>::iterator No_Existe_Carril_Vecino;
        
  //______________________Fijar propiedades_______________________________________________
  void Set_Vel_Max(int vel_max);
  void Set_Fronteras_Periodicas(bool fronteras);
  void Set_Largo(int largo);
  void Set_Self_Pointer(Avenida * carrera){Carrera=carrera;};
  void Fijar_Dimensiones(int cantidad_carriles, int largo);
  bool Crear_Fronteras(int vel_max, int largo);
  //Set iterators a los casos especiales
  void Set_Iterators_To_Special_Cases();
  //Fijar pointers de los carros y pointers de los carriles
  void Set_Self_Pointer_Carriles();
  void Set_Pointer_To_Avenida_Carriles();
  void Set_Pointers_Carriles();
  void Set_Pointers_To_Carril_Carros();
  void Set_Pointers_To_Avenida_Carros();
  void Set_Pointers_Carros();
  //______________________Posiciones iniciales____________________________________________
  //Carriles de la zona de simulacion
  void Posiciones_Iniciales_Carriles_Simulacion(Carro ** carros, int cantidad_carros[], int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Carriles_Simulacion(int cantidad_carros[], Crandom & ran2, int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Simulacion(int cantidad_carros[], Crandom & ran2, Carro ** carros_estaticos, int cantidad_carros_estaticos[],int& contador_id);
  //Carriles para fijar las condiciones de frontera
  //Entrada
  void Posiciones_Iniciales_Carriles_Frontera_Entrada(Carro_Sencillo ** carros, int cantidad_carros[], int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Carriles_Frontera_Entrada(int cantidad_carros[], Crandom & ran2, int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Frontera_Entrada(int cantidad_carros[], Crandom & ran2, Carro_Sencillo ** carros_estaticos, int cantidad_carros_estaticos[], int& contador_id);
  //Salida
  void Posiciones_Iniciales_Carriles_Frontera_Salida(Carro_Sencillo ** carros, int cantidad_carros[], int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Carriles_Frontera_Salida(int cantidad_carros[], Crandom & ran2, int& contador_id);
  void Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Frontera_Salida(int cantidad_carros[], Crandom & ran2, Carro_Sencillo ** carros_estaticos, int cantidad_carros_estaticos[], int& contador_id);
        
  //______________________Dinamica del sistema____________________________________________ 
  //Carriles de la zona de simulacion
  void Encontrar_Primeros_Vecinos_Carriles_Simulacion(bool multicarril,bool condiciones_frontera);
  void Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Simulacion(bool multicarril);
  void Decide_Change_Lines_And_Direccionales_Carriles_Simulacion(int tipo,bool direccionales,Crandom & ran2);
  void Decide_Ceder_El_Paso_Carriles_Simulacion(Crandom & ran2,double amabilidad);
  void Decide_Change_Lines_After_Asking_For_Way_Carriles_Simulacion(int tipo,Crandom & ran2);
  void Cambio_De_Carril_Carriles_Simulacion(void);
  void Evolucionar_Variables_Internas_Carros_Carriles_Simulacion(void);
  void Mover_Carros_Carriles_Simulacion(void);
  //Carriles para fijar las condiciones de frontera
  //Entrada
  void Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Frontera_Entrada(void);
  void Evolucionar_Variables_Internas_Carros_Carriles_Frontera_Entrada(void);
  void Mover_Carros_Carriles_Frontera_Entrada(void);
  //Salida
  void Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Frontera_Salida(void);
  void Evolucionar_Variables_Internas_Carros_Carriles_Frontera_Salida(void);
  void Mover_Carros_Carriles_Frontera_Salida(void);
  //Funciones generalizadas para la dinamica del sistema
  void Encontrar_Primeros_Vecinos(bool multicarril,bool condiciones_frontera,Tipo_Carril tipo_carril);
  void Intercambiar_Informacion_Y_Calcular_Gaps(bool multicarril,Tipo_Carril tipo_carril);
  void Decide_Change_Lines_And_Direccionales(int tipo,bool direccionales,Tipo_Carril tipo_carrill,Crandom & ran2);
  void Decide_Ceder_El_Paso(Crandom & ran2,double amabilidad,Tipo_Carril tipo_carril);
  void Decide_Change_Lines_After_Asking_For_Way(int tipo,Tipo_Carril tipo_carril,Crandom & ran2);
  void Cambio_De_Carril(Tipo_Carril tipo_carril);
  void Evolucionar_Variables_Internas_Carros(Tipo_Carril tipo_carril);
  void Mover_Carros(Tipo_Carril tipo_carril);
  void Interaccion_Con_Carriles_Frontera(void);

  //_________________Funciones para modificar el sistema___________________________________
  //Globales
  void Borrar_Carros_Carriles_Simulacion(void);
  void Borrar_Carros_Carriles_Frontera_Entrada(void);
  void Borrar_Carros_Carriles_Frontera_Salida(void);
  void Borrar_Carros(Tipo_Carril tipo_carril);
  void Borrar_Carriles_Simulacion(void){Avenida_1.clear();};
  void Borrar_Carriles_Frontera_Entrada(void){Carriles_Frontera_Entrada.clear();};
  void Borrar_Carriles_Frontera_Salida(void){Carriles_Frontera_Salida.clear();};
  void Borrar_Carriles(Tipo_Carril tipo_carril);
  void Set_Cars_Time_Stationary(int time_sta);//Los carros estaticos permanecen en este estado tmax pasos
  void Set_Cars_Inset_Lane(int inset_lane);
  //Locales
  bool Stop_Car(int position,int time_estacionarias,int numero_carril);
  //_________________Imprimir informacion del sistema______________________________________
  //Avenida
  int Get_Vel_Max(void){return VELMAX;};
  bool Get_Fronteras_Periodicas(void){return Fronteras_Periodicas;};
  Avenida * Get_Self_Pointer(void){return Carrera;};
        
  int Get_Cantidad_Carriles_Simulacion(void){return Avenida_1.size();};
  int Get_Cantidad_Carriles_Frontera_Entrada(void){return Carriles_Frontera_Entrada.size();};
  int Get_Cantidad_Carriles_Frontera_Salida(void){return Carriles_Frontera_Salida.size();};
  int Get_Cantidad_Carriles(Tipo_Carril tipo_carril);
        
  int Get_Cantidad_Carros_Carriles_Simulacion(void);
  int Get_Cantidad_Carros_Carriles_Frontera_Entrada(void);
  int Get_Cantidad_Carros_Carriles_Frontera_Salida(void);
  int Get_Cantidad_Carros(Tipo_Carril tipo_carril);
        
  int Get_Largo_Carriles_Simulacion(void);
  int Get_Largo_Carriles_Frontera_Entrada(void);
  int Get_Largo_Carriles_Frontera_Salida(void);
  int Get_Largo(Tipo_Carril tipo_carril);
        
  double Get_Rho_Carriles_Simulacion(bool simulacion);
  double Get_Rho_Carriles_Frontera_Entrada(bool simulacion);
  double Get_Rho_Carriles_Frontera_Salida(bool simulacion);
  double Get_Rho(Tipo_Carril tipo_carril,bool simulacion);

  double Get_Flujo_Espacial_Carriles_Simulacion(void);
  double Get_Flujo_Espacial_Carriles_Frontera_Entrada(void);
  double Get_Flujo_Espacial_Carriles_Frontera_Salida(void);
  double Get_Flujo_Espacial(Tipo_Carril tipo_carril);

  std::list<Carro> Get_Info_Carros_Que_Salen(Tipo_Carril tipo_carril);
  bool Cars_Have_Stoped(Tipo_Carril tipo_carril,int punto_inicial,int punto_final);
        
  int Get_Id_For_Car(void);
  void Add_Id_For_Cars(int id);
  //Carriles 
  int Get_Max_Carros(Tipo_Carril tipo_carril);
  double Get_Flujo_Espacial_Carril(Tipo_Carril tipo_carril,int carr);
  double Get_Rho_Carril(Tipo_Carril tipo_carril,int carr,bool simulacion);
  std::list<Carro> Get_Info_Carros_Que_Salen_Por_Carril(Tipo_Carril tipo_carril,int carr);
  //Carros
  void Carril_Ocupado_Carros(Tipo_Carril tipo_carril,std::vector<int> & carril);
  void Id_Carros(Tipo_Carril tipo_carril,std::vector<int> & id);
  void Posiciones_Carros(Tipo_Carril tipo_carril,std::vector<int> & pos);
  void Velocidades_Carros(Tipo_Carril tipo_carril,std::vector<int> & vel);
  void Brake_Carros(Tipo_Carril tipo_carril,std::vector<int> & brake);
  void Direccionales_Izquierda_Carros(Tipo_Carril tipo_carril,std::vector<int> & dir);
  void Direccionales_Derecha_Carros(Tipo_Carril tipo_carril,std::vector<int> & dir);
  void Ceder_El_Paso_Izquierda_Carros(Tipo_Carril tipo_carril,std::vector<int> & ceder_el_paso);
  void Ceder_El_Paso_Derecha_Carros(Tipo_Carril tipo_carril,std::vector<int> & ceder_el_paso);
  void Cambio_Carril_Izquierda_Carros(Tipo_Carril tipo_carril,std::vector<int> & cambio_carril);
  void Cambio_Carril_Derecha_Carros(Tipo_Carril tipo_carril,std::vector<int> & cambio_carril);
  void Estacionarias_Carros(Tipo_Carril tipo_carril,std::vector<int> & estacionarias);
  void Stoping_Safely_Carros(Tipo_Carril tipo_carril,std::vector<int> & stoping_safely);
  void Inset_Lane_Carros(Tipo_Carril tipo_carril,std::vector<int> & inset_lane);
  void Distancia_Al_Siguiente_Carro(Tipo_Carril tipo_carril,std::vector<int> & distancia);
  void Razones_De_Velocidades(Tipo_Carril tipo_carril,std::vector<double> & razones);
  void Velocidad_Vs_Distancia_Al_Carro_Siguiente(Tipo_Carril tipo_carril,std::vector<double> & razones);
};


//______________________________Dinamica del sistemas multicarril_______________________________________
//______________________________________________________________________________________________________

class Dinamica{
 private:
  Avenida * Carrera;
  bool MultiCarril;
  double Amabilidad;
  Crandom * Random_Gen;
  bool Ceder_El_Paso;
  int Tipo_Cambio_Carril;
  Carro ** Carros_Estaticos;
  int * Cantidad_Carros_Estaticos;

 public:
  //________________Fijar_Informacion_Del_Sistema______________________________
  void Set_Carrera(Avenida * avenida){Carrera=avenida;};
  void Set_MultiCarril(bool multi){MultiCarril=multi;};
  void Set_Amabilidad(double kindness){Amabilidad=kindness;};
  void Set_Random_Gen(Crandom * ran){Random_Gen=ran;};
  void Set_Ceder_El_Paso(bool dar_paso){Ceder_El_Paso=dar_paso;};
  void Set_Tipo_Cambio_Carril(int tipo_cambio){Tipo_Cambio_Carril=tipo_cambio;};
  void Set_Carros_Estaticos(Carro ** carros){Carros_Estaticos=carros;};
  void Set_Cantidad_Carros_Estaticos(int * cantidad_carros){Cantidad_Carros_Estaticos=cantidad_carros;};
  //______________Inicializar_Carros___________________________________________
  void Set_Posiciones_Iniciales_Aleatorias(Tipo_Carril tipo_carril,double rho);
  //______________Imprimir_Informacion_Del_Sistema_____________________________
  Avenida * Get_Carrera(void){return Carrera;};
  bool Get_MultiCarril(void){return MultiCarril;};
  double Get_Amabilidad(void){return Amabilidad;};
  Crandom * Get_Random_Gen(void){return Random_Gen;};
  bool Get_Ceder_El_Paso(void){return Ceder_El_Paso;};
  int Get_Tipo_Cambio_Carril(void){return Tipo_Cambio_Carril;};
  Carro ** Get_Carros_Estaticos(void){return Carros_Estaticos;};
  int * Get_Cantidad_Carros_Estaticos(void){return Cantidad_Carros_Estaticos;};
  //_______________Dinamica______________________________________________________
  void Avenida_Evolucionar_Variables_Carros(Tipo_Carril tipo_carril,bool verbose);
  void Avenida_Mover_Carros(Tipo_Carril tipo_carril,bool verbose);
  void Avenida_Evolucionar(Tipo_Carril tipo_carril, int tmax,bool verbose);
  bool Avenida_Detener_Carro(Tipo_Carril tipo_carril,double rho,int carril,int posicion,int position_delta,int time_stoped,bool verbose);
  //______________Observables____________________________________________________
  void Avenida_Diagrama_Fundamental(Tipo_Carril tipo_carril,int tmax,double rho_inicial,double rho_final,double delta_rho,int configuraciones,bool verbose,std::string folder);
  //______________Visualizacion__________________________________________________
  void Avenida_Visualizacion_Carros(Tipo_Carril tipo_carril,int max_time,int num_particiones,bool verbose,std::string folder);
  //______________Evolucion_Flujo________________________________________________
  void Avenida_Evolucion_Flujo(Tipo_Carril tipo_carril,int max_time,int paso,bool verbose,std::string folder);
  //______________Histograma_Tiempo_Recorrido____________________________________
  void Avenida_Histograma_Tiempo_Recorrido_Carriles_Simulacion(int max_time,bool verbose,std::string folder);
  //______________Analisis_Variables_Auxiliares_____________________________________
  void Avenida_Analisis_Variables_Auxiliares(Tipo_Carril tipo_carril,int tmax,double paso,int configuraciones,bool verbose,std::string folder);
  //______________Tiempo_De_Viaje___________________________________________________
  void Tiempo_De_Viaje(Tipo_Carril tipo_carril,int tmax,int configuraciones,double rho,double kindness,bool verbose,std::string folder);
};

#endif





