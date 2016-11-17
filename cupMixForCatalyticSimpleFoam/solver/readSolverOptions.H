Info<< "\nReading Solver options\n"<< endl;
IOdictionary solverOptions
(
    IOobject
    (
        "solverOptions",
        runTime.constant(),
        runTime.db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

// Read the kinetic scheme in XML format
OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>*                thermodynamicsMapXML; 
OpenSMOKE::KineticsMap_CHEMKIN<double>*                      kineticsMapXML;
OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>*        surfaceThermodynamicsMapXML; 
OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>*       surfaceKineticsMapXML;
OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>*           transportMapXML;

const dictionary& kineticsDictionary = solverOptions.subDict("Kinetics");
{
    //- Kinetics folder
    Foam::string kinetics_folder= kineticsDictionary.lookup("folder");
    
    // Reading homogeneous data
    {    
        word kinetics("opensmoke");
        boost::filesystem::path path_kinetics = kinetics_folder;

        rapidxml::xml_document<> doc;
        std::vector<char> xml_string;
        OpenSMOKE::OpenInputFileXML(doc, xml_string,path_kinetics / "kinetics.xml");

        thermodynamicsMapXML = new OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>(doc); 
        transportMapXML = new OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>(doc); 
        kineticsMapXML = new OpenSMOKE::KineticsMap_CHEMKIN<double>(*thermodynamicsMapXML, doc); 
    }
    
    // Reading heterogeneous data
    {    
        word kinetics("opensmoke");
        boost::filesystem::path path_kinetics = kinetics_folder;

        rapidxml::xml_document<> doc;
        std::vector<char> xml_string;
        OpenSMOKE::OpenInputFileXML(doc, xml_string,path_kinetics / "kinetics.surface.xml");
        
        surfaceThermodynamicsMapXML = new OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>(doc); 
        surfaceKineticsMapXML = new OpenSMOKE::KineticsMap_Surface_CHEMKIN_Lumped<double>(*surfaceThermodynamicsMapXML, doc); 
    }    
}

// Physical model
enum            { DIFFUSIVITY_MODEL_MULTICOMPONENT, DIFFUSIVITY_MODEL_LEWIS_NUMBERS} diffusivityModel;
Eigen::VectorXd LewisNumbers;

Switch               surfaceChemistry;
Switch               gasChemistry;
Switch               energy;
Switch               coverage;

label                inertIndex;
label                catalystIndex;

scalar               alfa;

const dictionary& physicalModelDictionary = solverOptions.subDict("PhysicalModel");
{
    surfaceChemistry = Switch(physicalModelDictionary.lookup(word("surfaceChemistry")));
    gasChemistry     = Switch(physicalModelDictionary.lookup(word("gasChemistry")));
    energy           = Switch(physicalModelDictionary.lookup(word("energy")));
    coverage         = Switch(physicalModelDictionary.lookup(word("coverage")));

    // Diffusivity of species
    {
        word diffusivity(physicalModelDictionary.lookup("diffusivityModel"));
        if      (diffusivity == "multi-component")  diffusivityModel = DIFFUSIVITY_MODEL_MULTICOMPONENT;
        else if (diffusivity == "lewis-numbers")    diffusivityModel = DIFFUSIVITY_MODEL_LEWIS_NUMBERS;
        else
        {
            FatalError << "wrong diffusivityModel option: multi-component || lewis-numbers" << exit(FatalError);
        }
        
        if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
        {
            LewisNumbers.resize(thermodynamicsMapXML->NumberOfSpecies());
            
            const dictionary& lewisNumberDictionary = solverOptions.subDict("LewisNumbers");

            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                LewisNumbers(i) = readScalar(lewisNumberDictionary.lookup(thermodynamicsMapXML->NamesOfSpecies()[i]));
            }
            
            Info << endl;
            Info << "Diffusivities of species will be calculated using the following Lewis numbers: " << endl;
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                Info << " " << thermodynamicsMapXML->NamesOfSpecies()[i] << "\t" << LewisNumbers(i) << endl;
            }
            Info << endl;
        }
    }

    word inertSpecies(physicalModelDictionary.lookup("inert"));
    inertIndex = thermodynamicsMapXML->IndexOfSpecies(inertSpecies)-1;

    word catalystSpecies(physicalModelDictionary.lookup("catalyst"));
    catalystIndex = surfaceThermodynamicsMapXML->IndexOfSpecies(catalystSpecies)-surfaceThermodynamicsMapXML->number_of_gas_species()-1;

    alfa          = readScalar(physicalModelDictionary.lookup(word("catalyticLoad")));
}

Switch  implicit = true;
Switch  sparse   = false;
scalar  rF       = 0.;

if ( gasChemistry == true )
{
    const dictionary& gasDictionary = solverOptions.subDict("GasChemistry");
    {
        implicit = Switch(gasDictionary.lookup(word("implicit")));
        sparse   = Switch(gasDictionary.lookup(word("sparse")));
        rF       = readScalar(gasDictionary.lookup(word("reactionUpdate")));
    }
}

const dictionary& odeDictionary = solverOptions.subDict("Ode");
OpenSMOKE::ODE_Parameters odeParameter;
{
    //- Mass fractions tolerance
    scalar relTolerance = readScalar(odeDictionary.lookup("relTolerance"));
    scalar absTolerance = readScalar(odeDictionary.lookup("absTolerance"));

    odeParameter.SetRelativeTolerance(relTolerance);
    odeParameter.SetAbsoluteTolerance(absTolerance);

    //- Full pivoting (only for OpenSMOKE solver)
    Switch fullPivoting(odeDictionary.lookup("fullPivoting"));
    odeParameter.SetFullPivoting(fullPivoting);

    //- Maximum order of integration (only for OpenSMOKE solver)
    label maximumOrder = readLabel(odeDictionary.lookup("maximumOrder"));
    odeParameter.SetMaximumOrder(maximumOrder);

    odeParameter.SetType(OpenSMOKE::ODE_Parameters::ODE_INTEGRATOR_OPENSMOKE);

}

Info<< "Solver options correctly read" << nl << endl;

