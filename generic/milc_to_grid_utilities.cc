/******************* milc_to_grid_utilities.cc ************************/
/* For the Grid interface */
/* MIMD version 7 */

#include <Grid/Grid.h>
#include <vector>
#include <iostream>
#include <qmp.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#undef GRID_EXTERN

#include "../include/mGrid/mGrid_assert.h"

extern "C" {
#include "generic_includes.h"
#include "../include/generic_grid.h"
}
using namespace std;
using namespace Grid;
using namespace Grid::QCD;

GridCartesian *CGrid;
GridRedBlackCartesian *RBGrid;

static int is_grid_env_setup = 0;

GRID_evenodd_t milc2grid_parity(int milc_parity){
  switch(milc_parity){
  case(EVEN):       return GRID_EVEN;
  case(ODD ):       return GRID_ODD;
  case(EVENANDODD): return GRID_EVENODD;
  default:
    printf("milc2grid_parity: Bad MILC parity %d\n", milc_parity);
    terminate(1);
  }
  return (GRID_evenodd_t)-999;
}

int grid2milc_parity(GRID_evenodd_t grid_parity){
  switch(grid_parity){
  case(GRID_EVEN):     return EVEN;
  case(GRID_ODD ):     return ODD;
  case(GRID_EVENODD ): return EVENANDODD;
  default:
    printf("grid2milc_parity: Bad GRID parity %d\n", grid_parity);
    terminate(1);
  }
  return -999;
}

void finalize_grid(void)
{
  /* We omit MPI_Finalize() because most likely it will break a lot of things */
  Grid_unquiesce_nodes();
}

 
GRID_status_t 
initialize_grid(void){
  
  int  argc = 3;
  char **argv;

  if(is_grid_env_setup)
    return GRID_SUCCESS;

  /* We simulate the command line parameters to initialize Grid, and
   * call the standard routine. This doesn't look very elegant, but
   * this way we let Grid handle the parameters. */
  
  GRID_ASSERT((argv = (char **) malloc(sizeof(char *)*3)) != NULL, GRID_MEM_ERROR);
  
  GRID_ASSERT((argv[0] = (char *) malloc(sizeof(char)*32)) != NULL, GRID_MEM_ERROR);
  GRID_ASSERT((argv[1] = (char *) malloc(sizeof(char)*32)) != NULL, GRID_MEM_ERROR);
  GRID_ASSERT((argv[2] = (char *) malloc(sizeof(char)*32)) != NULL, GRID_MEM_ERROR);
  
  const int *machsize = get_logical_dimensions(); // dimensions of the array of MPI ranks
  int mpiX = nx/machsize[0];
  int mpiY = ny/machsize[1];
  int mpiZ = nz/machsize[2];
  int mpiT = nt/machsize[3];

  sprintf (argv[0], "--grid %d.%d.%d.%d\0", nx, ny, nz, nt);
  sprintf (argv[1], "--mpi %d.%d.%d.%d\0",  mpiX,   mpiY,   mpiZ,   mpiT);

  Grid_init(&argc, &argv);
  printf("finished Grid_init\n"); fflush(stdout);

  std::vector<int> latt_size   = GridDefaultLatt();
  std::vector<int> simd_layout = GridDefaultSimd(Nd,vComplex::Nsimd());
  std::vector<int> mpi_layout  = GridDefaultMpi();
  CGrid  = new GridCartesian(latt_size,simd_layout,mpi_layout);
  RBGrid = new GridRedBlackCartesian(latt_size,simd_layout,mpi_layout);

  node0_printf("milc_to_grid_utilities: Initialized Grid\n");
  node0_printf("Grid threads %d\n", Grid::GridThread::GetThreads());
  fflush(stdout);

  free (argv[2]);
  free (argv[1]);
  free (argv[0]);
  free (argv);
  
  return GRID_SUCCESS;

}

// Return the coordinate as assigned by Grid
// Code from CPS via Chulwoo Jung

int *query_grid_node_mapping(int peGrid[]){

  if(is_grid_env_setup){
    printf("query_grid_node_mapping: Grid must first be initialized\n");
    terminate(1);
  }

  // Hack to reset node mapping for Grid (from Chulwoo Jung)
  static int pePos[4];  /*!< Position of this process in the grid.*/
  static QMP_comm_t qmp_comm;

  std::vector<int> processors;
  printf("setting up CartesianCommunicator with %d %d %d %d\n", 
	 peGrid[0], peGrid[1], peGrid[2], peGrid[3]); fflush(stdout);
  for(int i=0;i<4;i++) processors.push_back(peGrid[i]);
  Grid::CartesianCommunicator grid_cart(processors);
  printf("done with CartesianCommunicator\n"); fflush(stdout);
  for(int i=3;i>=0;i--){
    pePos[i] = grid_cart._processor_coor[i];
  }
  return pePos;
}