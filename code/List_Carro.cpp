#include "List_Multilane.h"

//______________________Funciones del generador aleatorio________________________________________________
//__________________________________________________________________________________________________________

Crandom::Crandom(unsigned long long j){
    v=4101842887655102017LL; w=1;
    u = j ^ v; int64();
    v = u; int64();
    w = v; int64();
}
unsigned long long Crandom::int64(){
    u = u * 2862933555777941757LL + 7046029254386353087LL;
    v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
    w = 4294957665U*(w & 0xffffffff) + (w >> 32);
    unsigned long long x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
    return (x + v) ^ w;
}

double Crandom::exponencial(float tau){
	return -tau*log(r());
}
double Crandom:: gauss(float mu,float sigma){
  	return sigma*sqrt(-2*log(r()))*cos(2*M_PI*r())+mu;
}



//_________________________________________Clase Carro_____________________________________________


//_________________________________Funciones carga___________________________________________________
//Iteradores a los carros a los cuales les da la via
void Carro::Set_Carro_Ceder_El_Paso_Derecha(std::list<Carro>::iterator car){
	if(Ceder_Paso_Derecha){
		if((*Line).Distancia_Entre_Carros(Position,(*Carro_Ceder_El_Paso_Derecha).Get_Position())>
			(*Line).Distancia_Entre_Carros(Position,(*car).Get_Position())){
			Carro_Ceder_El_Paso_Derecha=car;
		}
	}
	else{
		Carro_Ceder_El_Paso_Derecha=car;
	}
}

void Carro::Set_Carro_Ceder_El_Paso_Izquierda(std::list<Carro>::iterator car){
	if(Ceder_Paso_Izquierda){
		if((*Line).Distancia_Entre_Carros(Position,(*Carro_Ceder_El_Paso_Izquierda).Get_Position())>
			(*Line).Distancia_Entre_Carros(Position,(*car).Get_Position())){
			Carro_Ceder_El_Paso_Izquierda=car;
		}
	}
	else{
		Carro_Ceder_El_Paso_Izquierda=car;
	}
}

//_____________________________Imprimir informacion_________________________________________________


//____________________Funciones para la evolucion de las variables internas_________________________________________

//Calcular los gaps con los primeros vecinos
void Carro::Calcule_Gap_Adelante(int position_adelante, int velocity_adelante){
	Gap_Position_Ahead=position_adelante-Position-1;
  	Gap_Velocity_Ahead=velocity_adelante-Velocity;
  	Gap_Ahead=Gap_Position_Ahead+Gap_Velocity_Ahead;
  	if(Gap_Position_Ahead<1){
  		std::cout<<""<<std::endl;
  		std::cout<<Position<<" "<<(*Carro_Adelante).Get_Position()<<std::endl;
  		std::cout<<Inset_Lane<<" "<<(*Carro_Adelante).Get_Inset_Lane()<<std::endl;
  		std::cout<<Id<<" "<<(*Carro_Adelante).Get_Id()<<std::endl;
  		std::cout<<(*Line).Get_Cantidad_Carros(false)<<std::endl;
  	}
}

void Carro::Calcule_Gap_Atras(int position_atras, int velocity_atras){
  	Gap_Position_Back=Position-position_atras-1;
  	Gap_Velocity_Back=Velocity-velocity_atras;
  	Gap_Back=Gap_Position_Back+Gap_Velocity_Back;
}

void Carro::Calcule_Gap_Der_Adelante(int position_der_adelante, int velocity_der_adelante){
  	Gap_Position_Der_Ahead=position_der_adelante-Position-1;
  	Gap_Velocity_Der_Ahead=velocity_der_adelante-Velocity;
  	Gap_Der_Ahead=Gap_Position_Der_Ahead+Gap_Velocity_Der_Ahead;
}

void Carro::Calcule_Gap_Der_Atras(int position_der_atras, int velocity_der_atras){
  	Gap_Position_Der_Back=Position-position_der_atras-1;
  	Gap_Velocity_Der_Back=Velocity-velocity_der_atras;
  	Gap_Der_Back=Gap_Position_Der_Back+Gap_Velocity_Der_Back;
}

void Carro::Calcule_Gap_Izq_Adelante(int position_izq_adelante, int velocity_izq_adelante){
  	Gap_Position_Izq_Ahead=position_izq_adelante-Position-1;
  	Gap_Velocity_Izq_Ahead=velocity_izq_adelante-Velocity;
  	Gap_Izq_Ahead=Gap_Position_Izq_Ahead+Gap_Velocity_Izq_Ahead;
}

void Carro::Calcule_Gap_Izq_Atras(int position_izq_atras, int velocity_izq_atras){
  	Gap_Position_Izq_Back=Position-position_izq_atras-1;
  	Gap_Velocity_Izq_Back=Velocity-velocity_izq_atras;
  	Gap_Izq_Back=Gap_Position_Izq_Back+Gap_Velocity_Izq_Back;
}

//Funciones para deteminar si es seguro cambiar de carril
bool Carro::Is_Safe_To_Change_Lines_Izq_Ahead(void){
	if(Carro_Izq_Adelante==(*Line).No_Existe_Carril_Vecino){
		//No existe carril vecino izquierdo
		return false;
	}
	else if(Gap_Position_Izq_Back<1){
		//La posicion de destino esta ocupada	
		return false;
	}
	else if(Carro_Izq_Adelante==(*Line).Carril_Vecino_Vacio){
		//El carril de llegada esta desocupado
		return true;
	}
	else{
		int vel,vel_2,del;
		bool bra;
		vel=Velocity;
		del=Delay;
		bra=Brake;

		if(vel+(*Line).Safe_Exceding_Distance[vel]<=(Gap_Position_Izq_Ahead-1)){
			vel_2=vel;
			Carro::Evolucion_Olmos_Munoz(vel,del,bra,Gap_Izq_Ahead,Brake_Izq_Ahead);
			if((vel>=(*Line).Safe_Minimum_Next_Velocity[vel_2])&&(vel>0)){
				if(!(*Carro_Izq_Adelante).Get_Estacionarias()){
					return true;
				}
				else if((*Carro_Izq_Adelante).Get_Estacionarias()&&
					((Gap_Position_Izq_Ahead-1)>=(*Line).Safe_Braking_Distance[vel_2+1]+vel_2+1)){
					return true;
				}
				else{
					false;
				}
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
}

bool Carro::Is_Safe_To_Change_Lines_Izq_Back(void){
	if(Carro_Izq_Atras==(*Line).No_Existe_Carril_Vecino){
		//No existe carril vecino izquierdo
		return false;
	}
	else if(Gap_Position_Izq_Back<1){
		//La posicion de destino esta ocupada	
		return false;
	}
	else if(Carro_Izq_Atras==(*Line).Carril_Vecino_Vacio){
		//El carril de llegada esta desocupado
		return true;
	}
	else{
		int vel_back,vel_back_2,delay_back;
		bool brake_back;
		vel_back=(*Carro_Izq_Atras).Get_Velocity();
		vel_back_2=vel_back;
		delay_back=(*Carro_Izq_Atras).Get_Delay();
		brake_back=(*Carro_Izq_Atras).Get_Brake();
		if(vel_back+(*Line).Safe_Exceding_Distance[vel_back]<=(Gap_Position_Izq_Back-1)){
			(*Carro_Izq_Atras).Evolucion_Olmos_Munoz(vel_back,delay_back,brake_back,Gap_Izq_Back,Brake);	
			if(vel_back>=(*Line).Safe_Minimum_Next_Velocity[vel_back_2]){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
}

bool Carro::Is_Safe_To_Change_Lines_Der_Ahead(void){
	if(Carro_Der_Adelante==(*Line).No_Existe_Carril_Vecino){
		//No existe carril vecino derecho
		return false;
	}
	else if(Gap_Position_Der_Back<1){
		//La posicion de destino esta ocupada	
		return false;
	}
	else if(Carro_Der_Adelante==(*Line).Carril_Vecino_Vacio){
		//El carril de llegada esta desocupado
		return true;
	}
	else{
		int vel,vel_2,del;
		bool bra;
		vel=Velocity;
		del=Delay;
		bra=Brake;

		if(vel+(*Line).Safe_Exceding_Distance[vel]<=(Gap_Position_Der_Ahead-1)){
			vel_2=vel;
			Carro::Evolucion_Olmos_Munoz(vel,del,bra,Gap_Der_Ahead,Brake_Der_Ahead);
			if((vel>=(*Line).Safe_Minimum_Next_Velocity[vel_2])&&(vel>0)){
				if(!(*Carro_Der_Adelante).Get_Estacionarias()){
					return true;
				}
				else if((*Carro_Der_Adelante).Get_Estacionarias()&&
					((Gap_Position_Der_Ahead-1)>=(*Line).Safe_Braking_Distance[vel_2+1]+vel_2+1)){
					return true;
				}
				else{
					false;
				}
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
}

bool Carro::Is_Safe_To_Change_Lines_Der_Back(void){
	if(Carro_Der_Atras==(*Line).No_Existe_Carril_Vecino){
		//No existe carril vecino derecho
		return false;
	}
	else if(Gap_Position_Der_Back<1){
		//La posicion de destino esta ocupada	
		return false;
	}
	else if(Carro_Der_Atras==(*Line).Carril_Vecino_Vacio){
		//El carril de llegada esta desocupado
		return true;
	}
	else{
		int vel_back,vel_back_2,delay_back;
		bool brake_back;
		vel_back=(*Carro_Der_Atras).Get_Velocity();
		vel_back_2=vel_back;
		delay_back=(*Carro_Der_Atras).Get_Delay();
		brake_back=(*Carro_Der_Atras).Get_Brake();
		if(vel_back+(*Line).Safe_Exceding_Distance[vel_back]<=(Gap_Position_Der_Back-1)){
			(*Carro_Der_Atras).Evolucion_Olmos_Munoz(vel_back,delay_back,brake_back,Gap_Der_Back,Brake);	
			if(vel_back>=(*Line).Safe_Minimum_Next_Velocity[vel_back_2]){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
}

bool Carro::Is_Worth_To_Ask_For_Way_Derecha(void){
	if(Carro_Der_Atras==(*Line).No_Existe_Carril_Vecino){
		//No existe carril vecino derecho
		return false;
	}
	else if(Carro_Der_Adelante==(*Line).Carril_Vecino_Vacio){
		//El carril de llegada esta desocupado
		return false;
	}
	else if(Gap_Position_Der_Back<1){
		//La posicion de destino esta ocupada
		int vel_back=(*Carro_Der_Atras).Get_Velocity();
		int carro_atras_gap_pos_back=(*Carro_Der_Atras).Get_Gap_Posicion_Atras();
		if((-Gap_Position_Der_Back+vel_back>=3)&&(Gap_Position_Der_Back+carro_atras_gap_pos_back>=0)){
			//El carro ve factible entrar al carril en el siguiente paso de tiempo
			return true;
		}	
		else{
			return false;
		}
	}
	else{
		return true;
	}
}

bool Carro::Is_Worth_To_Ask_For_Way_Izquierda(void){
	if(Carro_Izq_Atras==(*Line).No_Existe_Carril_Vecino){
		//No existe carril vecino derecho
		return false;
	}
	else if(Carro_Izq_Adelante==(*Line).Carril_Vecino_Vacio){
		//El carril de llegada esta desocupado
		return false;
	}
	else if(Gap_Position_Izq_Back<1){
		//La posicion de destino esta ocupada
		int vel_back=(*Carro_Izq_Atras).Get_Velocity();
		int carro_atras_gap_pos_back=(*Carro_Izq_Atras).Get_Gap_Posicion_Atras();
		if((-Gap_Position_Izq_Back+vel_back>=3)&&(Gap_Position_Izq_Back+carro_atras_gap_pos_back>=0)){
			//El carro ve factible entrar al carril en el siguiente paso de tiempo
			return true;
		}	
		else{
			return false;
		}
	}
	else{
		return true;
	}
}

//El carro decide si cambia de carril o prende las direccionales
void Carro::Decide_Change_Lines_And_Direccionales(int tipo,bool direccionales,Crandom & ran2){
	Change_Lines_Izq=false; Change_Lines_Der=false;
	if(direccionales){
		Direccional_Izquierda=false; Direccional_Derecha=false;
	}
	if(tipo==1){
	    //Cambio de carril implementado por Plinio Triana
	    //Incentivo v_{i+1}(t-1)<v_{i}(t-1); GapVelocity = v_{i+1}-v_{i}
	    if(Gap_Velocity_Ahead<0){
	      	//Criterio de seguridad para ir a la izquierda
	      	if(Gap_Position_Ahead<Gap_Position_Izq_Ahead){
  	      		if(Carro::Is_Safe_To_Change_Lines_Izq_Back()&&Carro::Is_Safe_To_Change_Lines_Izq_Ahead()){
		        	Change_Lines_Izq=true;
		        }
	      		else if(direccionales&&Carro::Is_Worth_To_Ask_For_Way_Izquierda()){
	      		  	Direccional_Izquierda=true;
	      		}
	      	}
	      	//Criterio de seguridad para ir a la derecha
	      	if(Gap_Position_Ahead<Gap_Position_Der_Ahead){
	      		if(Carro::Is_Safe_To_Change_Lines_Der_Back()&&Carro::Is_Safe_To_Change_Lines_Der_Ahead()){
		        	Change_Lines_Der=true;
		        }
	      		else if(direccionales&&Carro::Is_Worth_To_Ask_For_Way_Derecha()){
	      		  	Direccional_Derecha=true;
	      		}	
	        }
	      	//Si puede girar a ambos lados escoge uno
	      	if(Change_Lines_Izq&&Change_Lines_Der){
	            //Se utiliza el gap posicion con los carriles
	            //vecinos para decidir a cuál cambiarse. Si existe el mismo gap en ambos
	            //carriles el carro escoge cambiarse al carril de la derecha.
	        	if(Gap_Position_Der_Ahead>Gap_Position_Izq_Ahead){
	          		Change_Lines_Der=true; Change_Lines_Izq=false;
	        	}
	        	else if(Gap_Position_Der_Ahead<Gap_Position_Izq_Ahead){
	          		Change_Lines_Izq=true; Change_Lines_Der=false;  
	        	}
	        	else if(Gap_Position_Izq_Ahead=Gap_Position_Der_Ahead){
	        		double random_number=ran2.r();
	        		if(random_number<0.5){
	        			Change_Lines_Der=true; Change_Lines_Izq=false;		
	        		}
	        		else{
	        			Change_Lines_Izq=true; Change_Lines_Der=false;
	        		}
	        	}	
		    }
	    }  
  	}
  	else if(tipo==2){
  		//Cambio de carril implementado por Plinio Triana con modificaciones propias
	    //Incentivo Gap<GapIzq o Gap<GapDer
	    if((Gap_Ahead<Gap_Izq_Ahead)||(Gap_Ahead<Gap_Der_Ahead)){
	      	//Criterio de seguridad para ir a la izquierda
	      	if(Gap_Ahead<Gap_Izq_Ahead){
  	      		if(Carro::Is_Safe_To_Change_Lines_Izq_Back()&&Carro::Is_Safe_To_Change_Lines_Izq_Ahead()){
		        	Change_Lines_Izq=true;
		        }
	      		else if(direccionales&&Carro::Is_Worth_To_Ask_For_Way_Izquierda()){
	      		  	Direccional_Izquierda=true;
	      		}
	      	}
	      	//Criterio de seguridad para ir a la derecha
	      	if(Gap_Ahead<Gap_Der_Ahead){
	      		if(Carro::Is_Safe_To_Change_Lines_Der_Back()&&Carro::Is_Safe_To_Change_Lines_Der_Ahead()){
		        	Change_Lines_Der=true;
		        }
	      		else if(direccionales&&Carro::Is_Worth_To_Ask_For_Way_Derecha()){
	      		  	Direccional_Derecha=true;
	      		}	
	        }
	        //Si puede girar a ambos lados escoge uno
	      	if(Change_Lines_Izq&&Change_Lines_Der){
	            //Se utiliza el gap total = gap posicion + gap velocidad con los carriles
	            //vecinos para decidir a cuál cambiarse. Si existe el mismo gap en ambos
	            //carriles el carro escoge cambiarse al carril de la derecha.
	        	if(Gap_Der_Ahead>Gap_Izq_Ahead){
	          		Change_Lines_Der=true; Change_Lines_Izq=false;
	        	}
	        	else if(Gap_Der_Ahead<Gap_Izq_Ahead){
	          		Change_Lines_Izq=true; Change_Lines_Der=false;  
	        	}
		        else if(Gap_Der_Ahead=Gap_Izq_Ahead){
	    	    	double random_number=ran2.r();
	        		if(random_number<0.5){
	        			Change_Lines_Der=true; Change_Lines_Izq=false;		
	        		}
	        		else{
	        			Change_Lines_Izq=true; Change_Lines_Der=false;
	        		}
	    	    }
	      	}
	    }  
  	}
  	//Si el carro de adelante prende las estacionarias y adicionalmente se encuentra lo
	//suficientemente cerca se prenden las direccionales
	if((*Carro_Adelante).Get_Estacionarias()&&direccionales&&
	   	((Gap_Position_Ahead-1)<=(*Line).Safe_Braking_Distance[Velocity+1]+Velocity+1)){
	   	if(Carro_Der_Atras!=(*Line).No_Existe_Carril_Vecino){
	       	Direccional_Derecha=true;
	   	}
	   	if(Carro_Izq_Atras!=(*Line).No_Existe_Carril_Vecino){
	       	Direccional_Izquierda=true;
	   	}
	}
}

//Funciones para decidir si es seguro dar la via
bool Carro::Is_Safe_To_Give_Way_Right(int & vel_after_giving_way){
	//Encontrar la velocidad que debe tener el carro si quiere que el 
	//carro del carril derecho entre al carril
	if(Ceder_Paso_Derecha){
		if((Gap_Position_Der_Ahead-1)>(*Line).Safe_Braking_Distance[vel_after_giving_way+1]+vel_after_giving_way+1){
			return false;
		}
		else if((Gap_Position_Der_Ahead-1)>=(*Line).Safe_Braking_Distance[vel_after_giving_way]+vel_after_giving_way){
			vel_after_giving_way=vel_after_giving_way;
		}
		else{
			vel_after_giving_way=(*Line).Safe_Minimum_Next_Velocity[vel_after_giving_way];
		}
		return true;
	}
	else{
		return false;
	}
}

bool Carro::Is_Safe_To_Give_Way_Left(int & vel_after_giving_way){
	if(Ceder_Paso_Izquierda){
		if((Gap_Position_Izq_Ahead-1)>(*Line).Safe_Braking_Distance[vel_after_giving_way+1]+vel_after_giving_way+1){
			return false;
		}
		else if((Gap_Position_Izq_Ahead-1)>=(*Line).Safe_Braking_Distance[vel_after_giving_way]+vel_after_giving_way){
			vel_after_giving_way=vel_after_giving_way;
		}
		else{
			vel_after_giving_way=(*Line).Safe_Minimum_Next_Velocity[vel_after_giving_way];
		}
		return true;
	}
	else{
		return false;
	}
}

//El carro decide si da el paso a carros que encienden las direccionales
void Carro::Decide_Ceder_El_Paso(Crandom & ran2,double amabilidad){
  	
  	double pp;
  	int vel_give_way_right=Velocity;
  	int vel_give_way_left=Velocity;
  	if(Ceder_Paso_Derecha){
  		if(!Carro::Is_Safe_To_Give_Way_Right(vel_give_way_right)){
  			Ceder_Paso_Derecha=false;
  		}
  	}
  	if(Ceder_Paso_Izquierda){
  		if(!Carro::Is_Safe_To_Give_Way_Left(vel_give_way_left)){
  			Ceder_Paso_Izquierda=false;
  		}
  	}

  	if(Ceder_Paso_Derecha||Ceder_Paso_Izquierda){
    	pp=ran2.r();
    	if(pp<amabilidad){
      		if(Ceder_Paso_Derecha&&Ceder_Paso_Izquierda){
      			if(ran2.r()<0.5){//Aleatoriamente se escoge a cual carro darle la via
      				Ceder_Paso_Izquierda=true;
          			Ceder_Paso_Derecha=false;
      			}
      			else{
      				Ceder_Paso_Derecha=true;
          			Ceder_Paso_Izquierda=false;
      			}
      		}
    	}
    	else{//El carro no cede el paso
      		Ceder_Paso_Derecha=false;
      		Ceder_Paso_Izquierda=false;
    	}
  	}
  	if((!Change_Lines_Der)&&(!Change_Lines_Izq)&&(!Direccional_Derecha)&&(!Direccional_Izquierda)){
	  	if(Ceder_Paso_Derecha){
	  		if(Velocity<vel_give_way_right){
		  		Delay=0;
		  		Brake=true;
	  		}
	  		else{
		  		Delay=0;
		  	}
	  		Velocity=vel_give_way_right;
	  	}
	  	else if(Ceder_Paso_Izquierda){
	  		if(Velocity<vel_give_way_left){
		  		Delay=0;
		  		Brake=true;
	  		}
	  		else{
		  		Delay=0;
		  	}
	  		Velocity=vel_give_way_left;
	  	}
	}
	else{
		Ceder_Paso_Izquierda=false;
		Ceder_Paso_Derecha=false;
	}
}

void Carro::Safe_Stop(int& vel, int& delay, bool& brake, bool& stoping_safely){
	vel=(*Line).Safe_Minimum_Next_Velocity[vel];
	delay=0;
	brake=true;
	if(vel==0){
		stoping_safely=false;
	}
}

//Se calcula la nueva velocidad con la implementacion hecha por Juan Florez del algoritmo
//Olmos-Muñoz
void Carro::Evolucion_Olmos_Munoz(int& vel, int& delay, bool& brake, int gap, bool brake_adelante){
 	//Modificacion de la velocidad del carro de atrás
 	if(gap<(*Line).Gap_Frenado_Carro[vel]){
 	    //Frenado normal
	    while((gap<(*Line).Gap_Frenado_Carro[vel])){//||(gap>(*Line).Gap_Aceleracion_Carro[vel])){
		    vel=vel-1;
	    	gap+=1;
	    }
	    //Modificar el valor de las variables
	    brake=true;
	    delay=0;
	}
	else if((gap>=(*Line).Gap_Frenado_Carro[vel])&&(gap<=(*Line).Gap_Aceleracion_Carro[vel])){
		//if(vel==0&&gap==(*Line).Gap_Aceleracion_Carro[vel]){
		//	vel+=1;
		//}
		delay=0;
		brake=false;
	}
	else if((gap>=(*Line).Gap_Aceleracion_Carro[vel])){
	    if((gap<(*Line).Gap_Aceleracion_Carro[vel]+2)&&(brake_adelante)){
	      	//Frenado instantaneo
	      	vel=std::max(vel-1,0);  //La velocidad no puede ser negativa
	      	delay=0;
	      	brake=true;
	    }
	    else{
		    brake=false;
		    delay+=1;  
		    if(delay>=(*Line).Retardo_Carro[vel]){
		    	//Acelerate
		    	vel=std::min(vel+1,VELMAX); //La velocidad no puede exceder VELMAX
		    	delay=0;
	    	}
		} 
	}
}


//Se actualiza el estado de las variables internas de los carros siguiendo 
//la regla postulada por L.D. Olmos y J.D. Muñoz
//Algoritmo propuesto por Juan Sebastiań Flórez para tener en cuenta el efecto
//de ceder el paso en la evolucion de la velocidad, los carros deben considerar
//el carro que tiene detras para no frenar demasiado al momento de ceder el paso
//pues esto puede generar choques
void Carro::Calcular_Nuevo_Estado(int& vel, int& delay, bool& brake, bool& estacionarias, int& tiempo_estacionarias, bool& stoping_safely){
	//std::cout<<"Carro evolucionando "<<Id<<std::endl;
	if(stoping_safely){
		Carro::Safe_Stop(vel,delay,brake,stoping_safely);
		//std::cout<<"Stoping_Safely"<<std::endl;
	} 
	else if(estacionarias&&(!stoping_safely)){
		vel=0;
		brake=true;
		delay=0;
		tiempo_estacionarias-=1;
		if(tiempo_estacionarias<=0){
			estacionarias=false;
			tiempo_estacionarias=0;
		}
		//std::cout<<"Estacionarias"<<std::endl;
	}
	else if((*Carro_Adelante).Get_Changed_Lines_Der()||(*Carro_Adelante).Get_Changed_Lines_Izq()||
		Changed_Lines_Izq||Changed_Lines_Der){

		int velocidad,retardo;
		bool freno;
		velocidad=vel;
		retardo=delay;
		freno=brake;
		Carro::Evolucion_Olmos_Munoz(velocidad,retardo,freno,Gap_Ahead,Brake_Ahead);

		if(velocidad>vel){
			if(Gap_Position_Ahead-1>=(*Line).Safe_Braking_Distance[velocidad]+vel){
				vel=velocidad;
				delay=retardo;
				brake=freno;
			}
		}
		else{
			vel=velocidad;
			delay=retardo;
			brake=freno;
		}
		if(vel>=(Gap_Position_Ahead-1)){
			vel=std::max(0,Gap_Position_Ahead-2);
			delay=0;
			brake=true;
		}
	}
	else if(Ceder_Paso_Izquierda||Ceder_Paso_Derecha){
		int velocidad,del;
		bool freno;
		velocidad=vel;
		del=delay;
		freno=brake;
		Carro::Evolucion_Olmos_Munoz(vel,delay,brake,Gap_Ahead,Brake_Ahead);
		if(vel>=velocidad){
			vel=velocidad;
			delay=del;
			brake=freno;
		}
		//std::cout<<"ceder el paso"<<std::endl;
	}
	else{
		Carro::Evolucion_Olmos_Munoz(vel,delay,brake,Gap_Ahead,Brake_Ahead);
		//std::cout<<"olmos munoz"<<std::endl;	
	}
}

void Carro::Evolucionar_Variables_Internas(void){
	Carro::Calcular_Nuevo_Estado(Velocity,Delay,Brake,Estacionarias,Tiempo_Estacionarias,Stoping_Safely);
	if(Position>=0&&Position<(*Line).Get_Largo()){
		Tiempo_Simulacion+=1;
	}
	if(Velocity<0){
		std::cout<<Id<<std::endl;
		std::cout<<"Negative Velocity"<<std::endl;
		if(Position<(*Line).Get_Largo()){
			int test;
			std::cin>>test;
		}
	}
}

//Borrar informacion de la interaccion multicarril
void Carro::Reinicie_Interaccion_Multicarril(void){
  	//Se reinician los indicadores de cambio de carril
  	Change_Lines_Izq=false;
  	Change_Lines_Der=false;
  	Changed_Lines_Der=false;
  	Changed_Lines_Izq=false;
  	//Se reinician los indicadores de luces direccionales
  	Direccional_Derecha=false;
  	Direccional_Izquierda=false;
  	//Se reinician los indicadores de ceder el paso
  	Ceder_Paso_Derecha=false;
  	Ceder_Paso_Izquierda=false;
}

//___________________________________Clase Carro_Sencillo___________________________________________________

//________________________________Funciones carga______________________________________________

//________________________Imprimir informacion_________________________________________________
    
//____________________Funciones para la evolucion de las variables internas____________________________
    
void Carro_Sencillo::Calcule_Gap_Adelante(int position_adelante, int velocity_adelante){
  	Gap_Position_Ahead=position_adelante-Position-1;
  	Gap_Velocity_Ahead=velocity_adelante-Velocity;
  	Gap_Ahead=Gap_Position_Ahead+Gap_Velocity_Ahead;
}

void Carro_Sencillo::Calcule_Gap_Atras(int position_atras, int velocity_atras){
  	Gap_Position_Back=Position-position_atras-1;
  	Gap_Velocity_Back=Velocity-velocity_atras;
  	Gap_Back=Gap_Position_Back+Gap_Velocity_Back;
}

void Carro_Sencillo::Evolucion_Olmos_Munoz(int& vel, int& delay, bool& brake, int gap, bool brake_adelante, bool tipo_juan){
 	//Version de Juan de la evolucion
 	if(tipo_juan){
 		//Modificacion de la velocidad del carro de atrás
	 	if(gap<(*Line).Gap_Frenado_Carro[vel]){
	 	    //Frenado normal
		    while((gap<(*Line).Gap_Frenado_Carro[vel])){//||(gap>(*Line).Gap_Aceleracion_Carro[vel])){
			    vel=vel-1;
		    	gap+=1;
		    }
		    //Modificar el valor de las variables
		    brake=true;
		    delay=0;
		}
		//else if((gap>=(*Line).Gap_Frenado_Carro[vel])&&(gap<=(*Line).Gap_Aceleracion_Carro[vel])){
		//    delay=0;
		//    brake=false;
		//}
		else if((gap>(*Line).Gap_Aceleracion_Carro[vel])){
		    if((gap<=(*Line).Gap_Aceleracion_Carro[vel]+2)&&(brake_adelante)){
		      	//Frenado instantaneo
		      	vel=std::max(vel-1,0);  //La velocidad no puede ser negativa
		      	delay=0;
		      	brake=true;
		    }
		    else{
			    brake=false;
			    delay+=1;  
			    if(delay>=(*Line).Retardo_Carro[vel]){
			    	//Acelerate
			    	vel=std::min(vel+1,VELMAX); //La velocidad no puede exceder VELMAX
			    	delay=0;
		    	}
		//      else{delayold+=1;}
			} 
		    /*    
		    brakeold=0;
		    delayold+=1;
		    if(delayold==retardo[velocityold]){
		      //Acelerate
		      velocityold=min(velocityold+1,VELMAX); //La velocidad no puede exceder VELMAX
		      delayold=0;
		    }
		    */
		}
		else{
			delay=0;
			brake=false;
		}
 	}
 	else{
		//Version de Olmos de la evolucion
		int b=1;
	  	//frenadoinsta1=frenadoinsta2;
	  	for(int m=1;m<20;m++)
	    {
	    	//cout<<vx<<"\t"<<gapfrenado<<"\t"<<gapaceleracion<<"\t"<<b<<"\t"<<gap<<"\n";
	      	int gapfrenado,gapaceleracion;
	      	int retardo;
	      	switch (vel)
			{
				case 0:gapfrenado=0;gapaceleracion=2;retardo=1;break;
				case 1:gapfrenado=3;gapaceleracion=4;retardo=1;break;
				case 2:gapfrenado=3;gapaceleracion=5;retardo=1;break;
				case 3:gapfrenado=4;gapaceleracion=5;retardo=1;break;
				case 4:gapfrenado=5;gapaceleracion=6;retardo=2;break;
				case 5:gapfrenado=6;gapaceleracion=7;retardo=2;break;
				case 6:gapfrenado=6;gapaceleracion=8;retardo=2;break;
				case 7:gapfrenado=7;gapaceleracion=9;retardo=2;break;
				//	case 8:gapfrenado=8;gapaceleracion=10;retardo=3;break;
				//	case 9:gapfrenado=8;gapaceleracion=11;retardo=3;break;
				//	case 10:gapfrenado=10;gapaceleracion=13;retardo=3;break;
				//	case 11:gapfrenado=10;gapaceleracion=13;retardo=3;break;
				//	case 12:gapfrenado=11;gapaceleracion=13;retardo=3;break;
				//	case 13:gapfrenado=11;gapaceleracion=13;retardo=3;break;
				//	case 14:gapfrenado=12;gapaceleracion=13;retardo=3;break;
				//	case 15:gapfrenado=13;gapaceleracion=14;retardo=3;break;
			}
	      	if(gapfrenado>gap){
	      		vel=vel-1;
	      		gap+=1;
	      		b=0;
	      		delay=0;
	      		brake=true;
	      	}
	      	else if((gapfrenado<=gap)&&(gap<=gapaceleracion)){
	      		vel=vel;
	      		if(b!=0)brake=false;
	      		b=1;
	      		delay=0;
	      	}
	    	else if(gap>=gapaceleracion)	
			{
		  		if((brake_adelante)&&((gap==gapaceleracion)||(gap==gapaceleracion+1)))
			  	{
			    	if(vel!=0)vel=vel-1;
			    	//if((vx==1))vx=vx-1;
			    	//if((vx!=0)&&(vx>1))vx=vx-2;
			    	brake=true;
			    	delay=0;
			  	}
		  		else
		  		{
		      		delay+=1;
		      		brake=0;
		      		//cout<<conretardo;
		      		if(delay==retardo)
					{
			  			vel=std::min(vel+1,VELMAX);
			  			delay=0;
					}
		    	}
		  	b=1;
			}
	        if(b==1)break;
	    }
 	}
}

void Carro_Sencillo::Calcular_Nuevo_Estado(int& vel, int& delay, bool& brake){
	Carro_Sencillo::Evolucion_Olmos_Munoz(vel,delay,brake,Gap_Ahead,Brake_Ahead,true);
}

void Carro_Sencillo::Evolucionar_Variables_Internas(void){
	Carro_Sencillo::Calcular_Nuevo_Estado(Velocity,Delay,Brake);
}
