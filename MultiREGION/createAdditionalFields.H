Info << endl;
Info<< " * Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Dmix(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> HSpecies(thermodynamicsMapXML->NumberOfSpecies());
PtrList<volScalarField> Ygrad(thermodynamicsMapXML->NumberOfSpecies());


for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
{

	Info << "   Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

	IOobject header
	(
		thermodynamicsMapXML->NamesOfSpecies()[i],
		fluidRegions[j].time().timeName(),
		fluidRegions[j],
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
					fluidRegions[j].time().timeName(),
					fluidRegions[j],
					IOobject::MUST_READ,
					IOobject::AUTO_WRITE
				),
				fluidRegions[j]
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
					fluidRegions[j].time().timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				fluidRegions[j],
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
				fluidRegions[j].time().timeName(),
				fluidRegions[j],
				IOobject::MUST_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j]
		);

		Y.set
		(
			i,
			new volScalarField
			(
				IOobject
				(
					thermodynamicsMapXML->NamesOfSpecies()[i],
					fluidRegions[j].time().timeName(),
					fluidRegions[j],
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
					fluidRegions[j].time().timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				fluidRegions[j],
				dimensionSet(0, 0, -1, 0, 0)
			)
		);
	}

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
					fluidRegions[j].time().timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				fluidRegions[j],
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
				fluidRegions[j].time().timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
				),
				fluidRegions[j],
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

// Create fluxes fields
//#include "createFluxes.H"

//- Creating element fields
std::vector<std::string> elementNames_;
elementNames_ = thermodynamicsMapXML->elements();
PtrList<volScalarField> element_;
element_.resize(elementNames_.size());

for (unsigned int i=0;i<elementNames_.size();i++)
{
	element_.set
	(
		i,
		new volScalarField
		(
			IOobject
			(
			"element_"+ elementNames_[i],
			fluidRegions[j].time().timeName(),
			fluidRegions[j],
			IOobject::NO_READ,
			IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 0, 0, 0, 0)
		)
	);
}
