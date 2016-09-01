Info<< "Reading Solver options" << nl << endl;
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
    Info<< " * loading homogeneous kinetics...\n" << endl;
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
    Info<< " * loading heterogeneous kinetics...\n" << endl;
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

Info<< "Solver options correctly read" << nl << endl;

