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
OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>*            thermodynamicsMapXML; 
OpenSMOKE::KineticsMap_CHEMKIN<double>*                 kineticsMapXML;
OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>*    surfaceThermodynamicsMapXML; 
OpenSMOKE::KineticsMap_Surface_CHEMKIN<double>*         surfaceKineticsMapXML;
OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>*         transportMapXML;

const dictionary& kineticsDictionary = solverOptions.subDict("Kinetics");
{
    Foam::string kinetics_folder= kineticsDictionary.lookup("folder");

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

    {
        word kinetics("opensmoke");
        boost::filesystem::path path_kinetics = kinetics_folder;

        rapidxml::xml_document<> doc;
        std::vector<char> xml_string;
        OpenSMOKE::OpenInputFileXML(doc, xml_string,path_kinetics / "kinetics.surface.xml");
        
        surfaceThermodynamicsMapXML = new OpenSMOKE::ThermodynamicsMap_Surface_CHEMKIN<double>(doc); 
        surfaceKineticsMapXML = new OpenSMOKE::KineticsMap_Surface_CHEMKIN<double>(*surfaceThermodynamicsMapXML, doc); 
    }
}

unsigned int directionIndex;
Foam::string asaliFolder;

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
}
