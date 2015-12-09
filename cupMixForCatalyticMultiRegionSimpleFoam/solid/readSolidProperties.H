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

List<scalar>  rhoM(solidRegions.size());
List<scalar>  epsiM(solidRegions.size());
List<scalar>  lambdaM(solidRegions.size());
List<scalar>  cpM(solidRegions.size());
List<scalar>  tauM(solidRegions.size());
List<scalar>  alfaM(solidRegions.size());
List<scalar>  poreRadius(solidRegions.size());

List<word>    diffusivityLaw(solidRegions.size());
List<word>    permeabilityLaw(solidRegions.size());

List<Switch>  chemistrySolid(solidRegions.size());
List<Switch>  energySolid(solidRegions.size());
List<Switch>  transportSolid(solidRegions.size());
List<Switch>  implicitSolid(solidRegions.size());
List<Switch>  coverageSolid(solidRegions.size());

List<label>   inertIndexSolid(solidRegions.size());
List<label>   catalystIndexSolid(solidRegions.size());

forAll(solidRegions, j)
{
    const dictionary& solidDictionary = solidProperties.subDict(solidRegions[j].name());
    {
        rhoM[j]                  = readScalar(solidDictionary.lookup("density"));
        epsiM[j]                 = readScalar(solidDictionary.lookup("voidFraction"));
        lambdaM[j]               = readScalar(solidDictionary.lookup("thermalConductivity"));
        cpM[j]                   = readScalar(solidDictionary.lookup("specificHeat"));
        alfaM[j]                 = readScalar(solidDictionary.lookup("catalyticLoad"));    // ActiveArea/CatalystVolume(Total)
        
        chemistrySolid[j]        = Switch(solidDictionary.lookup("chemistry"));
        transportSolid[j]        = Switch(solidDictionary.lookup("transport"));
        implicitSolid[j]         = Switch(solidDictionary.lookup("implicit"));
        coverageSolid[j]         = Switch(solidDictionary.lookup("coverage"));
        energySolid[j]           = Switch(solidDictionary.lookup("energy"));

        word inertSpecies(solidDictionary.lookup("inert"));
        inertIndexSolid[j]    = thermodynamicsMapXML->IndexOfSpecies(inertSpecies)-1;

        word catalystSpecie(solidDictionary.lookup("catalyst"));
        catalystIndexSolid[j] = surfaceThermodynamicsMapXML->IndexOfSpecies(catalystSpecie)-surfaceThermodynamicsMapXML->number_of_gas_species()-1;

        diffusivityLaw[j]        = word(solidDictionary.lookup("diffusivityLaw"));

        if ( diffusivityLaw[j] == "DustyGasModel" )
        {
            permeabilityLaw[j]   = word(solidDictionary.lookup("permeabilityLaw"));
            tauM[j]              = readScalar(solidDictionary.lookup("tortuosity"));
            poreRadius[j]        = readScalar(solidDictionary.lookup("poreRadius"));
            Info << "Wrong NOT YET IMPLEMENTED" << endl;
            abort();
        }
        else if ( diffusivityLaw[j] == "EffectiveDiffusivityModel" )
        {
            tauM[j]        = readScalar(solidDictionary.lookup("tortuosity"));
        }
        else
        {
            Info << "Wrong diffusivityModel option: DustyGasModel || EffectiveDiffusivityModel" << endl;
            abort();
        }
    }
}
