IOdictionary solidProperties
(
    IOobject
    (
        "solidProperties",
        runTime.constant(),
        runTime.db(),
        IOobject::READ_IF_PRESENT,
        IOobject::NO_WRITE
    )
);

Info<< "Reading solid properties" << endl;
forAll(solidRegions, j)
{
    const dictionary& solidRegionProperties = solidProperties.subDict(solidRegions[j].name());
    {
        rhoSolid[j]         = dimensionedScalar(solidRegionProperties.lookup("solidDensity") );
        lambdaSolid[j]      = dimensionedScalar(solidRegionProperties.lookup("thermalConductivity") );
        cpSolid[j]          = dimensionedScalar(solidRegionProperties.lookup("specificHeat") );
    }
}

forAll(solidRegions, j)
{
