#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "..\source\c\dsprocessing.c"

/*
Main example for FFT and DFT, files or whatever as input (stdin)
whatch out for the maximum size
*/
#define nsMax 10000
void
Dft_Fft_Demo(){
    unsigned int i=0, ns, ns_2; /* counter, input size, power 2 size */
    double samples[nsMax]; /* input signal */
    double *an, *bn; /* real and imaginary coeficients */
	double *samples_2;	/* padded with zeros signal */
	unsigned int time_;
	double *isamples; /* ifft result */

    /* numero de amostras que vem, tudo o que estiver no arquivo */

	printf("Type the input signal just the values, after type any non number character\n");
    for(i=0; scanf("%lf", &samples[i])==1; i++){} /* leh as amostras */
    ns=i;  

    ns_2 = ns;
    samples_2 = samples;
    
	/* Comparing FFT with DFT, 
	so the input signal must be power2 size 
	because my function doesnt work for not power 2 size */
	if(isPower2(ns)==-1)
	{
		/* if its not get the next, realloc the array padding with zeros*/
		ns_2 = nextPower2(ns);
		
		samples_2 = (double*) malloc(sizeof(double)*ns_2);
		
		vCopiav(samples, samples_2, ns);

		/* padd the rest with zeros */
		for(i=ns; i<ns_2; i++)
			samples_2[i] = 0;

		/* ready */
	}
	/* after padding with zeros */
	fprintf(stderr,"\n n %u  ns_2 %u\n", ns, ns_2);
    vPrintf(stderr, samples_2, ns_2);

	/* Dft first */     
    dspDft_(samples_2, ns_2, &an, &bn); /* calcula os coeficientes real imaginarios*/
    fprintf(stderr, "\n complex coefs Dft\n");
    for(i=0; i<ns_2; i++) /* imprime as belezinhas */
        printf("%lf %lf\n", an[i], bn[i]);
    
    free(an);
    free(bn);		
    
	/* Fft after */
    dspFft_(samples_2, ns_2, &an, &bn);        
    fprintf(stderr, "\n complex coefs Fft\n");
    for(i=0; i<ns_2; i++) /* imprime as belezinhas */
        printf("%lf %lf\n", an[i], bn[i]);

    /* IFft after */
    dspIFft_(an, bn, ns_2, &isamples);
    fprintf(stderr, "\n Inverse Fft real values\n");
    vPrintf(stderr, isamples, ns_2);    

   free(an);
   free(bn);
}


/*
Convolution games
Demo
    Not efficient algorithm though working (Slide and sum)
    Convolution with Fft nicest way
*/

void
Convolution_Demo(){
    double *h, *x, *y; /* impulsiva, entrada, saida */
    int Nh, Nx; /* tamanho dos sinais */
    int t; /* counters */

    printf("Type the impulse response for the sistem, size and data\n");
    scanf("%d", &Nh);
    h = (double*) malloc(sizeof(double)*Nh);
    for(t=0;t<Nh;t++) scanf("%lf", &h[t]);

    printf("Type the input signal, size and data \n");
    scanf("%d", &Nx);
    x = (double*) malloc(sizeof(double)*Nx);
    for(t=0;t<Nx;t++) scanf("%lf", &x[t]);

    printf("Result Slow slide and sum approach\n");
    y = dspConv(x, Nx, h, Nh);
    vPrintf(stdout, y, Nx+Nh-1);    
    
    printf("Result Nicest approach (dspConvFft my C code)\n");
    y = dspConvFft(x, Nx, h, Nh);
    vPrintf(stdout, y, Nx+Nh-1);
}





/*
Correlation Demos
same size signal correlation 
Uses:
    mathlab code approach also not very effient (time performance)
    poor C slide invert convolution approach
    nicest fft conv  (time inverted) approach    
*/

void
Correlation_Demo(){
    double *a, *b, *y; /* entrada, entrada, saida */
    int Na, Nb, N; /* tamanho dos sinais */
    int t; /* counters */

    printf("first signal: size and data\n");
    scanf("%d", &Na);
    a = (double*) malloc(sizeof(double)*Na);
    for(t=0;t<Na;t++) scanf("%lf", &a[t]);
    
    printf("second signal: size and data\n");
    scanf("%d", &Nb);
    b = (double*) malloc(sizeof(double)*Nb);
    for(t=0;t<Nb;t++) scanf("%lf", &b[t]);

    /* padd with zeros so they can have the same size */
    N = Na;
    if(Na != Nb){ /* padd with zeros the smaller */
        N = (Na > Nb)? Na : Nb;        
        if(Na != N)
            a = dspAppend(a, Na, dspZeros(N-Na), N-Na);
        else
            b = dspAppend(b, Nb, dspZeros(N-Nb), N-Nb);   
    }    
    
    y = dspCorrx(b, a, N, -1);

    /* -1 force the use of size maximum as number of lags */
    printf("output signal (xCorr Matlab)\n");
    vPrintf(stdout, y, 2*N-1);

    /* 
    Calculates correlation between signals
    uses convolution approach (algorithm not performance focussed)
    */

    printf("output signal (dspCorr my C code)\n");
    y = dspCorr(b, N, a, N);
    vPrintf(stdout, y, 2*N-1);
    
    /* Calculates correlation between signals
    uses Fft convolution approach 
    */
    
    printf("output signal (dspCorrFft my C code)\n");
    y = dspCorrFft(a, N, b, N);
    vPrintf(stdout, y, 2*N-1);
}

/*
(You can use pipes < > to redirect the input/output from/to a file
Use the main to call any of the demo examples
*/

int main(){ 
   
    //Dft_Fft_Demo();
	//Convolution_Demo();
	Correlation_Demo(); 
	printf("\n");
	system("PAUSE");

    return 0;
}


