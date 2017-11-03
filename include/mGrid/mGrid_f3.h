/*************************************/
/* Function declarations of F-3 type */
/*************************************/

#ifndef _MGRID_F3_H
#define _MGRID_F3_H

#ifdef __cplusplus
extern "C" {
#endif

// The following strategem is intended to provide a strong chain of typedefs for both C and C++ compilation
// Above the API the data types are opaque (incomplete).
// If mGrid_internal.h is used, it must precede this header file

#if defined( _MGRID_INTERNAL_H) && defined( __cplusplus)
#define STRUCT
#else
#define STRUCT struct
#endif

#include "../include/generic_ks.h"


typedef STRUCT GRID_F3_ColorVector_struct		GRID_F3_ColorVector;
typedef STRUCT GRID_F3_FermionLinksAsqtad_struct	GRID_F3_FermionLinksAsqtad;

// create color vectors from MILC type
GRID_F3_ColorVector  *GRID_F3_create_V_from_vec( su3_vector *src, int milc_parity);

// copy color vectors from Grid structure to MILC type
void GRID_F3_extract_V_to_vec( su3_vector *dest, GRID_F3_ColorVector *src, int milc_parity);

// free color vectors
void GRID_F3_destroy_V(GRID_F3_ColorVector *V);

  /*********************/
  /*  Asqtad routines  */
  /*********************/

  /* fermion matrix link routines */

// create asqtad fermion links from MILC
GRID_F3_FermionLinksAsqtad  *GRID_F3_asqtad_create_L_from_MILC( su3_matrix *thn, su3_matrix *fat, su3_matrix *lng, 
								int milc_parity);

// free asqtad fermion links
void  GRID_F3_asqtad_destroy_L(GRID_F3_FermionLinksAsqtad *L);

// dslash
void GRID_F3_asqtad_dslash (GRID_F3_FermionLinksAsqtad *asqtad,
			    GRID_F3_ColorVector *out,
			    GRID_F3_ColorVector *in,
			    float mass,
			    int milc_parity);

// inverter
void GRID_F3_asqtad_invert (GRID_info_t *info,
			    GRID_F3_FermionLinksAsqtad *asqtad,
			    GRID_invert_arg_t *inv_arg,
			    GRID_resid_arg_t *res_arg,
			    float mass,
			    GRID_F3_ColorVector *out,
			    GRID_F3_ColorVector *in);

// multi-mass inverter
void GRID_F3_asqtad_invert_multi (GRID_info_t *info,
				  GRID_F3_FermionLinksAsqtad *asqtad,
				  GRID_invert_arg_t *inv_arg,
				  GRID_resid_arg_t *res_arg[],
				  float *mass,
				  int num_offsets,
				  GRID_F3_ColorVector *out[],
				  GRID_F3_ColorVector *in);

  /**************************************************/
  /* Mapping of generic names to specific precision */
  /**************************************************/

#if GRID_Precision == 'F'
#include "mGrid_f3_generic.h"
#endif


#ifdef __cplusplus
}
#endif

#endif /* _MGRID_F3_H */