forAll(fluidRegions, j)
{
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
    
    initialMassFluid[j] = fvc::domainIntegrate(rhoFluid[j]).value();
}


multivariateSurfaceInterpolationScheme<scalar>::fieldTable fields;
forAll(YFluid, k)
{
    fields.add(YFluid[k]);
}
