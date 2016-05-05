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

List<label>   inertIndexSolid(solidRegions.size());
List<label>   catalystIndexSolid(solidRegions.size());

forAll(solidRegions, j)
{
    const dictionary& solidDictionary = solidProperties.subDict(solidRegions[j].name());
    {
        coverageSolid[j]         = Switch(solidDictionary.lookup("coverage"));
        energySolid[j]           = Switch(solidDictionary.lookup("energy"));

        word inertSpecies(solidDictionary.lookup("inert"));
        inertIndexSolid[j]    = thermodynamicsMapXML->IndexOfSpecies(inertSpecies)-1;

        word catalystSpecie(solidDictionary.lookup("catalyst"));
        catalystIndexSolid[j] = surfaceThermodynamicsMapXML->IndexOfSpecies(catalystSpecie)-surfaceThermodynamicsMapXML->number_of_gas_species()-1;
    }
}
