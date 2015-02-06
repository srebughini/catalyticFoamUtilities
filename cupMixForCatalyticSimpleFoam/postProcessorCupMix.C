// OpenSMOKE
#include "OpenSMOKE_Definitions.h"
#include <string>
#include <iostream>
#include <numeric>
#include <Eigen/Dense>

// Base classes
#include "thermo/ThermoPolicy_CHEMKIN.h"
#include "kinetics/ReactionPolicy_CHEMKIN.h"
#include "math/PhysicalConstants.h"
#include "math/OpenSMOKEUtilities.h"

// Maps
#include "maps/ThermodynamicsMap_CHEMKIN.h"
#include "maps/KineticsMap_CHEMKIN.h"
#include "maps/TransportPropertiesMap_CHEMKIN.h"

// Surface
#include "maps/ThermodynamicsMap_Surface_CHEMKIN.h"
#include "maps/KineticsMap_Surface_CHEMKIN.h"

// ODE system
#include <stdio.h>

#include "fvCFD.H"
#include "psiCombustionModel.H"
#include "multivariateScheme.H"
#include "simpleControl.H"
#include "fvIOoptionList.H"

// Utilities
#include "Utilities.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readGravitationalAcceleration.H"

    simpleControl simple(mesh);

    #include "createFields.H"
    #include "createOpenSMOKEFields.H"
    #include "createExtraFields.H"
    #include "createFvOptions.H"
    #include "initContinuityErrs.H"
    
    while (simple.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;
        #include "properties.H"
        runTime++;
        runTime.write();

        Info<< "Run fake time step!!" << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}

// ************************************************************************* //
