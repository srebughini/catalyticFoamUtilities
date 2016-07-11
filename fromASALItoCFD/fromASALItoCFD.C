#include <string>
#include <iostream>
#include <math.h>
#include <ctime>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "OpenSMOKEpp"

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "reactors/utilities/Utilities"

#include "fvCFD.H"
#include "multivariateScheme.H"
#include "regionProperties.H"

#include "extraFunctions.H"

int main(int argc, char **argv)
{
    #include "setRootCase.H"
    #include "createTime.H"

    regionProperties rp(runTime); 

    #include "createFluidMeshes.H" 
    #include "createSolidMeshes.H" 

    #include "readSolverOptions.H"
    #include "readFluidProperties.H"
    #include "readSolidProperties.H"
    
    meshWarning();
    
    if ( asaliSolver == "heterogeneousPFR" )
    {
        #include "hetereogeneousPFRresults.H"
        #include "createFluidFields.H"
        #include "createSolidFields.H"
        #include "hetereogeneousPFRtoCFD.H"
    }
    else if ( asaliSolver == "2Ds" )
    {
        #include "2Dresults.H"
        #include "createFluidFields.H"
        #include "createSolidFields.H"
        #include "2DtoCFD.H"
    }

    return 0; 
}
