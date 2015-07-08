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

    Info<< "Reading thermophysical properties\n" << endl;

    autoPtr<psiThermo> pThermo
    (
        psiThermo::New(mesh)
    );
    psiThermo& thermo = pThermo();
    thermo.validate(args.executable(), "h", "e");

    volScalarField Pr
    (
        IOobject
        (
            "Pr",
            runTime.timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        thermo.mu()*thermo.Cp()/thermo.kappa()
    );

    Pr.write();

    return 0;
}
