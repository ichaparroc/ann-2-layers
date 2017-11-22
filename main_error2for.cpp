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
#define FLAG_DBG 1

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

b[0][0]=0.089934147;
w[0][0][0]=0.35451109;
w[0][1][0]=0.2065445;
b[0][1]=0.3680682;
w[0][0][1]=0.075206187;
w[0][1][1]=0.87480631;
b[1][0]=0.27468801;
w[1][0][0]=0.73506088;
w[1][1][0]=0.019401996;
b[1][1]=0.72682505;
w[1][0][1]=0.13331317;
w[1][1][1]=0.42937362;
b[2][0]=0.11452216;
w[2][0][0]=0.50894363;
w[2][1][0]=0.4243493;

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

		    //nuevos pesos y bias
			for(int neurona=0;neurona<n_oculta2;neurona++)
				for(int neurona_adelante=0;neurona_adelante<n_salidas;neurona_adelante++)
	        	{
   		       		w[2][neurona][neurona_adelante]-=fact_aprendizaje*d_b[2][neurona_adelante]*a[1][neurona];
        	  		b[2][neurona_adelante]-=fact_aprendizaje*d_b[2][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"w[2]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[2][neurona][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"b[2]["<<neurona_adelante<<"]="<<b[2][neurona_adelante];
        		}

			for(int neurona=0;neurona<n_oculta1;neurona++)
    	    	for(int neurona_adelante=0;neurona_adelante<n_oculta2;neurona_adelante++)
        		{
          			w[1][neurona][neurona_adelante]-=fact_aprendizaje*d_b[1][neurona_adelante]*a[0][neurona];
          			b[1][neurona_adelante]-=fact_aprendizaje*d_b[1][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"w[1]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[1][neurona][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"b[1]["<<neurona_adelante<<"]="<<b[1][neurona_adelante];
        		}

			for(int neurona=0;neurona<n_entradas;neurona++)
    	    	for(int neurona_adelante=0;neurona_adelante<n_oculta1;neurona_adelante++)
        		{
          			w[0][neurona][neurona_adelante]-=fact_aprendizaje*d_b[0][neurona_adelante]*entrada[instancia][neurona];
          			b[0][neurona_adelante]-=fact_aprendizaje*d_b[0][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"w[0]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[0][neurona][neurona_adelante];
          			if(FLAG_DBG==1) cout<<endl<<"b[0]["<<neurona_adelante<<"]="<<b[0][neurona_adelante];
        		}
		}

		cout<<endl<<epoca<<" : "<<error_epoca;
	}
}
