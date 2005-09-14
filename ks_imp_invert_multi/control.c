/************************* control.c *******************************/
/* MIMD version 7 */
/* Main procedure for SU3 with dynamical fermions 			*/
/* Naik plus fat link fermions, general gauge action */

/* This version combines code for the PHI algorithm (approriate for 4
   flavors) and the R algorithm for "epsilon squared" updating of 
   1 to 4 flavors.  Compilation should occur with PHI_ALGORITHM defined
   for the former and not defined for the latter.  It also contains code
   for the hybrid Monte Carlo algorithm, for which HMC_ALGORITHM and
   PHI_ALGORITHM should be defined.  (Actually, the
   changes to control.c are minimal and the real differences will appear
   in update.c */

#define CONTROL
#include "ks_imp_includes.h"	/* definitions files and prototypes */

EXTERN  gauge_header start_lat_hdr;     /* Input gauge field header */

int main( int argc, char **argv ){
  int meascount,traj_done;
  int prompt;
  int s_iters,avspect_iters;
  double dtime, fixtime, dclock();
  ks_prop_file *kspf;
  
  initialize_machine(argc,argv);
#ifdef HAVE_QDP
  QDP_initialize(argc, argv);
#endif
  g_sync();
  /* set up */
  prompt = setup();
  /* loop over input sets */
  while( readin(prompt) == 0){
    
    dtime = -dclock();
    
    /* perform measuring trajectories, reunitarizing and measuring 	*/
    meascount=0;		/* number of measurements 		*/
    avspect_iters =   0;
    
    /* call gauge_variable measuring routines */
    /* results are printed in output file */
    rephase(OFF);   /* note that the matching rephase call appears after
		       possible gauge fixing */
    g_measure( );
    if( fixflag == COULOMB_GAUGE_FIX)
      {
	if(this_node == 0)
	  printf("Fixing to Coulomb gauge\n");
	fixtime = -dclock();
	gaugefix(TUP,(Real)1.8,500,(Real)GAUGE_FIX_TOL,
		 F_OFFSET(tempmat1),F_OFFSET(tempvec[0]),
		 0,NULL,NULL,0,NULL,NULL);
	fixtime += dclock();
	if(this_node==0)printf("Time to gauge fix = %e\n",fixtime);
      }
    else
      if(this_node == 0)printf("COULOMB GAUGE FIXING SKIPPED.\n");
    
    rephase( ON );
    
#ifdef FN
    valid_fatlinks = valid_longlinks = 0;
#endif
#ifdef FPI
    avspect_iters += fpi_2( fpi_mass, fpi_nmasses, 2e-3 );
#endif
    mminv.tol = 2e-3;
    avspect_iters += multimass_inverter(&mminv);
    
    ++meascount;
    fflush(stdout);
    
    node0_printf("RUNNING COMPLETED\n"); fflush(stdout);
    
    node0_printf("average cg iters for spectrum = %e\n",
		 (double)avspect_iters/meascount);
    
    dtime += dclock();
    if(this_node==0){
      printf("Time = %e seconds\n",dtime);
      printf("total_iters = %d\n",total_iters);
    }
    fflush(stdout);
    
    /* save lattice if requested */
    if( saveflag != FORGET ){
      rephase( OFF );
      save_lattice( saveflag, savefile, stringLFN );
      rephase( ON );
    }
    
  }
#ifdef HAVE_QDP
  QDP_finalize();
  normal_exit(0);
#endif
  
  return 0;
}
