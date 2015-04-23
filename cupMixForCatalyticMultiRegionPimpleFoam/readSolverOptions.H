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
OpenSMOKE::KineticsMap_CHEMKIN<double>* 			kineticsMapXML;
OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>*		surfaceThermodynamicsMapXML; 
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
	
	//- Inert species
	word inertSpecies(kineticsDictionary.lookup("inertSpecies"));
	inertIndex = thermodynamicsMapXML->IndexOfSpecies(inertSpecies)-1;
}


//- PropertiesFromDictionary (on = from dictionar off = calculated) 
Switch propertiesFromDictionary(solverOptions.lookup("propertiesFromDictionary"));

//- Internal models
enum { STRANG_REACTION_TRANSPORT_REACTION, STRANG_REACTION_TRANSPORT, STRANG_TRANSPORT_REACTION, STRANG_REACTION_TRANSPORT_HYBRID, STRANG_TRANSPORT_REACTION_MOMENTUM} strangAlgorithm;

// Physical model
Switch energyEquation;
Switch includeDpDt;
const dictionary& physicalModelDictionary = solverOptions.subDict("PhysicalModel");
{
	energyEquation = Switch(physicalModelDictionary.lookup(word("energyEquation")));
	includeDpDt = Switch(physicalModelDictionary.lookup(word("includeDpDt")));

	// Algorithm
	{
		word strang(physicalModelDictionary.lookup("strangAlgorithm"));
		     if (strang == "ReactionTransportReaction")		strangAlgorithm = STRANG_REACTION_TRANSPORT_REACTION;
		else if (strang == "ReactionTransport")			strangAlgorithm = STRANG_REACTION_TRANSPORT;
		else if (strang == "TransportReaction")			strangAlgorithm = STRANG_TRANSPORT_REACTION;
		else if (strang == "ReactionTransportHybrid")		strangAlgorithm = STRANG_REACTION_TRANSPORT_HYBRID;
		else if (strang == "TransportReactionMomentum")		strangAlgorithm = STRANG_TRANSPORT_REACTION_MOMENTUM;
		
		else
		{
			Info << "Wrong strangAlgorithm option: ReactionTransportReaction || TransportReaction || ReactionTransport || ReactionTransportHybrid || TransportReactionMomentum" << endl;
			abort();
		}
	}
}


const dictionary& fluidRegionDictionary = solverOptions.subDict("fluidRegions");
List<Switch> solveFluidRegions(fluidRegionDictionary.lookup("solve"));
List<Switch> solveFluidChemistry(fluidRegionDictionary.lookup("homogeneousReactions"));
List<Switch> momentumEquation(fluidRegionDictionary.lookup("momentumEquation"));
List<Switch> fluidSpeciesEquations(fluidRegionDictionary.lookup("speciesEquations"));
List<Switch> iFluidMassDiffusionInEnergyEquation(fluidRegionDictionary.lookup("massDiffusionInEnergyEquation"));
List<word> patchNeighboringFluid(fluidRegionDictionary.lookup("patchNeighboring"));
List<word> patchAdjacentFluid(fluidRegionDictionary.lookup("patchAdjacent"));

enum { DIFFUSIVITY_MODEL_MULTICOMPONENT, DIFFUSIVITY_MODEL_LEWIS_NUMBERS} diffusivityModel;
Switch soretEffect;
std::vector<bool> soretEffectList;
std::vector<int> indexSoret;
Eigen::VectorXd LewisNumbers;
{
	// Soret effect
	soretEffect = Switch(fluidRegionDictionary.lookup(word("soretEffect")));
	soretEffectList.resize(thermodynamicsMapXML->NumberOfSpecies());
	indexSoret.resize(thermodynamicsMapXML->NumberOfSpecies());

	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		soretEffectList[i] = false;	
	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		indexSoret[i] = -1;
	
	// Diffusivity of species
	{
		word diffusivity(fluidRegionDictionary.lookup("diffusivityModel"));
		if (diffusivity == "multi-component")		diffusivityModel = DIFFUSIVITY_MODEL_MULTICOMPONENT;
		else if (diffusivity == "lewis-numbers")	diffusivityModel = DIFFUSIVITY_MODEL_LEWIS_NUMBERS;
		else
		{
			Info << "Wrong diffusivityModel option: multi-component || lewis-numbers" << endl;
			abort();
		}
		
		if (diffusivityModel == DIFFUSIVITY_MODEL_LEWIS_NUMBERS)
		{
			LewisNumbers.resize(thermodynamicsMapXML->NumberOfSpecies());
			
			const dictionary& lewisNumberDictionary = solverOptions.subDict("LewisNumbers");
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				LewisNumbers(i) = readScalar(lewisNumberDictionary.lookup(thermodynamicsMapXML->NamesOfSpecies()[i]));
			
			Info << endl;
			Info << "Diffusivities of species will be calculated using the following Lewis numbers: " << endl;
			for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
				Info << " " << thermodynamicsMapXML->NamesOfSpecies()[i] << "\t" << LewisNumbers(i) << endl;
			Info << endl;
		}
	}
}

const dictionary& solidRegionDictionary = solverOptions.subDict("solidRegions");
List<Switch> solveSolidRegions(solidRegionDictionary.lookup("solve"));
List<Switch> solveSolidChemistry(solidRegionDictionary.lookup("heterogeneousReactions"));
List<Switch> solidSpeciesEquations(solidRegionDictionary.lookup("speciesEquations"));
List<Switch> iSolidMassDiffusionInEnergyEquation(solidRegionDictionary.lookup("massDiffusionInEnergyEquation"));
List<word> diffusivitySolidLaw(solidRegionDictionary.lookup("diffusivityLaw"));
List<word> patchNeighboringSolid(solidRegionDictionary.lookup("patchNeighboring"));
List<word> patchAdjacentSolid(solidRegionDictionary.lookup("patchAdjacent"));

Info<< "Solver options correctly read\n" << endl;
