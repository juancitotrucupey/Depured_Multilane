#include "List_Multilane.h"

//______________________Fijar propiedades_______________________________________________
void Avenida::Set_Vel_Max(int vel_max){
  VELMAX=vel_max;
  std::vector<Carril>::iterator carril;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Set_Vel_Max(VELMAX);
  }
}

void Avenida::Set_Fronteras_Periodicas(bool fronteras){
  Fronteras_Periodicas=fronteras;
  std::vector<Carril>::iterator carril;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Set_Fronteras_Periodicas(Fronteras_Periodicas);
  }
}    

void Avenida::Set_Largo(int largo){
  std::vector<Carril>::iterator carril;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Set_Largo(largo);
  }
}

void Avenida::Fijar_Dimensiones(int cantidad_carriles, int largo){
  Carril carril_vacio;
  //Borrar los carriles existentes
  Avenida::Borrar_Carriles(Simulacion);
  //Crear los carriles de simulacion
  if(cantidad_carriles>1){
    Avenida_1.push_back(carril_vacio);
    (Avenida_1.back()).Set_Posicion(Izquierdo);
    for(int ii=1;ii<cantidad_carriles-1;ii++){
      Avenida_1.push_back(carril_vacio);
      (Avenida_1.back()).Set_Posicion(Central);
    }
    Avenida_1.push_back(carril_vacio);
    (Avenida_1.back()).Set_Posicion(Derecho);

    std::vector<Carril>::iterator carril,carril_der,carril_izq;
    for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
      if(Izquierdo==(*carril).Posicion){
	(*carril).Carril_Vecino_Izquierdo=No_Existe_Carril_Vecino;
	carril_der=carril;
	++carril_der;
	(*carril).Carril_Vecino_Derecho=carril_der;
      }
      else if(Derecho==(*carril).Posicion){
	(*carril).Carril_Vecino_Derecho=No_Existe_Carril_Vecino;
	carril_izq=carril;
	--carril_izq;
	(*carril).Carril_Vecino_Izquierdo=carril_izq;
      }
      else if(Central==(*carril).Posicion){
	carril_der=carril;
	++carril_der;
	carril_izq=carril;
	--carril_izq;
	(*carril).Carril_Vecino_Derecho=carril_der;
	(*carril).Carril_Vecino_Izquierdo=carril_izq;
      }
    }	
  }
  else{
    Avenida_1.push_back(carril_vacio);
    (Avenida_1.back()).Set_Posicion(Sin_Vecinos);
    (*(Avenida_1.begin())).Carril_Vecino_Izquierdo=No_Existe_Carril_Vecino;
    (*(Avenida_1.begin())).Carril_Vecino_Derecho=No_Existe_Carril_Vecino;
  }
  int numero_carril=0;
  for(std::vector<Carril>::iterator carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Set_Largo(largo);
    (*carril).Set_Fronteras_Periodicas(Fronteras_Periodicas);
    (*carril).Set_Vel_Max(VELMAX);
    (*carril).Set_Numero_De_Carril(numero_carril);
    numero_carril+=1;
  }
}

bool Avenida::Crear_Fronteras(int vel_max, int largo){
  if(vel_max<=VELMAX){
    //Borrar los carriles generadores existentes
    Avenida::Borrar_Carriles(Frontera_Entrada);
    Avenida::Borrar_Carriles(Frontera_Salida);
    //Crear los carriles de las condiciones de frontera
    Carril_Generador carril_generador_vacio;
    for(int ii=0;ii<Avenida_1.size();ii++){
      Carriles_Frontera_Entrada.push_back(carril_generador_vacio);
      Carriles_Frontera_Salida.push_back(carril_generador_vacio);
    }

    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      Carriles_Frontera_Entrada[ii].Set_Largo(largo);
      Carriles_Frontera_Entrada[ii].Set_Vel_Max(vel_max);
      Carriles_Frontera_Salida[ii].Set_Largo(largo);
      Carriles_Frontera_Salida[ii].Set_Vel_Max(vel_max);
    }

    //Asignar los carriles de frontera a los carriles de simulacion
    std::vector<Carril>::iterator carril;
    std::vector<Carril_Generador>::iterator carril_frontera_entrada,carril_frontera_salida;
    carril=Avenida_1.begin();
    carril_frontera_entrada=Carriles_Frontera_Entrada.begin(); 
    carril_frontera_salida=Carriles_Frontera_Salida.begin();
    while(carril!=Avenida_1.end()){
      (*carril).Carril_Frontera_Entrada=carril_frontera_entrada;
      (*carril).Carril_Frontera_Salida=carril_frontera_salida;
      carril++;
      carril_frontera_entrada++;
      carril_frontera_salida++;
    } 
    return true;
  }
  else{
    return false;
  }
}
//Set iterators to special cases
void Avenida::Set_Iterators_To_Special_Cases(void){
  if(None_Carril_Vecino.empty()){
    Carril carril;
    None_Carril_Vecino.push_back(carril);
  }
  No_Existe_Carril_Vecino=None_Carril_Vecino.begin();

  std::vector<Carril>::iterator carril_simulacion;
  for(carril_simulacion=Avenida_1.begin();carril_simulacion!=Avenida_1.end();carril_simulacion++){
    (*carril_simulacion).Set_Iterators_To_Special_Cases();
  }

  std::vector<Carril_Generador>::iterator carril_frontera;
  for(carril_frontera=Carriles_Frontera_Entrada.begin();carril_frontera!=Carriles_Frontera_Entrada.end();carril_frontera++){
    (*carril_frontera).Set_Iterators_To_Special_Cases();
  }	
  for(carril_frontera=Carriles_Frontera_Salida.begin();carril_frontera!=Carriles_Frontera_Salida.end();carril_frontera++){
    (*carril_frontera).Set_Iterators_To_Special_Cases();
  }
}

//Fijar poniters de los carros y pointers de los carriles
void Avenida::Set_Self_Pointer_Carriles(){
  std::vector<Carril>::iterator carril;
  std::vector<Carril_Generador>::iterator carril_gen;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();carril++){
    (*carril).Set_Self_Pointer(&(*carril));
  }
  for(carril_gen=Carriles_Frontera_Entrada.begin();carril_gen!=Carriles_Frontera_Entrada.end();carril_gen++){
    (*carril_gen).Set_Self_Pointer(&(*carril_gen));
  }
  for(carril_gen=Carriles_Frontera_Salida.begin();carril_gen!=Carriles_Frontera_Salida.end();carril_gen++){
    (*carril_gen).Set_Self_Pointer(&(*carril_gen));
  }
}
void Avenida::Set_Pointer_To_Avenida_Carriles(){
  std::vector<Carril>::iterator carril;
  std::vector<Carril_Generador>::iterator carril_gen;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();carril++){
    (*carril).Set_Pointer_Avenida(Carrera);
  }
  for(carril_gen=Carriles_Frontera_Entrada.begin();carril_gen!=Carriles_Frontera_Entrada.end();carril_gen++){
    (*carril_gen).Set_Pointer_Avenida(Carrera);
  }
  for(carril_gen=Carriles_Frontera_Salida.begin();carril_gen!=Carriles_Frontera_Salida.end();carril_gen++){
    (*carril_gen).Set_Pointer_Avenida(Carrera);
  }
}
void Avenida::Set_Pointers_Carriles(){
  Avenida::Set_Self_Pointer_Carriles();
  Avenida::Set_Pointer_To_Avenida_Carriles();
}
void Avenida::Set_Pointers_To_Carril_Carros(){
  std::vector<Carril>::iterator carril;
  std::vector<Carril_Generador>::iterator carril_gen;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();carril++){
    (*carril).Set_Pointer_Carril_Carros();
  }
  for(carril_gen=Carriles_Frontera_Entrada.begin();carril_gen!=Carriles_Frontera_Entrada.end();carril_gen++){
    (*carril_gen).Set_Pointer_Carril_Carros();
  }
  for(carril_gen=Carriles_Frontera_Salida.begin();carril_gen!=Carriles_Frontera_Salida.end();carril_gen++){
    (*carril_gen).Set_Pointer_Carril_Carros();
  }
}
void Avenida::Set_Pointers_To_Avenida_Carros(){
  std::vector<Carril>::iterator carril;
  std::vector<Carril_Generador>::iterator carril_gen;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();carril++){
    (*carril).Set_Pointer_Avenida_Carros();
  }
  for(carril_gen=Carriles_Frontera_Entrada.begin();carril_gen!=Carriles_Frontera_Entrada.end();carril_gen++){
    (*carril_gen).Set_Pointer_Avenida_Carros();
  }
  for(carril_gen=Carriles_Frontera_Salida.begin();carril_gen!=Carriles_Frontera_Salida.end();carril_gen++){
    (*carril_gen).Set_Pointer_Avenida_Carros();
  }
}
void Avenida::Set_Pointers_Carros(){
  Avenida::Set_Pointers_To_Carril_Carros();
  Avenida::Set_Pointers_To_Avenida_Carros();
}

//______________________Posiciones iniciales____________________________________________

//Carriles de la zona de simulacion
void Avenida::Posiciones_Iniciales_Carriles_Simulacion(Carro ** carros, int cantidad_carros[], int& contador_id){
  for(int cc=0;cc<Avenida_1.size();cc++){
    Avenida_1[cc].Posiciones_Iniciales(carros[cc],cantidad_carros[cc],contador_id);
  }
  Id_Disponibles.push_back(contador_id);
}

void Avenida::Posiciones_Iniciales_Aleatorias_Carriles_Simulacion(int cantidad_carros[], Crandom & ran2, int& contador_id){
  for(int cc=0;cc<Avenida_1.size();cc++){
    Avenida_1[cc].Posiciones_Iniciales_Aleatorias(cantidad_carros[cc],ran2,contador_id);
  }
  Id_Disponibles.push_back(contador_id);
}

void Avenida::Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Simulacion(int cantidad_carros[], Crandom & ran2, Carro ** carros_estaticos, int cantidad_carros_estaticos[], int& contador_id){
  for(int cc=0;cc<Avenida_1.size();cc++){
    Avenida_1[cc].Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(cantidad_carros[cc],ran2,contador_id,carros_estaticos[cc],cantidad_carros_estaticos[cc]);
  }
  Id_Disponibles.push_back(contador_id);
}

//Carriles para fijar las condiciones de frontera
//Entrada
void Avenida::Posiciones_Iniciales_Carriles_Frontera_Entrada(Carro_Sencillo ** carros, int cantidad_carros[], int& contador_id){
  for(int cc=0;cc<Carriles_Frontera_Entrada.size();cc++){
    Carriles_Frontera_Entrada[cc].Posiciones_Iniciales(carros[cc],cantidad_carros[cc],contador_id);
  }
}

void Avenida::Posiciones_Iniciales_Aleatorias_Carriles_Frontera_Entrada(int cantidad_carros[], Crandom & ran2, int& contador_id){
  for(int cc=0;cc<Carriles_Frontera_Entrada.size();cc++){
    Carriles_Frontera_Entrada[cc].Posiciones_Iniciales_Aleatorias(cantidad_carros[cc],ran2,contador_id);
  }
}

void Avenida::Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Frontera_Entrada(int cantidad_carros[], Crandom & ran2, Carro_Sencillo ** carros_estaticos, int cantidad_carros_estaticos[], int& contador_id){
  for(int cc=0;cc<Carriles_Frontera_Entrada.size();cc++){
    Carriles_Frontera_Entrada[cc].Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(cantidad_carros[cc],ran2,contador_id,carros_estaticos[cc],cantidad_carros_estaticos[cc]);
  }
}

//Salida
void Avenida::Posiciones_Iniciales_Carriles_Frontera_Salida(Carro_Sencillo ** carros, int cantidad_carros[], int& contador_id){
  for(int cc=0;cc<Carriles_Frontera_Salida.size();cc++){
    Carriles_Frontera_Salida[cc].Posiciones_Iniciales(carros[cc],cantidad_carros[cc],contador_id);
  }
}

void Avenida::Posiciones_Iniciales_Aleatorias_Carriles_Frontera_Salida(int cantidad_carros[], Crandom & ran2, int& contador_id){
  for(int cc=0;cc<Carriles_Frontera_Salida.size();cc++){
    Carriles_Frontera_Salida[cc].Posiciones_Iniciales_Aleatorias(cantidad_carros[cc],ran2,contador_id);
  }
}

void Avenida::Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos_Carriles_Frontera_Salida(int cantidad_carros[], Crandom & ran2, Carro_Sencillo ** carros_estaticos, int cantidad_carros_estaticos[], int& contador_id){
  for(int cc=0;cc<Carriles_Frontera_Salida.size();cc++){
    Carriles_Frontera_Salida[cc].Posiciones_Iniciales_Aleatorias_Con_Carros_Estaticos(cantidad_carros[cc],ran2,contador_id,carros_estaticos[cc],cantidad_carros_estaticos[cc]);
  }
}

//______________________Dinamica del sistema____________________________________________ 

//Carriles de la zona de simulacion
void Avenida::Encontrar_Primeros_Vecinos_Carriles_Simulacion(bool multicarril,bool condiciones_frontera){
  std::vector<Carril>::iterator carril;
  if(condiciones_frontera){
    for(carril=Avenida_1.begin();carril!=Avenida_1.end();carril++){
      (*carril).Fijar_Carros_Condiciones_De_Frontera();
    }
  }
  if(multicarril){
    std::list<Carro>::iterator carro,carro_izquierdo,carro_derecho,carro_atras;
    std::vector<Carril>::iterator carril_der,carril_izq;
    int posicion;
    //Fijar los primeros vecinos 
    for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
      carril_der=(*carril).Carril_Vecino_Derecho;
      carril_izq=(*carril).Carril_Vecino_Izquierdo;
			
      //Vecinos en el carril izquierdo
      if(carril_izq==No_Existe_Carril_Vecino){//Carril lateral izquierdo
	for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	  (*carro).Set_Carro_Izq_Adelante((*carril).No_Existe_Carril_Vecino);
	  (*carro).Set_Carro_Izq_Atras((*carril).No_Existe_Carril_Vecino);
	}
      }
      else{
	if(!(*carril_izq).Carril_1.empty()){
	  carro_izquierdo=(*carril_izq).Primer_Carro;
	  //Encontrar primeros vecinos en el carril izquierdo
	  for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();carro++){
	    posicion=(*carril_izq).Posicion_Real_En_Carril((*carro).Get_Position()+2);
	    carro_atras=(*carril_izq).Ultimo_Carro_Antes_De(posicion,carro_izquierdo);
	    (*carro).Set_Carro_Izq_Atras(carro_atras);
	    carro_izquierdo=(*carro_atras).Get_Carro_Adelante();
	    (*carro).Set_Carro_Izq_Adelante(carro_izquierdo);
	  }
	}
	else{
	  if((*carril_izq).Get_Fronteras_Periodicas()){
	    for(carro = (*carril).Carril_1.begin(); carro != (*carril).Carril_1.end(); ++carro){
	      (*carro).Set_Carro_Izq_Atras((*carril).Carril_Vecino_Vacio);
	      (*carro).Set_Carro_Izq_Adelante((*carril).Carril_Vecino_Vacio);
	    }
	  }
	  else{
	    for(carro = (*carril).Carril_1.begin(); carro != (*carril).Carril_1.end(); ++carro){
	      (*carro).Set_Carro_Izq_Atras((*carril_izq).Carro_Frontera_Entrada);
	      (*carro).Set_Carro_Izq_Adelante((*carril_izq).Carro_Frontera_Salida);
	    }
	  }
	}
      }

      //Vecinos en el carril derecho
      if(carril_der==No_Existe_Carril_Vecino){//Carril lateral derecho
	for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	  (*carro).Set_Carro_Der_Adelante((*carril).No_Existe_Carril_Vecino);
	  (*carro).Set_Carro_Der_Atras((*carril).No_Existe_Carril_Vecino);
	}
      }
      else{
	if(!(*carril_der).Carril_1.empty()){
	  carro_derecho=(*carril_der).Carril_1.begin();
	  //Encontrar primeros vecinos para los carros en el carril derecho
	  for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();carro++){
	    posicion=(*carril_der).Posicion_Real_En_Carril((*carro).Get_Position()+2);
	    carro_atras=(*carril_der).Ultimo_Carro_Antes_De(posicion,carro_derecho);
	    (*carro).Set_Carro_Der_Atras(carro_atras);
	    carro_derecho=(*carro_atras).Get_Carro_Adelante();
	    (*carro).Set_Carro_Der_Adelante(carro_derecho);
	  }
	}
	else{
	  if((*carril_der).Get_Fronteras_Periodicas()){
	    for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	      (*carro).Set_Carro_Der_Atras((*carril).Carril_Vecino_Vacio);
	      (*carro).Set_Carro_Der_Adelante((*carril).Carril_Vecino_Vacio);
	    }
	  }
	  else{
	    for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	      (*carro).Set_Carro_Der_Atras((*carril_der).Carro_Frontera_Entrada);
	      (*carro).Set_Carro_Der_Adelante((*carril_der).Carro_Frontera_Salida);
	    }
	  }
	}
      }	
    }
  }
}

void Avenida::Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Simulacion(bool multicarril){
  std::vector<Carril>::iterator carril,carril_der,carril_izq;
  std::list<Carro>::iterator carro,carro_siguiente,carro_anterior;
  int posicion,velocidad;
  bool freno;
	
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Intercambiar_Informacion_Y_Calcular_Gaps();
  }
  if(multicarril){
    for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
      //Vecinos en el carril izquierdo
      carril_izq=(*carril).Carril_Vecino_Izquierdo;
      for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	carro_siguiente=(*carro).Get_Carro_Izq_Adelante();
	if(carro_siguiente==(*carril).Carril_Vecino_Vacio){
	  posicion=(*carril_izq).Largo+(*carro).Get_Position();
	  freno=false;
	  velocidad=VELMAX;
	  (*carro).Calcule_Gap_Izq_Adelante(posicion,velocidad);
	  (*carro).Set_Brake_Izq_Ahead(freno);
	}
	else if(carro_siguiente==(*carril).No_Existe_Carril_Vecino){
	  (*carro).Calcule_Gap_Izq_Adelante((*carro).Get_Position()+2,0);
	  (*carro).Set_Brake_Izq_Ahead(true);
	}
	else{
	  posicion=(*carro_siguiente).Get_Position();
	  if(posicion<(*carro).Get_Position()){
	    posicion+=(*carril_izq).Largo;
	  }
	  freno=(*carro_siguiente).Get_Brake();
	  velocidad=(*carro_siguiente).Get_Velocity();
	  (*carro).Calcule_Gap_Izq_Adelante(posicion,velocidad);
	  (*carro).Set_Brake_Izq_Ahead(freno);
	}
	    		
	carro_anterior=(*carro).Get_Carro_Izq_Atras();
	if(carro_anterior==(*carril).Carril_Vecino_Vacio){
	  posicion=(*carro).Get_Position()-(*carril_izq).Largo;
	  freno=false;
	  velocidad=0;
	  (*carro).Calcule_Gap_Izq_Atras(posicion,velocidad);
	  (*carro).Set_Brake_Izq_Back(freno);
	}
	else if(carro_anterior==(*carril).No_Existe_Carril_Vecino){
	  (*carro).Calcule_Gap_Izq_Atras((*carro).Get_Position(),0);
	  (*carro).Set_Brake_Izq_Back(true);	
	}
	else{
	  posicion=(*carro_anterior).Get_Position();
	  if(posicion>(*carro).Get_Position()){
	    posicion-=(*carril_izq).Largo;
	  }
	  freno=(*carro_anterior).Get_Brake();
	  velocidad=(*carro_anterior).Get_Velocity();
	  (*carro).Calcule_Gap_Izq_Atras(posicion,velocidad);
	  (*carro).Set_Brake_Izq_Back(freno);
	}
      }
			
      //Vecinos en el carril derecho
      carril_der=(*carril).Carril_Vecino_Derecho;
      for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	carro_siguiente=(*carro).Get_Carro_Der_Adelante();
	if(carro_siguiente==(*carril).Carril_Vecino_Vacio){
	  posicion=(*carril_der).Largo+(*carro).Get_Position();
	  freno=false;
	  velocidad=VELMAX;
	  (*carro).Calcule_Gap_Der_Adelante(posicion,velocidad);
	  (*carro).Set_Brake_Der_Ahead(freno);
	}
	else if(carro_siguiente==(*carril).No_Existe_Carril_Vecino){
	  (*carro).Calcule_Gap_Der_Adelante((*carro).Get_Position()+2,0);
	  (*carro).Set_Brake_Der_Ahead(true);
	}
	else{
	  posicion=(*carro_siguiente).Get_Position();
	  if(posicion<(*carro).Get_Position()){
	    posicion+=(*carril_der).Largo;
	  }
	  freno=(*carro_siguiente).Get_Brake();
	  velocidad=(*carro_siguiente).Get_Velocity();
	  (*carro).Calcule_Gap_Der_Adelante(posicion,velocidad);
	  (*carro).Set_Brake_Der_Ahead(freno);
	}

	carro_anterior=(*carro).Get_Carro_Der_Atras();
	if(carro_anterior==(*carril).Carril_Vecino_Vacio){
	  posicion=(*carro).Get_Position()-(*carril_der).Largo;
	  freno=true;
	  velocidad=0;
	  (*carro).Calcule_Gap_Der_Atras(posicion,velocidad);
	  (*carro).Set_Brake_Der_Back(freno);
	}
	else if(carro_anterior==(*carril).No_Existe_Carril_Vecino){
	  (*carro).Calcule_Gap_Der_Atras((*carro).Get_Position(),0);
	  (*carro).Set_Brake_Der_Back(true);	
	}
	else{
	  posicion=(*carro_anterior).Get_Position();
	  if(posicion>(*carro).Get_Position()){
	    posicion-=(*carril_der).Largo;
	  }
	  freno=(*carro_anterior).Get_Brake();
	  velocidad=(*carro_anterior).Get_Velocity();
	  (*carro).Calcule_Gap_Der_Atras(posicion,velocidad);
	  (*carro).Set_Brake_Der_Back(freno);
	}
      }
    }		
  }
}

void Avenida::Decide_Change_Lines_And_Direccionales_Carriles_Simulacion(int tipo,bool direccionales,Crandom & ran2){
  //Cada carro decide si va a cambiar de carril y guarda esa información en los booleanos
  // bool Change_Lines_Izq , bool Change_Lines_Der;
  //Si el carro quiere cambiar de carril pero no puede entonces activa las direccionales
  // bool Direccional_Derecha, bool Direccional_Izquierda;
  std::vector<Carril>::iterator carril;
  for(carril = Avenida_1.begin(); carril != Avenida_1.end(); ++carril){
    (*carril).Decide_Change_Lines_And_Direccionales(tipo,direccionales,ran2);
  }
}


void Avenida::Decide_Ceder_El_Paso_Carriles_Simulacion(Crandom & ran2,double amabilidad){
  //Los carros reciben la información de los carros de adelante, miran
  //si el carro tiene las direccionales prendidas
  std::vector<Carril>::iterator carril,carril_der,carril_izq;
  std::list<Carro>::iterator carro,anterior_izq,anterior_der,carro_atras;
  int posicion_der,posicion_izq;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    if(!(*carril).Carril_1.empty()){
      carril_der=(*carril).Carril_Vecino_Derecho;
      carril_izq=(*carril).Carril_Vecino_Izquierdo;
      //Ceder el paso a los carros en el carril derecho
      if(No_Existe_Carril_Vecino!=carril_der){
	for(carro=(*carril_der).Carril_1.begin();carro!=(*carril_der).Carril_1.end();++carro){
	  if((*carro).Get_Direccional_Izquierda()&&(!(*carro).Get_Estacionarias())){
	    posicion_der=(*carril_der).Posicion_Real_En_Carril((*carro).Get_Position()-1);
	    carro_atras=(*carro).Get_Carro_Izq_Atras();
	    anterior_izq=(*carril).Ultimo_Carro_Antes_De(posicion_der,carro_atras);
	    (*anterior_izq).Set_Carro_Ceder_El_Paso_Derecha(carro);
	    (*anterior_izq).Set_Ceder_Paso_Derecha(true);
	  }
	}
      }
      //Ceder el paso a los carros en el carril izquierdo
      if(No_Existe_Carril_Vecino!=carril_izq){
	for(carro=(*carril_izq).Carril_1.begin();carro!=(*carril_izq).Carril_1.end();++carro){
	  if((*carro).Get_Direccional_Derecha()&&(!(*carro).Get_Estacionarias())){
	    posicion_izq=(*carril_izq).Posicion_Real_En_Carril((*carro).Get_Position()-1);
	    carro_atras=(*carro).Get_Carro_Der_Atras();
	    anterior_der=(*carril).Ultimo_Carro_Antes_De(posicion_izq,carro_atras);
	    (*anterior_der).Set_Carro_Ceder_El_Paso_Izquierda(carro);
	    (*anterior_der).Set_Ceder_Paso_Izquierda(true);
	  }
	}
      }
    }
  }
  for(carril = Avenida_1.begin(); carril != Avenida_1.end(); ++carril){
    (*carril).Decide_Ceder_El_Paso(ran2,amabilidad);
  }
}

void Avenida::Decide_Change_Lines_After_Asking_For_Way_Carriles_Simulacion(int tipo,Crandom & ran2){
  //Los carros reciben la información de los carros de adelante, miran
  //si el carro tiene las direccionales prendidas
  std::vector<Carril>::iterator carril,carril_der,carril_izq;
  std::list<Carro>::iterator carro,anterior_izq,anterior_der;
  int posicion,velocidad;
  bool freno;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    if(!(*carril).Carril_1.empty()){
      carril_der=(*carril).Carril_Vecino_Derecho;
      carril_izq=(*carril).Carril_Vecino_Izquierdo;
      //Ceder el paso a los carros en el carril derecho
      if(No_Existe_Carril_Vecino!=carril_izq){
	for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	  if((*carro).Get_Direccional_Izquierda()){
	    anterior_izq=(*carro).Get_Carro_Izq_Atras();
	    if((*anterior_izq).Get_Ceder_Paso_Derecha()){
	      velocidad=(*anterior_izq).Get_Velocity();
	      freno=(*anterior_izq).Get_Velocity();
	      posicion=(*anterior_izq).Get_Position();
	      if(posicion>(*carro).Get_Position()){
		posicion-=(*carril_izq).Largo;
	      }
	      (*carro).Calcule_Gap_Izq_Atras(posicion,velocidad);
	      (*carro).Set_Brake_Izq_Back(freno);
	    }
	  }
	}
      }
      //Ceder el paso a los carros en el carril izquierdo
      if(No_Existe_Carril_Vecino!=carril_der){
	for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();++carro){
	  if((*carro).Get_Direccional_Derecha()){
	    anterior_der=(*carro).Get_Carro_Der_Atras();
	    if((*anterior_der).Get_Ceder_Paso_Izquierda()){
	      velocidad=(*anterior_der).Get_Velocity();
	      freno=(*anterior_der).Get_Velocity();
	      posicion=(*anterior_der).Get_Position();
	      if(posicion>(*carro).Get_Position()){
		posicion-=(*carril_der).Largo;
	      }
	      (*carro).Calcule_Gap_Der_Atras(posicion,velocidad);
	      (*carro).Set_Brake_Der_Back(freno);
	    }
	  }
	}
      }
			
      for(carro=(*carril).Carril_1.begin();carro != (*carril).Carril_1.end();++carro){
	if((*carro).Get_Direccional_Derecha()||(*carro).Get_Direccional_Izquierda()){
	  (*carro).Decide_Change_Lines_And_Direccionales(tipo,false,ran2);
	}
      }	
    }
  }
}

void Avenida::Cambio_De_Carril_Carriles_Simulacion(void){
  std::vector<Carril>::iterator carril,carril_der,carril_izq;
  std::vector<Carril>::reverse_iterator reverse_carril;
  std::list<Carro>::iterator carro,lugar_insercion;
  Carro carro_nuevo;
  int posicion;
	
  //Cambiar de carril hacia la derecha
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    carril_der=(*carril).Carril_Vecino_Derecho;
    if(carril_der!=No_Existe_Carril_Vecino){
      for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();){
	if((*carro).Get_Change_Lines_Der()){
	  posicion=(*carro).Get_Position();
	  lugar_insercion=(*carro).Get_Carro_Der_Adelante();
	  if((*carril_der).Verificar_Casilla_Vacia(posicion,lugar_insercion)){
	    //Agregar carro al carril derecho
	    carro_nuevo=(*carro);
	    carro_nuevo.Reinicie_Interaccion_Multicarril();
	    carro_nuevo.Set_Changed_Lines_Der(true);
	    lugar_insercion=(*carril_der).Agregar_Carro(lugar_insercion,carro_nuevo);
						
	    //Eliminar carro del carril de origen
	    carro=(*carril).Eliminar_Carro(carro);
	  }
	  else{
	    (*carro).Set_Change_Lines_Der(false);
	    ++carro;
	  }
	}
	else{
	  ++carro;
	}
      }
    }
  }

  //Encontrar primeros vecinos para que los carros puedan cambiar de carril
  Avenida::Encontrar_Primeros_Vecinos_Carriles_Simulacion(true,false);

  //Cambiar de carril hacia la izquierda
  for(reverse_carril=Avenida_1.rbegin();reverse_carril!=Avenida_1.rend();++reverse_carril){
    carril=--(reverse_carril.base());
    carril_izq=(*carril).Carril_Vecino_Izquierdo;
    if(carril_izq!=No_Existe_Carril_Vecino){
      for(carro=(*carril).Carril_1.begin();carro!=(*carril).Carril_1.end();){
	if((*carro).Get_Change_Lines_Izq()){
	  posicion=(*carro).Get_Position();
	  lugar_insercion=(*carro).Get_Carro_Izq_Adelante();
	  if((*carril_izq).Verificar_Casilla_Vacia(posicion,lugar_insercion)){
	    //Agregar carro al carril izquierdo
	    carro_nuevo=(*carro);
	    carro_nuevo.Reinicie_Interaccion_Multicarril();
	    carro_nuevo.Set_Changed_Lines_Izq(true);
	    lugar_insercion=(*carril_izq).Agregar_Carro(lugar_insercion,carro_nuevo);
						
	    //Eliminar carro del carril de origen
	    carro=(*carril).Eliminar_Carro(carro);
	  }
	  else{
	    (*carro).Set_Change_Lines_Izq(false);
	    ++carro;
	  }
	}
	else{
	  ++carro;
	}
      }
    }
  }
}


void Avenida::Evolucionar_Variables_Internas_Carros_Carriles_Simulacion(void){
  //Se intercambia informacion y con ella los carros calculan la velocidad
  //en el tiempo t
  std::vector<Carril>::iterator carril;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Evolucionar_Variables_Internas_Carros();
  }
}

void Avenida::Mover_Carros_Carriles_Simulacion(void){
  //Los carros se mueven
  std::vector<Carril>::iterator carril;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Mover_Carros();
  }
}


//Carriles para fijar las condiciones de frontera
//Entrada
void Avenida::Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Frontera_Entrada(void){
  std::vector<Carril_Generador>::iterator carril;
  for(carril=Carriles_Frontera_Entrada.begin();carril!=Carriles_Frontera_Entrada.end();++carril){
    (*carril).Intercambiar_Informacion_Y_Calcular_Gaps();
  }
}

void Avenida::Evolucionar_Variables_Internas_Carros_Carriles_Frontera_Entrada(void){
  std::vector<Carril_Generador>::iterator carril;
  for(carril=Carriles_Frontera_Entrada.begin();carril!=Carriles_Frontera_Entrada.end();++carril){
    (*carril).Evolucionar_Variables_Internas_Carros();
  }
}

void Avenida::Mover_Carros_Carriles_Frontera_Entrada(void){
  std::vector<Carril_Generador>::iterator carril;
  for(carril=Carriles_Frontera_Entrada.begin();carril!=Carriles_Frontera_Entrada.end();++carril){
    (*carril).Mover_Carros();
  }
}

//Salida
void Avenida::Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Frontera_Salida(void){
  std::vector<Carril_Generador>::iterator carril;
  for(carril=Carriles_Frontera_Salida.begin();carril!=Carriles_Frontera_Salida.end();++carril){
    (*carril).Intercambiar_Informacion_Y_Calcular_Gaps();
  }
}

void Avenida::Evolucionar_Variables_Internas_Carros_Carriles_Frontera_Salida(void){
  std::vector<Carril_Generador>::iterator carril;
  for(carril=Carriles_Frontera_Salida.begin();carril!=Carriles_Frontera_Salida.end();++carril){
    (*carril).Evolucionar_Variables_Internas_Carros();
  }
}

void Avenida::Mover_Carros_Carriles_Frontera_Salida(void){
  std::vector<Carril_Generador>::iterator carril;
  for(carril=Carriles_Frontera_Salida.begin();carril!=Carriles_Frontera_Salida.end();++carril){
    (*carril).Mover_Carros();
  }

}

//Funciones generalizadas para la dinamica del sistema
void Avenida::Encontrar_Primeros_Vecinos(bool multicarril,bool condiciones_frontera,Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Encontrar_Primeros_Vecinos_Carriles_Simulacion(multicarril,condiciones_frontera);
  }
}

void Avenida::Intercambiar_Informacion_Y_Calcular_Gaps(bool multicarril,Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Simulacion(multicarril);
  }
  else if(Frontera_Entrada==tipo_carril){
    Avenida::Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    Avenida::Intercambiar_Informacion_Y_Calcular_Gaps_Carriles_Frontera_Salida();
  }	
}

void Avenida::Decide_Change_Lines_And_Direccionales(int tipo,bool direccionales,Tipo_Carril tipo_carril,Crandom & ran2){
  if(Simulacion==tipo_carril){
    Avenida::Decide_Change_Lines_And_Direccionales_Carriles_Simulacion(tipo,direccionales,ran2);
  }
}

void Avenida::Decide_Ceder_El_Paso(Crandom & ran2,double amabilidad,Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Decide_Ceder_El_Paso_Carriles_Simulacion(ran2,amabilidad);
  }
}

void Avenida::Decide_Change_Lines_After_Asking_For_Way(int tipo,Tipo_Carril tipo_carril,Crandom & ran2){
  if(Simulacion==tipo_carril){
    Avenida::Decide_Change_Lines_After_Asking_For_Way_Carriles_Simulacion(tipo,ran2);
  }	
}

void Avenida::Cambio_De_Carril(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Cambio_De_Carril_Carriles_Simulacion();
  }
}

void Avenida::Evolucionar_Variables_Internas_Carros(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Evolucionar_Variables_Internas_Carros_Carriles_Simulacion();
  }
  else if(Frontera_Entrada==tipo_carril){
    Avenida::Evolucionar_Variables_Internas_Carros_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    Avenida::Evolucionar_Variables_Internas_Carros_Carriles_Frontera_Salida();
  }
}

void Avenida::Mover_Carros(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Mover_Carros_Carriles_Simulacion();
  }
  else if(Frontera_Entrada==tipo_carril){
    Avenida::Mover_Carros_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    Avenida::Mover_Carros_Carriles_Frontera_Salida();
  }	
}


void Avenida::Interaccion_Con_Carriles_Frontera(void){
  std::vector<Carril>::iterator carril;
  for(carril=Avenida_1.begin();carril!=Avenida_1.end();++carril){
    (*carril).Interaccion_Con_Carriles_Frontera();
  }	
}

//_________________Funciones para modificar el sistema___________________________________

//Globales
void Avenida::Borrar_Carros_Carriles_Simulacion(void){
  for(int cc=0;cc<Avenida_1.size();cc++){
    Avenida_1[cc].Borrar_Carros();
  }
}
void Avenida::Borrar_Carros_Carriles_Frontera_Entrada(void){
  for(int cc=0;cc<Carriles_Frontera_Entrada.size();cc++){
    Carriles_Frontera_Entrada[cc].Borrar_Carros();
  }
}
void Avenida::Borrar_Carros_Carriles_Frontera_Salida(void){
  for(int cc=0;cc<Carriles_Frontera_Salida.size();cc++){
    Carriles_Frontera_Salida[cc].Borrar_Carros();
  }
}
void Avenida::Borrar_Carros(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Borrar_Carros_Carriles_Simulacion();
  }
  else if(Frontera_Entrada==tipo_carril){
    Avenida::Borrar_Carros_Carriles_Frontera_Entrada();
  } 
  else if(Frontera_Salida==tipo_carril){
    Avenida::Borrar_Carros_Carriles_Frontera_Salida();
  }
}

void Avenida::Borrar_Carriles(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    Avenida::Borrar_Carriles_Simulacion();
  }
  else if(Frontera_Entrada){
    Avenida::Borrar_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida){
    Avenida::Borrar_Carriles_Frontera_Salida();
  }
}

void Avenida::Set_Cars_Time_Stationary(int time_stationary){
  for(int cc=0;cc<Avenida_1.size();cc++){
    Avenida_1[cc].Set_Cars_Time_Stationary(time_stationary);
  }
}

void Avenida::Set_Cars_Inset_Lane(int inset_lane){
  for(int cc=0;cc<Avenida_1.size();cc++){
    Avenida_1[cc].Set_Cars_Inset_Lane(inset_lane);
  }	
}

//Locales
bool Avenida::Stop_Car(int position,int time_estacionarias,int numero_carril){
  if(numero_carril>=Avenida_1.size()){
    return false;
  }
  else{
    return Avenida_1[numero_carril].Stop_Car(position,time_estacionarias);
  }
}
//_________________Imprimir informacion del sistema______________________________________
//Avenida
int Avenida::Get_Cantidad_Carriles(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    return Avenida::Get_Cantidad_Carriles_Simulacion(); 
  }
  else if(Frontera_Entrada==tipo_carril){
    return Avenida::Get_Cantidad_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    return Avenida::Get_Cantidad_Carriles_Frontera_Salida();
  }
}

int Avenida::Get_Cantidad_Carros_Carriles_Simulacion(void){
  double amount=0.0;
  for(int cc=0;cc<Avenida_1.size();cc++){
    amount+=Avenida_1[cc].Carril_1.size();
  }
  return amount;
}

int Avenida::Get_Cantidad_Carros_Carriles_Frontera_Entrada(void){
  double amount=0.0;
  for(int cc=0;cc<Carriles_Frontera_Entrada.size();cc++){
    amount+=Carriles_Frontera_Entrada[cc].Carril_1.size();
  }
  return amount;
}

int Avenida::Get_Cantidad_Carros_Carriles_Frontera_Salida(void){
  double amount=0.0;
  for(int cc=0;cc<Carriles_Frontera_Salida.size();cc++){
    amount+=Carriles_Frontera_Salida[cc].Carril_1.size();
  }
  return amount;
}

int Avenida::Get_Cantidad_Carros(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    return Avenida::Get_Cantidad_Carros_Carriles_Simulacion(); 
  }
  else if(Frontera_Entrada==tipo_carril){
    return Avenida::Get_Cantidad_Carros_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    return Avenida::Get_Cantidad_Carros_Carriles_Frontera_Salida();
  }	
}

int Avenida::Get_Largo_Carriles_Simulacion(void){
  if(Avenida_1.empty()){
    return -1;
  }
  else{
    return Avenida_1[0].Largo;
  }
}
int Avenida::Get_Largo_Carriles_Frontera_Entrada(void){
  if(Carriles_Frontera_Entrada.empty()){
    return -1;
  }
  else{
    return Carriles_Frontera_Entrada[0].Largo;
  }
}
int Avenida::Get_Largo_Carriles_Frontera_Salida(void){
  if(Carriles_Frontera_Salida.empty()){
    return -1;
  }
  else{
    return Carriles_Frontera_Salida[0].Largo;
  }
}
int Avenida::Get_Largo(Tipo_Carril tipo_carril){
  if(Simulacion==tipo_carril){
    return Avenida::Get_Largo_Carriles_Simulacion();
  }
  else if(Frontera_Entrada==tipo_carril){
    return Avenida::Get_Largo_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    return Avenida::Get_Largo_Carriles_Frontera_Salida();
  }
}

double Avenida::Get_Rho_Carriles_Simulacion(bool simulacion){
  double rho=0.0;
  for(int ii=0;ii<Avenida_1.size();ii++){
    rho+=Avenida::Get_Rho_Carril(Simulacion,ii,simulacion);
  }
  return rho/((double)Avenida_1.size());
}
double Avenida::Get_Rho_Carriles_Frontera_Entrada(bool simulacion){
  double rho=0.0;
  for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
    rho+=Avenida::Get_Rho_Carril(Frontera_Entrada,ii,simulacion);
  }
  return rho/((double)Carriles_Frontera_Entrada.size());
}
double Avenida::Get_Rho_Carriles_Frontera_Salida(bool simulacion){
  double rho=0.0;
  for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
    rho+=Avenida::Get_Rho_Carril(Frontera_Salida,ii,simulacion);
  }
  return rho/((double)Carriles_Frontera_Salida.size());
}
double Avenida::Get_Rho(Tipo_Carril tipo_carril,bool simulacion){
  if(Frontera_Entrada==tipo_carril){
    return Avenida::Get_Rho_Carriles_Frontera_Entrada(simulacion);
  }
  else if(Frontera_Salida==tipo_carril){
    return Avenida::Get_Rho_Carriles_Frontera_Salida(simulacion);
  }
  else if(Simulacion==tipo_carril){
    return Avenida::Get_Rho_Carriles_Simulacion(simulacion);
  }
}

double Avenida::Get_Flujo_Espacial_Carriles_Simulacion(void){
  double flux=0.0;
  for(int ii=0;ii<Avenida_1.size();ii++){
    flux+=Avenida::Get_Flujo_Espacial_Carril(Simulacion,ii);
  }
  return flux/((double)Avenida_1.size());
}
double Avenida::Get_Flujo_Espacial_Carriles_Frontera_Entrada(void){
  double flux=0.0;
  for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
    flux+=Avenida::Get_Flujo_Espacial_Carril(Frontera_Entrada,ii);
  }
  return flux/((double)Carriles_Frontera_Entrada.size());	
}
double Avenida::Get_Flujo_Espacial_Carriles_Frontera_Salida(void){
  double flux=0.0;
  for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
    flux+=Avenida::Get_Flujo_Espacial_Carril(Frontera_Salida,ii);
  }
  return flux/((double)Carriles_Frontera_Salida.size());
}
double Avenida::Get_Flujo_Espacial(Tipo_Carril tipo_carril){
  if(Frontera_Entrada==tipo_carril){
    return Avenida::Get_Flujo_Espacial_Carriles_Frontera_Entrada();
  }
  else if(Frontera_Salida==tipo_carril){
    return Avenida::Get_Flujo_Espacial_Carriles_Frontera_Salida();
  }
  else if(Simulacion==tipo_carril){
    return Avenida::Get_Flujo_Espacial_Carriles_Simulacion();	
  }
}

std::list<Carro> Avenida::Get_Info_Carros_Que_Salen(Tipo_Carril tipo_carril){
  std::list<Carro> carros_que_salen,carros_que_salen_2;
  std::list<Carro>::iterator carro;
  for(int ii=0;ii<Avenida_1.size();ii++){
    carros_que_salen_2=Avenida::Get_Info_Carros_Que_Salen_Por_Carril(tipo_carril,ii);
    for(carro=carros_que_salen_2.begin();carro!=carros_que_salen_2.end();carro++){
      carros_que_salen.push_back((*carro));
    }
  }
  return carros_que_salen;
}

bool Avenida::Cars_Have_Stoped(Tipo_Carril tipo_carril,int punto_inicial,int punto_final){
  if(tipo_carril==Simulacion){
    bool cars_have_stoped=true;
    for(int ii=0;(ii<Avenida_1.size())&&(cars_have_stoped);ii++){
      cars_have_stoped=Avenida_1[ii].Cars_Have_Stoped(punto_inicial,punto_final);
    }
    return cars_have_stoped;
  }
  else{
    return true;
  }
}

int Avenida::Get_Id_For_Car(void){
  int id=Id_Disponibles.back();
  Id_Disponibles.pop_back();
  if(Id_Disponibles.empty()){
    Id_Disponibles.push_back(Avenida::Get_Cantidad_Carros(Simulacion));
  }
  return id;
}

void Avenida::Add_Id_For_Cars(int id){
  Id_Disponibles.push_back(id);
}

//Carriles
int Avenida::Get_Max_Carros(Tipo_Carril tipo_carril){
  if(Frontera_Entrada==tipo_carril){
    return Carriles_Frontera_Entrada[0].Cantidad_Maxima_Carros;
  }
  else if(Frontera_Salida==tipo_carril){
    return Carriles_Frontera_Salida[0].Cantidad_Maxima_Carros;
  }
  else if(Simulacion==tipo_carril){
    return Avenida_1[0].Cantidad_Maxima_Carros;
  }
}

double Avenida::Get_Flujo_Espacial_Carril(Tipo_Carril tipo_carril,int carr){
  if(Frontera_Entrada==tipo_carril){
    return Carriles_Frontera_Entrada[carr].Get_Flujo_Espacial();
  }
  else if(Frontera_Salida==tipo_carril){
    return Carriles_Frontera_Salida[carr].Get_Flujo_Espacial();
  }
  else if(Simulacion==tipo_carril){
    return Avenida_1[carr].Get_Flujo_Espacial();
  }
}

double Avenida::Get_Rho_Carril(Tipo_Carril tipo_carril,int carr,bool simulacion){
  if(Frontera_Entrada==tipo_carril){
    return Carriles_Frontera_Entrada[carr].Get_Rho(simulacion);
  }
  else if(Frontera_Salida==tipo_carril){
    return Carriles_Frontera_Salida[carr].Get_Rho(simulacion);
  }
  else if(Simulacion==tipo_carril){
    return Avenida_1[carr].Get_Rho(simulacion);
  }
}

std::list<Carro> Avenida::Get_Info_Carros_Que_Salen_Por_Carril(Tipo_Carril tipo_carril,int carr){
  if(Simulacion==tipo_carril){
    return Avenida_1[carr].Get_Info_Carros_Que_Salen();
  }
}

//Carros
void Avenida::Carril_Ocupado_Carros(Tipo_Carril tipo_carril,std::vector<int> & carril){
  carril.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	carril.push_back(ii);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	carril.push_back(ii);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	carril.push_back(ii);
      }
    }
  }
}

void Avenida::Id_Carros(Tipo_Carril tipo_carril,std::vector<int> & id){
  id.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	id.push_back((*carro).Get_Id());
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	id.push_back((*carro).Get_Id());
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	id.push_back((*carro).Get_Id());
      }
    }
  }
}

void Avenida::Posiciones_Carros(Tipo_Carril tipo_carril,std::vector<int> & pos){
  pos.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	pos.push_back((*carro).Get_Position());
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	pos.push_back((*carro).Get_Position());
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	pos.push_back((*carro).Get_Position());
      }
    }
  }	
}

void Avenida::Velocidades_Carros(Tipo_Carril tipo_carril,std::vector<int> & vel){
  vel.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	vel.push_back((*carro).Get_Velocity());
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	vel.push_back((*carro).Get_Velocity());
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	vel.push_back((*carro).Get_Velocity());
      }
    }
  }
}

void Avenida::Brake_Carros(Tipo_Carril tipo_carril,std::vector<int> & brake){
  brake.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	if((*carro).Get_Brake()){
	  brake.push_back(1);
	}
	else{
	  brake.push_back(0);
	}
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	if((*carro).Get_Brake()){
	  brake.push_back(1);
	}
	else{
	  brake.push_back(0);
	}
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Brake()){
	  brake.push_back(1);
	}
	else{
	  brake.push_back(0);
	}
      }
    }
  }
}

void Avenida::Direccionales_Izquierda_Carros(Tipo_Carril tipo_carril,std::vector<int> & dir){
  dir.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	dir.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	dir.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Direccional_Izquierda()){
	  dir.push_back(1);
	}
	else{
	  dir.push_back(0);
	}
      }
    }
  }
}

void Avenida::Direccionales_Derecha_Carros(Tipo_Carril tipo_carril,std::vector<int> & dir){
  dir.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	dir.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	dir.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Direccional_Derecha()){
	  dir.push_back(1);
	}
	else{
	  dir.push_back(0);
	}
      }
    }
  }
}

void Avenida::Ceder_El_Paso_Izquierda_Carros(Tipo_Carril tipo_carril,std::vector<int> & ceder_el_paso){
  ceder_el_paso.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	ceder_el_paso.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	ceder_el_paso.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Ceder_Paso_Izquierda()){
	  ceder_el_paso.push_back(1);
	}
	else{
	  ceder_el_paso.push_back(0);
	}
      }
    }
  }
}

void Avenida::Ceder_El_Paso_Derecha_Carros(Tipo_Carril tipo_carril,std::vector<int> & ceder_el_paso){
  ceder_el_paso.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	ceder_el_paso.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	ceder_el_paso.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Ceder_Paso_Derecha()){
	  ceder_el_paso.push_back(1);
	}
	else{
	  ceder_el_paso.push_back(0);
	}
      }
    }
  }
}

void Avenida::Cambio_Carril_Izquierda_Carros(Tipo_Carril tipo_carril,std::vector<int> & cambio_carril){
  cambio_carril.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	cambio_carril.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	cambio_carril.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Changed_Lines_Izq()){
	  cambio_carril.push_back(1);
	}
	else{
	  cambio_carril.push_back(0);
	}
      }
    }
  }	
}

void Avenida::Cambio_Carril_Derecha_Carros(Tipo_Carril tipo_carril,std::vector<int> & cambio_carril){
  cambio_carril.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	cambio_carril.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	cambio_carril.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Changed_Lines_Der()){
	  cambio_carril.push_back(1);
	}
	else{
	  cambio_carril.push_back(0);
	}
      }
    }
  }	
}

void Avenida::Estacionarias_Carros(Tipo_Carril tipo_carril,std::vector<int> & estacionarias){
  estacionarias.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	estacionarias.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	estacionarias.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Estacionarias()){
	  estacionarias.push_back(1);
	}
	else{
	  estacionarias.push_back(0);
	}
      }
    }
  }	
}

void Avenida::Stoping_Safely_Carros(Tipo_Carril tipo_carril,std::vector<int> & stoping_safely){
  stoping_safely.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	stoping_safely.push_back(0);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	stoping_safely.push_back(0);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	if((*carro).Get_Stoping_Safely()){
	  stoping_safely.push_back(1);
	}
	else{
	  stoping_safely.push_back(0);
	}
      }
    }
  }	
}

void Avenida::Inset_Lane_Carros(Tipo_Carril tipo_carril,std::vector<int> & inset_lane){
  inset_lane.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	inset_lane.push_back(-1);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	inset_lane.push_back(-1);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	inset_lane.push_back((*carro).Get_Inset_Lane());
      }
    }
  }	
}


void Avenida::Distancia_Al_Siguiente_Carro(Tipo_Carril tipo_carril,std::vector<int> & distancia){
  distancia.clear();
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	distancia.push_back((*carro).Get_Gap_Posicion_Adelante());
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	distancia.push_back((*carro).Get_Gap_Posicion_Adelante());
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	distancia.push_back((*carro).Get_Gap_Posicion_Adelante());
      }
    }
  }
}

void Avenida::Razones_De_Velocidades(Tipo_Carril tipo_carril,std::vector<double> & razones){
  razones.clear();
  double razones_vel;
  double vel_ahead;
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro,carro_adelante;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	carro_adelante=(*carro).Get_Carro_Adelante();
	razones_vel=((double((*carro_adelante).Get_Velocity())+0.1)/(double((*carro).Get_Velocity())+0.1));
	razones.push_back(razones_vel);
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro,carro_adelante;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	carro_adelante=(*carro).Get_Carro_Adelante();
	razones_vel=((double((*carro_adelante).Get_Velocity())+0.1)/(double((*carro).Get_Velocity())+0.1));
	razones.push_back(razones_vel);
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro,carro_adelante;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	carro_adelante=(*carro).Get_Carro_Adelante();
	razones_vel=((double((*carro_adelante).Get_Velocity())+0.1)/(double((*carro).Get_Velocity())+0.1));
	razones.push_back(razones_vel);
      }
    }
  }
}

void Avenida::Velocidad_Vs_Distancia_Al_Carro_Siguiente(Tipo_Carril tipo_carril,std::vector<double> & razones){
  razones.clear();
  double razones_vel;
  double vel_ahead;
  if(Frontera_Entrada==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro,carro_adelante;
    for(int ii=0;ii<Carriles_Frontera_Entrada.size();ii++){
      for(carro=Carriles_Frontera_Entrada[ii].Carril_1.begin();carro!=Carriles_Frontera_Entrada[ii].Carril_1.end();carro++){
	carro_adelante=(*carro).Get_Carro_Adelante();
	razones_vel=((double((*carro_adelante).Get_Velocity())+0.1)/(double((*carro).Get_Velocity())+0.1));
	razones.push_back(double((*carro).Get_Velocity())/(double((*carro).Get_Gap_Posicion_Adelante())+0.001));
      }
    }
  }
  else if(Frontera_Salida==tipo_carril){
    std::list<Carro_Sencillo>::iterator carro,carro_adelante;
    for(int ii=0;ii<Carriles_Frontera_Salida.size();ii++){
      for(carro=Carriles_Frontera_Salida[ii].Carril_1.begin();carro!=Carriles_Frontera_Salida[ii].Carril_1.end();carro++){
	razones.push_back(double((*carro).Get_Velocity())/(double((*carro).Get_Gap_Posicion_Adelante())+0.0001));
      }
    }
  }
  else if(Simulacion==tipo_carril){
    std::list<Carro>::iterator carro,carro_adelante;
    for(int ii=0;ii<Avenida_1.size();ii++){
      for(carro=Avenida_1[ii].Carril_1.begin();carro!=Avenida_1[ii].Carril_1.end();carro++){
	razones.push_back(double((*carro).Get_Velocity())/(double((*carro).Get_Gap_Posicion_Adelante())+0.0001));
      }
    }
  }
}
