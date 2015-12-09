Info<< "Reading file for FLUID properties\n" << endl;
IOdictionary fluidProperties
(
    IOobject
    (
        "fluidProperties",
        runTime.constant(),
        runTime.db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

List<Switch>  chemistryFluid(fluidRegions.size());
List<Switch>  energyFluid(fluidRegions.size());
List<label>   inertIndexFluid(solidRegions.size());

forAll(fluidRegions, j)
{
    const dictionary& fluidDictionary = fluidProperties.subDict(fluidRegions[j].name());
    {
        chemistryFluid[j]   = Switch(fluidDictionary.lookup("chemistry"));
        energyFluid[j]      = Switch(fluidDictionary.lookup("energy"));

        word inertSpecies(fluidDictionary.lookup("inert"));
        inertIndexFluid[j]    = thermodynamicsMapXML->IndexOfSpecies(inertSpecies)-1;

    }
}

