#ifndef compressibleCourantNo_H
#define compressibleCourantNo_H

#include "fvMesh.H"

namespace Foam
{
    scalar compressibleCourantNo
    (
        const fvMesh& mesh,
        const Time& runTime,
        const volScalarField& rho,
        const surfaceScalarField& phi
    );
}

#endif
