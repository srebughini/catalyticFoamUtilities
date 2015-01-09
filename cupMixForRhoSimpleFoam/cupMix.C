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

    while (simple.loop())
    {
        Info<< "Time = " << runTime.timeName() << nl << endl;

        runTime.write();

        Info<< "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
            << "  ClockTime = " << runTime.elapsedClockTime() << " s"
            << nl << endl;
    }

    Info<< "End\n" << endl;

    return 0;
}
