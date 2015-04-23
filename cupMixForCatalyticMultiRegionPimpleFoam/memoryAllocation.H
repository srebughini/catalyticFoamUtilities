//- Memory allocation: gas-phase properties
OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

//- Memory allocation: gas-phase chemistry
OpenSMOKE::OpenSMOKEVectorDouble omega(thermodynamicsMapXML->NumberOfSpecies());

// Properties Dictionaries creation

IOdictionary solidProperties
(
	IOobject
	(
		"solidProperties",
		runTime.constant(),
		runTime.db(),
		IOobject::READ_IF_PRESENT,
		IOobject::NO_WRITE
	)
);
	

IOdictionary fluidProperties
(
	IOobject
	(
		"fluidProperties",
		runTime.constant(),
		runTime.db(),
		IOobject::READ_IF_PRESENT,
		IOobject::NO_WRITE
	)
);
