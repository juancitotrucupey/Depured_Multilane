#include "List_Multilane.h"

//______________________Clase Carril____________________________________________________________

//__________________Fijar propiedades___________________________________________________________
//Set iterators a los primeros vecinos
void Carril::Set_Iterators_To_Special_Cases(void){

  if(Carril_Lateral_Extremo.empty()){
    Carro carro;
    Carril_Lateral_Extremo.push_back(carro);
  }
  No_Existe_Carril_Vecino=Carril_Lateral_Extremo.begin();

  if(Carril_Vacio.empty()){
    Carro carro;
    Carril_Vacio.push_back(carro);
  }
  Carril_Vecino_Vacio=Carril_Vacio.begin();

  if(Condiciones_De_Frontera_Entrada.empty()){
    Carro carro;
    Condiciones_De_Frontera_Entrada.push_back(carro);
  }
  Carro_Frontera_Entrada=Condiciones_De_Frontera_Entrada.begin();

  if(Condiciones_De_Frontera_Salida.empty()){
    Carro carro;
    Condiciones_De_Frontera_Salida.push_back(carro);
  }
  Carro_Frontera_Salida=Condiciones_De_Frontera_Salida.begin();
}

//Fijar pointers de los carros
void Carril::Set_Pointer_Carril_Carros(void){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Set_Pointer_Carril(Line);
  }
}
void Carril::Set_Pointer_Avenida_Carros(){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Set_Pointer_Avenida(Carrera);
  }
}    
//__________________Organizar los carros del carril_____________________________________________
    
void Carril::Organizar_Carril(void){
  //Organizar los carros segun sus posiciones
  Carril_1.sort(Carril::Comparar_Posiciones);
  //Crear indicadores de siguiente y anterior
  std::list<Carro>::iterator carro,carro_a,carro_b;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    if(carro!=Carril_1.begin()){
      carro_a=carro;
      --carro_a;
      (*carro).Set_Carro_Atras(carro_a);
    }
    carro_b=carro;
    ++carro_b;
    if(carro_b!=Carril_1.end()){
      (*carro).Set_Carro_Adelante(carro_b);
    }
  }
	
  if(!Carril_1.empty()){
    carro_a=Carril_1.begin();
    carro_b=Carril_1.end();
    --carro_b;
    if(Fronteras_Periodicas){
      Primer_Carro=carro_a;
      Ultimo_Carro=carro_b;
      (*Primer_Carro).Set_Carro_Atras(Ultimo_Carro);
      (*Ultimo_Carro).Set_Carro_Adelante(Primer_Carro);
    }
    else{
      Primer_Carro=carro_a;
      Ultimo_Carro=carro_b;
      (*Primer_Carro).Set_Carro_Atras(Carro_Frontera_Entrada);
      (*Ultimo_Carro).Set_Carro_Adelante(Carro_Frontera_Salida);
    }
  }
}

void Carril::Rearrange_Positions(void){
  if(!Fronteras_Periodicas){
    std::list<Carro>::iterator carro;
    int position;
    for(carro=Carril_1.begin();carro!=Carril_1.end();carro++){
      position=(*carro).Get_Position();
      position-=Largo_Entrada;
      (*carro).Set_Position(position);
    }
    position=(*Carro_Frontera_Entrada).Get_Position();
    position-=Largo_Entrada;
    (*Carro_Frontera_Entrada).Set_Position(position);
    position=(*Carro_Frontera_Salida).Get_Position();
    position-=Largo_Entrada;
    (*Carro_Frontera_Salida).Set_Position(position);
  }
}

//____________________Posiciones iniciales_____________________________________________

//El arreglo carros[] tiene la informacion de todos los carros
void Carril::Posiciones_Iniciales(Carro * carros, int cantidad_carros, int &contador_id){
  Carril::Borrar_Carros();
  int largo_real;
  if(Fronteras_Periodicas){
    largo_real=Largo;
  }
  else{
    largo_real=Largo_Entrada+Largo+Largo_Salida;
  }
  //bool posiciones_ocupadas[largo_real]={false};
  std::vector<bool> posiciones_ocupadas(largo_real,false);
  int casilla,siguiente,anterior,casillas_ocupadas;
  for(int ii=0,casillas_ocupadas=0;(ii<cantidad_carros)&&(casillas_ocupadas<largo_real);ii++){
    Carril_1.push_back(carros[ii]);
    (Carril_1.back()).Set_Id(contador_id);
    (Carril_1.back()).Set_Vel_Max(VELMAX);
    (Carril_1.back()).Set_Tiempo_Simulacion(0);
    (Carril_1.back()).Set_Inset_Lane(Numero_De_Carril);
    (Carril_1.back()).Set_Outset_Lane(-1);
    contador_id++;
    casilla=(Carril_1.back()).Get_Position();
    siguiente=(casilla+1)%largo_real;//Condiciones de fronteras periodicas
    anterior=(casilla+largo_real-1)%largo_real;//Condiciones de fronteras periodicas
    posiciones_ocupadas[casilla]=true;
    casillas_ocupadas+=1;
    if(!posiciones_ocupadas[anterior]){
      casillas_ocupadas+=1;
      posiciones_ocupadas[anterior]=true;
    }
    if(!posiciones_ocupadas[siguiente]){
      casillas_ocupadas+=1;
      posiciones_ocupadas[siguiente]=true;	
    }	
  }
  Carril::Organizar_Carril();
  Carril::Fijar_Carros_Condiciones_De_Frontera();
  Carril::Rearrange_Positions();
  Carril::Set_Pointer_Carril_Carros();
  Carril::Set_Pointer_Avenida_Carros();
}

//Se dan posiciones iniciales aleatorias y velocidades iniciales tales que los carros no se choquen
void Carril::Posiciones_Iniciales_Aleatorias(int cantidad_carros, Crandom & ran2, int& contador_id){
  Carril::Borrar_Carros();
  int largo_real;
  if(Fronteras_Periodicas){
    largo_real=Largo;
  }
  else{
    largo_real=Largo_Entrada+Largo+Largo_Salida;
  }
  //bool posiciones_ocupadas[largo_real]={false};
  std::vector<bool> posiciones_ocupadas(largo_real,false);
  int casilla,siguiente,anterior,casillas_ocupadas;
  Carro carro_nuevo;
  for(int ii=0,casillas_ocupadas=0;(ii<cantidad_carros)&&(casillas_ocupadas<largo_real);ii++){
    casilla=(int)(ran2.r()*double(largo_real));
    siguiente=(casilla+1)%largo_real;//Condiciones de fronteras periodicas
    anterior=(casilla+largo_real-1)%largo_real;//Condiciones de fronteras periodicas
    if(!posiciones_ocupadas[casilla]){
      Carril_1.push_back(carro_nuevo);
      (Carril_1.back()).Set_Position(casilla);
      posiciones_ocupadas[casilla]=true;
      casillas_ocupadas+=1;
      if(!posiciones_ocupadas[anterior]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[anterior]=true;
      }
      if(!posiciones_ocupadas[siguiente]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[siguiente]=true;	
      }	
    }	  
    else{
      while(posiciones_ocupadas[casilla]){
	casilla=(casilla+1)%largo_real;
	siguiente=(casilla+1)%largo_real;//Condiciones de fronteras periodicas
	anterior=(casilla+largo_real-1)%largo_real;//Condiciones de fronteras periodicas
      }
      Carril_1.push_back(carro_nuevo);
      (Carril_1.back()).Set_Position(casilla);
      posiciones_ocupadas[casilla]=true;
      casillas_ocupadas+=1;
      if(!posiciones_ocupadas[anterior]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[anterior]=true;
      }
      if(!posiciones_ocupadas[siguiente]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[siguiente]=true;	
      }
    }
  }    
	
  Carril::Organizar_Carril();
  Carril::Fijar_Carros_Condiciones_De_Frontera();

  //Asignar los valores iniciales de las variables internas
  std::list<Carro>::iterator carro,siguiente_carro;
  int vel,posicion,posicion_siguiente;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    siguiente_carro=(*carro).Get_Carro_Adelante();
    posicion=(*carro).Get_Position();
    posicion_siguiente=(*siguiente_carro).Get_Position();
    if(posicion_siguiente>posicion){
      vel=(int)(ran2.r()*(std::min(posicion_siguiente-posicion-2,VELMAX)));
    }
    else{
      vel=(int)(ran2.r()*(std::min(posicion_siguiente+largo_real-posicion-2,VELMAX)));
    }
    (*carro).Set_Velocity(vel);
    (*carro).Set_Brake(false);
    (*carro).Set_Delay(0);
    (*carro).Set_Id(contador_id);
    (*carro).Set_Tiempo_Estacionarias(0);
    (*carro).Set_Estacionarias(false);
    (*carro).Set_Tiempo_Simulacion(0);
    (*carro).Set_Vel_Max(VELMAX);
    (*carro).Set_Inset_Lane(Numero_De_Carril);
    (*carro).Set_Outset_Lane(-1);
    (*carro).Set_Stoping_Safely(false);
    (*carro).Reinicie_Interaccion_Multicarril();
    contador_id++;
  }
  Carril::Rearrange_Positions();
  Carril::Set_Pointer_Carril_Carros();
  Carril::Set_Pointer_Avenida_Carros();
}

//Se dan posiciones iniciales aleatorias y velocidades iniciales tales que los carros no se choquen
void Carril::Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(int cantidad_carros, Crandom & ran2, int& contador_id, Carro * carros_estaticos, int cantidad_carros_estaticos){
  Carril::Borrar_Carros();
  int largo_real;
  if(Fronteras_Periodicas){
    largo_real=Largo;
  }
  else{
    largo_real=Largo_Entrada+Largo+Largo_Salida;
  }
  //bool posiciones_ocupadas[largo_real]={false};
  std::vector<bool> posiciones_ocupadas(largo_real,false);
  int casilla,anterior,siguiente,casillas_ocupadas;
  //Crear los carros estaticos
  for(int ii=0,casillas_ocupadas=0;(ii<cantidad_carros_estaticos)&&(casillas_ocupadas<largo_real);ii++){
    Carril_1.push_back(carros_estaticos[ii]);
    (Carril_1.back()).Set_Estacionarias(true);
    casilla=(Carril_1.back()).Get_Position();
    anterior=(casilla+largo_real-1)%largo_real;//Condiciones de frontera periodicas
    siguiente=(casilla+1)%largo_real;//Condiciones de frontera periodicas
    posiciones_ocupadas[casilla]=true;
    casillas_ocupadas+=1;
    if(!posiciones_ocupadas[anterior]){
      casillas_ocupadas+=1;
      posiciones_ocupadas[anterior]=true;
    }
    if(!posiciones_ocupadas[siguiente]){
      casillas_ocupadas+=1;
      posiciones_ocupadas[siguiente]=true;	
    }
  }
    
  Carro carro_nuevo;
  for(int ii=0;(ii<cantidad_carros-cantidad_carros_estaticos)&&(casillas_ocupadas<largo_real);ii++){
    casilla=(int)(ran2.r()*double(largo_real));
    siguiente=(casilla+1)%largo_real;//Condiciones de fronteras periodicas
    anterior=(casilla+largo_real-1)%largo_real;//Condiciones de fronteras periodicas
    if(!posiciones_ocupadas[casilla]){
      Carril_1.push_back(carro_nuevo);
      (Carril_1.back()).Set_Position(casilla);
      (Carril_1.back()).Set_Estacionarias(false);
      posiciones_ocupadas[casilla]=true;
      casillas_ocupadas+=1;
      if(!posiciones_ocupadas[anterior]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[anterior]=true;
      }
      if(!posiciones_ocupadas[siguiente]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[siguiente]=true;	
      }
    }  
    else{
      while(posiciones_ocupadas[casilla]){
	casilla=(casilla+1)%largo_real;
	siguiente=(casilla+1)%largo_real;//Condiciones de fronteras periodicas
	anterior=(casilla+largo_real-1)%largo_real;//Condiciones de fronteras periodicas
      }
      Carril_1.push_back(carro_nuevo);
      (Carril_1.back()).Set_Position(casilla);
      (Carril_1.back()).Set_Estacionarias(false);
      posiciones_ocupadas[casilla]=true;
      casillas_ocupadas+=1;
      if(!posiciones_ocupadas[anterior]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[anterior]=true;
      }
      if(!posiciones_ocupadas[siguiente]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[siguiente]=true;	
      }
    }
  }    

  Carril::Organizar_Carril();
  Carril::Fijar_Carros_Condiciones_De_Frontera();

  //Asignar las velocidades iniciales
  std::list<Carro>::iterator carro,siguiente_carro;
  int vel,posicion,posicion_siguiente;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    if((*carro).Get_Estacionarias()){
      vel=0;
      (*carro).Set_Brake(true);
      (*carro).Set_Estacionarias(true);
    }
    else{
      siguiente_carro=(*carro).Get_Carro_Adelante();
      posicion=(*carro).Get_Position();
      posicion_siguiente=(*siguiente_carro).Get_Position();
      if(posicion_siguiente>posicion){
	vel=(int)(ran2.r()*(std::min(posicion_siguiente-posicion-2,VELMAX)));
      }
      else{
	vel=(int)(ran2.r()*(std::min(posicion_siguiente+largo_real-posicion-2,VELMAX)));
      }
      (*carro).Set_Brake(false);
      (*carro).Set_Estacionarias(false);
    }
    (*carro).Set_Velocity(vel);
    (*carro).Set_Delay(0);
    (*carro).Set_Id(contador_id);
    (*carro).Set_Tiempo_Estacionarias(0);
    (*carro).Set_Tiempo_Simulacion(0);
    (*carro).Set_Vel_Max(VELMAX);
    (*carro).Set_Inset_Lane(Numero_De_Carril);
    (*carro).Set_Outset_Lane(-1);
    (*carro).Set_Stoping_Safely(false);
    (*carro).Reinicie_Interaccion_Multicarril();
    contador_id++;
  }
    
  Carril::Rearrange_Positions();
  Carril::Set_Pointer_Carril_Carros();
  Carril::Set_Pointer_Avenida_Carros();
}

//___________________Dinamica del sistema______________________________________________

void Carril::Fijar_Carros_Condiciones_De_Frontera(void){
  if(!Fronteras_Periodicas){
    int punto_entrada,punto_salida;
    punto_entrada=-Largo_Entrada;
    punto_salida=Largo+Largo_Salida;
    std::list<Carro_Sencillo>::iterator carro_frontera_entrada,carro_frontera_salida;
    int posicion,velocidad,delay;
    bool brake;
    int posicion_adelante,velocidad_adelante,gap_posicion_adelante,gap_velocidad_adelante;
    bool brake_ahead;
    int posicion_atras,velocidad_atras,gap_posicion_atras,gap_velocidad_atras;
    bool brake_back;

    carro_frontera_salida=(*Carril_Frontera_Salida).Carro_Condiciones_De_Frontera_Salida;
    carro_frontera_entrada=(*Carril_Frontera_Entrada).Ultimo_Carro;

    if(!Carril_1.empty()){
      posicion=std::min((*Primer_Carro).Get_Position()-((*carro_frontera_entrada).Get_Gap_Posicion_Adelante()+2),punto_entrada);
    }
    else{
      posicion=punto_entrada-((*carro_frontera_entrada).Get_Gap_Posicion_Adelante()+2);
    }

    velocidad=(*carro_frontera_entrada).Get_Velocity();
    delay=(*carro_frontera_entrada).Get_Delay();
    brake=(*carro_frontera_entrada).Get_Brake();
    brake_back=(*carro_frontera_entrada).Get_Brake_Back();
    gap_posicion_atras=(*carro_frontera_entrada).Get_Gap_Posicion_Atras()+1;
    gap_velocidad_atras=(*carro_frontera_entrada).Get_Gap_Velocidad_Atras();
    posicion_atras=posicion-(gap_posicion_atras+1);
    velocidad_atras=velocidad-(gap_velocidad_atras);

    (*Carro_Frontera_Entrada).Set_Velocity(velocidad);
    (*Carro_Frontera_Entrada).Set_Brake(brake);
    (*Carro_Frontera_Entrada).Set_Delay(delay);
    (*Carro_Frontera_Entrada).Set_Position(posicion);
    (*Carro_Frontera_Entrada).Calcule_Gap_Atras(posicion_atras,velocidad_atras);
    (*Carro_Frontera_Entrada).Set_Brake_Back(brake_back);
    (*Carro_Frontera_Entrada).Reinicie_Interaccion_Multicarril();
    (*Carro_Frontera_Entrada).Set_Estacionarias(false);
    (*Carro_Frontera_Entrada).Set_Stoping_Safely(false);
    (*Carro_Frontera_Entrada).Set_Pointer_Carril(Line);
    (*Carro_Frontera_Entrada).Set_Pointer_Avenida(Carrera);
    (*Carro_Frontera_Entrada).Set_Id(-1);
		
    posicion=punto_salida+(*carro_frontera_salida).Get_Gap_Posicion_Atras()+1;
    velocidad=(*carro_frontera_salida).Get_Velocity();
    delay=(*carro_frontera_salida).Get_Delay();
    brake=(*carro_frontera_salida).Get_Brake();
    brake_ahead=(*carro_frontera_salida).Get_Brake_Ahead();
    gap_posicion_adelante=(*carro_frontera_salida).Get_Gap_Posicion_Adelante()+1;
    gap_velocidad_adelante=(*carro_frontera_salida).Get_Gap_Velocidad_Adelante();
    posicion_adelante=(gap_posicion_adelante+1)+posicion;
    velocidad_adelante=gap_velocidad_adelante+velocidad;

    (*Carro_Frontera_Salida).Set_Velocity(velocidad);
    (*Carro_Frontera_Salida).Set_Brake(brake);
    (*Carro_Frontera_Salida).Set_Delay(delay);
    (*Carro_Frontera_Salida).Set_Position(posicion);
    (*Carro_Frontera_Salida).Calcule_Gap_Adelante(posicion_adelante,velocidad_adelante);
    (*Carro_Frontera_Salida).Set_Brake_Ahead(brake_ahead);
    (*Carro_Frontera_Salida).Reinicie_Interaccion_Multicarril();
    (*Carro_Frontera_Entrada).Set_Estacionarias(false);
    (*Carro_Frontera_Entrada).Set_Stoping_Safely(false);
    (*Carro_Frontera_Salida).Set_Pointer_Carril(Line);
    (*Carro_Frontera_Salida).Set_Pointer_Avenida(Carrera);
    (*Carro_Frontera_Salida).Set_Id(-2);

    if(!Carril_1.empty()){
      (*Ultimo_Carro).Set_Carro_Adelante(Carro_Frontera_Salida);
      (*Primer_Carro).Set_Carro_Atras(Carro_Frontera_Entrada);
      (*Carro_Frontera_Entrada).Set_Carro_Adelante(Primer_Carro);
      (*Carro_Frontera_Salida).Set_Carro_Atras(Ultimo_Carro);
    }
    else{
      (*Carro_Frontera_Salida).Set_Carro_Atras(Carro_Frontera_Entrada);
      (*Carro_Frontera_Entrada).Set_Carro_Adelante(Carro_Frontera_Salida);
    }
  }
  else{
    (*Carro_Frontera_Salida).Set_Position(Largo+20);
    (*Carro_Frontera_Entrada).Set_Position(-20);
  }
}
    
void Carril::Intercambiar_Informacion_Y_Calcular_Gaps(void){

  std::list<Carro>::iterator carro,carro_siguiente,carro_anterior;
  int posicion,velocidad,delay;
  bool freno;
  //Carros en la zona de simulacion
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    carro_siguiente=(*carro).Get_Carro_Adelante();
    posicion=(*carro_siguiente).Get_Position();
    if((posicion<(*carro).Get_Position())||(carro==carro_siguiente)){
      posicion+=Largo;
    }
    freno=(*carro_siguiente).Get_Brake();
    velocidad=(*carro_siguiente).Get_Velocity();
    (*carro).Calcule_Gap_Adelante(posicion,velocidad);
    (*carro).Set_Brake_Ahead(freno);
	    
    carro_anterior=(*carro).Get_Carro_Atras();
    posicion=(*carro_anterior).Get_Position();
    if((posicion>(*carro).Get_Position())||(carro==carro_siguiente)){
      posicion-=Largo;
    }
    freno=(*carro_anterior).Get_Brake();
    velocidad=(*carro_anterior).Get_Velocity();
    delay=(*carro_anterior).Get_Delay();
    (*carro).Calcule_Gap_Atras(posicion,velocidad);
    (*carro).Set_Brake_Back(freno);
    (*carro).Set_Delay_Back(delay);
  }
  if(!Fronteras_Periodicas){
    //Carros de las fronteras
    carro_siguiente=(*Carro_Frontera_Entrada).Get_Carro_Adelante();
    posicion=(*carro_siguiente).Get_Position();
    if(posicion<(*Carro_Frontera_Entrada).Get_Position()){
      posicion+=Largo;
    }
    freno=(*carro_siguiente).Get_Brake();
    velocidad=(*carro_siguiente).Get_Velocity();
    (*Carro_Frontera_Entrada).Calcule_Gap_Adelante(posicion,velocidad);
    (*Carro_Frontera_Entrada).Set_Brake_Ahead(freno);
	    
    carro_anterior=(*Carro_Frontera_Salida).Get_Carro_Atras();
    posicion=(*carro_anterior).Get_Position();
    if(posicion>(*Carro_Frontera_Salida).Get_Position()+1){
      posicion-=Largo;
    }
    freno=(*carro_anterior).Get_Brake();
    velocidad=(*carro_anterior).Get_Velocity();
    delay=(*carro_anterior).Get_Delay();
    (*Carro_Frontera_Salida).Calcule_Gap_Atras(posicion,velocidad);
    (*Carro_Frontera_Salida).Set_Brake_Back(freno);
    (*Carro_Frontera_Salida).Set_Delay_Back(delay);
  }
}

void Carril::Decide_Change_Lines_And_Direccionales(int tipo,bool direccionales,Crandom & ran2){
  std::list<Carro>::iterator carro;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Decide_Change_Lines_And_Direccionales(tipo,direccionales,ran2);
  }
}

void Carril::Decide_Ceder_El_Paso(Crandom & ran2,double amabilidad){
  std::list<Carro>::iterator carro;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Decide_Ceder_El_Paso(ran2,amabilidad);
  }
}

//Los carros calculan la velocidad en el tiempo t
void Carril::Evolucionar_Variables_Internas_Carros(void){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Evolucionar_Variables_Internas();
  }
}

//Los carros se mueven
void Carril::Mover_Carros(void){
  //Usando iteradores 
  int next_position,next_position_adelante;
  if(Fronteras_Periodicas){
    std::list<Carro>::iterator carro,carro_adelante,carro_atras;
    Carro nuevo_carro;
    //Testing for collision
    for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
      next_position=((*carro).Get_Position()+(*carro).Get_Velocity())%Largo;
      carro_adelante=(*carro).Get_Carro_Adelante();
      next_position_adelante=((*carro_adelante).Get_Position()+(*carro_adelante).Get_Velocity())%Largo;
      if(Carril::Carros_Se_Chocan((*carro).Get_Position(),next_position,
				  (*carro_adelante).Get_Position(),next_position_adelante)){
	if(Carril_1.size()>1){
	  int testeo;
	  std::cout<<"Los carros se chocan"<<std::endl; 
	  std::cout<<(*carro).Get_Id()<<" "<<(*carro_adelante).Get_Id()<<std::endl;
	  std::cout<<(*carro).Get_Position()<<" "<<(*carro_adelante).Get_Position()<<std::endl;
	  std::cout<<(*carro).Get_Velocity()<<" "<<(*carro_adelante).Get_Velocity()<<std::endl;
	  std::cout<<Numero_De_Carril<<std::endl;
	  std::cin>>testeo;
	  std::cout<<" "<<std::endl;
	}
      }
    }
    //Mover Carros
    for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
      next_position=((*carro).Get_Position()+(*carro).Get_Velocity())%Largo;
      (*carro).Set_Position(next_position);
      (*carro).Reinicie_Interaccion_Multicarril();
    }
    if(!Carril_1.empty()){
      carro=Ultimo_Carro;
      carro_atras=(*carro).Get_Carro_Atras();
      while((*carro_atras).Get_Position()>(*carro).Get_Position()){
	nuevo_carro=(*carro);
	Carril::Eliminar_Carro(carro);
	Carril::Agregar_Carro(Carril_1.begin(),nuevo_carro);
	carro=Ultimo_Carro;
	carro_atras=(*carro).Get_Carro_Atras();
      }
    }
    //std::cout<<"Reorganizar"<<std::endl;
  }
  else{
    std::list<Carro>::iterator carro,carro_adelante;
    int next_position,next_position_adelante;
    //Testing for collision
    for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
      next_position=(*carro).Get_Position()+(*carro).Get_Velocity();
      carro_adelante=(*carro).Get_Carro_Adelante();
      next_position_adelante=(*carro_adelante).Get_Position()+(*carro_adelante).Get_Velocity();
      if(carro!=Ultimo_Carro){
	if(Carril::Carros_Se_Chocan((*carro).Get_Position(),next_position,
				    (*carro_adelante).Get_Position(),next_position_adelante)){
	  if(Carril_1.size()>1){
	    int testeo;
	    std::cout<<"Los carros se chocan"<<std::endl;
	    std::cout<<(*carro).Get_Id()<<" "<<(*carro_adelante).Get_Id()<<std::endl;
	    std::cout<<(*carro).Get_Position()<<" "<<(*carro_adelante).Get_Position()<<std::endl;
	    std::cout<<(*carro).Get_Velocity()<<" "<<(*carro_adelante).Get_Velocity()<<std::endl;
	    std::cout<<Numero_De_Carril<<std::endl;
	    if((*carro).Get_Position()<Largo){
	      std::cin>>testeo;
	    }
	    std::cout<<" "<<std::endl;
	  }
	}
      }
    }
    //Mover Carros
    for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
      next_position=(*carro).Get_Position()+(*carro).Get_Velocity();
      if(next_position>=(*Line).Get_Largo()){
	(*carro).Set_Outset_Lane(Numero_De_Carril);
      }
      if(((*carro).Get_Position()<0)&&(next_position>=0)){
	(*carro).Set_Inset_Lane(Numero_De_Carril); 
	(*carro).Set_Outset_Lane(-1); 
	(*carro).Set_Tiempo_Simulacion(0);
      }
      (*carro).Set_Position(next_position);
      (*carro).Reinicie_Interaccion_Multicarril();
    }
  }
}

void Carril::Interaccion_Con_Carriles_Frontera(void){
  if(!Fronteras_Periodicas){
    std::list<Carro>::iterator carro,carro_adelante;
    std::list<Carro_Sencillo>::iterator carro_sencillo;
    Carro carro_nuevo;
    int posicion,velocidad,velocidad_2,delay,id,position_ahead,velocity_ahead,gap,punto_entrada;
    bool freno,brake_ahead;
    punto_entrada=-Largo_Entrada;
    //Eliminar los carros que salen del carril
    if(!Carril_1.empty()){
      carro=Ultimo_Carro;
      while(Carril_1.begin()!=Carril_1.end()){
	if((*carro).Get_Position()>=Largo+Largo_Salida){
	  id=(*carro).Get_Id();
	  (*Carrera).Add_Id_For_Cars(id);
	  Carros_Que_Salen.push_back((*carro));
	  Carril::Eliminar_Carro(carro);
	  carro=Ultimo_Carro;
	}
	else{
	  break;
	}
      }
    }

    carro_sencillo=(*Carril_Frontera_Entrada).Ultimo_Carro;
    if(!Carril_1.empty()){
      posicion=std::min((*Primer_Carro).Get_Position()-((*carro_sencillo).Get_Gap_Posicion_Adelante()+2),punto_entrada);
    }
    else{
      posicion=punto_entrada-((*carro_sencillo).Get_Gap_Posicion_Adelante()+2);
    }
    while(carro_sencillo!=(*Carril_Frontera_Entrada).Primer_Carro){
      if(posicion>=punto_entrada-Solapamiento_Con_Frontera_Entrada){
	carro=Primer_Carro;
	velocidad=(*Carro_Frontera_Entrada).Get_Velocity();
	velocidad_2=velocidad;
	delay=(*Carro_Frontera_Entrada).Get_Delay();
	freno=(*Carro_Frontera_Entrada).Get_Brake();
	id=(*Carrera).Get_Id_For_Car();
	carro_nuevo.Set_Velocity(velocidad);
	carro_nuevo.Set_Brake(freno);
	carro_nuevo.Set_Delay(delay);
	carro_nuevo.Set_Position(posicion);
	carro_nuevo.Set_Id(id);
	carro_nuevo.Set_Tiempo_Estacionarias(0);
	carro_nuevo.Set_Estacionarias(false);
	carro_nuevo.Set_Stoping_Safely(false);
	carro_nuevo.Set_Tiempo_Simulacion(0);
	carro_nuevo.Reinicie_Interaccion_Multicarril();
	carro_nuevo.Set_Inset_Lane(-1);
	carro_nuevo.Set_Outset_Lane(-1);
	carro_nuevo.Set_Stoping_Safely(false);
	carro=Carril::Agregar_Carro(carro,carro_nuevo);
	carro_adelante=(*carro).Get_Carro_Adelante();
	position_ahead=(*carro_adelante).Get_Position();
	velocity_ahead=(*carro_adelante).Get_Velocity();
	brake_ahead=(*carro_adelante).Get_Brake();
	(*carro).Calcule_Gap_Adelante(position_ahead,velocity_ahead);
	gap=(*carro).Get_Gap_Total_Adelante();
	(*carro).Evolucion_Olmos_Munoz(velocidad,delay,freno,gap,brake_ahead);
	velocidad=std::min(velocidad_2,velocidad);
	(*carro).Set_Velocity(velocidad);
	(*carro).Set_Brake(freno);
	(*carro).Set_Delay(delay);
	carro_sencillo=(*carro_sencillo).Get_Carro_Atras();
	posicion-=((*carro_sencillo).Get_Gap_Posicion_Adelante()+2);
      }
      else{
	break;
      }
    }
  }
}


//__________________Funciones para modificar el sistema________________________________
//Globales
void Carril::Borrar_Carros(void){
  Carril_1.clear();
}

void Carril::Borrar_Carros_Que_Salen(void){
  Carros_Que_Salen.clear();
}

void Carril::Set_Cars_Time_Stationary(int time_sta){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro) {
    if((*carro).Get_Estacionarias()){
      (*carro).Set_Tiempo_Estacionarias(time_sta);
    }
  }
}

void Carril::Set_Cars_Inset_Lane(int inset_lane){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro) {
    (*carro).Set_Inset_Lane(inset_lane);
  }
}

void Carril::Set_Cars_Outset_Lane(int outset_lane){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro) {
    (*carro).Set_Outset_Lane(outset_lane);
  }
}

void Carril::Set_Cars_Estacionarias(bool estacionarias){
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro) {
    (*carro).Set_Estacionarias(estacionarias);
  }
}

//Locales
std::list<Carro>::iterator Carril::Agregar_Carro(std::list<Carro>::iterator lugar, Carro carro){
  std::list<Carro>::iterator carro_agregado,carro_anterior,carro_siguiente;
  if(Carril_1.empty()){
    carro_agregado=Carril_1.insert(Carril_1.begin(),carro);
    Primer_Carro=carro_agregado;
    Ultimo_Carro=carro_agregado;
    if(Fronteras_Periodicas){
      (*carro_agregado).Set_Carro_Atras(carro_agregado);
      (*carro_agregado).Set_Carro_Adelante(carro_agregado);
    }
    else{
      (*carro_agregado).Set_Carro_Atras(Carro_Frontera_Entrada);
      (*carro_agregado).Set_Carro_Adelante(Carro_Frontera_Salida);
      (*Carro_Frontera_Entrada).Set_Carro_Adelante(carro_agregado);
      (*Carro_Frontera_Salida).Set_Carro_Atras(carro_agregado);
    }
    (*carro_agregado).Set_Pointer_Carril(Line);
    (*carro_agregado).Set_Pointer_Avenida(Carrera);
    (*carro_agregado).Set_Vel_Max(VELMAX);

    return carro_agregado;
  }
  else{
    if(Fronteras_Periodicas){
      if(lugar!=Carril_1.end()){
	carro_anterior=(*lugar).Get_Carro_Atras();
	carro_siguiente=lugar;
      }
      else{
	carro_anterior=Ultimo_Carro;
	carro_siguiente=Primer_Carro;
      }
    }
    else{
      if(lugar!=Carril_1.end()){
	carro_anterior=(*lugar).Get_Carro_Atras();
	carro_siguiente=lugar;
      }
      else{
	carro_anterior=Ultimo_Carro;
	carro_siguiente=Carro_Frontera_Salida;
      }
    }
    carro_agregado=Carril_1.insert(lugar,carro);
    (*carro_siguiente).Set_Carro_Atras(carro_agregado);
    (*carro_anterior).Set_Carro_Adelante(carro_agregado);
    (*carro_agregado).Set_Carro_Adelante(carro_siguiente);
    (*carro_agregado).Set_Carro_Atras(carro_anterior);

    if((*carro_agregado).Get_Position()<(*Primer_Carro).Get_Position()){
      Primer_Carro=carro_agregado;
    }
    if((*carro_agregado).Get_Position()>(*Ultimo_Carro).Get_Position()){
      Ultimo_Carro=carro_agregado;
    }
    (*carro_agregado).Set_Pointer_Carril(Line);
    (*carro_agregado).Set_Pointer_Avenida(Carrera);
    (*carro_agregado).Set_Vel_Max(VELMAX);
    return carro_agregado;
  }
}

std::list<Carro>::iterator Carril::Eliminar_Carro(std::list<Carro>::iterator lugar){
  std::list<Carro>::iterator carro_adelante,carro_atras;
  carro_adelante=(*lugar).Get_Carro_Adelante();
  carro_atras=(*lugar).Get_Carro_Atras();
  (*carro_adelante).Set_Carro_Atras(carro_atras);
  (*carro_atras).Set_Carro_Adelante(carro_adelante);

  if(Primer_Carro!=Ultimo_Carro){
    if(lugar==Primer_Carro){
      Primer_Carro=carro_adelante;
    }
    if(lugar==Ultimo_Carro){
      Ultimo_Carro=carro_atras;
    }
  }
	
  lugar=Carril_1.erase(lugar);

  return lugar;
}

bool Carril::Stop_Car(int position,int time_estacionarias){
  if(Carril_1.empty()){
    return false;
  }
  else{
    std::list<Carro>::iterator carro;
    carro=Carril::Ultimo_Carro_Antes_De(position,Ultimo_Carro);
    if(((*carro).Get_Position()+(*carro).Get_Velocity()>=position)||
       (((*carro).Get_Velocity()==0)&&(Carril::Distancia_Entre_Carros((*carro).Get_Position(),position)<7))){
      (*carro).Set_Stoping_Safely(true);
      (*carro).Set_Estacionarias(true);
      (*carro).Set_Tiempo_Estacionarias(time_estacionarias);
      return true;
    }
    else{
      return false;
    }
  }
}

    
//________________Imprimir informacion del sistema_____________________________________

//Locales    
bool Carril::Verificar_Casilla_Vacia(int casilla,std::list<Carro>::iterator & lugar_carro){
  if(Carril_1.empty()){
    lugar_carro=Carril_1.begin();
    return true;
  }
  else{
    std::list<Carro>::iterator carro_inicial,carro_siguiente,carro_anterior;
    int posicion_anterior,posicion_siguiente;
    carro_inicial=lugar_carro;

    carro_anterior=Carril::Ultimo_Carro_Antes_De(casilla,carro_inicial);
    posicion_anterior=(*carro_anterior).Get_Position();
    carro_siguiente=(*carro_anterior).Get_Carro_Adelante();
    posicion_siguiente=(*carro_siguiente).Get_Position();
    lugar_carro=carro_siguiente;
	  	
    if((Carril::Distancia_Entre_Carros(posicion_anterior,casilla)>=2)
       &&(Carril::Distancia_Entre_Carros(posicion_siguiente,casilla)>=2)){
      if((carro_anterior==Ultimo_Carro)&&(carro_siguiente==Primer_Carro)){
	if(casilla>posicion_anterior){
	  lugar_carro=Carril_1.end();
	}
	else{
	  lugar_carro=Carril_1.begin();
	}
      }
      else if(carro_siguiente==Carro_Frontera_Salida){
	lugar_carro=Carril_1.end();
      }
      return true;
    }
    else{
      if((Carril::Distancia_Entre_Carros(posicion_siguiente,casilla)>=2)){
	if(carro_anterior==Carro_Frontera_Entrada){
	  lugar_carro=Primer_Carro;					
	}
	else{
	  lugar_carro=carro_anterior;
	}
      }
      else if(carro_siguiente==Carro_Frontera_Salida){
	lugar_carro=Ultimo_Carro;
      }
      return false;
    }
  }
}
    

std::list<Carro>::iterator Carril::Ultimo_Carro_Antes_De(int posicion, std::list<Carro>::iterator carro_inicial){
  if(Carril_1.empty()){
    return Carro_Frontera_Entrada;
  }
  else{
    //if(posicion<0||posicion>=Largo){posicion = Carril::Posicion_Real_En_Carril(posicion);}
    std::list<Carro>::iterator carro_iterator;
    if((carro_inicial==Carro_Frontera_Entrada)||(carro_inicial==Carro_Frontera_Salida)||
       (carro_inicial==No_Existe_Carril_Vecino)||(carro_inicial==Carril_Vecino_Vacio)){
      if(std::abs(posicion-(*Primer_Carro).Get_Position())>
	 std::abs(posicion-(*Ultimo_Carro).Get_Position())){
	carro_iterator=Ultimo_Carro;
      }
      else{
	carro_iterator=Primer_Carro;
      }
    }
    else{
      carro_iterator=carro_inicial;
    } 

    if(std::abs(posicion-(*carro_iterator).Get_Position())>
       std::abs(posicion-(*Primer_Carro).Get_Position())){
      carro_iterator=Primer_Carro;
    }	
    if(std::abs(posicion-(*carro_iterator).Get_Position())>
       std::abs(posicion-(*Ultimo_Carro).Get_Position())){
      carro_iterator=Ultimo_Carro;
    }

    if((*carro_iterator).Get_Position()>=posicion){
      while((*carro_iterator).Get_Position()>=posicion){
	if(carro_iterator==Primer_Carro){
	  return (*carro_iterator).Get_Carro_Atras();
	}
	else{
	  carro_iterator=(*carro_iterator).Get_Carro_Atras();      
	}
      }
      return carro_iterator;	
    }
    else{
      while((*carro_iterator).Get_Position()<posicion){
	if(carro_iterator==Ultimo_Carro){
	  return carro_iterator;
	}
	else{
	  carro_iterator=(*carro_iterator).Get_Carro_Adelante();         
	}
      }
      return (*carro_iterator).Get_Carro_Atras();
    }	
  }
}


int Carril::Distancia_Entre_Carros(int posicion1, int posicion2){
  if(Fronteras_Periodicas){
    if(posicion1<posicion2){
      return std::min(abs(posicion1-posicion2),abs(posicion2-posicion1-Largo));
    }
    else{
      return std::min(abs(posicion1-posicion2),abs(posicion1-posicion2-Largo));	
    }
  }
  else{
    return abs(posicion1-posicion2);
  }
}

int Carril::Posicion_Real_En_Carril(int posicion){
  if(Fronteras_Periodicas){
    if(posicion<0){
      while(posicion<0){
	posicion+=Largo;
      }
    }
    return posicion%Largo;
  }
  else{
    return posicion;
  }
}

bool Carril::Carros_Se_Chocan(int pos_1_before, int pos_1_after, int pos_2_before, int pos_2_after){
  int largo_real;
  if(Fronteras_Periodicas){
    largo_real=Largo;
  }
  else{
    largo_real=Largo_Entrada+Largo+Largo_Salida;
  }
  if(pos_1_after<pos_1_before){
    pos_1_after+=largo_real;
  }
  if(pos_2_after<pos_2_before){
    pos_2_after+=largo_real;
  }
  if(pos_1_before>pos_2_before){
    int position=pos_1_before;
    pos_1_before=pos_2_before;
    pos_2_before=position;
    position=pos_1_after;
    pos_1_after=pos_2_after;
    pos_2_after=position;
  }
  if(pos_1_after+1>=pos_2_after){
    return true;
  }
  else{
    return false;
  }
}

//Globales
double Carril::Get_Flujo_Espacial(void){
  //Usando iteradores 
  double suma=0.0;
  for(std::list<Carro>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    if((*carro).Get_Position()>=0&&(*carro).Get_Position()<Largo){
      suma+=(double)((*carro).Get_Velocity());    
    }
  }
  if(!Carril_1.empty()){
    suma/=((double)(Largo)*0.5);
    return suma;    
  }
  else{
    return 0.0;
  }	    
}


double Carril::Get_Rho(bool simulacion){
  int largo_real;
  if(Fronteras_Periodicas||simulacion){
    largo_real=Largo;
  }
  else{
    largo_real=Largo_Entrada+Largo+Largo_Salida;
  }
  return (2.0*((double)(Carril::Get_Cantidad_Carros(simulacion)))/((double)(largo_real)));
}

int Carril::Get_Cantidad_Carros(bool simulacion){
  int cantidad_carros=0;
  if(simulacion&&(!Fronteras_Periodicas)){
    std::list<Carro>::iterator carro;
    for(carro=Carril_1.begin();carro!=Carril_1.end();carro++){
      if(((*carro).Get_Position()>=0)&&((*carro).Get_Position()<Largo)){
        cantidad_carros+=1;
      }
    }
  }
  else{
    cantidad_carros=Carril_1.size();
  }
  return cantidad_carros;
}

std::list<Carro> Carril::Get_Info_Carros_Que_Salen(void){
  std::list<Carro> carros_que_salen;
  std::list<Carro>::iterator carro;
  for(carro=Carros_Que_Salen.begin();carro!=Carros_Que_Salen.end();carro++){
    if((*carro).Get_Inset_Lane()>=0){
      carros_que_salen.push_back((*carro));
    }  
  }
  Carril::Borrar_Carros_Que_Salen();
  return carros_que_salen;
} 

bool Carril::Cars_Have_Stoped(int punto_inicial,int punto_final){
  std::list<Carro>::iterator carro;
  bool cars_have_stoped=true;
  int amount_of_cars_stoping=0;
  for(carro=Carril_1.begin();(carro!=Carril_1.end())&&(cars_have_stoped);carro++){
    if((*carro).Get_Estacionarias()){
      amount_of_cars_stoping+=1;
      if((*carro).Get_Velocity()>0){
        cars_have_stoped=false;
      }
      else{
        if((*carro).Get_Position()<punto_inicial||(*carro).Get_Position()>punto_final){
          (*carro).Set_Estacionarias(false);
          (*carro).Set_Tiempo_Estacionarias(0);
          (*carro).Set_Stoping_Safely(false);
          cars_have_stoped=false;
        }
      }
    }
  }
  if(amount_of_cars_stoping>0){
    return cars_have_stoped;
  }
  else{
    false;
  }
}

//______________________Clase Carril_Generador_________________________________________________________________

//__________________Fijar propiedades________________________________________________
//Set iterators to special cases
void Carril_Generador::Set_Iterators_To_Special_Cases(void){
  if(Carril_Sin_Carros.empty()){
    Carro_Sencillo carro;
    Carril_Sin_Carros.push_back(carro);
  }
  Carril_Generador_Vacio=Carril_Sin_Carros.begin();
}

//Fijar pointers de los carros
void Carril_Generador::Set_Pointer_Carril_Carros(void){
  for(std::list<Carro_Sencillo>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Set_Pointer_Carril(Line);
  }
}
void Carril_Generador::Set_Pointer_Avenida_Carros(void){
  for(std::list<Carro_Sencillo>::iterator carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    (*carro).Set_Pointer_Avenida(Carrera);
  }
}     
//_________________Organizar los carros del carril___________________________________
    
void Carril_Generador::Organizar_Carril(void){
  //Organizar los carros segun sus posiciones
  Carril_1.sort(Carril_Generador::Comparar_Posiciones);
  //Crear indicadores de siguiente y anterior
  std::list<Carro_Sencillo>::iterator carro,carro_a,carro_b;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    if(carro!=Carril_1.begin()){
      carro_a=carro;
      --carro_a;
      (*carro).Set_Carro_Atras(carro_a);
    }
    carro_a=carro;
    ++carro_a;
    if(carro_a!=Carril_1.end()){
      (*carro).Set_Carro_Adelante(carro_a);
    }
  }
		
  if(!Carril_1.empty()){
    carro_a=Carril_1.begin();
    carro_b=Carril_1.end();
    --carro_b;
    Primer_Carro=carro_a;
    Ultimo_Carro=carro_b;
    (*Primer_Carro).Set_Carro_Atras(Ultimo_Carro);
    (*Ultimo_Carro).Set_Carro_Adelante(Primer_Carro);
  }
  else{
    Primer_Carro=Carril_Generador_Vacio;
    Ultimo_Carro=Carril_Generador_Vacio;
  }
}

//________________Posiciones inciales_________________________________________________
    
void Carril_Generador::Posiciones_Iniciales(Carro_Sencillo * carros, int cantidad_carros, int& contador_id){
  Carril_Generador::Borrar_Carros();
  //bool posiciones_ocupadas[Largo]={false};
  std::vector<bool> posiciones_ocupadas(Largo,false);
  int casilla,siguiente,anterior,casillas_ocupadas;
  for(int ii=0,casillas_ocupadas=0;(ii<cantidad_carros)&&(casillas_ocupadas<Largo);ii++){
    Carril_1.push_back(carros[ii]);
    (Carril_1.back()).Set_Id(contador_id);
    (Carril_1.back()).Set_Vel_Max(VELMAX);
    contador_id++;
    casilla=(Carril_1.back()).Get_Position();
    siguiente=(casilla+1)%Largo;//Condiciones de fronteras periodicas
    anterior=(casilla+Largo-1)%Largo;//Condiciones de fronteras periodicas
    posiciones_ocupadas[casilla]=true;
    casillas_ocupadas+=1;
    if(!posiciones_ocupadas[anterior]){
      casillas_ocupadas+=1;
      posiciones_ocupadas[anterior]=true;
    }
    if(!posiciones_ocupadas[siguiente]){
      casillas_ocupadas+=1;
      posiciones_ocupadas[siguiente]=true;	
    }	
  }
  Carril_Generador::Organizar_Carril();
  Carril_Generador::Set_Pointer_Carril_Carros();
  Carril_Generador::Set_Pointer_Avenida_Carros();
  //Carro que establece las condiciones de frontera
  Carro_Condiciones_De_Frontera_Salida=Primer_Carro;
}

void Carril_Generador::Posiciones_Iniciales_Aleatorias(int cantidad_carros, Crandom & ran2, int& contador_id){
  Carril_Generador::Borrar_Carros();
  //bool posiciones_ocupadas[Largo]={false};
  std::vector<bool> posiciones_ocupadas(Largo,false);
  int casilla,siguiente,anterior,casillas_ocupadas;
  Carro_Sencillo carro_nuevo;
  for(int ii=0,casillas_ocupadas=0;(ii<cantidad_carros)&&(casillas_ocupadas<Largo);ii++){
    casilla=(int)(ran2.r()*double(Largo));
    siguiente=(casilla+1)%Largo;//Condiciones de fronteras periodicas
    anterior=(casilla+Largo-1)%Largo;//Condiciones de fronteras periodicas
    if(!posiciones_ocupadas[casilla]){
      Carril_1.push_back(carro_nuevo);
      (Carril_1.back()).Set_Position(casilla);
      posiciones_ocupadas[casilla]=true;
      casillas_ocupadas+=1;
      if(!posiciones_ocupadas[anterior]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[anterior]=true;
      }
      if(!posiciones_ocupadas[siguiente]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[siguiente]=true;	
      }	
    }	  
    else{
      while(posiciones_ocupadas[casilla]){
	casilla=(casilla+1)%Largo;
	siguiente=(casilla+1)%Largo;//Condiciones de fronteras periodicas
	anterior=(casilla+Largo-1)%Largo;//Condiciones de fronteras periodicas
      }
      Carril_1.push_back(carro_nuevo);
      (Carril_1.back()).Set_Position(casilla);
      posiciones_ocupadas[casilla]=true;
      casillas_ocupadas+=1;
      if(!posiciones_ocupadas[anterior]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[anterior]=true;
      }
      if(!posiciones_ocupadas[siguiente]){
	casillas_ocupadas+=1;
	posiciones_ocupadas[siguiente]=true;	
      }
    }
  }  

  Carril_Generador::Organizar_Carril();

  //Asignar las valores iniciales de las variables internas
  std::list<Carro_Sencillo>::iterator carro,siguiente_carro;
  int vel,posicion,posicion_siguiente;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    siguiente_carro=(*carro).Get_Carro_Adelante();
    posicion=(*carro).Get_Position();
    posicion_siguiente=(*siguiente_carro).Get_Position();
    if(posicion_siguiente>posicion){
      vel=(int)(ran2.r()*(std::min(posicion_siguiente-posicion-2,VELMAX)));
    }
    else{
      vel=(int)(ran2.r()*(std::min(posicion_siguiente+Largo-posicion-2,VELMAX)));
    }
    (*carro).Set_Velocity(vel);
    (*carro).Set_Brake(false);
    (*carro).Set_Delay(0);
    (*carro).Set_Id(contador_id);
    (*carro).Set_Vel_Max(VELMAX);
    contador_id++;
  }

  Carril_Generador::Set_Pointer_Carril_Carros();
  Carril_Generador::Set_Pointer_Avenida_Carros();
  //Carro que establece las condiciones de frontera
  Carro_Condiciones_De_Frontera_Salida=Primer_Carro;
}

void Carril_Generador::Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(int cantidad_carros, Crandom & ran2, int& contador_id, Carro_Sencillo * carros_estaticos, int cantidad_carros_estaticos){
  int test;
  std::cout<<"Los carriles de las fronteras no pueden tener carros estaticos"<<std::endl;
  std::cin>>test;
}


//_______________Dinamica del sistema__________________________________________________
void Carril_Generador::Intercambiar_Informacion_Y_Calcular_Gaps(void){
  //Usando iteradores 
  std::list<Carro_Sencillo>::iterator carro,carro_siguiente,carro_anterior;
  //int siguiente, anterior;
  int posicion,velocidad,delay;
  bool freno;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    //Siguiente carro
    carro_siguiente=(*carro).Get_Carro_Adelante();
    posicion=(*carro_siguiente).Get_Position();
    freno=(*carro_siguiente).Get_Brake();
    velocidad=(*carro_siguiente).Get_Velocity();
    if(posicion<(*carro).Get_Position()){
      posicion+=Largo;
    }
    (*carro).Calcule_Gap_Adelante(posicion,velocidad);
    (*carro).Set_Brake_Ahead(freno);
    //Anterior carro
    carro_anterior=(*carro).Get_Carro_Atras();
    posicion=(*carro_anterior).Get_Position();
    freno=(*carro_anterior).Get_Brake();
    velocidad=(*carro_anterior).Get_Velocity();
    if(posicion>(*carro).Get_Position()+1){
      posicion-=Largo;
    }
    (*carro).Calcule_Gap_Atras(posicion,velocidad);
    (*carro).Set_Brake_Back(freno);
  }
}

//La velocidad del carro cambia siguiendo la dinámica propuesta 
void Carril_Generador::Evolucionar_Variables_Internas_Carros(void){
  //Usando iteradores 
  for(std::list<Carro_Sencillo>::iterator carro = Carril_1.begin(); carro != Carril_1.end(); ++carro) {
    (*carro).Evolucionar_Variables_Internas();
  }
}

//La posicion de cada carro cambia
void Carril_Generador::Mover_Carros(void){
  //Usando iteradores 
  int next_position;
  std::list<Carro_Sencillo>::iterator carro,carro_atras;
  Carro_Sencillo nuevo_carro;
  for(carro=Carril_1.begin();carro!=Carril_1.end();++carro){
    next_position=((*carro).Get_Position()+(*carro).Get_Velocity())%Largo;
    (*carro).Set_Position(next_position);
  }
  if(!Carril_1.empty()){
    carro=Ultimo_Carro;
    carro_atras=(*carro).Get_Carro_Atras();
    while((*carro_atras).Get_Position()>(*carro).Get_Position()){
      nuevo_carro=(*carro);
      Carril_Generador::Eliminar_Carro(carro);
      Carril_Generador::Agregar_Carro(Carril_1.begin(),nuevo_carro);
      carro=Ultimo_Carro;
      carro_atras=(*carro).Get_Carro_Atras();
    }
  }
}

//______________Funciones para modificar el sistema____________________________________
//Globales
void Carril_Generador::Borrar_Carros(void){
  Carril_1.clear();
}

//Locales
std::list<Carro_Sencillo>::iterator Carril_Generador::Agregar_Carro(std::list<Carro_Sencillo>::iterator lugar, Carro_Sencillo carro){
  std::list<Carro_Sencillo>::iterator carro_agregado,carro_anterior,carro_siguiente;
  if(Carril_1.empty()){
    carro_agregado=Carril_1.insert(Carril_1.begin(),carro);
    Primer_Carro=carro_agregado;
    Ultimo_Carro=carro_agregado;
    (*carro_agregado).Set_Carro_Atras(carro_agregado);
    (*carro_agregado).Set_Carro_Adelante(carro_agregado);
    (*carro_agregado).Set_Pointer_Carril(Line);
    (*carro_agregado).Set_Pointer_Avenida(Carrera);
    (*carro_agregado).Set_Vel_Max(VELMAX);
    return carro_agregado;
  }
  else{
    if(lugar!=Carril_1.end()){
      carro_anterior=(*lugar).Get_Carro_Atras();
      carro_siguiente=lugar;
    }
    else{
      carro_anterior=Ultimo_Carro;
      carro_siguiente=Primer_Carro;
    }
    carro_agregado=Carril_1.insert(lugar,carro);
    (*carro_siguiente).Set_Carro_Atras(carro_agregado);
    (*carro_anterior).Set_Carro_Adelante(carro_agregado);
    (*carro_agregado).Set_Carro_Adelante(carro_siguiente);
    (*carro_agregado).Set_Carro_Atras(carro_anterior);		
    if((*carro_agregado).Get_Position()<(*Primer_Carro).Get_Position()){
      Primer_Carro=carro_agregado;
    }
    if((*carro_agregado).Get_Position()>(*Ultimo_Carro).Get_Position()){
      Ultimo_Carro=carro_agregado;
    }
    (*carro_agregado).Set_Pointer_Carril(Line);
    (*carro_agregado).Set_Pointer_Avenida(Carrera);
    (*carro_agregado).Set_Vel_Max(VELMAX);

    return carro_agregado;
  }	
}


std::list<Carro_Sencillo>::iterator Carril_Generador::Eliminar_Carro(std::list<Carro_Sencillo>::iterator lugar){
  std::list<Carro_Sencillo>::iterator carro_adelante,carro_atras;
  carro_adelante=(*lugar).Get_Carro_Adelante();
  carro_atras=(*lugar).Get_Carro_Atras();
  (*carro_adelante).Set_Carro_Atras(carro_atras);
  (*carro_atras).Set_Carro_Adelante(carro_adelante);
	
  if(Primer_Carro!=Ultimo_Carro){
    if(lugar==Primer_Carro){
      Primer_Carro=carro_adelante;
    }		
    if(lugar==Ultimo_Carro){
      Ultimo_Carro=carro_atras;
    }
  }
  lugar=Carril_1.erase(lugar);

  return lugar;
}


//______________Imprimir informacion del sistema_______________________________________
//Locales
int Carril_Generador::Distancia_Entre_Carros(int posicion1, int posicion2){
  if(posicion1<posicion2){
    return std::min(abs(posicion2-posicion1),abs(posicion1+Largo-posicion2));
  }
  else{
    return std::min(abs(posicion1-posicion2),abs(posicion2+Largo-posicion1));	
  }
}


bool Carril_Generador::Verificar_Casilla_Vacia(int casilla,std::list<Carro_Sencillo>::iterator & lugar_carro){
  if(Carril_1.empty()){
    lugar_carro=Carril_1.begin();
    return true;
  }
  else{
    std::list<Carro_Sencillo>::iterator carro,carro_inicial,carro_siguiente,carro_anterior;
    int posicion_anterior,posicion_siguiente;
    carro_inicial=lugar_carro;

    carro_anterior=Carril_Generador::Ultimo_Carro_Antes_De(casilla,carro_inicial);
    carro_siguiente=(*carro_anterior).Get_Carro_Adelante();
    posicion_anterior=(*carro_anterior).Get_Position();
    posicion_siguiente=(*carro_siguiente).Get_Position();
    lugar_carro=carro_siguiente;
    if((Carril_Generador::Distancia_Entre_Carros(posicion_anterior,casilla)>=2)
       &&(Carril_Generador::Distancia_Entre_Carros(posicion_siguiente,casilla)>=2)){
      if((carro_anterior==Ultimo_Carro)&&(carro_siguiente==Primer_Carro)){
	if(casilla>posicion_anterior){
	  lugar_carro=Carril_1.end();
	}
	else{
	  lugar_carro=Carril_1.begin();
	}
      }
      return true;
    }
    else{
      if((Carril_Generador::Distancia_Entre_Carros(posicion_siguiente,casilla)>=2)){
	lugar_carro=carro_anterior;
      }
      return false;
    }
  }
}


std::list<Carro_Sencillo>::iterator Carril_Generador::Ultimo_Carro_Antes_De(int posicion, std::list<Carro_Sencillo>::iterator carro_inicial){
  if(Carril_1.empty()){
    return Carril_Generador_Vacio;
  }
  else{
    if(posicion<0||posicion>=Largo){posicion=Carril_Generador::Posicion_Real_En_Carril(posicion);}
	    
    std::list<Carro_Sencillo>::iterator carro_iterator;
	    
    if(carro_inicial==Carril_Generador_Vacio){
      if(std::abs(posicion-(*Primer_Carro).Get_Position()) > 
	 std::abs(posicion-(*Ultimo_Carro).Get_Position())){
	carro_iterator=Ultimo_Carro;
      }
      else{
	carro_iterator=Primer_Carro;
      }
    }
    else{
      carro_iterator=carro_inicial;
    }
	          
    if((*carro_iterator).Get_Position()>=posicion){
      while((*carro_iterator).Get_Position()>=posicion){
	if(carro_iterator==Primer_Carro){
	  return (*carro_iterator).Get_Carro_Atras();
	}
	else{
	  carro_iterator=(*carro_iterator).Get_Carro_Atras();      
	}
      }
      return carro_iterator;
    }
    else{
      while((*carro_iterator).Get_Position()<posicion){
	if(carro_iterator==Ultimo_Carro){
	  return carro_iterator;
	}
	else{
	  carro_iterator=(*carro_iterator).Get_Carro_Adelante();         
	}
      }
      return (*carro_iterator).Get_Carro_Atras();
    }
  }
}

int Carril_Generador::Posicion_Real_En_Carril(int posicion){
  if(posicion<0){
    while(posicion<0){
      posicion+=Largo;
    }
  }
  return posicion%Largo;
}
 
bool Carril_Generador::Carros_Se_Chocan(int pos_1_before, int pos_1_after, int pos_2_before, int pos_2_after){
  if(pos_1_after<pos_1_before){
    pos_1_after+=Largo;
  }
  if(pos_2_after<pos_2_before){
    pos_2_after+=Largo;
  }
  if(pos_1_before>pos_2_before){
    int position=pos_1_before;
    pos_1_before=pos_2_before;
    pos_2_before=position;
    position=pos_1_after;
    pos_1_after=pos_2_after;
    pos_2_after=position;
  }
  if(pos_1_after+1>=pos_2_after){
    return true;
  }
  else{
    return false;
  }	
}

//Globales
double Carril_Generador::Get_Flujo_Espacial(void){
  //Usando iteradores 
  double suma=0.0;
  for(std::list<Carro_Sencillo>::iterator carro = Carril_1.begin(); carro != Carril_1.end(); ++carro) {
    suma+=(double)(*carro).Get_Velocity();    
  }
  if(!Carril_1.empty()){
    //suma/=(double(Largo)*0.5);
    suma/=Carril_Generador::Get_Cantidad_Carros();
    suma*=Carril_Generador::Get_Rho(true);
    return suma;    
  }
  else{
    return 0.0;
  }	    
}

double Carril_Generador::Get_Rho(bool simulacion){
  return (2.0*double(Carril_Generador::Get_Cantidad_Carros())/(double(Largo)));
}
