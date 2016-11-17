// OpenSMOKE++ Definitions
#include "OpenSMOKEpp"

//C++
#include <functional>
#include <algorithm>

// CHEMKIN maps
#include "maps/Maps_CHEMKIN"

// Reactor utilities
#include "reactors/utilities/Utilities"

// ODE solvers
#include "math/multivalue-ode-solvers/MultiValueSolver"
#include "ode/ODE_Parameters.h"

// OpenFOAM
#include "fvCFD.H"
#include "multivariateScheme.H"
#include "simpleControl.H"

// Additional include files
#include "userDefinedFunctions.H"
#include "reactions/reactionRates.H"

// Heterogeneous reactors
#include "heterogeneousChemistry.H"
#include "heterogeneousChemistry_ODE_Interface.H"
#include "homogeneousChemistry.H"

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readGravitationalAcceleration.H"
    #include "readSolverOptions.H"

    simpleControl simple(mesh);

    #include "createFields.H"

    heterogeneousChemistry het(*thermodynamicsMapXML,
                               *kineticsMapXML,
                               *surfaceThermodynamicsMapXML,
                               *surfaceKineticsMapXML);

    homogeneousChemistry   hom(*thermodynamicsMapXML,
                               *kineticsMapXML);

    #include "createOdeSolver.H"
    
    #include "properties.H"
    #include "finalSetupOperations.H"

    #include "initContinuityErrs.H"

    dimensionedScalar initialMass = fvc::domainIntegrate(rho);

    #include "cupMix.H"

    bool cupMixExit = true;
    while (runTime.loop())
    {
        if (cupMixExit != true)
        {
            Info << "ERROR: please set writeNow in controlDict options" << endl;
            exit(-1);
        }
        runTime++;
        runTime.write();
        cupMixExit = false;
    }

    Info<< "End\n" << endl;

    return 0;
}
