Info << endl;

PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
PtrList<volScalarField>     DpDtFluid(fluidRegions.size());

forAll(fluidRegions, j)
{
    Info<< "*** Reading region " << fluidRegions[j].name() << "\n" << endl;

    Info<< "Creating field rho\n" << endl;
    rhoFluid.set
    (   j,
        new volScalarField
        (
            IOobject
            (
                "rho",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            Rho(pFluid[j],psiFluid[j])
        )
    );

    Info<< "Reading/calculating face flux field phi\n" << endl;
    phiFluid.set
    (   j,
        new surfaceScalarField
        (
            IOobject
            (
                "phi",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::READ_IF_PRESENT,
                IOobject::AUTO_WRITE
            ),
        linearInterpolate(rhoFluid[j]*UFluid[j]) & fluidRegions[j].Sf()
        )
    );
    
    Info<< "Creating field DpDt\n" << endl;
    DpDtFluid.set
    (   j,
        new volScalarField(fvc::DDt(surfaceScalarField("phiU", phiFluid[j]/fvc::interpolate(rhoFluid[j])), pFluid[j]))
    );
}

multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
forAll(YFluid, j)
{
    fields.add(YFluid[j]);
}
