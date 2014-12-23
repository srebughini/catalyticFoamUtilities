Info << endl;
Info<< " * Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Dmix(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> HSpecies(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Ygrad(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> x(thermodynamicsMapXML->NumberOfSpecies());

for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{

	Info << "   Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

	IOobject header
	(
		thermodynamicsMapXML->NamesOfSpecies()[i],
		mesh.time().timeName(),
		mesh,
		IOobject::NO_READ
	);

	// check if field exists and can be read
	if (header.headerOk())
	{
		Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
		Y.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					thermodynamicsMapXML->NamesOfSpecies()[i],
					mesh.time().timeName(),
					mesh,
					IOobject::MUST_READ,
					IOobject::AUTO_WRITE
				),
				mesh
			)
		);

		Ygrad.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					"grad_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				mesh,
				dimensionSet(0, -1, 0, 0, 0)
			)
		);
	}
	else
	{
		Info << "Ydefault" << endl;

		volScalarField Ydefault
		(
			IOobject
			(
				"Ydefault",
				mesh.time().timeName(),
				mesh,
				IOobject::MUST_READ,
				IOobject::NO_WRITE
			),
			mesh
		);

		Y.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					thermodynamicsMapXML->NamesOfSpecies()[i],
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::AUTO_WRITE
				),
				Ydefault
			)
		);

		Ygrad.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					"grad_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				mesh,
				dimensionSet(0, 0, -1, 0, 0)
			)
		);
	}

	x.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
				"mole_"+thermodynamicsMapXML->NamesOfSpecies()[i],
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			mesh,
			dimensionSet(0, 0, 0, 0, 0)
		)
	);

	// Sets of diffusivities and specific heats of species
	{
		Dmix.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					"gas::Dmix_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				mesh,
				dimensionSet(0, 2, -1, 0, 0)
			)
		);

		HSpecies.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
				"gas::H_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
				mesh.time().timeName(),
				mesh,
				IOobject::NO_READ,
				IOobject::NO_WRITE
				),
				mesh,
				dimensionSet(0, 2, -2, 0, 0)
			)
		);
	}

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
//	Viscosity and mass diffusion coefficients initialization
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
// The fields of viscosity and mass diffusion coefficients are initialized according to
// the values suggested by the user

mu = dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0), 1.8e-5);
for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
	Dmix[i] = dimensionedScalar("zero", dimensionSet(0, 2, -1, 0, 0), 1.8e-5);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//

// Soret effect
PtrList<volScalarField> Dsoret(transportMapXML->iThermalDiffusionRatios().size());
if (soretEffect == true)
{
	for(unsigned int i=0;i<transportMapXML->iThermalDiffusionRatios().size();i++)
	{ 
		unsigned int index = transportMapXML->iThermalDiffusionRatios()[i];
		indexSoret[index-1] = i;
		soretEffectList[index-1] = true;

		std::cout << "Soret effect: species " 	<< i+1 << "/" << transportMapXML->iThermalDiffusionRatios().size() 
							<< " " << thermodynamicsMapXML->NamesOfSpecies()[index-1] << " (" << index << ")" << std::endl;

		Dsoret.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					"gas::Dsoret_"+ thermodynamicsMapXML->NamesOfSpecies()[index-1],
					mesh.time().timeName(),
					mesh,
					IOobject::NO_READ,
					IOobject::AUTO_WRITE
				),
				mesh,
				dimensionSet(0, 2, -1, 0, 0)
			)
		);
	}
}

// Create fluxes fields
#include "createFluxes.H"

