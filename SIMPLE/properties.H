/*------------------------------------------------------------------------\
|   catalyticFOAM                                                         |
|   http://www.catalyticfoam.polimi.it/                                   |
|                                                                         |
|   Authors:                                                              |
|                                                                         |
|   Alberto Cuoci <alberto.cuoci@polimi.it>                               |
|   Department of Chemistry, Materials and Chemical Engineering           |
|   Politecnico di Milano                                                 |
|   P.zza Leonardo da Vinci 32, 20133 Milano                              |
|                                                                         |
|   Matteo Maestri <matteo.maestri@polimi.it>                             |
|   Department of Energy                                                  |
|   Politecnico di Milano                                                 |
|   P.zza Leonardo da Vinci 32, 20133 Milano                              |
|                                                                         |
|-------------------------------------------------------------------------|
|                                                                         |
|   This file is part of catalyticFOAM framework.                         |
|                                                                         |
|	License                                                           |
|                                                                         |
|   Copyright(C) 2014-2011, A.Cuoci, M.Maestri,                           |
|                2014-2013, S.Rebughini                                   |
|                     2013, T.Maffei                                      |
|                                                                         |
|   catalyticFOAM is free software: you can redistribute it and/or modify |
|   it under the terms of the GNU General Public License as published by  |
|   the Free Software Foundation, either version 3 of the License, or     |
|   (at your option) any later version.                                   |
|                                                                         |
|   OpenSMOKE++ is distributed in the hope that it will be useful,        |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of        |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
|   GNU General Public License for more details.                          |
|                                                                         |
|   You should have received a copy of the GNU General Public License     |
|   along with catalyticFOAM. If not, see <http://www.gnu.org/licenses/>. |
|                                                                         |
\*-----------------------------------------------------------------------*/

	{
		Info << "Properties..." << endl;
		const scalarField& pCells = p.internalField();
		const scalarField& TCells = T.internalField();

		scalarField& CpCells = cp.internalField();
		scalarField& mwCells = MWmix.internalField();
		scalarField& hCells = h.internalField();
		
		scalarField& muCells = mu.internalField();
		scalarField& lambdaCells = lambda.internalField();
		//scalarField& QCells = this->Q_.internalField();

		OpenSMOKE::OpenSMOKEVectorDouble y(Y.size());
		OpenSMOKE::OpenSMOKEVectorDouble x(Y.size());
		OpenSMOKE::OpenSMOKEVectorDouble c(Y.size());
		OpenSMOKE::OpenSMOKEVectorDouble gammamix(Y.size());
		OpenSMOKE::OpenSMOKEVectorDouble tetamixvector(Y.size());
		//OpenSMOKE::OpenSMOKEVectorDouble R(Y.size());
		OpenSMOKE::OpenSMOKEVectorDouble HVector(Y.size());

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
				//alphaCells[celli] = lambdaCells[celli]/CpCells[celli];
				
				// Dynamic viscosity [kg/m/s]
				transportMapXML->DynamicViscosity(muCells[celli], x);
				
				// Mass diffusion coefficients [m2/s]
				transportMapXML->MassDiffusionCoefficients(gammamix, x);
				for(unsigned int i=1;i<=Y.size();i++)
					Dmix_[i-1].internalField()[celli] = gammamix[i];
			}
			
			// Calculate formation rates
			/*
			if (calculate_reactions == true && homogeneous_reactions_ == true)
			{
				Product(cTot,x,&c);
				
				kineticsMapXML->SetPressure(pCells[celli]);
				kineticsMapXML->SetTemperature(TCells[celli]);
				
				kineticsMapXML->ReactionEnthalpiesAndEntropies();
				kineticsMapXML->ArrheniusKineticConstants();
				kineticsMapXML->ReactionRates(c);
				kineticsMapXML->FormationRates(&R);
				QCells[celli] = kineticsMapXML->HeatRelease(R);
				ElementByElementProduct(R,thermodynamicsMapXML->MW(),&R);
				
				for(unsigned int i=1;i<=Y.size();i++)
					RHomo_[i-1].internalField()[celli] = R[i];
			}
			*/
		}

		forAll(T.boundaryField(), patchi)
		{
			fvPatchScalarField& pp = p.boundaryField()[patchi];
			fvPatchScalarField& pT = T.boundaryField()[patchi];
			
			fvPatchScalarField& pmw = MWmix.boundaryField()[patchi];
			fvPatchScalarField& pcp = cp.boundaryField()[patchi];
			fvPatchScalarField& ph = h.boundaryField()[patchi];
			fvPatchScalarField& pmu = mu.boundaryField()[patchi];
			//fvPatchScalarField& palpha = this->alpha_.boundaryField()[patchi];
			fvPatchScalarField& plambda = lambda.boundaryField()[patchi];
			//fvPatchScalarField& pq = this->Q_.boundaryField()[patchi];

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
					//ppsi[facei] = rho / pp[facei];

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
						//palpha[facei] = plambda[facei]/pcp[facei];
						
						// Dynamic viscosity [kg/m/s]
						transportMapXML->DynamicViscosity(pmu[facei], x);
						
						// Mass diffusion coefficients [m2/s]
						transportMapXML->MassDiffusionCoefficients(gammamix, x);
						for(unsigned int i=1;i<=Y.size();i++)
							Dmix_[i-1].boundaryField()[patchi][facei] = gammamix[i];	
					}	
					
					// Calculate formation rates
					/*
					if (calculate_reactions == true && homogeneous_reactions_ == true)
					{
						Product(cTot,x,&c);
						
						kineticsMapXML->SetPressure(pp[facei]);
						kineticsMapXML->SetTemperature(pT[facei]);
						
						kineticsMapXML->ReactionEnthalpiesAndEntropies();
						kineticsMapXML->ArrheniusKineticConstants();
						kineticsMapXML->ReactionRates(c);
						kineticsMapXML->FormationRates(&R);
						pq[facei] = kineticsMapXML->HeatRelease(R);
						ElementByElementProduct(R,thermodynamicsMapXML->MW(),&R);
						
						for(unsigned int i=1;i<=Y.size();i++)
							RHomo_[i-1].boundaryField()[patchi][facei] = R[i];
					}	
					*/						
				}
			}
		}
	}
