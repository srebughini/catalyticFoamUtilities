//Evaluate Conversion
OpenSMOKE::OpenSMOKEVectorDouble ConversionVector(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble MassFractionInlet(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble MassFractionOutlet(thermodynamicsMapXML->NumberOfSpecies());
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	scalar speciesMassFlowInlet  = 0.;
	scalar speciesMassFlowOutlet = 0.;
	for (int j=0;j<listInletBoundaries.size();j++)
	{
		double Num = calculateSpeciesFlowRate(mesh,phi, Y[index], listInletBoundaries[j]);
		double Den = calculateMassFlowRate(mesh,phi,listInletBoundaries[j]);
		if ( Den != 0. )
			speciesMassFlowInlet += Num/Den;
	}
	for (int j=0;j<listOutletBoundaries.size();j++)
	{
		double Num = calculateSpeciesFlowRate(mesh,phi, Y[index], listOutletBoundaries[j]);
		double Den = calculateMassFlowRate(mesh,phi,listOutletBoundaries[j]);
		if ( Den != 0. )
			speciesMassFlowOutlet += Num/Den;
	}
	ConversionVector[index+1] = (speciesMassFlowInlet-speciesMassFlowOutlet)*100./max(1.e-64, speciesMassFlowInlet);
	MassFractionInlet[index+1] = speciesMassFlowInlet;
	MassFractionOutlet[index+1] = speciesMassFlowOutlet;
}
