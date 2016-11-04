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

#include "extraFunctions.H"

int main(int argc, char **argv)
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "readSolverOptions.H"

    if ( asaliSolver == "heterogeneousPFR" )
    {
        #include "hetereogeneousPFRresults.H"
        #include "createFields.H"
        #include "hetereogeneousPFRtoCFD.H"
    }
    else if ( asaliSolver == "2Ds" )
    {
        meshWarning();
        #include "2Dresults.H"
        #include "createFields.H"
        #include "2DtoCFD.H"
    }

    return 0; 
}
