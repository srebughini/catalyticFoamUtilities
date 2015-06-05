//Evaluate Conversion
OpenSMOKE::OpenSMOKEVectorDouble ConversionVector(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble MassFractionInlet(thermodynamicsMapXML->NumberOfSpecies());
OpenSMOKE::OpenSMOKEVectorDouble MassFractionOutlet(thermodynamicsMapXML->NumberOfSpecies());
for (int index=0;index<thermodynamicsMapXML->NumberOfSpecies();index++)
{
	scalar speciesMassFlowInlet  = 0.;
	scalar speciesMassFlowOutlet = 0.;
	for (int k=0;k<listInletBoundaries.size();k++)
	{
		double Num = calculateSpeciesFlowRate(fluidRegions[j],phi, Y[index], listInletBoundaries[k]);
		double Den = calculateMassFlowRate(fluidRegions[j],phi,listInletBoundaries[k]);
		speciesMassFlowInlet += Num/Den;
	}
	for (int k=0;k<listOutletBoundaries.size();k++)
	{
		double Num = calculateSpeciesFlowRate(fluidRegions[j],phi, Y[index], listOutletBoundaries[k]);
		double Den = calculateMassFlowRate(fluidRegions[j],phi,listOutletBoundaries[k]);
		speciesMassFlowOutlet += Num/Den;
	}
	ConversionVector[index+1] = (speciesMassFlowInlet-speciesMassFlowOutlet)*100./max(1.e-64, speciesMassFlowInlet);
	MassFractionInlet[index+1] = speciesMassFlowInlet;
	MassFractionOutlet[index+1] = speciesMassFlowOutlet;
}
