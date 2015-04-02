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
		double Num = calculateSpeciesFlowRate(fluidRegions[j],phi, Y[index], listInletBoundaries[j]);
		double Den = calculateMassFlowRate(fluidRegions[j],phi,listInletBoundaries[j]);
		speciesMassFlowInlet += Num/Den;
	}
	for (int j=0;j<listOutletBoundaries.size();j++)
	{
		double Num = calculateSpeciesFlowRate(fluidRegions[j],phi, Y[index], listOutletBoundaries[j]);
		double Den = calculateMassFlowRate(fluidRegions[j],phi,listOutletBoundaries[j]);
		speciesMassFlowOutlet += Num/Den;
	}
	ConversionVector[index+1] = (speciesMassFlowInlet-speciesMassFlowOutlet)*100./max(1.e-64, speciesMassFlowInlet);
	MassFractionInlet[index+1] = speciesMassFlowInlet;
	MassFractionOutlet[index+1] = speciesMassFlowOutlet;
}
