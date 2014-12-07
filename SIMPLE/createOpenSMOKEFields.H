// Read the kinetic scheme in XML format
OpenSMOKE::ThermodynamicsMap_CHEMKIN<double>* thermodynamicsMapXML; 
OpenSMOKE::TransportPropertiesMap_CHEMKIN<double>* transportMapXML;
OpenSMOKE::KineticsMap_CHEMKIN<double>* kineticsMapXML;

{	
	word kinetics("opensmoke");
	boost::filesystem::path path_kinetics = kinetics;

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

PtrList<volScalarField> Dmix_;
PtrList<volScalarField> HSpecies_;
PtrList<volScalarField> R_;


// Creating fields
{
	// Check the order of species
	if (thermodynamicsMapXML->NumberOfSpecies() != Y.size())
		FatalError << "Kinetic schemes do not match..." << endl;

	for (unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
		if (Y[i].name() != thermodynamicsMapXML->NamesOfSpecies()[i])
		{
			FatalError << "Kinetic schemes do not match..." << endl;
		}
	
	R_.resize(Y.size());
	Dmix_.resize(Y.size());
	HSpecies_.resize(Y.size());

	forAll(Y,i)
	{
	
		R_.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					"gas_R_"+ Y[i].name(),
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::AUTO_WRITE
				),
				mesh,
				dimensionedScalar("zero", dimensionSet(1, -3, -1, 0, 0), 0.)
			)
		);

		Dmix_.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					"gas_Dmix_"+ Y[i].name(),
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::AUTO_WRITE
				),
				mesh,
				dimensionedScalar("zero", dimensionSet(0, 2, -1, 0, 0), 0.)
			)
		);

		HSpecies_.set
		(
			i,
			new volScalarField
			(
			    IOobject
			    (
				"gas_H_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE
			    ),
			    mesh,
			    dimensionSet(0, 2, -2, 0, 0)
			)
	    	);		
	}

}

//- Mean molecular weight [kg/mol]
Info<< "Creating field MWmix\n" << endl;
volScalarField MWmix
(
 IOobject
 (
	   "gas_MW",
	   runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::AUTO_WRITE
 ),
 mesh,
  dimensionSet(1, 0, 0, 0, -1)
);

//- Dynamic viscosity [kg/m/s]
Info<< "Creating field mu\n" << endl;
volScalarField mu
(
  IOobject
  (
	  "gas_mu",
	  runTime.timeName(),
	  mesh,
	  IOobject::NO_READ,
	   IOobject::AUTO_WRITE
	),
 mesh,
 dimensionSet(1, -1, -1, 0, 0)
);

//- Thermal conductivity [W/m/K]
Info<< "Creating field lambda\n" << endl;
volScalarField lambda
(
 IOobject
 (
	   "gas_lambda",
	   runTime.timeName(),
	   mesh,
	 IOobject::NO_READ,
		IOobject::AUTO_WRITE
 ),
  mesh,
   dimensionSet(1, 1, -3, -1, 0)
);

//- Specific heat [J/kg/K]
Info<< "Creating field cp\n" << endl;
volScalarField cp
(
 IOobject
 (
	  "gas_Cp",
	  runTime.timeName(),
	   mesh,
	   IOobject::NO_READ,
	   IOobject::AUTO_WRITE
 ),
 mesh,
 dimensionSet(0, 2, -2, -1, 0)
);

//- Allocating memory for total enthalpy [m2/s2]
Info<< "Creating field enthalpy\n" << endl;
volScalarField h
(
  IOobject
 (
		"gas_h",
		runTime.timeName(),
		mesh,
	  IOobject::NO_READ,
		IOobject::AUTO_WRITE
  ),
   mesh,
  dimensionSet(0, 2, -2, 0, 0)
);


// Flux fields
#include "createFluxes.H"

//- Creating element fields
std::vector<std::string> elementNames_;
elementNames_ = thermodynamicsMapXML->elements();
PtrList<volScalarField> element_;
element_.resize(elementNames_.size());

for (unsigned int i=0;i<elementNames_.size();i++)
{
	element_.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
			"element_"+ elementNames_[i],
			mesh.time().timeName(),
			mesh,
			IOobject::NO_READ,
			IOobject::NO_WRITE
			),
			mesh,
			dimensionSet(0, 0, 0, 0, 0)
		)
	);
}
