#include "List_Multilane.h"
//#include <vector>
//________________Fijar_Informacion_Del_Sistema______________________________

//______________Inicializar_Carros___________________________________________
void Dinamica::Set_Posiciones_Iniciales_Aleatorias(Tipo_Carril tipo_carril,double rho){
  int id_count,cantidad_carros[(*Carrera).Get_Cantidad_Carriles(tipo_carril)];
  id_count=0;
  if(Frontera_Entrada==tipo_carril){
    for(int ii=0;ii<(*Carrera).Get_Cantidad_Carriles(tipo_carril);ii++){
      cantidad_carros[ii]=(int)(rho*(*Carrera).Get_Largo(tipo_carril)*0.5);
    }
    (*Carrera).Posiciones_Iniciales_Aleatorias_Carriles_Frontera_Entrada(cantidad_carros,(*Random_Gen),id_count);
  }
  else if(Frontera_Salida==tipo_carril){
    for(int ii=0;ii<(*Carrera).Get_Cantidad_Carriles(tipo_carril);ii++){
      cantidad_carros[ii]=(int)(rho*(*Carrera).Get_Largo(tipo_carril)*0.5);
    }
    (*Carrera).Posiciones_Iniciales_Aleatorias_Carriles_Frontera_Salida(cantidad_carros,(*Random_Gen),id_count);
  }
  else if(Simulacion==tipo_carril){
    //int cant_carriles=(*Carrera).Get_Cantidad_Carriles(tipo_carril);
    //int cant_carros_estaticos[cant_carriles];
    for(int ii=0;ii<(*Carrera).Get_Cantidad_Carriles(tipo_carril);ii++){
      cantidad_carros[ii]=(int)(rho*(*Carrera).Get_Largo(tipo_carril)*0.5);
      //cant_carros_estaticos[ii]=std::min(Cantidad_Carros_Estaticos[ii],cantidad_carros[ii]);
    }
    (*Carrera).Posiciones_Iniciales_Aleatorias_Carriles_Simulacion(cantidad_carros,(*Random_Gen),id_count);
    //(*Carrera).Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Simulacion(cantidad_carros,(*Random_Gen),Carros_Estaticos,cant_carros_estaticos,id_count);
  }
}

//_______________Dinamica______________________________________________________
void Dinamica::Avenida_Evolucionar_Variables_Carros(Tipo_Carril tipo_carril,bool verbose){
  if(verbose){std::cout<<"Encontrar primeros vecinos"<<std::endl;}
  (*Carrera).Encontrar_Primeros_Vecinos(MultiCarril,true,tipo_carril);
  if(verbose){std::cout<<"Intercambiar informacion y Calcular Gaps"<<std::endl;}
  (*Carrera).Intercambiar_Informacion_Y_Calcular_Gaps(MultiCarril,tipo_carril);
  if(MultiCarril){
    if(verbose){std::cout<<"Decide Change Lines and Direccionales"<<std::endl;}
    (*Carrera).Decide_Change_Lines_And_Direccionales(Tipo_Cambio_Carril,true,tipo_carril,(*Random_Gen));
    if(Ceder_El_Paso){
      if(verbose){std::cout<<"Decide Ceder El Paso"<<std::endl;}
      (*Carrera).Decide_Ceder_El_Paso((*Random_Gen),Amabilidad,tipo_carril);
      if(verbose){std::cout<<"Decide Change Lines After Asking for the Way"<<std::endl;}
      (*Carrera).Decide_Change_Lines_After_Asking_For_Way(Tipo_Cambio_Carril,tipo_carril,(*Random_Gen));
    }
    if(verbose){std::cout<<"Cambio de Carril"<<std::endl;}
    (*Carrera).Cambio_De_Carril(tipo_carril);
    if(verbose){std::cout<<"Intercambiar Informacion y Calcular Gaps"<<std::endl;}
    (*Carrera).Intercambiar_Informacion_Y_Calcular_Gaps(false,tipo_carril);
  }
  if(verbose){std::cout<<"Evolucionar Variables Internas Carros"<<std::endl;}
  (*Carrera).Evolucionar_Variables_Internas_Carros(tipo_carril);
  if(verbose){std::cout<<"Termina evolucion variables carros"<<std::endl;}
}

void Dinamica::Avenida_Mover_Carros(Tipo_Carril tipo_carril,bool verbose){
  if(verbose){std::cout<<"Mover Carros"<<std::endl;}
  (*Carrera).Mover_Carros(tipo_carril);
  if(verbose){std::cout<<"Carros Movidos"<<std::endl;}
}

void Dinamica::Avenida_Evolucionar(Tipo_Carril tipo_carril,int tmax,bool verbose){
  if(Frontera_Entrada==tipo_carril){
    for(int ii=0;ii<tmax;ii++){
      if(verbose){std::cout<<"Time "<<ii<<std::endl;}
      if(verbose){std::cout<<"Frontera Entrada Evolucionar Variables Carros"<<std::endl;}
      Dinamica::Avenida_Evolucionar_Variables_Carros(tipo_carril,verbose);
      if(verbose){std::cout<<"Frontera Entrada Mover Carros"<<std::endl;}
      Dinamica::Avenida_Mover_Carros(tipo_carril,verbose);
    }
  }
  else if(Frontera_Salida==tipo_carril){
    for(int ii=0;ii<tmax;ii++){ 
      if(verbose){std::cout<<"Time "<<ii<<std::endl;}
      if(verbose){std::cout<<"Frontera Salida Evolucionar Variables Carros"<<std::endl;}
      Dinamica::Avenida_Evolucionar_Variables_Carros(tipo_carril,verbose);
      if(verbose){std::cout<<"Frontera Salida Mover Carros"<<std::endl;}
      Dinamica::Avenida_Mover_Carros(tipo_carril,verbose);
    }
  }
  else if(Simulacion==tipo_carril){
    if((*Carrera).Get_Fronteras_Periodicas()){
      for(int ii=0;ii<tmax;ii++){
	if(verbose){std::cout<<"Time "<<ii<<std::endl;}
	if(verbose){std::cout<<"Avenida Fronteras Periodicas Evolucionar Variables Carros"<<std::endl;}
	Dinamica::Avenida_Evolucionar_Variables_Carros(tipo_carril,verbose);
	if(verbose){std::cout<<"Avenida Fronteras Periodicas Mover Carros"<<std::endl;}
	Dinamica::Avenida_Mover_Carros(tipo_carril,verbose);
      }
    }
    else{	
      for(int ii=0;ii<tmax;ii++){
	if(verbose){std::cout<<"Time "<<ii<<std::endl;}
	if(verbose){std::cout<<"Frontera Entradada Evolucionar Variables Carros"<<std::endl;}
	Dinamica::Avenida_Evolucionar_Variables_Carros(Frontera_Entrada,verbose);
	if(verbose){std::cout<<"Fronteras Salida Evolucionar Variables Carros"<<std::endl;}
	Dinamica::Avenida_Evolucionar_Variables_Carros(Frontera_Salida,verbose);
	if(verbose){std::cout<<"Avenida Fronteras Abiertas Evolucionar Variables Carros"<<std::endl;}
	Dinamica::Avenida_Evolucionar_Variables_Carros(tipo_carril,verbose);
	if(verbose){std::cout<<"Avenida Mover Carros"<<std::endl;}
	Dinamica::Avenida_Mover_Carros(tipo_carril,verbose);
	if(verbose){std::cout<<"Interaccion Con Carriles Frontera"<<std::endl;}
	(*Carrera).Interaccion_Con_Carriles_Frontera();
	if(verbose){std::cout<<"Frontera Entrada Mover Carros"<<std::endl;}
	Dinamica::Avenida_Mover_Carros(Frontera_Entrada,verbose);
	if(verbose){std::cout<<"Frontera Salida Mover Carros"<<std::endl;}
	Dinamica::Avenida_Mover_Carros(Frontera_Salida,verbose);	
      }
    }		
  }
  if(verbose){std::cout<<"Termina evolucion avenida"<<std::endl;}
}

bool Dinamica::Avenida_Detener_Carro(Tipo_Carril tipo_carril,double rho,int carril,int posicion,int position_delta,int time_stoped,bool verbose){
  bool cars_have_not_stoped=true;
  bool trying_to_stop_car=true;
  int number_of_tries=0;
  while(cars_have_not_stoped&&(number_of_tries<50)){
    std::cout<<"Number of tries to stop a car "<<number_of_tries<<std::endl;
    if(verbose){std::cout<<"Set Posiciones iniciales Fronteras"<<std::endl;}
    Dinamica::Set_Posiciones_Iniciales_Aleatorias(Frontera_Entrada,rho);
    Dinamica::Set_Posiciones_Iniciales_Aleatorias(Frontera_Salida,rho);
    if(verbose){std::cout<<"Evolucionar Fronteras"<<std::endl;}
    Dinamica::Avenida_Evolucionar(Frontera_Entrada,5000,false);
    Dinamica::Avenida_Evolucionar(Frontera_Salida,5000,false);
    if(verbose){std::cout<<"Set Posiciones Iniciales Carriles Simulacion"<<std::endl;}
    Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
    if(verbose){std::cout<<"Evolucionar Zona de Simulacion"<<std::endl;}
    Dinamica::Avenida_Evolucionar(tipo_carril,2000,false);
		
    if(verbose){std::cout<<"Detener carro en la mitad del carril"<<std::endl;}
    for(int stop=0;(stop<100)&&(cars_have_not_stoped);stop++){
      Dinamica::Avenida_Evolucionar_Variables_Carros(tipo_carril,false);
      if(verbose){std::cout<<"Avenida Mover Carros"<<std::endl;}
      Dinamica::Avenida_Mover_Carros(tipo_carril,verbose);
      if(verbose){std::cout<<"Interaccion Con Carriles Frontera"<<std::endl;}
      (*Carrera).Interaccion_Con_Carriles_Frontera();
      if(tipo_carril==Simulacion&&!(*Carrera).Get_Fronteras_Periodicas()){
	if(verbose){std::cout<<"Frontera Entrada Mover Carros"<<std::endl;}
	Dinamica::Avenida_Mover_Carros(Frontera_Entrada,verbose);
	if(verbose){std::cout<<"Frontera Salida Mover Carros"<<std::endl;}
	Dinamica::Avenida_Mover_Carros(Frontera_Salida,false);
      }
      if(trying_to_stop_car){
	if((*Carrera).Stop_Car(posicion,time_stoped,carril)){
	  trying_to_stop_car=false;
	}	
      }
      if(!trying_to_stop_car){
	if((*Carrera).Cars_Have_Stoped(tipo_carril,posicion-position_delta,posicion+position_delta)){
	  cars_have_not_stoped=false;
	  (*Carrera).Set_Cars_Inset_Lane(-1);
	}
      }
    }
    number_of_tries+=1;
  }

  if(number_of_tries<50){
    return true;
  }
  else{
    return false;
  }
}

//______________Observables____________________________________________________
void Dinamica::Avenida_Diagrama_Fundamental(Tipo_Carril tipo_carril,int tmax,double rho_inicial,double rho_final,double delta_rho,int configuraciones,bool verbose,std::string folder){
  std::vector<double> Densidad;
  std::vector<double> Flujo;
  std::ofstream datafile;
  std::ostringstream data_file_name;
  std::string name_file;
  Densidad.clear();
  Flujo.clear();
  double flujo;
  double densidad;
  if(verbose){std::cout<<"Diagrama Fundamental"<<std::endl;}
  if(Frontera_Entrada==tipo_carril||Frontera_Salida==tipo_carril){
    if(verbose){std::cout<<"Carril Frontera"<<std::endl;}
    data_file_name<<folder<<"/Fronteras_Carriles_Evolucion_Juan_+1_v2_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<".dat";	
    name_file=data_file_name.str();
    datafile.open(name_file.c_str());
    if(datafile.is_open()){
      for(double rho=rho_inicial;rho<rho_final;rho+=delta_rho){
	if(verbose){std::cout<<"Rho "<<rho<<std::endl;}
	for(int jj=0;jj<configuraciones;jj++){
	  if(verbose){std::cout<<"Configuracion "<<jj<<std::endl;}
	  if(verbose){std::cout<<"Set Posiciones Iniciales "<<std::endl;}
	  Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
	  if(verbose){std::cout<<"Avenida Evolucionar "<<std::endl;}
	  Dinamica::Avenida_Evolucionar(tipo_carril,tmax,false);
	  densidad=(*Carrera).Get_Rho(tipo_carril,true);
	  Densidad.push_back(densidad);
	  flujo=(*Carrera).Get_Flujo_Espacial(tipo_carril);//*(*Carrera).Get_Rho(tipo_carril);
	  Flujo.push_back(flujo);
	  if(verbose){std::cout<<densidad<<" "<<flujo<<std::endl;}
	  datafile<<Densidad.back()<<" "<<Flujo.back()<<"\n";
	}
      }	
      datafile.close();
    }
    else{
      std::cout<<"No se puede escribir el archivo"<<std::endl;
    }
  }
  else if(Simulacion==tipo_carril){
    if(verbose){std::cout<<"Carril Simulacion"<<std::endl;}
    if((*Carrera).Get_Fronteras_Periodicas()){
      if(verbose){std::cout<<"Fronteras Periodicas"<<std::endl;}
      if(MultiCarril){
	data_file_name<<folder<<"/Sim_Mul_FP_Cp_Olmos_+1_v2_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_amabilidad_"<<Amabilidad<<".dat";	
      }
      else{
	data_file_name<<folder<<"/Sim_No_Mul_FP_Cp_Olmos_+1_v2_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_amabilidad_"<<Amabilidad<<".dat";	
      }
      //data_file_name<<folder<<"/Simulacion_Carriles_Evolucion_Juan_Copia_Olmos_+1_v2.2_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<".dat";
      name_file = data_file_name.str();
      datafile.open(name_file.c_str());
      if(datafile.is_open()){
	for(double rho=rho_inicial;rho<rho_final;rho+=delta_rho){
	  if(verbose){std::cout<<"Rho "<<rho<<std::endl;}
	  for(int jj=0;jj<configuraciones;jj++){
	    if(verbose){std::cout<<"Configuracion "<<jj<<std::endl;}
	    if(verbose){std::cout<<"Set Posiciones Iniciales"<<std::endl;}
	    Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
	    if(verbose){std::cout<<"Avenida Evolucionar"<<std::endl;}
	    Dinamica::Avenida_Evolucionar(tipo_carril,tmax,false);
	    Densidad.push_back((*Carrera).Get_Rho(tipo_carril,true));
	    Flujo.push_back((*Carrera).Get_Flujo_Espacial(tipo_carril));
	    datafile<<Densidad.back()<<" "<<Flujo.back()<<"\n";
	    if(verbose){std::cout<<Densidad.back()<<" "<<Flujo.back()<<std::endl;}
	  }
	}
	datafile.close();
      }
      else{
	std::cout<<"No se puede escribir el archivo"<<std::endl;
      }
    }
    else{
      if(verbose){std::cout<<"Fronteras Abiertas"<<std::endl;}
      if(MultiCarril){
	if(Ceder_El_Paso){
	  data_file_name<<folder<<"/Sim_Mul_FA_CP_Cp_Olmos_+1_v2_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_amabilidad_"<<Amabilidad<<".dat";	
	}
	else{
	  data_file_name<<folder<<"/Sim_Mul_FA_Cp_Olmos_+1_v2_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<".dat";	
	}
      }
      else{
	data_file_name<<folder<<"/Sim_No_Mul_FA_Cp_Olmos_+1_v2_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<".dat";	
      }
      name_file=data_file_name.str();
      datafile.open(name_file.c_str());
      if(datafile.is_open()){
	for(double rho=rho_inicial;rho<rho_final;rho+=delta_rho){
	  if(verbose){std::cout<<"Rho "<<rho<<std::endl;}
	  for(int jj=0;jj<configuraciones;jj++){
	    if(verbose){std::cout<<"configuracion "<<jj<<std::endl;}
	    if(verbose){std::cout<<"Set Posiciones iniciales Fronteras"<<std::endl;}
	    Dinamica::Set_Posiciones_Iniciales_Aleatorias(Frontera_Entrada,rho);
	    Dinamica::Set_Posiciones_Iniciales_Aleatorias(Frontera_Salida,rho);
	    if(verbose){std::cout<<"Evolucion Fronteras"<<std::endl;}
	    Dinamica::Avenida_Evolucionar(Frontera_Entrada,3000,false);
	    Dinamica::Avenida_Evolucionar(Frontera_Salida,3000,false);
	    if(verbose){std::cout<<"Set Posiciones Iniciales Carriles Simulacion"<<std::endl;}
	    Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
	    if(verbose){std::cout<<"Avenida Evolucionar"<<std::endl;}
	    Dinamica::Avenida_Evolucionar(tipo_carril,tmax,false);
	    Densidad.push_back((*Carrera).Get_Rho(tipo_carril,true));
	    Flujo.push_back((*Carrera).Get_Flujo_Espacial(tipo_carril));
	    datafile<<Densidad.back()<<" "<<Flujo.back()<<"\n";
	    if(verbose){std::cout<<Densidad.back()<<" "<<Flujo.back()<<std::endl;}
	  }
	}
      }
      else{
	std::cout<<"No se puede escribir el archivo"<<std::endl;
      }
    }
  }
}

//______________Visualizacion__________________________________________________
void Dinamica::Avenida_Visualizacion_Carros(Tipo_Carril tipo_carril,int max_time,int num_particiones,bool verbose,std::string folder){

  //Encontrar el orden de magnitud del tiempo máximo
  int num_Data_Frames = max_time*num_particiones;
  int Order_Magnitude = 0;
  int nn = 0;
  while(num_Data_Frames/pow(10,nn)>=1){
    Order_Magnitude +=1;
    nn+=1;
  }	
  	
  std::vector<int> carril;
  std::vector<int> id;
  std::vector<int> posicion;
  std::vector<int> velocidad;
  std::vector<int> freno;
  std::vector<int> cambio_carril_izquierda;
  std::vector<int> cambio_carril_derecha;
  std::vector<int> ceder_paso_izquierda;
  std::vector<int> ceder_paso_derecha;

  double posicion_carro,carril_ocupado;

  std::ostringstream Time;
  std::ofstream datafile;
  std::ostringstream data_file_name;
  std::string name_file;
	
  if(verbose){std::cout<<"Visualizacion Carros"<<std::endl;}
  for(int tt=0;tt<max_time;tt++){
    if(verbose){std::cout<<"Time "<<tt<<std::endl;}
    //Evolucionar las variables de los carros
    if((tipo_carril==Simulacion)&&(!(*Carrera).Get_Fronteras_Periodicas())){
      if(verbose){std::cout<<"Evolucionan los carriles de la frontera"<<std::endl;}
      Dinamica::Avenida_Evolucionar_Variables_Carros(Frontera_Entrada,false);
      Dinamica::Avenida_Evolucionar_Variables_Carros(Frontera_Salida,false);
    }
    if(verbose){std::cout<<"Evolucionar Variables Carros"<<std::endl;}
    Dinamica::Avenida_Evolucionar_Variables_Carros(tipo_carril,verbose);
		
    if(verbose){std::cout<<"Get Informacion de los carros"<<std::endl;}
    (*Carrera).Carril_Ocupado_Carros(tipo_carril,carril);
    (*Carrera).Id_Carros(tipo_carril,id);
    (*Carrera).Posiciones_Carros(tipo_carril,posicion);
    (*Carrera).Velocidades_Carros(tipo_carril,velocidad);
    (*Carrera).Brake_Carros(tipo_carril,freno);
    (*Carrera).Cambio_Carril_Izquierda_Carros(tipo_carril,cambio_carril_izquierda);
    (*Carrera).Cambio_Carril_Derecha_Carros(tipo_carril,cambio_carril_derecha);
    (*Carrera).Ceder_El_Paso_Derecha_Carros(tipo_carril,ceder_paso_derecha);
    (*Carrera).Ceder_El_Paso_Izquierda_Carros(tipo_carril,ceder_paso_izquierda);
		
    //Imprimir las posiciones de los carros
    if(Frontera_Entrada==tipo_carril||Frontera_Salida==tipo_carril){
      for(int ii=0;ii<num_particiones;ii++){
	Time.str("");
	Time.clear();
	data_file_name.str("");
	data_file_name.clear();

	Time<<std::internal<<std::setfill('0')<<std::setw(Order_Magnitude)<<tt*num_particiones+ii;
      
	//Crear archivo con los datos
	data_file_name<<folder<<"/Fronteras_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Time_"<<Time.str()<<".txt";
	//data_file_name <<folder<<"/Test"<<"_Time_"<<Time.str()<<".txt";
	name_file=data_file_name.str();
	datafile.open(name_file.c_str()); 
			    
	for(int jj=0;jj<id.size();jj++){
	  posicion_carro=posicion[jj];
	  posicion_carro+=(double)(ii*velocidad[jj])/num_particiones;			    	
	  if(posicion_carro>=(*Carrera).Get_Largo(tipo_carril)){
	    posicion_carro-=(*Carrera).Get_Largo(tipo_carril);
	  }
	  carril_ocupado=carril[jj];
	  if(cambio_carril_izquierda[jj]){
	    carril_ocupado+=(1.0-((double)(ii)/num_particiones));
	  }
	  if(cambio_carril_derecha[jj]){
	    carril_ocupado+=(-1.0+((double)(ii)/num_particiones));
	  }
	  datafile<<id[jj]<<","<<posicion_carro<<","<<carril_ocupado<<","<<velocidad[jj]<<","<<freno[jj]<<","<<ceder_paso_derecha[jj]<<","<<ceder_paso_izquierda[jj]<<"\n";
	}
	datafile.close();
      }	
    }
    else if(Simulacion==tipo_carril){
      for(int ii=0;ii<num_particiones;ii++){
	Time.str("");
	Time.clear();
	Time<<std::internal<<std::setfill('0')<<std::setw(Order_Magnitude)<<tt*num_particiones+ii;
	//Crear archivo con los datos
	data_file_name.str("");
	data_file_name.clear();
	/*if((*Carrera).Get_Fronteras_Periodicas()){
	  data_file_name <<folder<<"/Simulacion_Fronteras_Periodicas_Largo_Dar_Paso_Cambio_Carril_2_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Time_"<<Time.str()<<".txt";
	  }
	  else{
	  data_file_name <<folder<<"/Simulacion_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Time_"<<Time.str()<<".txt";
	  }*/
	data_file_name<<folder<<"/Test_New_Implementation"<<"_Time_"<<Time.str()<<".txt";
	name_file=data_file_name.str();
	datafile.open(name_file.c_str());  
	if(datafile.is_open()){
	  for(int jj=0;jj<id.size();jj++){
	    posicion_carro=posicion[jj];
	    posicion_carro+=(double)(ii*velocidad[jj])/num_particiones;	
	    if((*Carrera).Get_Fronteras_Periodicas()){
	      if(posicion_carro>=(*Carrera).Get_Largo(tipo_carril)){
		posicion_carro-=(*Carrera).Get_Largo(tipo_carril);
	      }	
	    }		    	
	    carril_ocupado=carril[jj];
	    if(cambio_carril_izquierda[jj]){
	      carril_ocupado+=(1.0-((double)(ii)/num_particiones));
	    }
	    if(cambio_carril_derecha[jj]){
	      carril_ocupado+=(-1.0+((double)(ii)/num_particiones));
	    }
	    datafile<<id[jj]<<","<<posicion_carro<<","<<carril_ocupado<<","<<velocidad[jj]<<","<<freno[jj]<<","<<cambio_carril_izquierda[jj]<<","<<cambio_carril_derecha[jj]<<","<<ceder_paso_derecha[jj]<<","<<ceder_paso_izquierda[jj]<<"\n";
	  }
	  datafile.close();
	}
	else{	
	  std::cout<<"No se pudo abrir el archivo"<<std::endl;
	}
      }	
    }
    if(verbose){std::cout<<"Avenida Mover Carros"<<std::endl;}
    Dinamica::Avenida_Mover_Carros(tipo_carril,verbose);
    if(verbose){std::cout<<"Interaccion Con Carriles Frontera"<<std::endl;}
    (*Carrera).Interaccion_Con_Carriles_Frontera();
    if(tipo_carril==Simulacion&&!(*Carrera).Get_Fronteras_Periodicas()){
      if(verbose){std::cout<<"Frontera Entrada Mover Carros"<<std::endl;}
      Dinamica::Avenida_Mover_Carros(Frontera_Entrada,verbose);
      if(verbose){std::cout<<"Frontera Salida Mover Carros"<<std::endl;}
      Dinamica::Avenida_Mover_Carros(Frontera_Salida,verbose);
    }
  }
}

//______________Evolucion_Flujo________________________________________________
void Dinamica::Avenida_Evolucion_Flujo(Tipo_Carril tipo_carril,int max_time,int paso,bool verbose,std::string folder){
  if(verbose){std::cout<<"Avenida Evolucion Flujo"<<std::endl;}
  std::vector<double> Tiempo;
  std::vector<double> Flujo;
  if(Simulacion==tipo_carril&&(!(*Carrera).Get_Fronteras_Periodicas())){
    if(verbose){std::cout<<"Evolucionar Fronteras time "<<5000<<std::endl;}
    Dinamica::Avenida_Evolucionar(Frontera_Entrada,5000,verbose);
    Dinamica::Avenida_Evolucionar(Frontera_Salida,5000,verbose);
  }
	
  for(int ii=0;ii<max_time;ii+=paso){
    if(verbose){std::cout<<"Evolucionar Tiempo "<<ii<<std::endl;}
    Dinamica::Avenida_Evolucionar(tipo_carril,paso,verbose);
    Tiempo.push_back((double)(ii));
    Flujo.push_back((*Carrera).Get_Flujo_Espacial(tipo_carril));
  }

  std::ofstream datafile;
  std::ostringstream data_file_name;
  std::string name_file;
  /*if(Frontera_Entrada==tipo_carril){
    data_file_name<<folder<<"/Carriles_Entrada_"<<(*Carrera).Get_Cantidad_Carriles(Frontera_Entrada)<<"_Largo_"<<(*Carrera).Get_Largo(Frontera_Entrada)<<"_amabilidad_"<<Amabilidad<<".dat";	
    }
    else if(Frontera_Salida==tipo_carril){
    data_file_name<<folder<<"/Carriles_Salida_"<<(*Carrera).Get_Cantidad_Carriles(Frontera_Salida)<<"_Largo_"<<(*Carrera).Get_Largo(Frontera_Salida)<<"_amabilidad_"<<Amabilidad<<".dat";	
    }
    else if(Simulacion==tipo_carril){
    data_file_name<<folder<<"/Carriles_Simulacion_"<<(*Carrera).Get_Cantidad_Carriles(Simulacion)<<"_Largo_"<<(*Carrera).Get_Largo(Simulacion)<<"_amabilidad_"<<Amabilidad<<".dat";	
    }*/

  data_file_name<<folder<<"/Testing_New_Implementation.dat";

  name_file = data_file_name.str();
  datafile.open(name_file.c_str());
  if(datafile.is_open()){
    for(int ii=0;ii<Tiempo.size();ii++){
      datafile<<Tiempo[ii]<<" "<<Flujo[ii]<<"\n";
    }
    datafile.close();
  }
  else{
    std::cout<<"No se puede escribir el archivo"<<std::endl;
  }

}

//__________________Analisis variables Auxiliares____________________________________________________________________
void Dinamica::Avenida_Analisis_Variables_Auxiliares(Tipo_Carril tipo_carril,int tmax,double paso,int configuraciones,bool verbose,std::string folder){
  std::vector<double> Densidad,Distancia,dis;
  //std::vector<int> dis;
  std::ofstream datafile;
  std::ostringstream data_file_name;
  std::string name_file;
  Densidad.clear();
  Distancia.clear();
  dis.clear();
  if(verbose){std::cout<<"Analisis Variables Auxiliares"<<std::endl;}
  if(Frontera_Entrada==tipo_carril||Frontera_Salida==tipo_carril){
    if(verbose){std::cout<<"Carril Frontera"<<std::endl;}
    for(double rho=0.0;rho<0.66;rho+=paso){
      if(verbose){std::cout<<"Rho "<<rho<<std::endl;}
      for(int jj=0;jj<configuraciones;jj++){
	if(verbose){std::cout<<"Configuracion "<<jj<<std::endl;}
	if(verbose){std::cout<<"Set Posiciones Iniciales "<<std::endl;}
	Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
	if(verbose){std::cout<<"Avenida Evolucionar "<<std::endl;}
	Dinamica::Avenida_Evolucionar(tipo_carril,tmax,verbose);
	(*Carrera).Razones_De_Velocidades(tipo_carril,dis);
	for(int kk=0;kk<dis.size();kk++){
	  Densidad.push_back((*Carrera).Get_Rho(tipo_carril,true));
	  Distancia.push_back(dis[kk]);	
	}
      }
    }
    data_file_name<<folder<<"/Distancias_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<".dat";	
    name_file=data_file_name.str();
    datafile.open(name_file.c_str());
    if(datafile.is_open()){
      for(int ii=0;ii<Densidad.size();ii++){
	datafile<<Densidad[ii]<<" "<<Distancia[ii]<<"\n";
      }
      datafile.close();
    }
    else{
      std::cout<<"No se puede escribir el archivo"<<std::endl;
    }
  }
  else if(Simulacion==tipo_carril){
    if(verbose){std::cout<<"Carril Simulacion"<<std::endl;}
    if((*Carrera).Get_Fronteras_Periodicas()){
      if(verbose){std::cout<<"Fronteras Periodicas"<<std::endl;}
      if(MultiCarril){
	data_file_name<<folder<<"/Simulacion_Distancias_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_amabilidad_"<<Amabilidad<<".dat";	
      }
      else{
	data_file_name<<folder<<"/Simulacion_Distancias_Carriles_"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_amabilidad_"<<Amabilidad<<".dat";	
      }
      name_file=data_file_name.str();
      //std::cout<<data_file_name.str()<<std::endl;
      datafile.open(name_file.c_str());
      if(datafile.is_open()){
	for(double rho=0.01;rho<0.66;rho+=paso){
	  if(verbose){std::cout<<"Rho "<<rho<<std::endl;}
	  for(int jj=0;jj<configuraciones;jj++){
	    if(verbose){std::cout<<"Set Posiciones Iniciales"<<std::endl;}
	    Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
	    if(verbose){std::cout<<"Avenida Evolucionar"<<std::endl;}
	    Dinamica::Avenida_Evolucionar(tipo_carril,tmax,verbose);
	    (*Carrera).Razones_De_Velocidades(tipo_carril,dis);
	    for(int kk=0;kk<dis.size();kk++){
	      Densidad.push_back((*Carrera).Get_Rho(tipo_carril,true));
	      Distancia.push_back(dis[kk]);	
	    }
	  }
	}
	for(int ii=0;ii<Densidad.size();ii++){
	  datafile<<Densidad[ii]<<" "<<Distancia[ii]<<"\n";
	}
	datafile.close();
      }
      else{
	std::cout<<"No se puede escribir el archivo"<<std::endl;
      }
    }
    else{
      if(verbose){std::cout<<"Fronteras Abiertas"<<std::endl;}
      for(double rho=0.0;rho<0.66;rho+=paso){
	if(verbose){std::cout<<"Rho "<<rho<<std::endl;}
	for(int jj=0;jj<configuraciones;jj++){
	  if(verbose){std::cout<<"configuracion "<<jj<<std::endl;}
	  if(verbose){std::cout<<"Set Posiciones iniciales Fronteras"<<std::endl;}
	  Dinamica::Set_Posiciones_Iniciales_Aleatorias(Frontera_Entrada,rho);
	  Dinamica::Set_Posiciones_Iniciales_Aleatorias(Frontera_Salida,rho);
	  if(verbose){std::cout<<"Evolucionr Fronteras"<<std::endl;}
	  Dinamica::Avenida_Evolucionar(Frontera_Entrada,5000,verbose);
	  Dinamica::Avenida_Evolucionar(Frontera_Salida,5000,verbose);
	  if(verbose){std::cout<<"Set Posiciones Iniciales Carriles Simulacion"<<std::endl;}
	  Dinamica::Set_Posiciones_Iniciales_Aleatorias(tipo_carril,rho);
	  if(verbose){std::cout<<"Avenida Evolucionar"<<std::endl;}
	  Dinamica::Avenida_Evolucionar(tipo_carril,tmax,verbose);
	  (*Carrera).Razones_De_Velocidades(tipo_carril,dis);
	  for(int kk=0;kk<dis.size();kk++){
	    Densidad.push_back((*Carrera).Get_Rho(tipo_carril,true));
	    Distancia.push_back(dis[kk]);
	  }
	}
      }
      data_file_name<<folder<<"/Distancias"<<(*Carrera).Get_Cantidad_Carriles(tipo_carril)<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_amabilidad_"<<Amabilidad<<".dat";	
      name_file = data_file_name.str();
      datafile.open(name_file.c_str());
      if(datafile.is_open()){
	for(int ii=0;ii<Densidad.size();ii++){
	  datafile<<Densidad[ii]<<" "<<Distancia[ii]<<"\n";
	}
	datafile.close();
      }
      else{
	std::cout<<"No se puede escribir el archivo"<<std::endl;
      }
    }
  }
}

//_______________Tiempo de Viaje____________________________________________________________________________________
void Dinamica::Tiempo_De_Viaje(Tipo_Carril tipo_carril,int tmax,int configuraciones,double rho,double kindness,bool verbose,std::string folder){
  if(tipo_carril==Simulacion){
    std::list<Carro> carros_que_salen;
    std::list<Carro>::iterator carro;
    std::vector<int> inset_lane;
    std::vector<int> outset_lane;
    std::vector<int> travel_time;
    std::vector<int> simulation_time;
    std::vector<double> flujo;
    std::vector<double> densidad;	
    double flux,density;
    int significant_simulation_time=((*Carrera).Get_Largo(tipo_carril)/(*Carrera).Get_Vel_Max())*10;

    Amabilidad=kindness;
    for(int conf=0;conf<configuraciones;conf++){
      inset_lane.clear();
      outset_lane.clear();
      travel_time.clear();
      simulation_time.clear();
      flujo.clear();
      densidad.clear();
      if(verbose){std::cout<<"Configuracion "<<conf<<std::endl;}
      if(Dinamica::Avenida_Detener_Carro(tipo_carril,rho,0,(*Carrera).Get_Largo(tipo_carril)/2,(*Carrera).Get_Largo(tipo_carril)/2,tmax*2,false)){
	for(int tt=0;tt<tmax;tt++){
	  //if(verbose){std::cout<<"Evolucionar Zona de Simulacion"<<std::endl;}
	  Dinamica::Avenida_Evolucionar(tipo_carril,1,false);
	  density=(*Carrera).Get_Rho(tipo_carril,true);
	  flux=(*Carrera).Get_Flujo_Espacial(tipo_carril);
	  if(tt%significant_simulation_time==0){
	    carros_que_salen.clear();
	    carros_que_salen=(*Carrera).Get_Info_Carros_Que_Salen(tipo_carril);
	    for(carro=carros_que_salen.begin();carro!=carros_que_salen.end();carro++){
	      inset_lane.push_back((*carro).Get_Inset_Lane());
	      outset_lane.push_back((*carro).Get_Outset_Lane());
	      travel_time.push_back((*carro).Get_Tiempo_Simulacion());
	      simulation_time.push_back(tt);
	      flujo.push_back(flux);
	      densidad.push_back(density);
	    }
	  }
	}

	std::ofstream datafile;
	std::ostringstream data_file_name;
	std::string name_file;

	for(int kk=0;kk<(*Carrera).Get_Cantidad_Carriles(tipo_carril);kk++){
	  data_file_name.str("");
	  data_file_name.clear();
	  //Crear archivo con los datos
	  double true_rho=(*Carrera).Get_Rho(tipo_carril,true);
	  data_file_name<<folder<<"/Inset_Lane_"<<kk<<"_Largo_"<<(*Carrera).Get_Largo(tipo_carril)<<"_Time_"<<tmax<<"_Densidad_"<<std::setprecision(2)<<true_rho<<"_Amabilidad_"<<std::setprecision(2)<<Amabilidad<<".txt";
	  //data_file_name <<folder<<"/Test"<<"_Time_"<<Time.str()<<".txt";
	  name_file=data_file_name.str();
	  if(conf>0){
	    datafile.open(name_file.c_str(),std::ios_base::app); 
	  }
	  else{
	    datafile.open(name_file.c_str()); 
	  }

	  for(int ii=0;ii<inset_lane.size();ii++){
	    if(inset_lane[ii]==kk){
	      datafile<<inset_lane[ii]<<" "<<outset_lane[ii]<<" "<<travel_time[ii]<<" "<<simulation_time[ii]<<" "<<flujo[ii]<<" "<<densidad[ii]<<"\n";
	    }
	  }

	  datafile.close();
	}
      }
      else{
	int test;
	std::cout<<"No se pudo detener el carro"<<std::endl;
	std::cin>>test;
      }
    }
  }
}

