Info<< "Reading Solver options\n" << endl;
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
label inertIndex;
OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>*			thermodynamicsMapXML; 
OpenSMOKE::KineticsMap_CHEMKIN<double>* 				kineticsMapXML;
OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>*	surfaceThermodynamicsMapXML; 
OpenSMOKE::KineticsMap_Surface_CHEMKIN<double>* 		surfaceKineticsMapXML;
OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>* 		transportMapXML;

const dictionary& kineticsDictionary = solverOptions.subDict("Kinetics");
{
	//- Kinetics folder
	Foam::string kinetics_folder= kineticsDictionary.lookup("folder");

	// Reading homogeneous data
	Info<< " * loading homogeneous kinetics...\n" << endl;
	{
		word kinetics("opensmoke");
		boost::filesystem::path path_kinetics = kinetics_folder;

		rapidxml::xml_document<> doc;
		std::vector<char> xml_string;
		OpenSMOKE::OpenInputFileXML(doc, xml_string,path_kinetics / "kinetics.xml");

		double tStart = OpenSMOKE::OpenSMOKEGetCpuTime();
		
		thermodynamicsMapXML = new OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>(doc); 
		transportMapXML = new OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>(doc); 
		kineticsMapXML = new OpenSMOKE::KineticsMap_CHEMKIN<double>(*thermodynamicsMapXML, doc); 
							
		double tEnd = OpenSMOKE::OpenSMOKEGetCpuTime();
		std::cout << " * Time to read XML file: " << tEnd-tStart << std::endl;
	}

	// Reading heterogeneous data
	Info<< " * loading heterogeneous kinetics...\n" << endl;
	{
		word kinetics("opensmoke");
		boost::filesystem::path path_kinetics = kinetics_folder;

		rapidxml::xml_document<> doc;
		std::vector<char> xml_string;
		OpenSMOKE::OpenInputFileXML(doc, xml_string,path_kinetics / "kinetics.surface.xml");

		double tStart = OpenSMOKE::OpenSMOKEGetCpuTime();
		
		surfaceThermodynamicsMapXML = new OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>(doc); 
		surfaceKineticsMapXML = new OpenSMOKE::KineticsMap_Surface_CHEMKIN<double>(*surfaceThermodynamicsMapXML, doc); 
							
		double tEnd = OpenSMOKE::OpenSMOKEGetCpuTime();
		std::cout << " * Time to read XML file: " << tEnd-tStart << std::endl;
	}
}

unsigned int directionIndex;
Foam::string asaliFolder;
List<bool>   energySolid;
List<bool>   energyFluid;
List<bool>   specieSolid;
List<bool>   specieFluid;
List<bool>   siteFluid;

const dictionary& preProcessingDictionary = solverOptions.subDict("FromASALItoCFD");
{
	word mainDirection(preProcessingDictionary.lookup("axialDirection"));

	if ( mainDirection == "x" )
	{
		directionIndex = 0;
	}
	else if ( mainDirection == "y" )
	{
		directionIndex = 1;
	}
	else if ( mainDirection == "z" )
	{
		directionIndex = 2;
	}
	else
	{
		Info << "Wrong axialDirection option: || x || y || z ||" << endl;
		abort();
	}

	asaliFolder = Foam::string(preProcessingDictionary.lookup("ASALIfolder"));
	
	energySolid = List<bool>(preProcessingDictionary.lookup("energyInSolid"));
	energyFluid = List<bool>(preProcessingDictionary.lookup("energyInFluid"));
	specieSolid = List<bool>(preProcessingDictionary.lookup("specieInSolid"));
	specieFluid = List<bool>(preProcessingDictionary.lookup("specieInFluid"));
	siteFluid   = List<bool>(preProcessingDictionary.lookup("siteInFluid"));
}

List<word>    patchCatalytic;
label         catalystIndex;
bool          catalyst = false;

const dictionary& catalystDictionary = solverOptions.subDict("Catalyst");
{
    patchCatalytic = List<word>(catalystDictionary.lookup("catalyticPatchName"));

    if ( patchCatalytic.size() == 0 )
    {
        Info << "Warning catalytic patches not detected..." << endl;
        Info << "Press Enter to continue..." << endl;
        getchar();
    }
    else
    {
        catalyst = true;
    }

    word catalystMaterial(catalystDictionary.lookup("catalystMaterial"));
    catalystIndex = surfaceThermodynamicsMapXML->IndexOfSpecies(catalystMaterial)-thermodynamicsMapXML->NumberOfSpecies()-1;
}

Info<< "Solver options correctly read\n" << endl;
