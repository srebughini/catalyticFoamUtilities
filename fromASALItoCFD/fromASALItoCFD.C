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

#include "catalyticReactorClass.H"

#include "fvCFD.H"
#include "multivariateScheme.H"
#include "pimpleControl.H"
#include "fvIOoptionList.H"
#include "regionProperties.H"

#include "extraFunctions.H"

int main(int argc, char **argv)
{
	#include "setRootCase.H"
	#include "createTime.H"

	regionProperties rp(runTime); 

	#include "readSolverOptions.H"
	#include "readASALIresults.H"

	#include "createFluidMeshes.H" 
	#include "createSolidMeshes.H" 

	#include "createFluidFields.H"
	#include "createSolidFields.H"
    #include "catalystTopology.H"

	#include "pasteASALItoCFD.H"

	return 0; 
}
