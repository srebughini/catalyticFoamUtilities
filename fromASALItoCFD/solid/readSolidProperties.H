Info<< "Reading file for SOLID properties\n" << endl;
IOdictionary solidProperties
(
    IOobject
    (
        "solidProperties",
        runTime.constant(),
        runTime.db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);


List<Switch>  energySolid(solidRegions.size());
List<Switch>  coverageSolid(solidRegions.size());

forAll(solidRegions, j)
{
    const dictionary& solidDictionary = solidProperties.subDict(solidRegions[j].name());
    {
        coverageSolid[j]         = Switch(solidDictionary.lookup("coverage"));
        energySolid[j]           = Switch(solidDictionary.lookup("energy"));
    }
}
