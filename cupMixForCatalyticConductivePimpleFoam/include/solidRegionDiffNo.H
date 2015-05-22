#ifndef solidRegionDiff_H
#define solidRegionDiff_H

#include "fvMesh.H"

namespace Foam
{
    scalar solidRegionDiffNo
    (
        const fvMesh& mesh,
        const Time& runTime,
        const volScalarField& Cprho,
        const volScalarField& kappa
    );
}

#endif
