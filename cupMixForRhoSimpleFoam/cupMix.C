#include "fvCFD.H"
#include "psiThermo.H"
#include "RASModel.H"
#include "simpleControl.H"
#include "fvIOoptionList.H"

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    simpleControl simple(mesh);

    #include "createFields.H"
    #include "cupMixFields.H"
    #include "createFvOptions.H"
    #include "initContinuityErrs.H"
    #include "cupMixFieldsEvaluation.H"

    Info<< "\nStarting FAKE time loop\n" << endl;

	bool cupMixExit = true;
	while (runTime.run())
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

    return 0;
}
