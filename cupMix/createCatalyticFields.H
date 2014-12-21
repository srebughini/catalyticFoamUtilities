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
|                     2013, G.Gentile, F.Manelli                          |
|                     2012, M.Calonaci, F.Furnari                         |
|                     2011, S.Goisis, A.Osio                              |
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

	bool catalyst = false;

	forAll(Y[0].boundaryField(), patchi)
    	if (Y[0].boundaryField()[patchi].type() == "catalyticWall")
        {
			catalyst = true;
			break;
		}

	if (catalyst == false)
	{
		Info << "WARNING: Catalyst not found!" << endl;        
		getchar();
	}

	//- Reading adsorbed species
	Info << endl;	
	Info << " * Reading Surface Species:"<<endl;
	PtrList<volScalarField> siteFraction(surfaceThermodynamicsMapXML->number_of_site_species());

	for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
	{
		std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
		Info << "   Reading adsorbed specie: " << name << " > ";
	
		IOobject header
		(
			name,
			mesh.time().timeName(),
			mesh,
			IOobject::NO_READ
		);

		// check if field exists and can be read
		if (header.headerOk())
		{
			Info << name << " found at time " << mesh.time().timeName() << endl;
				
			siteFraction.set
			(
				i,
				new volScalarField
				(
					IOobject
					(
						name,
						mesh.time().timeName(),
						mesh,
						IOobject::MUST_READ,
						IOobject::AUTO_WRITE
					),
					mesh
				)
			);
		}
		else
		{
			volScalarField thetaDefault
			(
				IOobject
				(
					"thetaDefault",
					mesh.time().timeName(),
					mesh,
					IOobject::MUST_READ,
					IOobject::NO_WRITE
				),
				mesh
			);

			siteFraction.set
			(
				i,
				new volScalarField
				(
					IOobject
					(
						name,
						mesh.time().timeName(),
						mesh,
						IOobject::NO_READ,
						IOobject::AUTO_WRITE
					),
					thetaDefault
				)
			);

			if(i == 0)
			{
				forAll(siteFraction[i].boundaryField(), patchi)
				{
					if (Y[i].boundaryField()[patchi].type() == "catalyticWall")
					{
						forAll(siteFraction[i].boundaryField()[patchi], facei)
							siteFraction[i].boundaryField()[patchi][facei] = 1.;
					}
					else
					{
						forAll(siteFraction[i].boundaryField()[patchi], facei)
							siteFraction[i].boundaryField()[patchi][facei] = 0.;
					}
				}

				Info << "Catalyst's material " <<endl;
			}
			else
			{
				Info << "specie not adsorbed" << endl;
			}
		}
	}
