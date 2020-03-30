#include <cmath>
#include "List_Multilane.h"

using namespace std;

//______________________Funciones del generador aleatorio________________________________________________
//__________________________________________________________________________________________________________

Crandom::Crandom(unsigned long long j){
    v=4101842887655102017LL; w=1;
    u = j ^ v; int64();
    v = u; int64();
    w = v; int64();
  }
unsigned long long Crandom::int64() {
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

//______________________Funciones de los carros_____________________________________________________________
//__________________________________________________________________________________________________________

//Tablas de evolución
Carro::Carro(void){
  //Tabla de evolucion 
 gapfrenado[0]=0;gapaceleracion[0]=3;retardo[0]=1;
 gapfrenado[1]=3;gapaceleracion[1]=4;retardo[1]=1;
 gapfrenado[2]=3;gapaceleracion[2]=5;retardo[2]=1;
 gapfrenado[3]=4;gapaceleracion[3]=5;retardo[3]=1;
 gapfrenado[4]=5;gapaceleracion[4]=6;retardo[4]=2;
 gapfrenado[5]=6;gapaceleracion[5]=7;retardo[5]=2;
 gapfrenado[6]=6;gapaceleracion[6]=8;retardo[6]=2;
 gapfrenado[7]=7;gapaceleracion[7]=9;retardo[7]=2;
 gapfrenado[8]=8;gapaceleracion[8]=10;retardo[8]=3;
 gapfrenado[9]=8;gapaceleracion[9]=11;retardo[9]=3;
 gapfrenado[10]=10;gapaceleracion[10]=13;retardo[10]=3;
 gapfrenado[11]=10;gapaceleracion[11]=13;retardo[11]=3;
 gapfrenado[12]=11;gapaceleracion[12]=13;retardo[12]=3;
 gapfrenado[13]=11;gapaceleracion[13]=13;retardo[13]=3;
 gapfrenado[14]=12;gapaceleracion[14]=13;retardo[14]=3;
 gapfrenado[15]=13;gapaceleracion[15]=14;retardo[15]=3;
 //Velocidad maxima
 VELMAX=7;
}

//Funciones carga

void Carro::Cargueold(int vel, int freno, int del){
  velocityold=vel;
  brakeold=freno;
  delayold=del;
}

void Carro::Cargueold_V2(int vel, int freno, int del, int ID, int Time_Static, int Static){
  velocityold=vel;
  brakeold=freno;
  delayold=del;
  Id_old=ID;
  Time_Static_old=Time_Static;
  Static_old=Static;
}

void Carro::Carguenew(int vel, int freno, int del){
  velocitynew=vel;
  brakenew=freno;
  delaynew=del;
}

void Carro::Carguenew_V2(int vel, int freno, int del, int ID, int Time_Static, int Static){
  velocitynew=vel;
  brakenew=freno;
  delaynew=del;
  Id_new=ID;
  Time_Static_new=Time_Static;
  Static_new=Static;
}
 //Calculo del gap
void Carro::CalculeGap(int gapPosition, int VelAhead, int BrakeAhead){
  //Guardar la información del gap de posición
  GapPosition=gapPosition;
  //Guardar la información del freno del carro de adelante
  BrakeAdelante=BrakeAhead;
  //Calcular el gap de velocidad  
  int gapVelocity=(VelAhead-velocityold);
  GapVelocity=gapVelocity;
  //Calcular el gap total
  Gap=GapPosition+GapVelocity;
}
 //Calculo del gap con el carril de la izquierda
void Carro::CalculeGapIzq(int gapPosition, int VelAhead, int BrakeAhead,int gapPosition_back, int VelAhead_back, int BrakeAhead_back){
  //Guardar la información del gap de posición
  GapPositionIzq=gapPosition;
  GapPositionIzq_back=gapPosition_back;
  //Guardar la información del freno del carro de adelante
  BrakeAdelanteIzq=BrakeAhead;
  BrakeIzq_back=BrakeAhead_back;
  //Calcular el gap de velocidad  
  int gapVelocity=(VelAhead-velocityold);
  GapVelocityIzq=gapVelocity;
  int gapVelocity_back=(VelAhead_back-velocityold);
  GapVelocityIzq_back=gapVelocity_back;
  //Calcular el gap total
  GapIzq=GapPositionIzq+GapVelocityIzq;
  GapIzq_back=GapPositionIzq_back+GapVelocityIzq_back;
  //El carro determina si prende o no las direcciona

}
 //Calculo del gap con el carril de la derecha
void Carro::CalculeGapDer(int gapPosition, int VelAhead, int BrakeAhead,int gapPosition_back, int VelAhead_back, int BrakeAhead_back){
  //Guardar la información del gap de posición
  GapPositionDer=gapPosition;
  GapPositionDer_back=gapPosition_back;
  //Guardar la información del freno del carro de adelante
  BrakeAdelanteDer=BrakeAhead;
  BrakeDer_back=BrakeAhead_back;
  //Calcular el gap de velocidad  
  int gapVelocity=(VelAhead-velocityold);
  GapVelocityDer=gapVelocity;
  int gapVelocity_back=(VelAhead_back-velocityold);
  GapVelocityDer_back=gapVelocity_back;
  //Calcular el gap total
  GapDer=GapPositionDer+GapVelocityDer;
  GapDer_back=GapPositionDer_back+GapVelocityDer_back;
}

//El carro decide si va a cambiar de carril
void Carro::DecideChangeLines(int tipo){
	Change_Lines_Izq=false; Change_Lines_Der=false;
  if(tipo==1){
    //Cambio de carril implementado por Plinio Triana
    //Incentivo v_{i+1}(t-1)<v_{i}(t-1); GapVelocity = v_{i+1}-v_{i}
    if(GapVelocity<0){
      //Criterio de seguridad para ir a la izquierda
      if((GapPosition<GapPositionIzq)&&(GapPositionIzq_back+GapVelocityIzq_back+velocityold<0)){
        Change_Lines_Izq=true;
      }
      else if(Gap<GapIzq){
        Direccional_Izquierda=true;
      }
      //Criterio de seguridad para ir a la derecha
      if((GapPosition<GapPositionDer)&&(GapPositionDer_back+GapVelocityDer_back+velocityold<0)){
        Change_Lines_Der=true;
      }
      else if(Gap<GapDer){
        Direccional_Derecha=true;
      }
      //Si puede girar a ambos lados escoge uno
      if(Change_Lines_Izq&&Change_Lines_Der){
            //Se utiliza el gap total = gap posicion + gap velocidad con los carriles
            //vecinos para decidir a cuál cambiarse. Si existe el mismo gap en ambos
            //carriles el carro escoge cambiarse al carril de la derecha.
        if(GapPositionDer+GapVelocityDer>=GapPositionIzq+GapVelocityIzq){
          Change_Lines_Der=true; Change_Lines_Izq=false;
        }
        else{// if(GapPositionDer<GapPositionIzq){
          Change_Lines_Izq=true; Change_Lines_Der=false;  
        }
        /*
        else if(GapPositionIzq=GapPositionDer){

        }*/
      }
    }  
  }
  else if(tipo==2){
    //Cambio de carril implementado por Plinio Triana con modificaciones propias
    //Incentivo Gap<GapIzq o Gap<GapDer
    if((Gap<GapIzq)||(Gap<GapDer)){
      //Criterio de seguridad para ir a la izquierda
      if((GapPosition<GapPositionIzq)&&(GapPositionIzq_back+GapVelocityIzq_back+velocityold<0)){
        Change_Lines_Izq=true;
      }
      else if(Gap<GapIzq){
        Direccional_Izquierda=true;
      }
      //Criterio de seguridad para ir a la derecha
      if((GapPosition<GapPositionDer)&&(GapPositionDer_back+GapVelocityDer_back+velocityold<0)){
        cout<<GapPosition<<" "<<GapPositionDer<<" "<<GapVelocityDer_back<<" "<<GapPositionDer_back<<endl;
        Change_Lines_Der=true;
      }
      else if(Gap<GapDer){
        Direccional_Derecha=true;
      }
      //Si puede girar a ambos lados escoge uno
      if(Change_Lines_Izq&&Change_Lines_Der){
            //Se utiliza el gap total = gap posicion + gap velocidad con los carriles
            //vecinos para decidir a cuál cambiarse. Si existe el mismo gap en ambos
            //carriles el carro escoge cambiarse al carril de la derecha.
        if(GapPositionDer+GapVelocityDer>=GapPositionIzq+GapVelocityIzq){
          Change_Lines_Der=true; Change_Lines_Izq=false;
        }
        else{// if(GapPositionDer<GapPositionIzq){
          Change_Lines_Izq=true; Change_Lines_Der=false;  
        }
        /*
        else if(GapPositionIzq=GapPositionDer){

        }*/
      }
    }  
  }
	
}

void Carro::Decide_Ceder_El_Paso(Crandom & ran2,double Amabilidad){
  //Criterio de seguridad para evitar que los carros frenen demasiado 
  //cuando el carro está demasiado cerca
  if(Ceder_Paso_Derecha){
    if(GapPositionDer<velocityold/3){
      Ceder_Paso_Derecha=false;
    }
  }
  if(Ceder_Paso_Izquierda){
    if(GapPositionIzq<velocityold/3){
      Ceder_Paso_Izquierda=false;
    }
  }

  double pp;
  if(Ceder_Paso_Derecha||Ceder_Paso_Izquierda){
    pp=ran2.r();
    if(pp<Amabilidad){
      if(Ceder_Paso_Derecha&&Ceder_Paso_Izquierda){
        if(GapPositionIzq>GapPositionDer){
          Ceder_Paso_Izquierda=true;
          Ceder_Paso_Derecha=false;
        }
        else{//Cuando GapPositionDer=GapPositionIzq se da el paso 
          //al carro en la derecha
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

  //Llenar la información del carro de delante con la información
  //del carro en el carril de al lado a quien se le cede el paso
  if(Ceder_Paso_Derecha){
    Gap=GapDer;
    BrakeAdelante=BrakeAdelanteDer;
  }

  if(Ceder_Paso_Izquierda){
    Gap=GapIzq;
    BrakeAdelante=BrakeAdelanteIzq;
  }

}
//Se actualiza la velocidad de los carros siguiendo la regla postulada por L.D. Olmos y J.D. Muñoz
//Algoritmo propuesto por Juan Sebastiań Flórez
void Carro::InteractueVelocity(void)
{
  int gap=Gap;
  
  //Modificacion de la velocidad
  
  if(gapfrenado[velocityold]>gap){
    //Frenado normal
    while((gap<gapfrenado[velocityold])||(gap>gapaceleracion[velocityold])){
      velocityold=velocityold-1;
      gap+=1;
    }
    //Modificar el valor de las variables
    brakeold=1;
    delayold=0;
  }
  else if((gap>gapaceleracion[velocityold])){
    if((gap<gapaceleracion[velocityold]+2)&&(BrakeAdelante==1)){
      //Frenado instantaneo
      velocityold=max(velocityold-1,0);  //La velocidad no puede ser negativa
      delayold=0;
      brakeold=1;
    }
    
    else{
      brakeold=0;
      delayold+=1;  
      if(delayold==retardo[velocityold]){
      //Acelerate
      velocityold=min(velocityold+1,VELMAX); //La velocidad no puede exceder VELMAX
      delayold=0;
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
    delayold=0;
    brakeold=0;
  }
  
}

// Interactue velocity 2
//Algoritmo propuesto por Luis Eduardo Olmos
void Carro::InteractueVelocity2(void)
{
  int b=1;
  float nuevogap;
  nuevogap=Gap;

  for(int m=1;m<20;m++)
    {

      int gapfrenado,gapaceleracion;
      int retardo;
      switch (velocityold)
	{
	case 0:gapfrenado=0;gapaceleracion=3;retardo=1;break;
	case 1:gapfrenado=3;gapaceleracion=4;retardo=1;break;
	case 2:gapfrenado=3;gapaceleracion=5;retardo=1;break;
	case 3:gapfrenado=4;gapaceleracion=5;retardo=1;break;
	case 4:gapfrenado=5;gapaceleracion=6;retardo=2;break;
	case 5:gapfrenado=6;gapaceleracion=7;retardo=2;break;
	case 6:gapfrenado=6;gapaceleracion=8;retardo=2;break;
	case 7:gapfrenado=7;gapaceleracion=9;retardo=2;break;
	}
      if(gapfrenado>nuevogap){velocityold=velocityold-1;nuevogap+=1;b=0;delayold=0;brakeold=1;}
      else if((gapfrenado<=nuevogap)&&(nuevogap<=gapaceleracion)){
	velocityold=velocityold;
	if(b!=0)brakeold=0;
	b=1;delayold=0;}
      else if(nuevogap>=gapaceleracion)	
	{
	  if((BrakeAdelante==1)&&((nuevogap==gapaceleracion)||(nuevogap==gapaceleracion+1)))
	    {
	      if(velocityold!=0)velocityold=velocityold-1;
	      brakeold=1;delayold=0;
	    }
	  else
	    {
	      delayold+=1;
	      brakeold=0;
	      if(delayold==retardo)
		{
		  velocityold=min(velocityold+1,VELMAX);delayold=0;
		}
	    }
	  b=1;
	}
      
      if(b==1)break;
    }
}


void Carro::Actualice(void){
  velocityold=velocitynew;
  brakeold=brakenew;
  delayold=delaynew;
  occupiedold=occupiednew;
  Id_old=Id_new;
  Static_old=Static_new;
  Time_Static_old=Time_Static_new;
  occupiednew=false;
  //Se reinician los indicadores de cambio de carril
  Change_Lines_Izq=false;
  Change_Lines_Der=false;
  Change_Lines_Der_1=false;
  Change_Lines_Izq_1=false;
  //Se reinician los indicadores de luces direccionales
  Direccional_Derecha=false;
  Direccional_Izquierda=false;
  //Se reinician los indicadores de ceder el paso
  Ceder_Paso_Derecha=false;
  Ceder_Paso_Izquierda=false;
}


//______________________Funciones del carril_____________________________________________________________
//_______________________________________________________________________________________________________

Carril::Carril(void){VELMAX=7;}

int Carril::Inicie(int CantidadCarros1, Crandom & ran2, int contador_id){
  //Iniciar todos las casillas de la calle con variables nulas
  int ii;
  for(ii=0;ii<Lx;ii++){
    Calle[ii].Emptyold();
    Calle[ii].Emptynew();
    Calle[ii].State_Change_Lines_Izq(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].State_Change_Lines_Der(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].State_Change_Lines_Izq_1(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].State_Change_Lines_Der_1(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].Set_Static_old(false);//Ningún carro inicia como estático
    Calle[ii].Set_Static_new(false);//Ningún carro inicia como estático
    Calle[ii].Set_Time_Static_old(0);//Ningún carro inicia como estático
    Calle[ii].Set_Time_Static_new(0);//Ningún carro inicia como estático
    Calle[ii].Set_Direccional_Derecha(false);//Ningún carro inicia con direccionales
    Calle[ii].Set_Direccional_Izquierda(false);//Ningún carro inicia con direccionales
    Calle[ii].Set_Ceder_Paso_Derecha(false);//Ningún carro cede el paso inicialmente 
    Calle[ii].Set_Ceder_Paso_Izquierda(false);//Ningún carro cede el paso inicialmente 
  }
  //Posiciones iniciales de los carros
  int CantidadCarros=min(CantidadCarros1,CantidadMaxima);
  CarAmount=CantidadCarros;
  int ix,place;
  int CantidadCasillas;
  int CasillasOcupadas=0;
  place=0;
  //Con este generador de posiciones no se puede garantizar que se genera una distribución de carros pegados,
  //para evitar loops sin fin al contar las casillas vacias se debe imponer que cada carro ocupa tres casillas  
  for(ix=0;ix<CantidadCarros;ix++){
    CantidadCasillas=(int)(ran2.r()*double(Lx-CasillasOcupadas));
    ii=0;
    while(ii<CantidadCasillas){ //Contar la cantidad de casillas vacias
      if((Calle[place].GetOccupiedold()==false)&&(Calle[(place+1)%Lx].GetOccupiedold()==false)&&(Calle[(place+Lx-1)%Lx].GetOccupiedold()==false)){
	      ii+=1;
      }
      place=(place+1)%Lx;
    }
    //Encontrar la siguiente casilla vacia y ocuparla
    while((Calle[place].GetOccupiedold()==true)||(Calle[(place+1)%Lx].GetOccupiedold()==true)||(Calle[(place+Lx-1)%Lx].GetOccupiedold()==true)){
      place=(place+1)%Lx;
    }
    Calle[place].Ocuparold(); //ocupar la casilla
    //Calle[place].SetId(contador_id); //darle un Id al carro
    //contador_id+=1;
    //Si el carro queda a separado una casilla de algún otro carro puesto anteriormente entonces quita tres casillas
    if((true==Calle[(place+3)%Lx].GetOccupiedold())||(true==Calle[(place+Lx-3)%Lx].GetOccupiedold())){CasillasOcupadas+=3;}
    else{CasillasOcupadas+=2;}
    //Empezar a contar desde la siguiente casilla que no esta ocupada por el carro puesto
    place=(place+2)%Lx;
  }
  //Velocidades iniciales
  int jj,vel,freno,espera;
  int VelIni;
  for(ii=0;ii<Lx;){
    if(Calle[ii].GetOccupiedold()==true){
      //Encontrar la cantidad de casillas hasta el proximo carro
      jj=ii+2;
      CantidadCasillas=0;
      while(Calle[jj%Lx].GetOccupiedold()==false){
	      CantidadCasillas+=1;
	      jj+=1;
      }
      //Determinar la velocidad inicial
      VelIni=(int)(round(ran2.r()*CantidadCasillas));
      vel=min(VelIni,VELMAX);
      //Calle[ii].Cargueold(vel,0,0);
      Calle[ii].Cargueold_V2(vel,0,0,contador_id,0,false);
      contador_id+=1;
      ii=jj;
    }
    else{ii+=1;}
  } 

  return contador_id;
}

int Carril::Inicie_Carros_Estaticos(int CantidadCarros1, Crandom & ran2, int contador_id, int Carros_Estaticos[]){
//Iniciar todos las casillas de la calle con variables nulas
  int ii;
  for(ii=0;ii<Lx;ii++){
    Calle[ii].Emptyold();
    Calle[ii].Emptynew();
    Calle[ii].State_Change_Lines_Izq(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].State_Change_Lines_Der(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].State_Change_Lines_Izq_1(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].State_Change_Lines_Der_1(false); //Ninigún carro desea de cambiar de carril en t=0
    Calle[ii].Set_Static_old(false);//Ningún carro inicia como estático
    Calle[ii].Set_Static_new(false);//Ningún carro inicia como estático
    Calle[ii].Set_Time_Static_old(0);//Ningún carro inicia como estático
    Calle[ii].Set_Time_Static_new(0);//Ningún carro inicia como estático
    Calle[ii].Set_Direccional_Derecha(false);//Ningún carro inicia con direccionales
    Calle[ii].Set_Direccional_Izquierda(false);//Ningún carro inicia con direccionales
    Calle[ii].Set_Ceder_Paso_Derecha(false);//Ningún carro cede el paso inicialmente 
    Calle[ii].Set_Ceder_Paso_Izquierda(false);//Ningún carro cede el paso inicialmente 
  }
  //Posiciones iniciales de los carros
  int CantidadCarros=min(CantidadCarros1,CantidadMaxima);
  CarAmount=CantidadCarros;
  int Cant_Carros_Est = min(Carros_Estaticos[0],CantidadMaxima);
  int Cant_Grupos_Carros_Est=Carros_Estaticos[1];
  int ix,place;
  int CantidadCasillas;
  int CasillasOcupadas=0;
  cout<<CantidadCarros<<" "<<Carros_Estaticos[0]<<" "<<Carros_Estaticos[1]<<" "<<CantidadMaxima<<endl;
  

  //Primero se ponen los carros que van a permanecer estáticos
  place=Lx/3;;
  for(int ii=0;ii<Cant_Grupos_Carros_Est;ii++){
    //Se pone un grupo de carros estáticos seguidos de longitud 
    //Carros_Estaticos[2+ii*2]
    for(int jj=0;jj<Carros_Estaticos[2+ii*2];jj++){
      //cout<<"Carro estatico en "<<place<<endl;
      Calle[place].Ocuparold();//Ocupar la casilla
      Calle[place].Set_Static_old(true);//El carro es marcado como estatico
      //Si el carro queda a separado una casilla de algún otro carro puesto anteriormente entonces quita tres casillas
      if((true==Calle[(place+3)%Lx].GetOccupiedold())||(true==Calle[(place+Lx-3)%Lx].GetOccupiedold())){CasillasOcupadas+=3;}
      else{CasillasOcupadas+=2;}
      place = (place+2)%Lx;
    }
    //Se cuentan Carros_Estaticos[2+(1+2*ii)] casillas luego del grupo de 
    //carros estaticos
    for(int jj=0;jj<Carros_Estaticos[2+(1+2*ii)];jj++){
      place = (place+1)%Lx;
    }
  }

  place=0;
  for(ix=0;ix<CantidadCarros-Cant_Carros_Est;ix++){
    CantidadCasillas=(int)(ran2.r()*double(Lx-CasillasOcupadas));
    ii=0;
    while(ii<CantidadCasillas){ //Contar la cantidad de casillas vacias
      if((Calle[place].GetOccupiedold()==false)&&(Calle[(place+1)%Lx].GetOccupiedold()==false)&&(Calle[(place+Lx-1)%Lx].GetOccupiedold()==false)){
        ii+=1;
      }
      place=(place+1)%Lx;
    }
    //Encontrar la siguiente casilla vacia y ocuparla
    while((Calle[place].GetOccupiedold()==true)||(Calle[(place+1)%Lx].GetOccupiedold()==true)||(Calle[(place+Lx-1)%Lx].GetOccupiedold()==true)){
      place=(place+1)%Lx;
    }
    Calle[place].Ocuparold(); //ocupar la casilla
    //Si el carro queda a separado una casilla de algún otro carro puesto anteriormente entonces quita tres casillas
    if((true==Calle[(place+3)%Lx].GetOccupiedold())||(true==Calle[(place+Lx-3)%Lx].GetOccupiedold())){CasillasOcupadas+=3;}
    else{CasillasOcupadas+=2;}
    //Empezar a contar desde la siguiente casilla que no esta ocupada por el carro puesto
    place=(place+2)%Lx;
  }


  //Velocidades iniciales
  int jj,vel,freno,espera;
  int VelIni;
  for(ii=0;ii<Lx;){
    if(Calle[ii].GetOccupiedold()==true){
      //Si el carro está detenido entonces se su velocidad es nula
      //y tiene el freno activado
      if(Calle[ii].Get_Static_old()){
        //Encontrar la cantidad de casillas hasta el proximo carro
        //cout<<"Car Static "<<ii<<endl;
        jj=ii+2;
        while(Calle[jj%Lx].GetOccupiedold()==false){
          jj+=1;
        }
        Calle[ii].Cargueold(0,1,0);
        Calle[ii].SetId_old(contador_id);
        //cout<<contador_id<<endl;
        contador_id+=1;
        ii=jj;
      }
      else{
        //Encontrar la cantidad de casillas hasta el proximo carro
        jj=ii+2;
        CantidadCasillas=0;
        while(Calle[jj%Lx].GetOccupiedold()==false){
          CantidadCasillas+=1;
          jj+=1;
        }
        //Determinar la velocidad inicial
        VelIni=(int)(round(ran2.r()*CantidadCasillas));
        vel=min(VelIni,VELMAX);
        //Calle[ii].Cargueold(vel,0,0);
        Calle[ii].Cargueold_V2(vel,0,0,contador_id,0,false);
        contador_id+=1;
        ii=jj;  
      }
      
    }
    else{ii+=1;}
  } 

  return contador_id;  
}

//Time_Static_Old > tmax para todos los carros que se encuentren estaticos
void Carril::Set_Static_Cars_Permanent(int tmax){
  for(int ii=0;ii<Lx;ii++){
    if(Calle[ii].Get_Static_old()){
      //cout<<"Casilla "<<ii<<endl;
      Calle[ii].Set_Time_Static_old(tmax+100);
    }
  }
}

//Interaccione intercambia información entre los carros para que puedan calcular el gap

void Carril::Interaccione(void){
  int ii,jj;
  int gapposition,velocidad,freno;
  //Los carros intercambian información a través del carril para calcular el gap
  for(ii=0;ii<Lx;){
    if(Calle[ii].GetOccupiedold()==true){
      jj=ii+2;
      while(Calle[jj%Lx].GetOccupiedold()==false){
	      jj=jj+1;         
      }
      gapposition=jj-ii-1; //Calculo del Gap de posición
      velocidad=Calle[jj%Lx].GetVelold();
      freno=Calle[jj%Lx].GetLightold();
      if(Calle[ii].Get_Ceder_Paso_Derecha()||Calle[ii].Get_Ceder_Paso_Izquierda()){
      }
      else{
        Calle[ii].CalculeGap(gapposition,velocidad,freno);
      }
      ii=jj;
    }
    else{ii+=1;}
  }
  
}

//Los carros calculan la velocidad en el tiepo t
void Carril::Evolucionar_Velocidad(void){
  int ii,jj;
  //La velocidad de cada carro evoluciona a partir de la regla postulada por Lucho Olmos
  for(ii=0;ii<Lx;){
    if(Calle[ii].GetOccupiedold()==true){
      //Calle[ii].InteractueVelocity2();
      if(Calle[ii].Get_Static_old()){
        //cout<<"Carro estatico en posicion "<<ii<<endl;
        if(Calle[ii].Get_Time_Static_old()<=1){
          //cout<<1<<endl;
          Calle[ii].Cargueold(0,1,0);
          Calle[ii].Set_Static_old(false);
          Calle[ii].Set_Time_Static_old(0);
        }
        else{
          //cout<<2<<endl;
          Calle[ii].Disminuir_Time_Static_old();
          Calle[ii].Cargueold(0,1,0);
        }
      }
      else{
        Calle[ii].InteractueVelocity();
      }
      jj=ii+2;
      while(Calle[jj%Lx].GetOccupiedold()==false){
        jj=jj+1;         
      }
      ii=jj;
    }
    else{ii+=1;}
  }
}

//Los carros se mueven
void Carril::Muevase(void){
  int ii,jj;
  int VEL,FRENO,RETARDO,POSITION,TIME_STA,ID;
  bool STA;
  
  //Se intercambia información entre las celdas (los carros se mueven), las nuevas posiciones son guardadas en las variables *new
  for(ii=0;ii<Lx;){
    //Si la casilla estaba oupada en el tiempo acterior se hace evolucionar en el tiempo
    if(true==Calle[ii].GetOccupiedold()){
      VEL=Calle[ii].GetVelold();
      FRENO=Calle[ii].GetLightold();
      RETARDO=Calle[ii].GetDelayold();
      TIME_STA=Calle[ii].Get_Time_Static_old();
      STA=Calle[ii].Get_Static_old();
      ID=Calle[ii].GetId_old();
      POSITION=(ii+VEL)%Lx; //Nueva posición del carro (condiciones periodicas)
      // void Cargueold(int vel, int freno, int del);
      Calle[POSITION].Carguenew_V2(VEL,FRENO,RETARDO,ID,TIME_STA,STA); 
      Calle[POSITION].Ocuparnew();
      //Calle[POSITION].SetId(Calle[ii].GetId());
      //Calle[POSITION].Set_Time_Static(TIME_STA);
      //Calle[POSITION].Set_Static(STA);
      Calle[ii].Emptyold();
      jj=ii+2;
      while((Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
        jj=jj+1;         
      }
      ii=jj;
    }
    else{ii+=1;}
  }
  //Se actualizan las posiciones de los carros
  for(ii=0;ii<Lx;){
    if(true==Calle[ii].GetOccupiednew()){
      Calle[ii].Actualice();
      jj=ii+2;
      while((Calle[jj%Lx].GetOccupiednew()==false)&&(jj<Lx)){
        jj=jj+1;         
      }
      ii=jj;
    }
    else{ii+=1;}
  }
}

//Funciones de carga para los carros del carril
int Carril::Set_Car_Static(int Casilla,int Time_Sta){
  if(Calle[Casilla].GetOccupiedold()){
    Calle[Casilla].Set_Static_old(true);
    Calle[Casilla].Set_Time_Static_old(Time_Sta);
    return 1;
  }
  else{
    return -1;
  }
}
//____________________Calculo de Variables del sistema____________


double Carril::FlujoEspacial(int tipo){
  double suma=0,vel;
  int ii,jj,contador=0;
  for(ii=0;ii<Lx;){
    if(true==Calle[ii].GetOccupiedold()){
      suma+=Calle[ii].GetVelold();
      contador+=1;
      jj=ii+2;
      while(Calle[jj%Lx].GetOccupiedold()==false){
        jj=jj+1;         
      }
      ii=jj;
    }
    else{ii+=1;}
  }
  if(tipo==1){//Flujo espacial
  	//cout<<"FlujoEspacial suma "<<suma<<endl;
  	//cout<<"FlujoEspacial CarAmount "<<contador<<endl;
    if(contador > 0){
      suma/=contador;
      return suma;  
    }
  	else{ return 0.0;}
  }
  else{//Flujo temporal
    //No se ha implementado
  }
}

double Carril::GetCarrilRho(void){
	double suma=0.0;
	int ii,jj;
	for(ii=0;ii<Lx;){
    	if(true==Calle[ii].GetOccupiedold()){
      		suma+=1.0;
      		jj=ii+2;
      		while((Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
        		jj=jj+1;         
      		}
     		ii=jj;
    	}
    	else{ii+=1;}
  	}
  	return (2*suma)/double(Lx);
} 


//______________________________Interaccion entre carriles______________________________________________
void Avenida::Empezar(int CantCarril, int Largo){
	Carriles=CantCarril; 
	Avenida_1 = new Carril[Carriles];
  Lx=Largo;
	for(int ii=0;ii<Carriles;ii++){
		Avenida_1[ii].Empezar(Lx);
	}
}

void Avenida::Inicie(int CantidadCarros[], Crandom & ran2){
	int id=0;
	for(int cc=0;cc<Carriles;cc++){
		cout<<"Carril "<<cc<<" Id "<<id<<endl; 
    //id guarda el contador del número de identificación, el cual cada carril
    //modifica si el carril tiene carros
		id=Avenida_1[cc].Inicie(CantidadCarros[cc],ran2,id);
    cout<<CantidadCarros[cc]<<endl;
    cout<<"Carril "<<cc<<" Id "<<id<<endl; 
  }
  //Guardar la información de la cantidad de carros que hoy en la avenida
  Cantidad_Carros=id;
}

void Avenida::Inicie_Carros_Estaticos(int CantidadCarros[], Crandom & ran2, int Carros_Estaticos[]){
  int **Carros_Est;
  Carros_Est = new int*[Carriles];
  int cc=0;
  int contador_1=0,contador_2=0;
  cout<<"Bla "<<Carros_Estaticos[0]<<endl;
  while(cc<Carriles){
    while(Carros_Estaticos[contador_2]>=0){
      contador_2+=1;
      cout<<contador_2<<" "<<Carros_Estaticos[contador_2]<<endl;
    }
    //cout<<Carriles<<endl;
    Carros_Est[cc]=new int[contador_2-contador_1];
    for(int ii=0;ii<contador_2-contador_1;ii++){
      Carros_Est[cc][ii]=Carros_Estaticos[contador_1+ii];
      cout<<cc<<" "<<Carros_Estaticos[contador_1+ii]<<" "<<ii<<" "<<contador_1<<" "<<contador_2<<endl;
      //cout<<"Carril "<<cc<<endl;
      //cout<<Carros_Estaticos[contador_1+ii]<<endl;
    }
    contador_1=contador_2+1;
    contador_2+=1;
    cc+=1;
  }

  int id=0;
  for(cc=0;cc<Carriles;cc++){
    //cout<<"Carril "<<cc<<" Id "<<id<<endl; 
    //id guarda el contador del número de identificación, el cual cada carril
    //modifica si el carril tiene carros
    //cout<<"Carril "<<cc<<endl;
    cout<<"Inicie 1, carril "<<cc<<" Cantidad de Carros "<<CantidadCarros[cc]<<endl;
    id=Avenida_1[cc].Inicie_Carros_Estaticos(CantidadCarros[cc],ran2,id,Carros_Est[cc]);
    cout<<"Inicie, carril "<<cc<<" id "<<id<<endl;
  } 
  //Guardar la información de la cantidad de carros que hoy en la avenida
  Cantidad_Carros=id;

  //Borrar los punteros usados
  for(int ii=0;ii<Carriles;ii++){
    delete Carros_Est[ii];
  }
  delete Carros_Est;
}

void Avenida::Set_Static_Cars_Permanent(int tmax){
  for(int cc=0;cc<Carriles;cc++){
    Avenida_1[cc].Set_Static_Cars_Permanent(tmax);
  }
}

void Avenida::Calcular_Gaps(bool Multicarril){
	int ii,jj;
	int gapposition,velocidad,freno,gapposition_back,velocidad_back,freno_back;
	//Calcular los gaps de cada carril en un modelo de un solo carril
	for(int cc=0;cc<Carriles;cc++){
		Avenida_1[cc].Interaccione();
	}
	if(Multicarril){
		//Calcular los gaps de cada carril respecto de los vecinos
		for(int cc=0;cc<Carriles;cc++){//Para cada carril
			if(cc==0){//El carril 0 se carga con información similar a tener un arreglo de carros a la
				//izquierda que se encuentren quietos
				for(int kk=0;kk<Lx;){//kk++){//Para cada carro calcular el gap con el carril de la izquierda
					if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
		      			//Calcular el gap con el carro adelante y atrás en el otro carril
		      			Avenida_1[cc].Calle[kk].CalculeGapIzq(0,0,1,0,0,1);
		      			//Encontrar el siguiente carro en el carril
		      			//cout<<cc<<" "<<kk<<" "<<Lx<<endl;
		      			jj=kk+2;
		      			while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
			   			    jj=jj+1;         
		      			}	
		      			kk=jj;
		    		    //kk+=1; 
	                    //cout<<cc<<" "<<kk<<" "<<Lx<<endl; 
	          }
		    		else{kk+=1;}	
				}
			}
			if(cc==Carriles-1){//El carril Carriles-1 se carga con información similar a tener un arreglo de carros a la
				//derecha que se encuentren quietos
				for(int kk=0;kk<Lx;){//Para cada carro calcular el gap con el carril de la izquierda
					if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
		      			//Calcular el gap con el carro adelante y atrás en el otro carril
		      			Avenida_1[cc].Calle[kk].CalculeGapDer(0,0,1,0,0,1);
		      			//Encontrar el siguiente carro en el carril
		      			jj=kk+2;
		      			while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
			   			   jj=jj+1;         
		      			}	
		      			kk=jj;
		    		}
		    		else{kk+=1;}	
				}
			}
			if(cc>0){//El único carril sin vecino izquierdo es el carril 0
				for(int kk=0;kk<Lx;){//Para cada carro calcular el gap con el carril de la izquierda
					if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
		      			//Gap con el carro de adelante de la izquierda
			    			jj=kk+2;
		      			while((Avenida_1[cc-1].Calle[jj%Lx].GetOccupiedold()==false)&&(jj-kk<Lx+2)){
			      			//(jj-kk)<Lx+2 restringe la busqueda a una sola pasada sobre el carril completo
		      				//y está relacionado con las condiciones de frontera periodicas para cada carril
			      			jj=jj+1;         
		      			}
		      			//Si hay carros en el carril de la izquierda
		      			if(jj-kk<Lx+2){
			      			gapposition=jj-kk-1; //Calculo del Gap de posición
			      			velocidad=Avenida_1[cc-1].Calle[jj%Lx].GetVelold();
			      			freno=Avenida_1[cc-1].Calle[jj%Lx].GetLightold();
			      		}
			      		else{//No hay carros en el carril de la izquierda
			      			gapposition=Lx;
			      			velocidad=Avenida_1[cc].GetVelMax()+1;
			      			freno=0;
			      		}
		      			//Gap con el carro de atras de la izquierda
		      			jj=kk+1;
		      			while((Avenida_1[cc-1].Calle[(jj+Lx)%Lx].GetOccupiedold()==false)&&(kk-jj<Lx-1)){
			      			//(kk-jj)<Lx-1 restringe la busqueda a una sola pasada sobre el carril completo
		      				//y está relacionado con las condiciones de frontera periodicas para cada carril
			      			jj=jj-1;         
		      			}
		      			//Si hay carros en el carril de la izquierda
		      			if(kk-jj<Lx-1){
		      				gapposition_back=-(kk-jj-1); //Calculo del Gap de posición
		      				jj=(jj+Lx)%Lx;//jj puede ser negativo, en tal caso debe encontrarse
		      				//a cual casilla del carril del lado corresponde tal número
			      			velocidad_back=Avenida_1[cc-1].Calle[jj%Lx].GetVelold();
			      			freno_back=Avenida_1[cc-1].Calle[jj%Lx].GetLightold();	
		      			}
		      			else{//No hay carros en el carril de la izquierda
		      				gapposition_back=-Lx;
		      				velocidad_back=0;
		      				freno_back=1;
		      			}
		      			//Calcular el gap con el carro adelante y atrás en el otro carril
		      			Avenida_1[cc].Calle[kk].CalculeGapIzq(gapposition,velocidad,freno,gapposition_back,velocidad_back,freno_back);
		      			//Encontrar el siguiente carro en el carril
		      			jj=kk+2;
		      			while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
			   	  		  jj=jj+1;         
		      			}	
		      			kk=jj;
		    		}
		    		else{kk+=1;}	
				}
			}
			if(cc<Carriles-1){//El único carril sin vecino derecho es el carril Carriles-1
				for(int kk=0;kk<Lx;){//Para cada carro calcular el gap con el carril de la derecha
					if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
						//Gap con el carro de adelante de la derecha
						jj=kk+2;
		      			while((Avenida_1[cc+1].Calle[jj%Lx].GetOccupiedold()==false)&&(jj-kk<Lx+2)){
		      				//(jj-kk)<Lx+2 restringe la busqueda a una sola pasada sobre el carril completo
		      				//y está relacionado con las condiciones de frontera periodicas para cada carril
			      			jj=jj+1;         
		      			}
		      			//Si hay carros en el carril de la derecha
		      			if(jj-kk<Lx+2){
			      			gapposition=jj-kk-1; //Calculo del Gap de posición
			      			velocidad=Avenida_1[cc+1].Calle[jj%Lx].GetVelold();
			      			freno=Avenida_1[cc+1].Calle[jj%Lx].GetLightold();
			      		}
			      		else{//No hay carros en el carril de la derecha
			      			gapposition=Lx;
			      			velocidad=Avenida_1[cc].GetVelMax()+1;
			      			freno=0;
			      		}
		      			//Gap con el carro de atras de la derecha
		      			jj=kk+1;
		      			while((Avenida_1[cc+1].Calle[(jj+Lx)%Lx].GetOccupiedold()==false)&&(kk-jj<Lx-1)){
		      				//(kk-jj)<Lx-1 restringe la busqueda a una sola pasada sobre el carril completo
		      				//y está relacionado con las condiciones de frontera periodicas para cada carril
			      			jj=jj-1;         
		      			}
		      			//Si hay carros en el carril de la derecha
		      			if(kk-jj<Lx-1){
		      				gapposition_back=-(kk-jj-1); //Calculo del Gap de posición
		      				jj = (jj+Lx)%Lx; //jj puede ser negativo, en tal caso debe encontrarse
		      				//a qué casilla del carril del lado corresponde tal número
			      			velocidad_back=Avenida_1[cc+1].Calle[jj%Lx].GetVelold();
			      			freno_back=Avenida_1[cc+1].Calle[jj%Lx].GetLightold();	
		      			}
		      			else{//No hay carros en el carril de la derecha
		      				gapposition_back=-Lx;
		      				velocidad_back=0;
		      				freno_back=1;
		      			}
		      			Avenida_1[cc].Calle[kk].CalculeGapDer(gapposition,velocidad,freno,gapposition_back,velocidad_back,freno_back);
		      			//Encontrar el siguiente carro en el carril
		      			jj=kk+2;
		      			while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
			   			   jj=jj+1;         
		      			}	
		      			kk=jj;
		    		}
		    		else{kk+=1;}	
				}
			}
		}		
	}
}

void Avenida::Decide_Change_Lines(int tipo){
  //Cada carro decide si va a cambiar de carril y guarda esa información en los booleanos
  // bool Change_Lines_Izq , bool Change_Lines_Der;
  //Si el carro quiere cambiar de carril pero no puede entonces activa las direccionales
  // bool Direccional_Derecha, bool Direccional_Izquierda;
  int jj;
  for(int cc=0;cc<Carriles;cc++){
    for(int kk=0;kk<Lx;){
      if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
        Avenida_1[cc].Calle[kk].DecideChangeLines(tipo);
        jj=kk+2;
        while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
          jj=jj+1;         
        }
        kk=jj;
      }
      else{kk+=1;}
    }
  }
}

void Avenida::Cambio_Carril(void){
	//Los carros cambian de carril
  int jj;
	int vel,freno,del,car_id,Time_Sta;
  bool Sta;
	for(int cc=0;cc<Carriles;cc++){
		for(int kk=0;kk<Lx;){
			if(Avenida_1[cc].Calle[kk].GetOccupiedold()){
				if(Avenida_1[cc].Calle[kk].GetChangeLines_Izq()){
          if(Avenida_1[cc].Calle[kk].Get_Static_old()){}
          else{
            if((cc>0)&&(Avenida_1[cc-1].Calle[kk].GetOccupiedold()||Avenida_1[cc-1].Calle[(kk+1)%Lx].GetOccupiedold()||
              Avenida_1[cc-1].Calle[(kk-1+Lx)%Lx].GetOccupiedold())){
              //Por ahora tienen prioridad los carros que van en los carriles izquierdos, pues
              //son los primeros en ser recorridos por el ciclo
            }
            else{
              cout<<"Carril "<<cc<<" Cambio izquerda "<<kk<<" Car ID "<<Avenida_1[cc].Calle[kk].GetId_old()<<endl;
              // Se saca información del carro que desea cambiar de carril
              vel=Avenida_1[cc].Calle[kk].GetVelold();
              freno=Avenida_1[cc].Calle[kk].GetLightold();
              del=Avenida_1[cc].Calle[kk].GetDelayold();
              car_id=Avenida_1[cc].Calle[kk].GetId_old();
              Time_Sta=Avenida_1[cc].Calle[kk].Get_Time_Static_old();
              Sta=Avenida_1[cc].Calle[kk].Get_Static_old();
              // Se guarda la información en el carril de la izquierda
              Avenida_1[cc-1].Calle[kk].Emptynew();
              Avenida_1[cc-1].Calle[kk].Ocuparold();
              Avenida_1[cc-1].Calle[kk].Cargueold_V2(vel,freno,del,car_id,Time_Sta,Sta);
              Avenida_1[cc-1].Calle[kk].State_Change_Lines_Izq_1(true);
              Avenida_1[cc-1].Calle[kk].State_Change_Lines_Der_1(false);
              //Avenida_1[cc-1].Calle[kk].Cargueold(vel,freno,del);
              //Avenida_1[cc-1].Calle[kk].SetId(car_id);
              //Avenida_1[cc-1].Calle[kk].Set_Time_Static(Time_Sta);
              //Avenida_1[cc-1].Calle[kk].Set_Static(Sta);
              //Desocupar el carro que se cambio de carril
              Avenida_1[cc].Calle[kk].Emptynew();
              Avenida_1[cc].Calle[kk].Emptyold();  
            }
          }
		    }
		    else if(Avenida_1[cc].Calle[kk].GetChangeLines_Der()){
          if(Avenida_1[cc].Calle[kk].Get_Static_old()){}
          else{
            if((cc<Carriles-1)&&(Avenida_1[cc+1].Calle[kk].GetOccupiedold()||Avenida_1[cc+1].Calle[(kk+1)%Lx].GetOccupiedold()||
              Avenida_1[cc+1].Calle[(kk-1+Lx)%Lx].GetOccupiedold())){
              //Por ahora tienen prioridad los carros que van en los carriles izquierdos, pues
              //son los primeros en ser recorridos por el ciclo
            }
            else{
              cout<<"Carril "<<cc<<" Cambio derecha "<<kk<<" Car ID "<<Avenida_1[cc].Calle[kk].GetId_old()<<endl;
              // Se saca información del carro que desea cambiar de carril
              vel=Avenida_1[cc].Calle[kk].GetVelold();
              freno=Avenida_1[cc].Calle[kk].GetLightold();
              del=Avenida_1[cc].Calle[kk].GetDelayold();
              car_id=Avenida_1[cc].Calle[kk].GetId_old();
              Time_Sta=Avenida_1[cc].Calle[kk].Get_Time_Static_old();
              Sta=Avenida_1[cc].Calle[kk].Get_Static_old();
              // Se guarda la información en el carril de la derecha
              Avenida_1[cc+1].Calle[kk].Emptynew();
              Avenida_1[cc+1].Calle[kk].Ocuparold();
              Avenida_1[cc+1].Calle[kk].Cargueold_V2(vel,freno,del,car_id,Time_Sta,Sta);
              Avenida_1[cc+1].Calle[kk].State_Change_Lines_Der_1(true);
              Avenida_1[cc+1].Calle[kk].State_Change_Lines_Izq_1(false);
              //Avenida_1[cc+1].Calle[kk].Cargueold(vel,freno,del);
              //Avenida_1[cc+1].Calle[kk].SetId(car_id);
              //Avenida_1[cc+1].Calle[kk].Set_Time_Static(Time_Sta);
              //Avenida_1[cc+1].Calle[kk].Set_Static(Sta);
              //Desocupar el carro que se cambio de carril
              Avenida_1[cc].Calle[kk].Emptynew();
              Avenida_1[cc].Calle[kk].Emptyold();
            }
  				}
		    }
		    jj=kk+2;
		    //Se encuentra el siguiente carro
				while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
		    	jj=jj+1;         
	      }
	      kk=jj;
	    	}
	    else{kk+=1;}
		}
	}
}

void Avenida::Ceder_El_Paso(Crandom & ran2,double Amabilidad){
  //Los carros reciben la información de los carros de adelante, miran
  //si el carro tiene las direccionales prendidas
  int jj;
  for(int cc=0;cc<Carriles;cc++){
    if(cc>0){//El único carril sin vecino izquierdo es el carril 0
        for(int kk=0;kk<Lx;){//Para cada carro encontrar el carro que le sigue en el carril de la izquierda
          if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
                //Gap con el carro de adelante de la izquierda
                jj=kk+2;
                while((Avenida_1[cc-1].Calle[jj%Lx].GetOccupiedold()==false)&&(jj-kk<Lx+2)){
                  //(jj-kk)<Lx+2 restringe la busqueda a una sola pasada sobre el carril completo
                  //y está relacionado con las condiciones de frontera periodicas para cada carril
                  jj=jj+1;         
                }
                //Si hay carros en el carril de la izquierda
                if(jj-kk<Lx+2){
                  if(Avenida_1[cc-1].Calle[jj%Lx].Get_Direccional_Derecha()){
                    Avenida_1[cc].Calle[kk].Set_Ceder_Paso_Izquierda(true);
                  }
                }
                else{//No hay carros en el carril de la izquierda
                }
                //Encontrar el siguiente carro en el carril
                jj=kk+2;
                while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
                  jj=jj+1;         
                } 
                kk=jj;
            }
            else{kk+=1;}  
        }
      }
      if(cc<Carriles-1){//El único carril sin vecino derecho es el carril Carriles-1
        for(int kk=0;kk<Lx;){//Para cada carro calcular el gap con el carril de la derecha
          if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
            //Gap con el carro de adelante de la derecha
            jj=kk+2;
                while((Avenida_1[cc+1].Calle[jj%Lx].GetOccupiedold()==false)&&(jj-kk<Lx+2)){
                  //(jj-kk)<Lx+2 restringe la busqueda a una sola pasada sobre el carril completo
                  //y está relacionado con las condiciones de frontera periodicas para cada carril
                  jj=jj+1;         
                }
                //Si hay carros en el carril de la derecha
                if(jj-kk<Lx+2){
                  if(Avenida_1[cc+1].Calle[jj%Lx].Get_Direccional_Izquierda()){
                    Avenida_1[cc].Calle[kk].Set_Ceder_Paso_Derecha(true);
                  }
                }
                else{//No hay carros en el carril de la derecha
                }
                //Encontrar el siguiente carro en el carril
                jj=kk+2;
                while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
                 jj=jj+1;         
                } 
                kk=jj;
            }
            else{kk+=1;}  
        }
      }
  }
  //Los carros deciden si ceden el paso 
  for(int cc=0;cc<Carriles;cc++){
    for(int kk=0;kk<Lx;){
      if(Avenida_1[cc].Calle[kk].GetOccupiedold()==true){
        Avenida_1[cc].Calle[kk].Decide_Ceder_El_Paso(ran2,Amabilidad);
        jj=kk+2;
        while((Avenida_1[cc].Calle[jj%Lx].GetOccupiedold()==false)&&(jj<Lx)){
          jj=jj+1;         
        }
        kk=jj;
      }
      else{kk+=1;}
    }
  }
}

void Avenida::Evolucionar_Velocidad_Carril(void){
  //Se intercambia informacion y con ella los carros calculan la velocidad
  //en el tiempo t
  for(int cc=0;cc<Carriles;cc++){
    Avenida_1[cc].Interaccione();
    Avenida_1[cc].Evolucionar_Velocidad();
  }
}

void Avenida::Mover_Carros_Carril(void){
  //Los carros se mueven
  for(int cc=0;cc<Carriles;cc++){
    Avenida_1[cc].Muevase();
  }
}

void Avenida::Avanzar_Carril(void){
	//Cada carril se mueve de forma independiente
	for(int cc=0;cc<Carriles;cc++){
		//cout<<"Carril "<<cc<<endl;
		Avenida_1[cc].Interaccione();
		//cout<<"Carril "<<cc<<" Interaccione"<<endl;
    Avenida_1[cc].Evolucionar_Velocidad();
    //cout<<"Carril "<<cc<<" Evolcionar_Velocidad"<<endl;
		Avenida_1[cc].Muevase();
		//cout<<"Carril "<<cc<<" Muevase"<<endl;
	}
}

//Obtener información del sistema
//Carriles

//Carros
int Avenida::Car_Id(int Carril_Ind, int Casilla_Ind){
	if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetOccupiedold()){
		return Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetId_old();
	}
	else{return -1;}
}

int Avenida::Car_Vel(int Carril_Ind, int Casilla_Ind){
	if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetOccupiedold()){
		return Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetVelold();
	}
	else{return -1;}
}

int Avenida::Car_Brake(int Carril_Ind, int Casilla_Ind){
	if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetOccupiedold()){
		return Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetLightold();
	}
	else{return -1;}
}

int Avenida::Car_Change_Lines(int Carril_Ind, int Casilla_Ind){
	if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetOccupiedold()){
		if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetChangeLines_Izq_1()){
			return 0;
		}
		else if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetChangeLines_Der_1()){
			return 2;
		}
		else{return 1;}
	}
	else{return -1;}	
}

int Avenida::Car_Print_Static(int Carril_Ind, int Casilla_Ind){
  if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetOccupiedold()){
    if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].Get_Static_old()){
      return 0;
    }
    else{
      return 1;
    }
  }
  else{return -1;}
}

int Avenida::Car_Print_Time_Static(int Carril_Ind, int Casilla_Ind){
  if(Avenida_1[Carril_Ind].Calle[Casilla_Ind].GetOccupiedold()){
    return Avenida_1[Carril_Ind].Calle[Casilla_Ind].Get_Time_Static_old();
  }
  else{return -1;}  
}