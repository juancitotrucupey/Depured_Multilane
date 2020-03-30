#include <iostream>
#include <cmath>
#include<fstream>
#include<algorithm>
#define NTAB 32
#define Celd 2000
#define Celda Celd/2
#define VMAX 7
#define sem 30

using namespace std;
//-----------------Random Generator------------------------
int min(int a,int b){
    if(a<=b) return a;
    else return b;
}

class CRandom {
    private:
        int A1,M1,Q1,R1;
        int A2,M2,Q2,R2;
        float AM, EPS, RNMX;
        int iy,idum,idum2,NDIV;
        int iv[NTAB];
    public:
        CRandom (int);
        float r(void);
};

CRandom::CRandom (int seed){
    int i,k;
    A1=40014, M1=2147483563, Q1=53668, R1=12211;
    A2=40692, M2=2147483399, Q2=52774, R2=3791 ;
    AM=1.0/M1; EPS=1.2e-7; RNMX=1.0-EPS; NDIV=(M1-1)/NTAB;

    idum=seed ^ 123456789;
    idum2=idum; 
    for (i=NTAB+8; i>0;i--){
        k=idum/Q1; idum=A1*(idum-k*Q1)-k*R1; if (idum<0) idum+=M1;
        if (i<NTAB) iv[i]=idum;
    }
    iy=iv[0];
}

float CRandom::r(void) {
    int j,k;
    float ranr;
    k=idum/Q1; idum=A1*(idum-k*Q1)-k*R1; if (idum<0) idum+=M1;
    k=idum/Q2; idum=A2*(idum-k*Q2)-k*R2; if (idum<0) idum+=M2;
    j=iy/NDIV;
    iy=iv[j]-idum2; if(iy<0) iy=iy+M1;
    iv[j]=idum;
    ranr=iy*AM;
    if (ranr<RNMX) return ranr; else return RNMX;
}

//---------------Simulacion de Flujo Vehicular----------------------
//Este programa variara el número de carros, es decir la densidad con Celd fijo, de uno en uno o de 10 en 10. Sacará promedios,pero en cuantos pasos de evolución?- Segun los java cada vez que se compelta una vuelta o se espera a que se estabilicen?. En ste se tomara un reloj de 10 pasos.
//Hay dos clases, carro-calle, la que me impoatara será calle, que tenfra valores promedio de velocidades y la densidad.
class Carro{
    private:
        int vx,gap,gapsolo,conretardo,frenadoinsta1,frenadoinsta2;
        int x;
        //  float gapfrenado,gapaceleracion;
        //  float gap,gapsolo;
        int id,siguiente;
    public:
        void inicie(int x0,int vx0,int iden,int idsiguiente);
        void dist(Carro Cuerpo[]);
        void velinicio(float numero);
        void velocidad(Carro Cuerpo[]);
        void posicion(void);
        int getsig(void){return siguiente;};
        void casiguiente(Carro Cuerpo[],int N);
        int getx(void) {return x;};
        int getvx(void) {return vx;};
        int getid(void){return id;};
        int frenadoinstantaneo(void){frenadoinsta2=frenadoinsta1;}
        int getfrenado(void){return frenadoinsta2;};
};

class Calle{
    private:
        Carro *honda;
        int N;
        int numeroreal;
    public:
        void inicialice(CRandom & Ran2,int n);
        void evolucione(void);
        double velpromedio(void); 
        int getcuantos(void){return numeroreal;};
        void imprimase(void);
        void limpiar(void);
};

void Calle::limpiar(void){delete(honda);}
//Este inicalicese tendra funciones de posiciones y velocidades de tal forma que en el comienzo no existan choques
void Calle::inicialice(CRandom & Ran2,int n)
{   
    int CarroNuevo,TotalVacias,PosicionPrueba,CarroAnterior,CeldasVacias,NuevaPosicion,ocupado,PosicionBien;
    int prueba;
    N=n;
    numeroreal=0;
    honda=new Carro[N];
    for(CarroNuevo=0,TotalVacias=Celda;CarroNuevo<N;CarroNuevo++,TotalVacias--){
        NuevaPosicion=int(TotalVacias*Ran2.r());
        for(PosicionPrueba=0,CeldasVacias=0;PosicionPrueba<Celd;PosicionPrueba++){
            ocupado=0;
            prueba=0;
            for(CarroAnterior=0;CarroAnterior<CarroNuevo;CarroAnterior++){
	              if((honda[CarroAnterior].getx()==PosicionPrueba)||(honda[CarroAnterior].getx()==(PosicionPrueba+1)%Celd)||
                  (honda[CarroAnterior].getx()==(Celd+PosicionPrueba-1)%Celd)) {ocupado=1; break;}
	          }
            if((CeldasVacias==NuevaPosicion)&&(ocupado==0)){numeroreal+=1;prueba=1;break;}
            if (ocupado==0) CeldasVacias++;
        }
        if(prueba==1)honda[numeroreal-1].inicie(PosicionPrueba,int((VMAX+1)*Ran2.r()),numeroreal-1,0);
    }
    N=numeroreal;
    for(int j=0;j<N;j++){
        honda[j].casiguiente(honda,N);
    }
    for(int k=0;k<N;k++){
        honda[k].dist(honda);
        honda[k].velinicio(Ran2.r());
    }
  //  cout<<numeroreal<<"\n";
}
//Imprimir información de los carros
void Calle::imprimase(void){
    int k,v;
    for(float i=0;i<Celd;i++){
        for(k=0,v=-1;k<N;k++)
            if(honda[k].getx()==i) v=honda[k].getvx();
        if(v==-1)cout<<".";
        else cout<<v;
    }
    cout<<"\n";
}

//Evolución de los carros
void Calle::evolucione(void){
    for(int k=0;k<N;k++)honda[k].dist(honda);
    for(int k=0;k<N;k++)honda[k].frenadoinstantaneo();
    for(int k=0;k<N;k++)honda[k].velocidad(honda);
    for(int k=0;k<N;k++)honda[k].posicion();
    //Probar que genera los choques entre los carros
    //for(int k=0;k<N;k++)honda[k].dist(honda);
}

//Obtener velocidad promedio de la calle
double Calle::velpromedio(void){
    double velpromedio;
    velpromedio=0;
    for(int k=0;k<N;k++){
        velpromedio+=honda[k].getvx();
    }
    velpromedio=velpromedio/N;
    return velpromedio;
}
void Carro::inicie(int x0,int vx0,int iden,int idsiguiente){
    x=x0;
    vx=vx0;
    id=iden;
    siguiente=idsiguiente;
    conretardo=0;
    frenadoinsta1=0;
    frenadoinsta2=0;
}
void Carro::casiguiente(Carro Cuerpo[],int N){
    int i,k,a;
    a=0;
    for(i=1;i<Celd;i++){
        for(k=0;k<N;k++){
	          if(Cuerpo[k].getx()==(x+i)%Celd){
	              siguiente=k;
	              a=1;
	              break;
            }
        }
        if(a==1)break;
    }
}
void Carro::dist(Carro Cuerpo[]){
    gapsolo=(Cuerpo[siguiente].getx()+Celd-x)%Celd;
    //gapsolo-=1;
    if(id==46||id==82){
    	//std::cout<<"Se extrae una vez informacion"<<std::endl;
        int test;
        //cout<<"Id "<<id<<" Carro siguiente Id "<<Cuerpo[siguiente].getid()<<endl;

        //cout<<gapsolo<<" "<<Cuerpo[siguiente].getvx()-vx<<" "<<vx<<endl;
        //cin>>test;
    }
    if(gapsolo<2){
        int test;
        cout<<"Se chocaron dos carros"<<endl;
        cout<<"Id "<<id<<endl;
        cout<<gapsolo<<" "<<Cuerpo[siguiente].getvx()-vx<<" "<<vx<<endl;
        //cin>>test;
    }
    gap=gapsolo+Cuerpo[siguiente].getvx()-vx;
    //gap-=1;
}

void Carro::velinicio(float numero){
    vx=min(int(VMAX*numero),int((gapsolo-2)*numero));
    vx=max(vx,0);
}
//Aqui se metera la taba de de velocidades segunsu gap.
void Carro::velocidad(Carro Cuerpo[]){
    int b=1;
    float nuevogap;
    nuevogap=gap;
    //frenadoinsta1=frenadoinsta2;
    for(int m=1;m<20;m++){
        //cout<<vx<<"\t"<<gapfrenado<<"\t"<<gapaceleracion<<"\t"<<b<<"\t"<<gap<<"\n";
        int gapfrenado,gapaceleracion;
        int retardo;
        switch (vx){
          	case 0:gapfrenado=0;gapaceleracion=3;retardo=1;break;
          	case 1:gapfrenado=3;gapaceleracion=4;retardo=1;break;
          	case 2:gapfrenado=3;gapaceleracion=5;retardo=1;break;
          	case 3:gapfrenado=4;gapaceleracion=5;retardo=1;break;
          	case 4:gapfrenado=5;gapaceleracion=6;retardo=2;break;
          	case 5:gapfrenado=6;gapaceleracion=7;retardo=2;break;
          	case 6:gapfrenado=6;gapaceleracion=8;retardo=2;break;
          	case 7:gapfrenado=7;gapaceleracion=9;retardo=2;break;
          	//case 8:gapfrenado=8;gapaceleracion=10;retardo=3;break;
          	//case 9:gapfrenado=8;gapaceleracion=11;retardo=3;break;
          	//case 10:gapfrenado=10;gapaceleracion=13;retardo=3;break;
          	//case 11:gapfrenado=10;gapaceleracion=13;retardo=3;break;
          	//case 12:gapfrenado=11;gapaceleracion=13;retardo=3;break;
          	//case 13:gapfrenado=11;gapaceleracion=13;retardo=3;break;
          	//case 14:gapfrenado=12;gapaceleracion=13;retardo=3;break;
          	//case 15:gapfrenado=13;gapaceleracion=14;retardo=3;break;
	      }
        if(gapfrenado>nuevogap){vx=vx-1;nuevogap+=1;b=0;conretardo=0;frenadoinsta1=1;}
        else if((gapfrenado<=nuevogap)&&(nuevogap<=gapaceleracion)){vx=vx;
            if(b!=0)frenadoinsta1=0;
            b=1;conretardo=0;
        }
        else if(nuevogap>gapaceleracion){
	        if((Cuerpo[siguiente].getfrenado()==1)&&((nuevogap==gapaceleracion)||(nuevogap==gapaceleracion+1))){
		        if(vx!=0)vx=vx-1;
		        //if((vx==1))vx=vx-1;
		        //if((vx!=0)&&(vx>1))vx=vx-2;
		        frenadoinsta1=1;conretardo=0;
		    }
	        else{
        	    conretardo+=1;
        	    frenadoinsta1=0;
        	    //cout<<conretardo;
	            if(conretardo==retardo){
		            vx=min(vx+1,VMAX);conretardo=0;
		        }
	        }
	        b=1;
	    }
	    //  else if((gapfrenado<=nuevogap)&&(nuevogap<=gapaceleracion)){vx=vx;
        //    if(b!=0)frenadoinsta1=0;
        //   b=1;conretardo=0;
        //}
        if(b==1)break;
    }
    //Criterio de seguridad para no acelerar cuando el carro de adelante va mas despacio
    // y no hay espacio suficiente para moverse con la velocidad actual
    if(Cuerpo[siguiente].getvx()-vx<0 && (gapsolo-2)<vx){
    	vx=gapsolo-2;
    	conretardo=0;
    	frenadoinsta1=0;
    }
}

void Carro::posicion(void){
    x+=vx;
    if(x>=Celd) x=x-Celd;
}

//------------------MAIN----------------------
int main(void){
    ofstream examplefile("Olmos_Implementation.dat");
    CRandom Ran2(0);
    int col,fila,ini=100,ttotal=3000;
    Calle AvEldorado;
    ini=1;
    int Numerocarros=200;

    for(int Numerocarros=20;Numerocarros<800;Numerocarros+=20){     
        //Este for indica cuantos distintas configuraciones de inicio para la misma densidad.
        for(int i=0;i<ini;i++){
        	  AvEldorado.inicialice(Ran2,Numerocarros);
        	  double rho=double(AvEldorado.getcuantos())/double(Celda);
        	  std::cout<<"Densidad "<<rho<<endl;
        	  //Este indica cuantos pasos de evolucion haga, y despues de éstos imprime la velocidad promediod e estos.
	          for(int t=0;t<ttotal;t++){
	              //AvEldorado.imprimase();
	              AvEldorado.evolucione();
	              //examplefile<<rho<<"\t"<<t<<"\t"<<AvEldorado.velpromedio()<<"\n";
	              //cout<<endl;
	              //cout<<t<<" "<<AvEldorado.velpromedio()<<endl;
            }
	          //AvEldorado.imprimase();
	          examplefile<<rho<<"\t"<<rho*AvEldorado.velpromedio()<<"\t"<<AvEldorado.velpromedio()<<"\n";
	      }
        AvEldorado.limpiar();      
    }
    examplefile.close();
    //file1.close();
    return 0;
}
