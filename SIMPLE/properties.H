{
	Info << "Properties..." << endl;
	const scalarField& pCells = p.internalField();
	const scalarField& TCells = T.internalField();

	scalarField& CpCells = cp.internalField();
	scalarField& mwCells = MWmix.internalField();
	scalarField& hCells = h.internalField();
	
	scalarField& muCells = mu.internalField();
	scalarField& lambdaCells = lambda.internalField();

	OpenSMOKE::OpenSMOKEVectorDouble y(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble x(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble c(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble gammamix(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(Y.size());
	OpenSMOKE::OpenSMOKEVectorDouble HVector(Y.size());

	Eigen::MatrixXd SpecieAtomicComposition;
	Eigen::VectorXd Element;
	std::vector<std::string> ElementsNames;

	forAll(TCells, celli)
	{
		if (TCells[celli] < 0. || pCells[celli]<0.)
			Info << "Wrong T and P: " << celli << " " << TCells[celli] << " " << pCells[celli] << endl;
				
		// Sets the maps
		thermodynamicsMapXML->SetTemperature(TCells[celli]);
		thermodynamicsMapXML->SetPressure(pCells[celli]);

		transportMapXML->SetPressure(pCells[celli]);
		transportMapXML->SetTemperature(TCells[celli]);

		// Extract the mass fractions
		for(unsigned int i=1;i<=Y.size();i++)
			y[i] = Y[i-1].internalField()[celli];

		// Calculate the molecular weight and the mole fractions
		thermodynamicsMapXML->MoleFractions_From_MassFractions(x, mwCells[celli], y);	
		
		// Calculate the density
		double const cTot = pCells[celli] / (PhysicalConstants::R_J_kmol * TCells[celli]);
		double const rho = cTot * mwCells[celli];

		// Calculate constant pressure and constant volume specific heats
		double CpMixMolar; 
		thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(CpMixMolar,x);
		CpCells[celli] = CpMixMolar / mwCells[celli];

		// Calculate the enthalpy
		thermodynamicsMapXML->hMolar_Mixture_From_MoleFractions(hCells[celli], x);
		hCells[celli] /= mwCells[celli];
		

		// Accurate transport properties
		{
			// Effective Thermal conductivity [kg/m/s]
			transportMapXML->ThermalConductivity(lambdaCells[celli], x);

			// Dynamic viscosity [kg/m/s]
			transportMapXML->DynamicViscosity(muCells[celli], x);
			
			// Mass diffusion coefficients [m2/s]
			transportMapXML->MassDiffusionCoefficients(gammamix, x);
			for(unsigned int i=1;i<=Y.size();i++)
				Dmix_[i-1].internalField()[celli] = gammamix[i];
		}
		
			// Atomic fields
			{
				ElementsNames = thermodynamicsMapXML->elements();
				SpecieAtomicComposition = thermodynamicsMapXML->atomic_composition();
				
				for (unsigned int k=0;k<ElementsNames.size();k++)
				{
					element_[k].internalField()[celli] = 0.;
					Element = SpecieAtomicComposition.col(k);
					double sum = 0.;

					for(unsigned int i=1;i<=y.Size();i++)
						sum = sum + y[i]*Element[i-1]/thermodynamicsMapXML->MW()[i];

					element_[k].internalField()[celli] = sum;
				
				}
			}
	}

	forAll(T.boundaryField(), patchi)
	{
		fvPatchScalarField& pp = p.boundaryField()[patchi];
		fvPatchScalarField& pT = T.boundaryField()[patchi];
		
		fvPatchScalarField& pmw = MWmix.boundaryField()[patchi];
		fvPatchScalarField& pcp = cp.boundaryField()[patchi];
		fvPatchScalarField& ph = h.boundaryField()[patchi];
		fvPatchScalarField& pmu = mu.boundaryField()[patchi];
		fvPatchScalarField& plambda = lambda.boundaryField()[patchi];

		{
			forAll(pT, facei)
			{
				// Sets the maps
				thermodynamicsMapXML->SetTemperature(pT[facei]);
				thermodynamicsMapXML->SetPressure(pp[facei]);

				transportMapXML->SetPressure(pp[facei]);
				transportMapXML->SetTemperature(pT[facei]);

				// Extract the mass fractions
				for(unsigned int i=1;i<=Y.size();i++)
					y[i] = Y[i-1].boundaryField()[patchi][facei];

				// Calculate the molecular weight and the mole fractions
				thermodynamicsMapXML->MoleFractions_From_MassFractions(x, pmw[facei], y);

				// Calculate the density
				double const cTot = pp[facei]  / (PhysicalConstants::R_J_kmol * pT[facei]);
				double const rho = cTot * pmw[facei];

				// Calculate constant pressure and constant volume specific heats
				double CpMixMolar; 
				thermodynamicsMapXML->cpMolar_Mixture_From_MoleFractions(CpMixMolar,x);
				pcp[facei]= CpMixMolar / pmw[facei];

				// Calculate the enthalpy
				thermodynamicsMapXML->hMolar_Mixture_From_MoleFractions(ph[facei], x);
				ph[facei] /= pmw[facei];
				
				// Calculate accurate transport properties
				{
					// Effective Thermal conductivity [kg/m/s]
					transportMapXML->ThermalConductivity(plambda[facei], x);

					// Dynamic viscosity [kg/m/s]
					transportMapXML->DynamicViscosity(pmu[facei], x);
					
					// Mass diffusion coefficients [m2/s]
					transportMapXML->MassDiffusionCoefficients(gammamix, x);
					for(unsigned int i=1;i<=Y.size();i++)
						Dmix_[i-1].boundaryField()[patchi][facei] = gammamix[i];	
				}

				// Atomic fields
				{
					ElementsNames = thermodynamicsMapXML->elements();
					SpecieAtomicComposition = thermodynamicsMapXML->atomic_composition();
					
					for (unsigned int k=0;k<ElementsNames.size();k++)
					{
						element_[k].boundaryField()[patchi][facei] = 0.;
						Element = SpecieAtomicComposition.col(k);
						double sum = 0.;

						for(unsigned int i=1;i<=y.Size();i++)
							sum = sum + y[i]*Element[i-1]/thermodynamicsMapXML->MW()[i];

						element_[k].boundaryField()[patchi][facei] = sum;
					
					}
				}
			}
		}
	}
}
