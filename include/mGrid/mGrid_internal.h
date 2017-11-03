#ifndef _MGRID_INTERNAL_H
#define _MGRID_INTERNAL_H

#include <Grid/Grid.h>

using namespace Grid;
using namespace Grid::QCD;

// The color-vector and gauge field objects used in the interface

template<typename ISF>
struct GRID_ColorVector_struct {
  typename ISF::FermionField *cv;
};

template<typename ISF5D>
struct GRID_ColorVector5D_struct {
  typename ISF5D::FermionField *cv;
};

template<typename LGF>
struct GRID_FermionLinksAsqtad_struct {
  LGF *thnlinks;
  LGF *fatlinks;
  LGF *lnglinks;
};

typedef struct GRID_ColorVector_struct<ImprovedStaggeredFermionF>  GRID_F3_ColorVector_struct;
typedef struct GRID_ColorVector5D_struct<ImprovedStaggeredFermion5DF>  GRID_F3_ColorVector5D_struct;
typedef struct GRID_FermionLinksAsqtad_struct<LatticeGaugeFieldF> GRID_F3_FermionLinksAsqtad_struct;

typedef struct GRID_ColorVector_struct<ImprovedStaggeredFermionD> GRID_D3_ColorVector_struct;
typedef struct GRID_ColorVector5D_struct<ImprovedStaggeredFermion5DD> GRID_D3_ColorVector5D_struct;
typedef struct GRID_FermionLinksAsqtad_struct<LatticeGaugeFieldD> GRID_D3_FermionLinksAsqtad_struct;

#endif /* _MGRID_INTERNAL_H */
