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

	bool verbose=true;
  	int RandomSeed=1.0;
  	Crandom Ran2(RandomSeed);
 	int Largo_Simulacion=3000;
 	int Largo_Fronteras=10;
  	int ii,tt,tmax=7000;
  	int Cantidad_Carriles=2;

  	if(verbose){std::cout<<"Crear Avenida y Dinamica del sistema"<<std::endl;}
  	Dinamica Evolucion;
	Avenida Carrera_30;

	if(verbose){std::cout<<"Definir directorios para guardar los datos"<<std::endl;}  	
  	std::string Datos_Diagrama_Fundamental,Datos_Evolucion_Flujo,Datos_Visualizacion_Carros,Datos_Tiempo_De_Viaje;
  	std::string Test_Diagrama_Fundamental,Test_Evolucion_Flujo,Test_Visualizacion_Carros,Test_Tiempo_De_Viaje;

  	Datos_Diagrama_Fundamental="./../Datos/Diagrama_Fundamental";
  	Test_Diagrama_Fundamental="./../Datos/Test_Diagrama_Fundamental";
  	Datos_Evolucion_Flujo="./../Datos/Evolucion_Flujo";
  	Test_Evolucion_Flujo="./../Datos/Test_Evolucion_Flujo";
  	Datos_Visualizacion_Carros="./../Datos/Visualizacion";
  	Test_Visualizacion_Carros="./../Datos/Test_Visualizacion";
  	Datos_Tiempo_De_Viaje="./../Datos/Tiempo_De_Viaje";
  	Test_Tiempo_De_Viaje="./../Datos/Test_Tiempo_De_Viaje";
	
	if(verbose){std::cout<<"Crear carros estaticos"<<std::endl;}
	Carro carro_vacio;
  	Carro ** Carros_Estaticos;
	int * Cantidad_Carros_Estaticos;

	Carros_Estaticos=new Carro*[Cantidad_Carriles];
	for(int ii=0;ii<Cantidad_Carriles;ii++){
		Carros_Estaticos[ii]=new Carro[1];
		Carros_Estaticos[ii][0]=carro_vacio;
	}
	Cantidad_Carros_Estaticos=new int[Cantidad_Carriles];
	for(int ii=0;ii<Cantidad_Carriles;ii++){
		Cantidad_Carros_Estaticos[ii]=0;
	}

	if(verbose){std::cout<<"Set Propiedades de la Avenida y la Dinamica del sistema"<<std::endl;}
	//Avenida
	Carrera_30.Set_Self_Pointer(&Carrera_30);
	Carrera_30.Set_Iterators_To_Special_Cases();
  	Carrera_30.Set_Fronteras_Periodicas(true);
  	Carrera_30.Set_Vel_Max(7);
  	Carrera_30.Fijar_Dimensiones(Cantidad_Carriles,Largo_Simulacion);
  	Carrera_30.Crear_Fronteras(7,Largo_Fronteras);
  	Carrera_30.Set_Pointers_Carriles();
  	Carrera_30.Set_Fronteras_Periodicas(true);
  	Carrera_30.Set_Iterators_To_Special_Cases();
	//Dinamica
	Evolucion.Set_Carrera(&Carrera_30);
	Evolucion.Set_MultiCarril(true);
	Evolucion.Set_Amabilidad(0.8);
	Evolucion.Set_Random_Gen(&Ran2);
	Evolucion.Set_Ceder_El_Paso(true);
	Evolucion.Set_Tipo_Cambio_Carril(2);
	Evolucion.Set_Carros_Estaticos(Carros_Estaticos);
	Evolucion.Set_Cantidad_Carros_Estaticos(Cantidad_Carros_Estaticos);

	/*if(verbose){std::cout<<"Set Posiciones Aleatorias"<<std::endl;}
	if(!Carrera_30.Get_Fronteras_Periodicas()){
	if(verbose){std::cout<<"	Carriles de las fronteras"<<std::endl;}
		Evolucion.Set_Posiciones_Iniciales_Aleatorias(Frontera_Entrada,0.3);
		Evolucion.Set_Posiciones_Iniciales_Aleatorias(Frontera_Salida,0.3);
		if(verbose){std::cout<<"	Evolucionar Carriles de las fronteras"<<std::endl;}
		Evolucion.Avenida_Evolucionar(Frontera_Entrada,5000,false);
		Evolucion.Avenida_Evolucionar(Frontera_Salida,5000,false);
	}*/
	
	//if(verbose){std::cout<<"	Carriles zona de simulacion"<<std::endl;}
	//Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.25);
	

	//if(verbose){std::cout<<"Avenida Evolucion del Flujo"<<std::endl;}
	//Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,true,Datos_Evolucion_Flujo);
	
	//if(verbose){std::cout<<"Visualizacion de los carros"<<std::endl;}
	//Evolucion.Avenida_Visualizacion_Carros(Simulacion,tmax,5,true,Test_Visualizacion_Carros);

	if(verbose){std::cout<<"Diagrama Fundamental"<<std::endl;}
	for(double kindness=0.0;kindness<=1.0;kindness+=0.2){
		Evolucion.Set_Amabilidad(kindness);
		Evolucion.Avenida_Diagrama_Fundamental(Simulacion,tmax,0.2,0.8,0.2,2,true,Test_Diagrama_Fundamental);
	}

	//Evolucion.Avenida_Diagrama_Fundamental(Simulacion,tmax,0.01,0.8,0.02,10,true,Test_Diagrama_Fundamental);
	
	//if(verbose){std::cout<<"Analisis Variables Auxiliares"<<std::endl;}
	//Evolucion.Avenida_Analisis_Variables_Auxiliares(Simulacion,tmax,0.02,5,true,Datos_Diagrama_Fundamental);

	/*f(verbose){std::cout<<"Datos Tiempo de Viaje"<<std::endl;}
	double rho_inicial=0.2,rho_final=0.8,delta_rho=0.02;
	double kindness_inicial=0.0,kindness_final=1.0,delta_kindness=0.1;
	int cantidad_de_configuraciones=1;
	for(double rho=rho_inicial;rho<=rho_final;rho+=delta_rho){
		for(double kindness=kindness_inicial;kindness<=kindness_final;kindness+=delta_kindness){
			std::cout<<"Densidad "<<rho<<" kindness "<<kindness<<std::endl;
			Evolucion.Tiempo_De_Viaje(Simulacion,tmax,cantidad_de_configuraciones,rho,kindness,verbose,Test_Tiempo_De_Viaje);
		}
	}*/
	

	
	if(verbose){std::cout<<"Eliminar pointers"<<std::endl;}
	for(int ii=0;ii<Cantidad_Carriles;ii++){
		delete[] Carros_Estaticos[ii];
	}
	delete[] Carros_Estaticos;

	delete[] Cantidad_Carros_Estaticos;  

	return 0;  
}
 


//Codigo que se ha usado para pruebas

//std::cout<<Carrera_30.Get_Cantidad_Carros(Frontera_Salida)<<std::endl;
//Carrera_30.Encontrar_Primeros_Vecinos(true,true,Simulacion);
//Carrera_30.Intercambiar_Informacion_Y_Calcular_Gaps(true,Simulacion);
//Carrera_30.Mover_Carros(Simulacion);
//Evolucion.Avenida_Evolucionar_Variables_Carros(Simulacion,false);
//Evolucion.Avenida_Diagrama_Fundamental(Simulacion,tmax,0.01,3,false,Datos_Diagrama_Fundamental);
/*for(double seed = 1.0; seed <=100; seed += 0.5){
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_1(seed);
	Evolucion.Set_Random_Gen(&Ran2_1);
	std::cout<<"Rho "<<0.2<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.20);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_2(seed);
	Evolucion.Set_Random_Gen(&Ran2_2);
    std::cout<<"Rho "<<0.25<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.25);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_3(seed);
	Evolucion.Set_Random_Gen(&Ran2_3);
	std::cout<<"Rho "<<0.30<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.30);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_4(seed);
	Evolucion.Set_Random_Gen(&Ran2_4);
	std::cout<<"Rho "<<0.35<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.35);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_5(seed);
	Evolucion.Set_Random_Gen(&Ran2_5);
	std::cout<<"Rho "<<0.4<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.4);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);std::cout<<"Seed "<<seed<<std::endl;
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_6(seed);
	Evolucion.Set_Random_Gen(&Ran2_6);
	std::cout<<"Rho "<<0.45<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.45);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);
	std::cout<<"Seed "<<seed<<std::endl;
	Crandom Ran2_7(seed);
	Evolucion.Set_Random_Gen(&Ran2_7);
	std::cout<<"Rho "<<0.5<<std::endl;
	Evolucion.Set_Posiciones_Iniciales_Aleatorias(Simulacion,0.5);
	Evolucion.Avenida_Evolucion_Flujo(Simulacion,tmax,1,1,false,Datos_Evolucion_Flujo);std::cout<<"Seed "<<seed<<std::endl;
}*/

/*
std::ofstream datafile;
std::ostringstream data_file_name;
std::string name_file;
data_file_name<<Datos_Diagrama_Fundamental<<"/Fronteras_Carriles_"<<3<<"_Largo_"<<50<<"_Time_"<<10<<"_paso_"<<2<<".dat";	
name_file = data_file_name.str();
datafile.open(name_file.c_str());
if(datafile.is_open()){
	std::cout<<"Archivo abierto"<<std::endl;
	datafile.close();
}
else{
	std::cout<<"No se puede escribir el archivo"<<std::endl;
}*/
