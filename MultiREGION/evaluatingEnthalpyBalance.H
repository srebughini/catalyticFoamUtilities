//Evaluate enthalpy
double EnthalpyFluxOut   = 0.;
double EnthalpyFluxIn    = 0.;
double InletTemperature  = 0.;
double OutletTemperature = 0.;
{
	//- Evaluating temperature
	for (int j=0;j<listInletBoundaries.size();j++)
	{
		label patchID = fluidRegions[j].boundaryMesh().findPatchID(listInletBoundaries[j]);
		forAll(Y[0].boundaryField()[patchID] , facei)
		{
			double area = fluidRegions[j].magSf().boundaryField()[patchID][facei];
			InletTemperature += area*T.boundaryField()[patchID][facei];
		}
	}
	for (int j=0;j<listOutletBoundaries.size();j++)
	{
		label patchID = fluidRegions[j].boundaryMesh().findPatchID(listOutletBoundaries[j]);
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

		{
			OpenSMOKE::OpenSMOKEVectorDouble h(thermodynamicsMapXML->NumberOfSpecies());
			thermodynamicsMapXML->SetPressure(InletPressure);
			thermodynamicsMapXML->SetTemperature(InletTemperature);
			thermodynamicsMapXML->hMolar_Species(h);
			double G = MassFractionInlet.SumElements();
			for (unsigned int j=1;j<=thermodynamicsMapXML->NumberOfSpecies();j++)
			{
				EnthalpyFluxIn += MassFractionInlet[j]*h[j]*G/thermodynamicsMapXML->MW()[j];
			}
		}

		{
			OpenSMOKE::OpenSMOKEVectorDouble h(thermodynamicsMapXML->NumberOfSpecies());
			thermodynamicsMapXML->SetPressure(OutletPressure);
			thermodynamicsMapXML->SetTemperature(OutletTemperature);
			thermodynamicsMapXML->hMolar_Species(h);
			double G = MassFractionOutlet.SumElements();
			for (unsigned int j=1;j<=thermodynamicsMapXML->NumberOfSpecies();j++)
			{
				EnthalpyFluxOut += MassFractionOutlet[j]*h[j]*G/thermodynamicsMapXML->MW()[j];
			}
		}

	}
}
