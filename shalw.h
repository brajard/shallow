//Declaration 
double dx,dy,dedt,svdedt,pcor,grav,dissip,hmoy,alpha,gb,gmx,gsx,gmy,gsy;

void xdisplay();
void appli_start(int argc, char *argv[]){}
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

void xdisplay(){
	int i,j;
	i=(int)(SZX/2);
	j=(int)(SZY/2);
	printf("point:%i,%i, Hfil(0)=% -23.15e Hfil(%d)==% -23.15e @J/@Hfil=% -23.15e\n ", i,j,
	       YS_Hfil(0,i,j,0), SZT,YS_Hfil(0,i,j,SZT-1),YG_Hfil(0,i,j,0));
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


