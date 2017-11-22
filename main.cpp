#include<iostream>
#include<math.h>
#include<time.h>
#include <stdlib.h>
#include <cstdio>

#define max_oculta1 64
#define max_oculta2 64
#define max_neuronas 64
#define max_entradas 20 //nro de materias de examen de admision
#define max_salidas 8 //1 salida promedio general
#define max_instancias 1000
#define FLAG_DBG 0

using namespace std;

double error(double t,double a)
{
	return 0.5*pow(t-a,2); //"t"arget = objetivo, "a" es el calculado
}

double d_error(double t,double a) //derivada de error
{
	return (a-t);
}

double sigmoide(double z) //a(z)
{
	return 1/(1+exp(-z));
}

double d_sigmoide(double a) //derivada sigmoide
{
	return a*(1-a);
}

int main(void)
{
	cout.precision(8); //cout tenga 8 decimales de precision
	int n_entradas,n_oculta1,n_oculta2,n_salidas,n_instancias,n_epocas;
	double a[3][max_neuronas],w[3][max_neuronas][max_neuronas],b[3][max_neuronas],d_b[3][max_neuronas];
	double entrada[max_instancias][max_entradas],salida[max_instancias][max_salidas];
	double fact_aprendizaje,error_epoca,z;

	cout<<endl<<"Nº de Entradas";cin>>n_entradas;
	cout<<endl<<"Nº de Neuronas en Capa Oculta 1";cin>>n_oculta1;
	cout<<endl<<"Nº de Neuronas en Capa Oculta 2";cin>>n_oculta2;
	cout<<endl<<"Nº de Salidas";cin>>n_salidas;
	cout<<"Nº de Epocas";cin>>n_epocas;
	cout<<"Factor de Aprendizaje";cin>>fact_aprendizaje;
	cout<<endl<<"Nº de Instancias";cin>>n_instancias;
	cout<<"Ingrese la Matriz de Instancias (inputs+outputs)*instancias";

	//Matriz de Instancias
	for(int instancia=0;instancia<n_instancias;instancia++)
	{
		for(int i=0;i<n_entradas;i++)
			cin>>entrada[instancia][i];
		for(int i=0;i<n_salidas;i++)
			cin>>salida[instancia][i];
  	}

  	if(FLAG_DBG==1)
  	{
  		cout<<endl<<"Matriz de entradas";
  		for(int instancia=0;instancia<n_instancias;instancia++)
  		{
  			cout<<endl<<"\tInstancia "<<instancia<<" : ";
  			for(int i=0;i<n_entradas;i++)
  				cout<<entrada[instancia][i]<<",";
  			cout<<"\t";
  			for(int i=0;i<n_salidas;i++)
  				cout<<salida[instancia][i]<<",";
  		}
  	}

  	srand(time(NULL)); //semilla aletorio

	//inicializacion de pesos y bias por capa
	if(FLAG_DBG==1) cout<<endl<<"Pesos y Bias";

	for(int neurona=0;neurona<n_oculta1;neurona++) //capa oculta 1
	{
	  	b[0][neurona]=(double)rand()/RAND_MAX;
	  	if(FLAG_DBG==1) cout<<endl<<"b[0]["<<neurona<<"]="<<b[0][neurona];
	  	for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
	  	{
  			w[0][neurona_atras][neurona]=(double)rand()/RAND_MAX;
			if(FLAG_DBG==1) cout<<endl<<"w[0]["<<neurona_atras<<"]["<<neurona<<"]="<<w[0][neurona_atras][neurona];
		}
	}

	for(int neurona=0;neurona<n_oculta2;neurona++) //capa oculta 2
	{
		b[1][neurona]=(double)rand()/RAND_MAX;
	  	if(FLAG_DBG==1) cout<<endl<<"b[1]["<<neurona<<"]="<<b[1][neurona];
	  	for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
	  	{
	  		w[1][neurona_atras][neurona]=(double)rand()/RAND_MAX;
  			if(FLAG_DBG==1) cout<<endl<<"w[1]["<<neurona_atras<<"]["<<neurona<<"]="<<w[1][neurona_atras][neurona];
  		}
  	}

	for(int neurona=0;neurona<n_salidas;neurona++) //capa salida
	{
		b[2][neurona]=(double)rand()/RAND_MAX;
		if(FLAG_DBG==1) cout<<endl<<"b[2]["<<neurona<<"]="<<b[2][neurona];
		for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
		{
			w[2][neurona_atras][neurona]=(double)rand()/RAND_MAX;
			if(FLAG_DBG==1) cout<<endl<<"w[2]["<<neurona_atras<<"]["<<neurona<<"]="<<w[2][neurona_atras][neurona];
		}
	}

b[0][0]=-1.3654122;
w[0][0][0]=0.38355793;
w[0][1][0]=-1.3196419;
w[0][2][0]=1.2843658;
w[0][3][0]=0.30664285;
w[0][4][0]=2.2304324;
w[0][5][0]=-0.90004702;
w[0][6][0]=0.010422574;
w[0][7][0]=2.0992607;
b[0][1]=-0.76387889;
w[0][0][1]=-2.3506134;
w[0][1][1]=-3.5456098;
w[0][2][1]=2.3238434;
w[0][3][1]=1.60798;
w[0][4][1]=1.2389453;
w[0][5][1]=3.340195;
w[0][6][1]=-2.6463289;
w[0][7][1]=1.2048172;
b[0][2]=-0.21628635;
w[0][0][2]=1.8823637;
w[0][1][2]=0.13831107;
w[0][2][2]=1.2483512;
w[0][3][2]=3.5414994;
w[0][4][2]=-0.54008225;
w[0][5][2]=-1.227993;
w[0][6][2]=0.10046995;
w[0][7][2]=1.3646934;
b[0][3]=-1.145493;
w[0][0][3]=3.0616275;
w[0][1][3]=3.4092187;
w[0][2][3]=-0.10118307;
w[0][3][3]=1.1106527;
w[0][4][3]=1.2485969;
w[0][5][3]=-3.2118895;
w[0][6][3]=-1.5438319;
w[0][7][3]=-3.1656497;
b[0][4]=-1.2402404;
w[0][0][4]=-2.3379058;
w[0][1][4]=0.47929428;
w[0][2][4]=-1.2364268;
w[0][3][4]=-1.5122039;
w[0][4][4]=3.7973017;
w[0][5][4]=0.22085828;
w[0][6][4]=-0.86407691;
w[0][7][4]=1.0882081;
b[0][5]=-4.589533;
w[0][0][5]=3.6765405;
w[0][1][5]=-0.60839967;
w[0][2][5]=1.5998559;
w[0][3][5]=-0.65527413;
w[0][4][5]=-2.8098337;
w[0][5][5]=-1.8970515;
w[0][6][5]=5.2608635;
w[0][7][5]=5.2899859;
b[1][0]=-0.96857021;
w[1][0][0]=-1.682205;
w[1][1][0]=0.89645639;
w[1][2][0]=-0.6901542;
w[1][3][0]=0.89643694;
w[1][4][0]=0.080005514;
w[1][5][0]=1.114314;
b[1][1]=0.012810532;
w[1][0][1]=0.63842535;
w[1][1][1]=-0.44504827;
w[1][2][1]=0.16932065;
w[1][3][1]=-0.12190351;
w[1][4][1]=-0.22603684;
w[1][5][1]=0.64527079;
b[1][2]=-1.2945159;
w[1][0][2]=-1.2911739;
w[1][1][2]=0.55813745;
w[1][2][2]=-1.1964286;
w[1][3][2]=1.2802085;
w[1][4][2]=0.3926348;
w[1][5][2]=1.4735293;
b[1][3]=-2.9983969;
w[1][0][3]=-2.247986;
w[1][1][3]=3.8747024;
w[1][2][3]=-3.6482209;
w[1][3][3]=3.6141508;
w[1][4][3]=3.4678364;
w[1][5][3]=4.4116989;
b[2][0]=-1.0329434;
w[2][0][0]=1.5924691;
w[2][1][0]=-0.72113586;
w[2][2][0]=1.649083;
w[2][3][0]=5.6328171;

	for(int epoca=0;epoca<n_epocas;epoca++) //epocas
	{
		if(FLAG_DBG==1) cout<<endl<<endl<<"Epoca "<<epoca;
	  	error_epoca=0.0;
		for(int instancia=0;instancia<n_instancias;instancia++) //instancias
	  	{
	    	//propagacion hacia delante

	    	//para la capa oculta 1
	    	for(int neurona=0;neurona<n_oculta1;neurona++)
	    	{
	      		z=b[0][neurona];
				for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
	        		z+=w[0][neurona_atras][neurona]*entrada[instancia][neurona_atras];
				if(FLAG_DBG==1) cout<<endl<<"z[0]["<<neurona<<"]="<<z;
	      		a[0][neurona]=sigmoide(z);
	      		if(FLAG_DBG==1) cout<<endl<<"a[0]["<<neurona<<"]="<<a[0][neurona]<<endl;
	    	}

	    	//para la capa oculta 2
	    	for(int neurona=0;neurona<n_oculta2;neurona++)
	    	{
	      		z=b[1][neurona];
	      		for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
	        		z+=w[1][neurona_atras][neurona]*a[0][neurona_atras];
	      		if(FLAG_DBG==1) cout<<endl<<"z[1]["<<neurona<<"]="<<z;
	      		a[1][neurona]=sigmoide(z);
	      		if(FLAG_DBG==1) cout<<endl<<"a[1]["<<neurona<<"]="<<a[1][neurona]<<endl;
	   		}

			//para la capa de salida
	    	for(int neurona=0;neurona<n_salidas;neurona++)
	    	{
	      		z=b[2][neurona];
				for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
	        		z+=w[2][neurona_atras][neurona]*a[1][neurona_atras];
				if(FLAG_DBG==1) cout<<endl<<"z[2]["<<neurona<<"]="<<z;
				a[2][neurona]=sigmoide(z);
				if(FLAG_DBG==1) cout<<endl<<"a[2]["<<neurona<<"]="<<a[2][neurona]<<endl;
	    	}

		    //calcula del ERROR
		    for(int neurona=0;neurona<n_salidas;neurona++)
	    	{
	    		error_epoca+=error(salida[instancia][neurona],a[2][neurona]);
				if(FLAG_DBG==1) cout<<endl<<"error salida "<<neurona<<"="<<error(salida[instancia][neurona],a[2][neurona]);
			}

	    	//propagacion hacia atras
	    	//para bias
	    	for(int neurona=0;neurona<n_salidas;neurona++)
	    	{
	    		d_b[2][neurona]=d_error(salida[instancia][neurona],a[2][neurona])*d_sigmoide(a[2][neurona]);
				if(FLAG_DBG==1) cout<<endl<<"d_b[2]["<<neurona<<"]="<<d_b[2][neurona];
	    	}

	    	for(int neurona=0;neurona<n_oculta2;neurona++)
	    	{
	    		z=0.0;
		      	for(int neurona_adelante=0;neurona_adelante<n_salidas;neurona_adelante++)
					z+=w[2][neurona][neurona_adelante]*d_b[2][neurona_adelante];
   		     	d_b[1][neurona]=z*d_sigmoide(a[1][neurona]);
	    		if(FLAG_DBG==1) cout<<endl<<"d_b[1]["<<neurona<<"]="<<d_b[1][neurona];
	    	}

		    for(int neurona=0;neurona<n_oculta1;neurona++)
		    {
		    	z=0.0;
		      	for(int neurona_adelante=0;neurona_adelante<n_oculta2;neurona_adelante++)
		        	z+=w[1][neurona][neurona_adelante]*d_b[1][neurona_adelante];
   	     		d_b[0][neurona]=z*d_sigmoide(a[0][neurona]);
   	    	 	if(FLAG_DBG==1) cout<<endl<<"d_b[0]["<<neurona<<"]="<<d_b[0][neurona];
		    }

/*
		    //nuevos pesos y bias
			for(int neurona_adelante=0;neurona_adelante<n_salidas;neurona_adelante++)
			{
				b[2][neurona_adelante]-=fact_aprendizaje*d_b[2][neurona_adelante];
				for(int neurona=0;neurona<n_oculta2;neurona++)
	        	{
   		       		w[2][neurona][neurona_adelante]-=fact_aprendizaje*d_b[2][neurona_adelante]*a[1][neurona];
          			if(FLAG_DBG==1) cout<<endl<<"w[2]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[2][neurona][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"b[2]["<<neurona_adelante<<"]="<<b[2][neurona_adelante];
        		}
			}


   	    	for(int neurona_adelante=0;neurona_adelante<n_oculta2;neurona_adelante++)
			{
    	    	b[1][neurona_adelante]-=fact_aprendizaje*d_b[1][neurona_adelante];
    	    	for(int neurona=0;neurona<n_oculta1;neurona++)
        		{
          			w[1][neurona][neurona_adelante]-=fact_aprendizaje*d_b[1][neurona_adelante]*a[0][neurona];
          			if(FLAG_DBG==1) cout<<endl<<"w[1]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[1][neurona][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"b[1]["<<neurona_adelante<<"]="<<b[1][neurona_adelante];
        		}

			}

			for(int neurona_adelante=0;neurona_adelante<n_oculta1;neurona_adelante++)
			{
				b[0][neurona_adelante]-=fact_aprendizaje*d_b[0][neurona_adelante];
    	    	for(int neurona=0;neurona<n_entradas;neurona++)
        		{
          			w[0][neurona][neurona_adelante]-=fact_aprendizaje*d_b[0][neurona_adelante]*entrada[instancia][neurona];
          			if(FLAG_DBG==1) cout<<endl<<"w[0]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[0][neurona][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"b[0]["<<neurona_adelante<<"]="<<b[0][neurona_adelante];
        		}
        	}
*/
		}

		cout<<endl<<epoca<<" : "<<error_epoca;
	}

	for(int neurona=0;neurona<n_oculta1;neurona++) //capa oculta 1
	{
	  	cout<<endl<<"b[0]["<<neurona<<"]="<<b[0][neurona];
	  	for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
			cout<<endl<<"w[0]["<<neurona_atras<<"]["<<neurona<<"]="<<w[0][neurona_atras][neurona];
	}

	for(int neurona=0;neurona<n_oculta2;neurona++) //capa oculta 2
	{
	  	cout<<endl<<"b[1]["<<neurona<<"]="<<b[1][neurona];
	  	for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
  			cout<<endl<<"w[1]["<<neurona_atras<<"]["<<neurona<<"]="<<w[1][neurona_atras][neurona];
  	}

	for(int neurona=0;neurona<n_salidas;neurona++) //capa salida
	{
		cout<<endl<<"b[2]["<<neurona<<"]="<<b[2][neurona];
		for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
			cout<<endl<<"w[2]["<<neurona_atras<<"]["<<neurona<<"]="<<w[2][neurona_atras][neurona];
	}

}
