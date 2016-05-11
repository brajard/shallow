//Declaration
#include <sys/time.h>
double dx,dy,dedt,svdedt,pcor,grav,dissip,hmoy,alpha,gb,gmx,gsx,gmy,gsy;
extern void       Yrazgrad_all();

void xdisplay();
void appli_start(int argc, char *argv[]){
  srand(1);
}
void before_it(int nit){}
void cost_function(int pdt){}
void adjust_target(){}
void after_it(int nit){
	xdisplay();
}
void forward_before(int ctrp){
	if(Yt==1) {svdedt=dedt;dedt=0.0;}
	if(Yt==2) dedt /= 2;
}
void forward_after(int ctrp){
	if(Yt==1) dedt=svdedt;
	if(Yt==2) dedt *= 2;
}

void backward_before(int ctrp){
	if(Yt==2) dedt /=2;
	if(Yt==1) {svdedt = dedt;dedt = 0.0;}
}

void backward_after(int ctrp){
	if(Yt==2) dedt *=2;
	if(Yt==1) dedt = svdedt;
}

short select_io(int indic, char *nmod, int sortie, int iaxe,
		int jaxe, int kaxe, int pdt, YREAL *val){
	if(indic==YIO_SAVESTATE){
	  if(iaxe == 0 && jaxe == 0) fprintf(Yiofp, "\n\n");
	    else if (jaxe == 0) fprintf(Yiofp, "\n");
	}
	return(1);
}

double my_gettimeofday(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L);
} 

void xdisplay(){
	int i,j;
	i=(int)(SZX/2);
	j=(int)(SZY/2);
#ifndef YO_CADNA
	double h1 = YS_Hfil(0,i,j,0);
	double h2 = YS_Hfil(0,i,j,SZT-1);
	double gg = YG_Hfil(0,i,j,0);

	printf("point:%i,%i, Hfil(0)=% -23.15e Hfil(%d)==% -23.15e @J/@Hfil=% -23.15e\n ", i,j,h1 , SZT,h2,gg);
#else
	printf("point:%i,%i, Hfil(0)=%s Hfil(%d)==%s @J/@Hfil=%s\n ", i,j,strp(YS_Hfil(0,i,j,0)) , SZT,strp(YS_Hfil(0,i,j,SZT-1)),strp(YG_Hfil(0,i,j,0)));
#endif
}
void savegrad(int argc, char *argv[]) {
  FILE *fid;
  fid = fopen(argv[1],"w");
  int i,j;
  for (i=0;i<SZX;i++) {
    for (j=0;j<SZY;j++) {
#ifndef YO_CADNA
      fprintf(fid,"%22.15e ",YG_Hfil(0,i,j,0));
#else
      fprintf(fid,"%s ",strp(YG_Hfil(0,i,j,0)));
#endif
    }
    fprintf(fid,"\n");
  }
  fclose(fid);
}

void print_normgrad() {
    int i,j;
    YREAL norml2=0;
    YREAL norminf = 0;
    for (i=0;i<SZX;i++)
      for (j =0;j<SZY;j++) {
	norml2+=YG_Hfil(0,i,j,0)*YG_Hfil(0,i,j,0);
	if (fabs(YG_Hfil(0,i,j,0))>norminf)
	  norminf = fabs(YG_Hfil(0,i,j,0));
      }
#ifndef YO_CADNA
    printf("%22.15e \n%22.15e\n",norml2,norminf);
#else
    printf("%s \n %s\n",strp(norml2),strp(norminf));
#endif
}

double randn (double mu, double sigma)
{
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (double) X1);
}




void adjoint() {
  double begin,end;
  double time_spent;
 Yset_modeltime(0);
    before_it(1);
    //printf("---forward(i=%d)---\n",i);
    Yforward(-1, 0);
    
    //lobs[i]->val = YS_Hfil(0,lobs[i]->Y,lobs[i]->X,lobs[i]->T);
    //YS_Hfil(0,lobs[i]->Y,lobs[i]->X,lobs[i]->T)++;
    //Yobs_insert_data("Hfil",0,lobs[i]->Y,lobs[i]->X,0,lobs[i]->T,lobs[i]->val);



    Yrazgrad_all();  /* avant fct de cout et backprop : sur tous les pas de temps, raz de tous les gradients de tous les modules */
    
    YTotalCost = 0.0;	/* Raz aussi du Cout avant les calculs de cout */
    
    begin = my_gettimeofday();
    Ybackward (-1, 0); // Ybackward (YNBSTEPTIME);/* AD (adjoint):-> d*x =M*(X).dX : Yjac * YG -> Ytbeta */
    end = my_gettimeofday();
    time_spent = end - begin;
    fprintf(stdout,"backward time : %g\n",time_spent);
    after_it(1);

}


void xgauss(int argc, char *argv[]){
	gb = atof(argv[1]);
	if (argc==2) {
	  gmx = SZX*dx / 2 ;
	  gmy = SZY*dy / 2 ;
	  gsx = SZX*dx / 20 ;
	  gsy = SZY*dy / 20 ;
	}
	else
	  {

	    gmx = atof(argv[2]); gsx = atof(argv[3]);
	    gmy = atof(argv[4]); gsy = atof(argv[5]);
	  }
        for (int j = 0; j<SZY; j++)
	  for (int i = 0; i<SZX; i++)
	   YS_Hfil(0,i,j,0) = gb*(exp( -pow((i*dx-gmx)/gsx,2) /2.))
			    *(exp(-pow((j*dy-gmy)/gsy,2) /2.));
}

void xvitgeo(){
	double gf, gfh;
	gf = (grav / pcor);
	for(int j=0; j<SZY; j++)
	  for(int i=0; i<SZX; i++){ 
	    gfh=gf*YS_Hfil(0,i,j,0);
	    YS_Ufil(0,i,j,0)=gfh*(j*dy-gmy)/(gsy*gsy);
            YS_Vfil(0,i,j,0)=gfh*-(i*dx-gmx)/(gsx*gsx);
	  }
}

void xivg(int argc,char *argv[]){
	double val;
	val = atof(argv[2]);
			
	if       (strcmp(argv[1], "dt") == 0) dedt=val;
	else if  (strcmp(argv[1], "dx") == 0) dx=val;
	else if  (strcmp(argv[1], "dy") == 0) dy=val;
	else if  (strcmp(argv[1], "pcor") == 0) pcor=val;
	else if  (strcmp(argv[1], "grav") == 0) grav=val;
	else if  (strcmp(argv[1], "dissip") == 0) dissip=val;
	else if  (strcmp(argv[1], "hmoy") == 0) hmoy=val;
	else if  (strcmp(argv[1], "alpha") == 0) alpha=val;
}



void compute_res() {
  YREAL pdx = 2e-6;
  //YREAL pdx = 0.2;
  YREAL alpha = 10;
  YREAL fdec = 10;
  YREAL nbloop = 1;

  int k,i,j;
   double begin,end;
  double time_spent;
  YREAL Jx,Jdx,scal_prod=0;
  YREAL *X0=new YREAL[SZX*SZY];
  YREAL *dx=new YREAL[SZX*SZY];
  YREAL *dJx=new YREAL[SZX*SZY];
  
  int iloop=0;

  //Create perturbation
    k=0;
    for (i=0;i<SZX;i++)
      for (j =0;j<SZY;j++) {
	dx[k] = randn(0,pdx);
	k++;
      }
    
  adjoint();
    Jx = YTotalCost;
    //Copy state in X0 and grad in dJx
    k=0;    
    for (i=0;i<SZX;i++)
      for (j = 0;j<SZY;j++) {
	X0[k] = YS_Hfil(0,i,j,0);
	dJx[k] = YG_Hfil(0,i,j,0);
	k++;
      }
    

 //Calculate scalaire product
    scal_prod=0;
    begin = my_gettimeofday();
    //#pragma omp parallel for reduction(+ : scal_prod) schedule(static)
    for (k=0;k<SZX*SZY;k++)
      scal_prod+=dx[k]*dJx[k];
 
    end = my_gettimeofday();
    time_spent = end - begin;
    fprintf(stdout,"scalar product time : %g\n",time_spent);
    while (iloop<nbloop) {
  
    //Forward perturbation
    k=0;
    for (i=0;i<SZX;i++)
      for (j = 0;j<SZY;j++) {
	YS_Hfil(0,i,j,0) = X0[k]+alpha*dx[k];
	k++;
      }
    adjoint();
    
    Jdx = YTotalCost;
    
   
    
    //Print summary
#ifdef YO_CADNA
    printf("J=%s, Jdx=%s,scal_prod=%s,Res=%s\n",strp(Jx),strp(Jdx),strp(alpha*scal_prod),strp(Jdx-Jx-alpha*scal_prod));
#else
    printf("J=%22.15e, Jdx=%22.15e,scal_prod=%22.15e,Res=%22.15e\n",Jx,Jdx,alpha*scal_prod,Jdx-Jx-alpha*scal_prod);
#endif
    iloop++;
    alpha=alpha/fdec;
  }
  //free memory
  delete[] X0; delete[] dx ; delete[]dJx;
}
