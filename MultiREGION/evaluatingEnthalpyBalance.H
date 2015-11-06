//Evaluate enthalpy
double EnthalpyFluxOut   = 0.;
double EnthalpyFluxIn    = 0.;
double InletTemperature  = 0.;
double OutletTemperature = 0.;
{
	//- Evaluating temperature
	for (int k=0;k<listInletBoundaries.size();k++)
	{
		label patchID = fluidRegions[j].boundaryfluidRegions[j]().findPatchID(listInletBoundaries[k]);
		forAll(Y[0].boundaryField()[patchID] , facei)
		{
			double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
			InletTemperature += area*T.boundaryField()[patchID][facei];
		}
	}
	for (int k=0;k<listOutletBoundaries.size();k++)
	{
		label patchID = fluidRegions[j].boundaryfluidRegions[j]().findPatchID(listOutletBoundaries[k]);
		forAll(Y[0].boundaryField()[patchID] , facei)
		{
			double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
			OutletTemperature += area*T.boundaryField()[patchID][facei];
		}
	}
	InletTemperature=InletTemperature/PatchInletArea;
	OutletTemperature=OutletTemperature/PatchOutletArea;

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

	{
		OpenSMOKE::OpenSMOKEVectorDouble h(thermodynamicsMapXML->NumberOfSpecies());
		thermodynamicsMapXML->SetPressure(InletPressure);
		thermodynamicsMapXML->SetTemperature(InletTemperature);
		thermodynamicsMapXML->hMolar_Species(h);

		double G;
		for (int k=0;k<listInletBoundaries.size();k++)
		{
			G += calculateMassFlowRate(fluidRegions[j],phi,listInletBoundaries[k]);
		}
		for (unsigned int k=1;k<=thermodynamicsMapXML->NumberOfSpecies();k++)
		{
			EnthalpyFluxIn += MassFractionInlet[k]*h[k]*G/thermodynamicsMapXML->MW()[k];
		}
	}

	{
		OpenSMOKE::OpenSMOKEVectorDouble h(thermodynamicsMapXML->NumberOfSpecies());
		thermodynamicsMapXML->SetPressure(OutletPressure);
		thermodynamicsMapXML->SetTemperature(OutletTemperature);
		thermodynamicsMapXML->hMolar_Species(h);

		double G;
		for (int k=0;k<listOutletBoundaries.size();k++)
		{
			G += calculateMassFlowRate(fluidRegions[j],phi,listOutletBoundaries[k]);
		}

		for (unsigned int k=1;k<=thermodynamicsMapXML->NumberOfSpecies();k++)
		{
			EnthalpyFluxOut += MassFractionOutlet[k]*h[k]*(-G)/thermodynamicsMapXML->MW()[k];
		}
	}
}
