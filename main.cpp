#include<iostream>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<cstdio>
#include<string>
#include<fstream>


#define max_oculta1 64
#define max_oculta2 64
#define max_neuronas 64
#define max_entradas 20
#define max_salidas 8
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

int main(int argc,char **argv)
{
	cout.precision(15); //cout tenga 8 decimales de precision
	if(argc==1)
	{
		cout<<endl<<"|°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°|";
		cout<<endl<<"|Perceptron de 2 capas ocultas By: Silvana Cabana|";
		cout<<endl<<"|________________________________________________|";
		cout<<endl<<endl<<"  Para modo entrenamiento usar : "<<endl;
		cout<<"\t"<<argv[0]<<" [0=entrenamiento] [entradas] [ocultas1] [ocultas2] [salidas] [error-minimo] [aprendizaje] [archivo-dataset]";
		cout<<endl<<"*Se generara un archivo de salida conteniendo topologia+pesos como [archivo-dataset].tp";
		cout<<endl<<endl<<"  Para modo prediccion-clasificacion usar : "<<endl;
		cout<<"\t"<<argv[0]<<" [1=prediccion-clasificacion] [archivo-red-neuronal] [archivo-data-set]";
		cout<<endl<<"*Se generara un archivo de salida conteniendo prediccion-clasificacion como [archivo-dataset].pc";
		cout<<endl<<endl<<"** Data-set delimitado por espacios"<<endl<<endl;
	}
	else
	{
		int n_entradas,n_oculta1,n_oculta2,n_salidas,n_instancias;
		double a[3][max_neuronas],w[3][max_neuronas][max_neuronas],b[3][max_neuronas],d_b[3][max_neuronas];
		double entrada[max_instancias][max_entradas],salida[max_instancias][max_salidas];
		double fact_aprendizaje,error_epoca,z,error_minimo;
		char buffer[50];

		//entrenamiento
		if(strtol(argv[1],NULL,10)==0)
		{
			//parametros
			n_entradas=strtol(argv[2],NULL,10);
			n_oculta1=strtol(argv[3],NULL,10);
			n_oculta2=strtol(argv[4],NULL,10);
			n_salidas=strtol(argv[5],NULL,10);
			error_minimo=strtod(argv[6],NULL);
			fact_aprendizaje=strtod(argv[7],NULL);

			//dataset
			ifstream txt_dataset(argv[8]);
			n_instancias=-1;
			while(!txt_dataset.eof())
			{
				n_instancias++;
				for(int i=0;i<n_entradas;i++)
					txt_dataset>>entrada[n_instancias][i];
				for(int i=0;i<n_salidas;i++)
					txt_dataset>>salida[n_instancias][i];
			}
			txt_dataset.close();

			//pesos
			for(int neurona=0;neurona<n_oculta1;neurona++)
			{
	  			b[0][neurona]=(double)rand()/RAND_MAX;
		  		for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
  					w[0][neurona_atras][neurona]=(double)rand()/RAND_MAX;
			}
			for(int neurona=0;neurona<n_oculta2;neurona++)
			{
				b[1][neurona]=(double)rand()/RAND_MAX;
	  			for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
		  			w[1][neurona_atras][neurona]=(double)rand()/RAND_MAX;
  			}
			for(int neurona=0;neurona<n_salidas;neurona++)
			{
				b[2][neurona]=(double)rand()/RAND_MAX;
				for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
					w[2][neurona_atras][neurona]=(double)rand()/RAND_MAX;
			}
		}

		//prediccion-clasificacion
		if(strtol(argv[1],NULL,10)==1)
		{
			cout<<"entro";
			//parametros
			ifstream txt_nn(argv[2]);
			txt_nn>>n_entradas;
			txt_nn>>n_oculta1;
			txt_nn>>n_oculta2;
			txt_nn>>n_salidas;
			fact_aprendizaje=0;

			//pesos
			for(int neurona=0;neurona<n_oculta1;neurona++)
			{
	  			txt_nn>>b[0][neurona];
		  		for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
  					txt_nn>>w[0][neurona_atras][neurona];
			}
			for(int neurona=0;neurona<n_oculta2;neurona++)
			{
				txt_nn>>b[1][neurona];
	  			for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
		  			txt_nn>>w[1][neurona_atras][neurona];
  			}
			for(int neurona=0;neurona<n_salidas;neurona++)
			{
				txt_nn>>b[2][neurona];
				for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
					txt_nn>>w[2][neurona_atras][neurona];
			}
			txt_nn.close();

			//dataset
			ifstream txt_dataset(argv[3]);
			n_instancias=-1;
			while(!txt_dataset.eof())
			{
				n_instancias++;
				for(int i=0;i<n_entradas;i++)
					txt_dataset>>entrada[n_instancias][i];
			}
			txt_dataset.close();
		}

		//DEBUG
  		if(FLAG_DBG==1)
  		{
  			cout<<endl<<"n_entradas :"<<n_entradas;
  			cout<<endl<<"n_oculta1 :"<<n_oculta1;
  			cout<<endl<<"n_oculta2 :"<<n_oculta2;
  			cout<<endl<<"n_salidas :"<<n_salidas;
  			cout<<endl<<"error_minimo :"<<error_minimo;
  			cout<<endl<<"fact_apren: "<<fact_aprendizaje;
  			cout<<endl<<endl<<"Matriz de entradas";
  			for(int instancia=0;instancia<n_instancias;instancia++)
  			{
  				cout<<endl<<"\tInstancia "<<instancia<<" : ";
  				for(int i=0;i<n_entradas;i++)
  					cout<<entrada[instancia][i]<<",";
  				cout<<"\t";
  				for(int i=0;i<n_salidas;i++)
  					cout<<salida[instancia][i]<<",";
  			}

			cout<<endl<<"Pesos y Bias";
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

		error_epoca=1.0;
		//epocas
		for(int epoca=0;error_epoca>error_minimo;epoca++)
		{
			if(FLAG_DBG==1) cout<<endl<<endl<<"Epoca "<<epoca;
	  		error_epoca=0.0;

			//instancias
			for(int instancia=0;instancia<n_instancias;instancia++)
	  		{
	    		//propagacion hacia delante
	    		for(int neurona=0;neurona<n_oculta1;neurona++)
	    		{
	      			z=b[0][neurona];
					for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
	        			z+=w[0][neurona_atras][neurona]*entrada[instancia][neurona_atras];
					a[0][neurona]=sigmoide(z);
	    		}
	    		for(int neurona=0;neurona<n_oculta2;neurona++)
	    		{
	      			z=b[1][neurona];
	      			for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
	        			z+=w[1][neurona_atras][neurona]*a[0][neurona_atras];
	      			a[1][neurona]=sigmoide(z);
	   			}
	    		for(int neurona=0;neurona<n_salidas;neurona++)
	    		{
		      		z=b[2][neurona];
					for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
	        			z+=w[2][neurona_atras][neurona]*a[1][neurona_atras];
					a[2][neurona]=sigmoide(z);
	    		}

				//prediccion-clasificacion
				if(strtol(argv[1],NULL,10)==1)
		    	{
					//guardar predicciones-clasficaciones
					sprintf(buffer,"%s_%d_%d.out",argv[3],n_oculta1,n_oculta2);
					if(FLAG_DBG==1) cout<<endl<<"archivo para resultados: "<<buffer;
					ofstream txt_out(buffer,ofstream::app);
					for(int neurona=0;neurona<n_salidas;neurona++)
		    			txt_out<<a[2][neurona]<<" ";
		    		txt_out<<endl;
					txt_out.close();
				}

				//entrenamiento
		    	if(strtol(argv[1],NULL,10)==0)
		    	{
					//calculo del ERROR
		    		for(int neurona=0;neurona<n_salidas;neurona++)
	    			{
	    				error_epoca+=error(salida[instancia][neurona],a[2][neurona]);
						if(FLAG_DBG==1) cout<<endl<<"error salida "<<neurona<<"="<<error(salida[instancia][neurona],a[2][neurona]);
					}

	    			//propagacion hacia atras
	    			for(int neurona=0;neurona<n_salidas;neurona++)
		    			d_b[2][neurona]=d_error(salida[instancia][neurona],a[2][neurona])*d_sigmoide(a[2][neurona]);
	    			for(int neurona=0;neurona<n_oculta2;neurona++)
	    			{
	    				z=0.0;
		      			for(int neurona_adelante=0;neurona_adelante<n_salidas;neurona_adelante++)
							z+=w[2][neurona][neurona_adelante]*d_b[2][neurona_adelante];
 		     			d_b[1][neurona]=z*d_sigmoide(a[1][neurona]);
		    		}
				    for(int neurona=0;neurona<n_oculta1;neurona++)
			    	{
			    		z=0.0;
		    	  		for(int neurona_adelante=0;neurona_adelante<n_oculta2;neurona_adelante++)
		        			z+=w[1][neurona][neurona_adelante]*d_b[1][neurona_adelante];
   	     				d_b[0][neurona]=z*d_sigmoide(a[0][neurona]);
		    		}

				    //nuevos pesos y bias
					for(int neurona_adelante=0;neurona_adelante<n_salidas;neurona_adelante++)
					{
						b[2][neurona_adelante]-=fact_aprendizaje*d_b[2][neurona_adelante];
						for(int neurona=0;neurona<n_oculta2;neurona++)
 			       			w[2][neurona][neurona_adelante]-=fact_aprendizaje*d_b[2][neurona_adelante]*a[1][neurona];
					}
		   	    	for(int neurona_adelante=0;neurona_adelante<n_oculta2;neurona_adelante++)
					{
	 			    	b[1][neurona_adelante]-=fact_aprendizaje*d_b[1][neurona_adelante];
    	    			for(int neurona=0;neurona<n_oculta1;neurona++)
        					w[1][neurona][neurona_adelante]-=fact_aprendizaje*d_b[1][neurona_adelante]*a[0][neurona];
					}
					for(int neurona_adelante=0;neurona_adelante<n_oculta1;neurona_adelante++)
					{
						b[0][neurona_adelante]-=fact_aprendizaje*d_b[0][neurona_adelante];
	   	    			for(int neurona=0;neurona<n_entradas;neurona++)
	          				w[0][neurona][neurona_adelante]-=fact_aprendizaje*d_b[0][neurona_adelante]*entrada[instancia][neurona];
        			}
				}

				//debug
				if(FLAG_DBG==1)
				{
					for(int neurona=0;neurona<n_oculta1;neurona++)
	    			{
						if(FLAG_DBG==1) cout<<endl<<"z[0]["<<neurona<<"]="<<z;
		      			if(FLAG_DBG==1) cout<<endl<<"a[0]["<<neurona<<"]="<<a[0][neurona]<<endl;
	    			}
	    			for(int neurona=0;neurona<n_oculta2;neurona++)
	    			{
						if(FLAG_DBG==1) cout<<endl<<"z[1]["<<neurona<<"]="<<z;
		      			if(FLAG_DBG==1) cout<<endl<<"a[1]["<<neurona<<"]="<<a[1][neurona]<<endl;
	   				}
	    			for(int neurona=0;neurona<n_salidas;neurona++)
	    			{
						if(FLAG_DBG==1) cout<<endl<<"z[2]["<<neurona<<"]="<<z;
						if(FLAG_DBG==1) cout<<endl<<"a[2]["<<neurona<<"]="<<a[2][neurona]<<endl;
	    			}

					//entrenamiento
		    		if(strtol(argv[1],NULL,10)==0)
		    		{
						//calculo del ERROR
						for(int neurona=0;neurona<n_salidas;neurona++)
							cout<<endl<<"error salida "<<neurona<<"="<<error(salida[instancia][neurona],a[2][neurona]);
						//propagacion hacia atras
						for(int neurona=0;neurona<n_salidas;neurona++)
							cout<<endl<<"d_b[2]["<<neurona<<"]="<<d_b[2][neurona];
						for(int neurona=0;neurona<n_oculta2;neurona++)
							cout<<endl<<"d_b[1]["<<neurona<<"]="<<d_b[1][neurona];
						for(int neurona=0;neurona<n_oculta1;neurona++)
	   	    	 			cout<<endl<<"d_b[0]["<<neurona<<"]="<<d_b[0][neurona];
						//nuevos pesos y bias
						for(int neurona_adelante=0;neurona_adelante<n_salidas;neurona_adelante++)
		      				cout<<endl<<"b[2]["<<neurona_adelante<<"]="<<b[2][neurona_adelante];
			   	    	for(int neurona_adelante=0;neurona_adelante<n_oculta2;neurona_adelante++)
						{
	      					cout<<endl<<"b[1]["<<neurona_adelante<<"]="<<b[1][neurona_adelante];
					    	for(int neurona=0;neurona<n_oculta1;neurona++)
		      					cout<<endl<<"w[1]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[1][neurona][neurona_adelante];
						}
						for(int neurona_adelante=0;neurona_adelante<n_oculta1;neurona_adelante++)
						{
	    	  				cout<<endl<<"b[0]["<<neurona_adelante<<"]="<<b[0][neurona_adelante];
			    			for(int neurona=0;neurona<n_entradas;neurona++)
			      				cout<<endl<<"w[0]["<<neurona<<"]["<<neurona_adelante<<"]="<<w[0][neurona][neurona_adelante];
						}
					}
				}
			}

			//entrenamiento
			if(strtol(argv[1],NULL,10)==0)
			{
				//guardar errores
				sprintf(buffer,"%s_%s_%s.error",argv[8],argv[3],argv[4]);
				if(FLAG_DBG==1) cout<<endl<<"archivo para error: "<<buffer;
				ofstream txt_error(buffer,ofstream::app);
				txt_error<<error_epoca<<endl;
				if(FLAG_DBG==1)	cout<<endl<<epoca<<" : "<<error_epoca;
				txt_error.close();
			}

		}

		//entrenamiento
		if(strtol(argv[1],NULL,10)==0)
		{
			//guardar red neuronal entrenada
			sprintf(buffer,"%s_%s_%s.nn",argv[8],argv[3],argv[4]);
			ofstream txt_nn(buffer);
			for(int parametro=2;parametro<6;parametro++)
				txt_nn<<argv[parametro]<<" ";
			for(int neurona=0;neurona<n_oculta1;neurona++) //capa oculta 1
			{
				if(FLAG_DBG==1) cout<<endl<<"b[0]["<<neurona<<"]="<<b[0][neurona];
				txt_nn<<b[0][neurona]<<" ";
	  			for(int neurona_atras=0;neurona_atras<n_entradas;neurona_atras++)
	  			{
					if(FLAG_DBG==1) cout<<endl<<"w[0]["<<neurona_atras<<"]["<<neurona<<"]="<<w[0][neurona_atras][neurona];
					txt_nn<<w[0][neurona_atras][neurona]<<" ";
				}
			}
			for(int neurona=0;neurona<n_oculta2;neurona++) //capa oculta 2
			{
	  			if(FLAG_DBG==1) cout<<endl<<"b[1]["<<neurona<<"]="<<b[1][neurona];
				txt_nn<<b[1][neurona]<<" ";
	  			for(int neurona_atras=0;neurona_atras<n_oculta1;neurona_atras++)
  				{
  					if(FLAG_DBG==1) cout<<endl<<"w[1]["<<neurona_atras<<"]["<<neurona<<"]="<<w[1][neurona_atras][neurona];
  					txt_nn<<w[1][neurona_atras][neurona]<<" ";
  				}
  			}
			for(int neurona=0;neurona<n_salidas;neurona++) //capa salida
			{
				if(FLAG_DBG==1) cout<<endl<<"b[2]["<<neurona<<"]="<<b[2][neurona];
				txt_nn<<b[2][neurona]<<" ";
				for(int neurona_atras=0;neurona_atras<n_oculta2;neurona_atras++)
				{
					if(FLAG_DBG==1) cout<<endl<<"w[2]["<<neurona_atras<<"]["<<neurona<<"]="<<w[2][neurona_atras][neurona];
					txt_nn<<w[2][neurona_atras][neurona]<<" ";
				}
			}
			txt_nn.close();
		}

		//entrenamiento
		if(strtol(argv[1],NULL,10)==0)
		{
			//analisis de sensibilidad
			sprintf(buffer,"%s_%s_%s.as",argv[8],argv[3],argv[4]);
			ofstream txt_as(buffer);
			double aux1;
			double aux2;
			double aux3;
			for(int neu_entrada=0;neu_entrada<n_entradas;neu_entrada++)
				for(int neu_salida=0;neu_salida<n_salidas;neu_salida++)
				{
					txt_as<<"entrada "<<neu_entrada<<" sobre salida "<<neu_salida<<": ";
					aux1=0.0;
					for(int oculta1=0;oculta1<n_oculta1;oculta1++)
					{
						aux2=0.0;
						for(int oculta2=0;oculta2<n_oculta2;oculta2++)
						{
							aux3=0.0;
							for(int salida=0;salida<n_salidas;salida++)
								aux3+=d_sigmoide(a[2][neu_salida])*w[2][oculta2][neu_salida];
							aux2+=aux3*d_sigmoide(a[1][oculta2])*w[1][oculta1][oculta2];
						}
						aux1+=aux2*d_sigmoide(a[0][oculta1])*w[0][neu_entrada][oculta1];
					}
					txt_as<<aux1<<endl;
				}
			txt_as.close();
		}

	}
}
