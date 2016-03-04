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

List<scalar>  epsiM(solidRegions.size());
List<scalar>  lambdaM(solidRegions.size());
List<scalar>  tauM(solidRegions.size());
List<scalar>  alfaM(solidRegions.size());
List<scalar>  poreRadiusM(solidRegions.size());
List<scalar>  edcM(solidRegions.size());

List<int>                       NbinSolid(solidRegions.size());
List<std::vector<double> >      dzSolid(solidRegions.size());
List<scalar>                    epsiBinSolid(solidRegions.size());
List<Switch>                    caSolid(solidRegions.size());

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
        lambdaM[j]               = readScalar(solidDictionary.lookup("thermalConductivity"));
        alfaM[j]                 = readScalar(solidDictionary.lookup("catalyticLoad"));    // ActiveArea/CatalystVolume(Total)
        
        caSolid[j]               = Switch(solidDictionary.lookup("cellAgglomeration"));
        if ( caSolid[j] == true )
        {
            epsiBinSolid[j]   = readScalar(solidDictionary.lookup("CAtollerance"));
            NbinSolid[j]      = int(1./epsiBinSolid[j]) + 1;

            dzSolid[j].resize(NbinSolid[j]);
            dzSolid[j][0]     = 0.;
            for (unsigned k=0;k<(NbinSolid[j]-1);k++)
            {
                dzSolid[j][k+1] = dzSolid[j][k] + 1./(double(NbinSolid[j])-1.);
            }
        }
        else
        {
            epsiBinSolid[j] = 0.;
            NbinSolid[j]    = 0;
            dzSolid[j].clear();
        }

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
            poreRadiusM[j]       = readScalar(solidDictionary.lookup("poreRadius"));
            epsiM[j]             = readScalar(solidDictionary.lookup("voidFraction"));
            Info << "Wrong NOT YET IMPLEMENTED" << endl;
            abort();
        }
        else if ( diffusivityLaw[j] == "EffectiveDiffusivityModel" )
        {
            edcM[j]   = readScalar(solidDictionary.lookup("coefficient"));
        }
        else
        {
            Info << "Wrong diffusivityModel option: DustyGasModel || EffectiveDiffusivityModel" << endl;
            abort();
        }
    }
}
