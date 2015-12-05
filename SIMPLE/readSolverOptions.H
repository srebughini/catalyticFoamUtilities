Info<< "Reading Solver options\n" << endl;
IOdictionary solverOptions
(
    IOobject
    (
        "solverOptions",
        U.time().constant(),
        U.db(),
        IOobject::MUST_READ,
        IOobject::NO_WRITE
    )
);

// Read the kinetic scheme in XML format
OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>*            thermodynamicsMapXML; 
OpenSMOKE::KineticsMap_CHEMKIN<double>*             kineticsMapXML;
OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>*        surfaceThermodynamicsMapXML; 
OpenSMOKE::KineticsMap_Surface_CHEMKIN<double>*         surfaceKineticsMapXML;
OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>*         transportMapXML;

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
    }    
}

List<word>  listInletBoundaries;
List<word>  listOutletBoundaries;
List<word>  listCatalyticBoundaries;
const dictionary& postProcessing = solverOptions.subDict("PostProcessing");
{
    // List of inlet boundaries
    listInletBoundaries = List<word>(postProcessing.lookup("inletBoundaries"));
    // List of outlet boundaries
    listOutletBoundaries = List<word>(postProcessing.lookup("outletBoundaries"));
    // List of catalytic boundaries
    listCatalyticBoundaries = List<word>(postProcessing.lookup("catalyticBoundaries"));
}

Info<< "Solver options correctly read\n" << endl;
