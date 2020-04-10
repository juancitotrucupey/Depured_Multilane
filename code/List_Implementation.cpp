#include <cmath>
//#include "Random64.h"
#include "List_Multilane.h"
//#include "Graph.h"
//#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>	
//Execute commands from the terminal
#include <cstdlib>
//Create and erase directories
//#include <sys/types.h>
//#include <sys/stat.h>
//Print an integer with a given width
//http://www.microhowto.info/howto/pad_an_integer_with_leading_zeros_in_c++.html
#include <iomanip>
#include <ios>   
//#include <sstream>

//using namespace std;


//______________________MAIN_________________________________________________________________
int main(int argc, char* argv[]){

  //Setting global variables initially
  bool verbose=true;
  int RandomSeed=1.0;
  Crandom Ran2(RandomSeed);
  int Simulation_Length=3000;
  int Boundary_Length=100;
  int ii,tt,tmax=7000;
  int Lanes_Amount=2;

  if(verbose){std::cout<<"Crear Avenida y Dinamica del sistema"<<std::endl;}
  Dinamica Evolution;
  Avenida Carrera_30;

  if(verbose){std::cout<<"Definir directorios para guardar los datos"<<std::endl;}  	
  std::string Data_Fundamental_Diagram,Data_Flux_Evolution,Data_Visualization,Data_Travel_Time;
 
  Data_Fundamental_Diagram="./../data/Fundamental_Diagram";
  Data_Flux_Evolution="./../data/Flux_Evolution";
  Data_Visualization="./../data/Visualization";
  Data_Travel_Time="./../data/Travel_Time";
	
  if(verbose){std::cout<<"Crear carros estaticos"<<std::endl;}
  Carro carro_vacio;
  Carro ** Carros_Estaticos;
  int * Cantidad_Carros_Estaticos;

  Carros_Estaticos=new Carro*[Lanes_Amount];
  for(int ii=0;ii<Lanes_Amount;ii++){
    Carros_Estaticos[ii]=new Carro[1];
    Carros_Estaticos[ii][0]=carro_vacio;
  }
  Cantidad_Carros_Estaticos=new int[Lanes_Amount];
  for(int ii=0;ii<Lanes_Amount;ii++){
    Cantidad_Carros_Estaticos[ii]=0;
  }

  if(verbose){std::cout<<"Set Propiedades de la Avenida y la Dinamica del sistema"<<std::endl;}
  //Avenida
  Carrera_30.Set_Self_Pointer(&Carrera_30);
  Carrera_30.Set_Iterators_To_Special_Cases();
  Carrera_30.Set_Fronteras_Periodicas(false);
  Carrera_30.Set_Vel_Max(7);
  Carrera_30.Fijar_Dimensiones(Lanes_Amount,Simulation_Length);
  Carrera_30.Crear_Fronteras(7,Boundary_Length);
  Carrera_30.Set_Pointers_Carriles();
  Carrera_30.Set_Fronteras_Periodicas(false);
  Carrera_30.Set_Iterators_To_Special_Cases();
  //Dinamica
  Evolution.Set_Carrera(&Carrera_30);
  Evolution.Set_MultiCarril(true);
  Evolution.Set_Amabilidad(0.8);
  Evolution.Set_Random_Gen(&Ran2);
  Evolution.Set_Ceder_El_Paso(true);
  Evolution.Set_Tipo_Cambio_Carril(2);
  Evolution.Set_Carros_Estaticos(Carros_Estaticos);
  Evolution.Set_Cantidad_Carros_Estaticos(Cantidad_Carros_Estaticos);

  //Set random positions for boundary lanes
  //if(verbose){std::cout<<"Set Posiciones Aleatorias"<<std::endl;}
  /*
  if(!Carrera_30.Get_Fronteras_Periodicas()){
    if(verbose){std::cout<<"	Carriles de las fronteras"<<std::endl;}
    Evolution.Set_Posiciones_Iniciales_Aleatorias(Frontera_Entrada,0.3);
    Evolution.Set_Posiciones_Iniciales_Aleatorias(Frontera_Salida,0.3);
    if(verbose){std::cout<<"	Evolucionar Carriles de las fronteras"<<std::endl;}
    Evolution.Avenida_Evolucionar(Frontera_Entrada,5000,false);
    Evolution.Avenida_Evolucionar(Frontera_Salida,5000,false);
  }
  */
  //if(verbose){std::cout<<"	Carriles zona de simulacion"<<std::endl;}
  //Evolution.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.25);
	

  //if(verbose){std::cout<<"Avenida Evolucion del Flujo"<<std::endl;}
  //Evolution.Avenida_Evolucion_Flujo(Simulacion,tmax,1,true,Data_Flux_Evolution);
	
  //if(verbose){std::cout<<"Visualizacion de los carros"<<std::endl;}
  //Evolution.Avenida_Visualizacion_Carros(Simulacion,tmax,5,true,Data_Visualization);

  if(verbose){std::cout<<"Diagrama Fundamental"<<std::endl;}
  for(double kindness=0.0;kindness<=1.0;kindness+=0.2){
    Evolution.Set_Amabilidad(kindness);
    Evolution.Avenida_Diagrama_Fundamental(Simulacion,tmax,0.2,0.8,0.2,2,true,Data_Fundamental_Diagram);
  }

  //Evolution.Avenida_Diagrama_Fundamental(Simulacion,tmax,0.01,0.8,0.02,10,true,Data_Fundamental_Diagram);
	
  //if(verbose){std::cout<<"Analisis Variables Auxiliares"<<std::endl;}
  //Evolution.Avenida_Analisis_Variables_Auxiliares(Simulacion,tmax,0.02,5,true,Data_Fundamental_Diagram);

  /*f(verbose){std::cout<<"Datos Tiempo de Viaje"<<std::endl;}
    double rho_inicial=0.2,rho_final=0.8,delta_rho=0.02;
    double kindness_inicial=0.0,kindness_final=1.0,delta_kindness=0.1;
    int cantidad_de_configuraciones=1;
    for(double rho=rho_inicial;rho<=rho_final;rho+=delta_rho){
    for(double kindness=kindness_inicial;kindness<=kindness_final;kindness+=delta_kindness){
    std::cout<<"Densidad "<<rho<<" kindness "<<kindness<<std::endl;
    Evolution.Tiempo_De_Viaje(Simulacion,tmax,cantidad_de_configuraciones,rho,kindness,verbose,Data_Travel_Time);
    }
    }*/
	

	
  if(verbose){std::cout<<"Eliminar pointers"<<std::endl;}
  for(int ii=0;ii<Lanes_Amount;ii++){
    delete[] Carros_Estaticos[ii];
  }
  delete[] Carros_Estaticos;

  delete[] Cantidad_Carros_Estaticos;  

  return 0;  
}
 

