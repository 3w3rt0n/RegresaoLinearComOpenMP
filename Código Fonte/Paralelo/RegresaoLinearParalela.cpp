/* Para compiular: g++ -g RegresaoLinear.cpp -o RegresaoLinear.exe -lrt -fopenmp -Wall */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void imprimir_vetor(double* my_vetor, long tamanho){
	
  printf("[ ");
  		
	for (long i = 0; i < tamanho; ++i) {				
		printf("%f ", my_vetor[i]);
	}
	
  printf("]\n");
}

void gravar_dataset(FILE* fpDataset, long tamanho, double X[], double y[]){
	
	fprintf(fpDataset, "seq; x; y\n"); 	
		
	for (long i = 0; i < tamanho; ++i) {		
	  fprintf(fpDataset, "%ld; %f; %f\n", i, X[i], y[i]);	
	}
	
}


int main(int argc, char* argv[])
{
  bool debug = true;
	struct timespec beginDataset, endDataset;
  struct timespec seedV;
  double elapsed;

	if (argc != 3) {
		printf("Quantidade argumentos invalido.");
		printf("\nFormato: RegressaoLinear.exe t e");
		printf("\nt: tamanho do dataset.");
		printf("\ne: numero threads.\n");

		return 0;
	}
 
  /* Variaveis a e b */
  clock_gettime(CLOCK_MONOTONIC, &seedV);
  unsigned int seedB = (unsigned int) seedV.tv_nsec;  
  double b = ((double)(rand_r(&seedB) % 400) + 100) / 10;  
  
  clock_gettime(CLOCK_MONOTONIC, &seedV);
  unsigned int seedA = (unsigned int) seedV.tv_nsec;  
  double a = ((double)(rand_r(&seedA) % 1500) - 200) / 10.0;  
 
  FILE* fpTempo;
	char filenameTempo[80];
  sprintf(filenameTempo, "%s_TEMPO_%s_THREAD_%s_A_%f_B_%f.csv", argv[0], argv[1], argv[2], a, b); 
  fpTempo = fopen(filenameTempo, "a");
	if (fpTempo == NULL) {
		fprintf(stderr, "Can't open output file %s!\n", filenameTempo);
		return 1;
	}
 
  FILE* fpDataset;
	char filenameDataset[80];
  sprintf(filenameDataset, "%s_DATASET_%s_THREAD_%s_A_%f_B_%f.csv", argv[0], argv[1], argv[2], a, b); 
  fpDataset = fopen(filenameDataset, "a");
	if (fpDataset == NULL) {
		fprintf(stderr, "Can't open output file %s!\n", filenameDataset);
		return 1;
	}
  
  FILE* fpVariaveis;
	char filenameVariaveis[80];
  sprintf(filenameVariaveis, "%s_Variaveis_%s_THREAD_%s_A_%f_B_%F.csv", argv[0], argv[1], argv[2], a, b); 
  fpVariaveis = fopen(filenameVariaveis, "a");
	if (fpDataset == NULL) {
		fprintf(stderr, "Can't open output file %s!\n", filenameVariaveis);
		return 1;
	}
 
  /* Tamanho do dataset */ 
	long tamanho = atol(argv[1]);
 
  /* Numero de Threads */ 
	int num_threads = atoi(argv[2]);
 
 
  if(tamanho > 50)
    debug = false;
 
  /* Variavel independente */
  double* X = new double[tamanho];
	for (long i = 0; i < tamanho; ++i) {
		X[i] = 0.0;
	}
  
  /* Variavel dependente */
  double* y = new double[tamanho];
	for (long i = 0; i < tamanho; ++i) {
		y[i] = 0.0;
	}  
 
  
  printf("Valores utilizados: y = a + b * x + E\n");
  printf("A: %f <> B: %f\n", a, b);  
  printf("y = %f + %f * x + E\n", a, b);
  
  /*****************************************/
  /*               Inicio                  */
  /*****************************************/
  clock_gettime(CLOCK_MONOTONIC, &beginDataset);
  
  // Gerar dataset
  #pragma omp parallel for shared(X, y, tamanho, a, b) \
   default(none) num_threads(num_threads) schedule(static, 1)
  for(int i=0; i<tamanho; i++){
  
     struct timespec seedtime1, seedtime2;
      
     clock_gettime(CLOCK_MONOTONIC, &seedtime1);
     unsigned int seedX = (unsigned int) seedtime1.tv_nsec;
    
     X[i] = (double)(rand_r(&seedX) % 10000) / 10;   
  
     clock_gettime(CLOCK_MONOTONIC, &seedtime2);
     unsigned int seedErro = (unsigned int) seedtime2.tv_nsec;
    
     double erro = (double)(rand_r(&seedErro) % 40);   
  
     if(erro > 0 && erro < 4){
       
       clock_gettime(CLOCK_MONOTONIC, &seedtime2);
       unsigned int seedErro = (unsigned int) seedtime2.tv_nsec;
       erro = (double)(rand_r(&seedErro) % 1000)/10; 
       
       y[i] = a + (b * X[i]) + erro;
     }else if(erro > 4 && erro < 10){
       
       clock_gettime(CLOCK_MONOTONIC, &seedtime2);
       unsigned int seedErro = (unsigned int) seedtime2.tv_nsec;
       erro = (double)(rand_r(&seedErro) % 1000)/10; 
     
       y[i] = a + (b * X[i]) - erro; 
     }else{
       y[i] = a + (b * X[i]);
     }
      
  }
  
  // Calcula media de X
  double media_X = 0;  
  #pragma omp parallel num_threads(num_threads)
  { 
    #pragma omp parallel for shared(X, i, tamanho) default(none) reduction(+:media_X) num_threads(num_threads)
    for(int i=0; i<tamanho; i++){        
        media_X += X[i];
    }  
    media_X /= tamanho;
    printf("Media de X: %f\n", media_X);  
  }
  
  // Calcula media de y
  double media_y = 0;  
  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp parallel for shared(y, i, tamanho) default(none) reduction(+:media_y) num_threads(num_threads) 
    for(int i=0; i<tamanho; i++){        
        media_y += y[i];
    }    
    media_y /= tamanho;
    printf("Media de y: %f\n", media_y);
  }
  
  
  
  // Calcular b
  double dividendo = 0;
  double divisor = 0;
  double b_estimado = 0;
  double a_estimado = 0; 
  #pragma omp parallel num_threads(num_threads)
  { 
  
    #pragma omp parallel for shared(X, media_X, y, media_y, i, tamanho) default(none) reduction(+:dividendo) reduction(+:divisor) num_threads(num_threads)
    for(int i=0; i<tamanho; i++){
        
        dividendo = (X[i] - media_X) * (y[i] - media_y);
        
        //#pragma omp parallel shared(X, media_X, i) default(none) reduction(+:divisor) num_threads(num_threads)  
        divisor = (X[i] - media_X) * (X[i] - media_X); 
    }  
    double b_estimado = dividendo / divisor;  
    printf("B real: %f\nB estimado: %f\n", b, b_estimado);
    
    
    // Calcular a
    double a_estimado = media_y - (b_estimado * media_X);
    printf("A real: %f\nA estimado: %f\n", a, a_estimado);
  
  }
  /*****************************************/
  /*                FIM                    */
  /*****************************************/
  clock_gettime(CLOCK_MONOTONIC, &endDataset);
	elapsed = endDataset.tv_sec - beginDataset.tv_sec;
	elapsed += (endDataset.tv_nsec - beginDataset.tv_nsec) / 1000000000.0;	
 
  if(debug){
    printf("X: ");
    imprimir_vetor(X, tamanho);
    printf("y: ");
    imprimir_vetor(y, tamanho);
  } 
 
  //gravar_dataset(fpDataset, tamanho, X, y);
  fclose(fpDataset);    
	
  printf("\nTempo: %f seg.\n\n", elapsed);  
  printf(fpTempo, "%f ", elapsed);
  fclose(fpTempo);
  
  
  fprintf(fpVariaveis, "Valores utilizado: y = a + b * x + E\n");
  fprintf(fpVariaveis, "A: %f <> B: %f\n", a, b);
  fprintf(fpVariaveis, "Valores obtidos: y = a + b * x + E\n");
  fprintf(fpVariaveis, "A: %f <> B: %f\n", a_estimado, b_estimado);  
  fprintf(fpVariaveis, "y = %f + %f * x\n", a_estimado, b_estimado);
  fclose(fpVariaveis);
  
  return 0;
  
}