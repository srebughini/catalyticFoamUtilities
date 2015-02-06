//- Memory allocation: gas-phase properties
OpenSMOKE::OpenSMOKEVectorDouble massFractions(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble moleFractions(thermodynamicsMapXML->NumberOfSpecies());

//- Memory allocation: gas-phase chemistry
OpenSMOKE::OpenSMOKEVectorDouble omega(thermodynamicsMapXML->NumberOfSpecies());
