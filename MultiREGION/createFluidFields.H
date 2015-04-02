// Initialise fluid field pointer lists
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<volScalarField> pFluid(fluidRegions.size());
PtrList<volScalarField> TFluid(fluidRegions.size());
PtrList<volScalarField> TFluid_Boundary(fluidRegions.size());  
PtrList<volScalarField> TFluid_Initial(fluidRegions.size()); 
PtrList<volScalarField> MWmixFluid(fluidRegions.size());
PtrList<volScalarField> cTotFluid(fluidRegions.size());
PtrList<volScalarField> psiFluid(fluidRegions.size());
PtrList<volScalarField> muFluid(fluidRegions.size());
PtrList<volScalarField> lambdaFluid(fluidRegions.size());
PtrList<volScalarField> cpFluid(fluidRegions.size());
PtrList<volScalarField> hFluid(fluidRegions.size());
PtrList<volScalarField> QradFluid(fluidRegions.size());
PtrList<volScalarField> QsurfFluid(fluidRegions.size());
PtrList<volScalarField> sumFluidDiffusionCorrections(fluidRegions.size());
PtrList<volScalarField> sumFluidCpSpecies(fluidRegions.size());

PtrList<volScalarField> rhoFluid(fluidRegions.size());

PtrList<fv::IOoptionList> fluidFvOptions(fluidRegions.size());

PtrList<volScalarField> UFluid_x(fluidRegions.size());
PtrList<volScalarField> UFluid_y(fluidRegions.size());
PtrList<volScalarField> UFluid_z(fluidRegions.size());

List<bool> chemistryEquationFluid(fluidRegions.size());

// Populate fluid field pointer lists
forAll(fluidRegions, j)
{
	Info<< "*** Reading and allocating fluid mesh thermophysical properties for region "
		<< fluidRegions[j].name() << nl << endl;

	Info<< "    Reading field UFluid\n" << endl;
	UFluid.set   // speed [m/s]
	(
		j,
		new volVectorField
		(
			IOobject
			(
				"U",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j]
		)
	);
        
	Info<< "    Reading field pFluid\n" << endl;
	pFluid.set   // pressure [Pa]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"p",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j]
		)
	);
        
	Info<< "    Reading field TFluid\n" << endl;
	TFluid.set   // temperature [K]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"T",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::MUST_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j]
		)
	);    
        
	Info<< "    Reading field TFluid_Boundary\n" << endl;
	TFluid_Boundary.set   // temperature [K]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"T.boundary",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 0, 0, 1, 0)
		)
	);
        
	Info<< "    Reading field TFluid_Init\n" << endl;
	TFluid_Initial.set   // temperature [K]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"T.init",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 0, 0, 1, 0)
			
			
		)
	);   
        
	Info<< "    Creating field MWmixFluid\n" << endl;
	MWmixFluid.set   // mean molecular weight [kg/mol]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"meanMolecularWeight",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, 0, 0, 0, -1)
		)
	);
        
	Info<< "    Creating field cTotFluid\n" << endl;
	cTotFluid.set   // total concentration [mol/m3]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"cTot",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, -3, 0, 0, 1)
		)
	);    
        
	Info<< "    Creating field psiFluid\n" << endl;
	psiFluid.set   // p/rho [s2/m2]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"psi",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, -2, 2, 0, 0)
		)
	); 
        
	Info<< "    Creating field muFluid\n" << endl;
	muFluid.set   // dynamic viscosity [kg/m/s]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"mu",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, -1, -1, 0, 0)
		)
	);
        
	Info<< "    Creating field lambdaFluid\n" << endl;
	lambdaFluid.set   // thermal conductivity [W/m/K]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"K",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, 1, -3, -1, 0)
		)
	);
        
	Info<< "    Creating field cpFluid\n" << endl;
	cpFluid.set   // specific heat [J/kg/K]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"specificHeats",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 2, -2, -1, 0)
		)
	);
        
	Info<< "    Creating field hFluid\n" << endl;
	hFluid.set   // total enthalpy [m2/s2]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"enthalpy",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 2, -2, 0, 0)
		)
	); 
        
	Info<< "    Creating field QradFluid\n" << endl;
	QradFluid.set   // radiative heat transfer term [W/m3]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"Qrad",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, -1, -3, 0, 0)
		)
	); 
        
	Info<< "    Creating field QsurfFluid\n" << endl;
	QsurfFluid.set   // surface heat flux [W/m2]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"Qsurf",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, 2, -3, 0, 0)
		)
	);
        
	Info << "Creating sumFluidCorrections\n" << endl;
	sumFluidDiffusionCorrections.set
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"sumFluidCorrections",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, -3, -1, 0, 0)
		)
	);
		
	Info<< "Creating sumFluidCpSpecies\n" << endl;
	sumFluidCpSpecies.set
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"sumFluidCp",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			),
			fluidRegions[j],
			dimensionSet(1, -1, -3, 0, 0)
		)
	);                                                                                                           
        
	Info<< "Creating UFluid_x\n" << endl;
	UFluid_x.set   // speed [m/s]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"ux",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 1, -1, 0, 0)
		)
	);	

	Info<< "Creating UFluid_y\n" << endl;
	UFluid_y.set   // speed [m/s]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"uy",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 1, -1, 0, 0)
		)
	);	
	
	Info<< "Creating UFluid_z\n" << endl;
	UFluid_z.set   // speed [m/s]
	(
		j,
		new volScalarField
		(
			IOobject
			(
				"uz",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::AUTO_WRITE
			),
			fluidRegions[j],
			dimensionSet(0, 1, -1, 0, 0)
		)
	);	

	UFluid_x[j]*=0.;
	UFluid_y[j]*=0.;
	UFluid_z[j]*=0.;
	       
	Info<< "    Adding fvOptions\n" << endl;
	fluidFvOptions.set
	(
		j,
		new fv::IOoptionList(fluidRegions[j])
	);				
}
	
Info << endl;
Info<< " * Reading chemical species mole fractions or mass fractions..." << endl;

PtrList<volScalarField> Y(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> Dmix(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> HFluidSpecies(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> Dsoret(transportMapXML->iThermalDiffusionRatios().size()*fluidRegions.size());
PtrList<volScalarField> Yboundary(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> Y_Initial(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> CpSpecies(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());

forAll (fluidRegions,j)
{   
	Info<< "*** Reading and allocating composition and diffusion properties for region "
            << fluidRegions[j].name() << nl << endl;

	for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
	{

		Info << "   Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";

		IOobject header
		(
			thermodynamicsMapXML->NamesOfSpecies()[i],
			runTime.timeName(),
			fluidRegions[j],
			IOobject::NO_READ
		);
		
		// check if field exists and can be read
		if (header.headerOk())
		{
			Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
			Y.set
			(
				j*thermodynamicsMapXML->NumberOfSpecies()+i,
				new volScalarField
				(
					IOobject
					(
						thermodynamicsMapXML->NamesOfSpecies()[i],
						runTime.timeName(),
						fluidRegions[j],
						IOobject::MUST_READ,
						IOobject::AUTO_WRITE
					),
					fluidRegions[j]
				)
			);
			
			Yboundary.set
			(
				j*thermodynamicsMapXML->NumberOfSpecies()+i,
				new volScalarField
				(
					IOobject
					(
						thermodynamicsMapXML->NamesOfSpecies()[i]+".boundary",
						runTime.timeName(),
						fluidRegions[j],
						IOobject::NO_READ,
						IOobject::NO_WRITE
					),
					fluidRegions[j],
					dimensionSet(0, 0, 0, 0, 0)
				)
			);
			
			Y_Initial.set
			(
				j*thermodynamicsMapXML->NumberOfSpecies()+i,
				new volScalarField
				(
					IOobject
					(
						thermodynamicsMapXML->NamesOfSpecies()[i]+".init",
						runTime.timeName(),
						fluidRegions[j],
						IOobject::NO_READ,
						IOobject::NO_WRITE
					),
					fluidRegions[j],
					dimensionSet(0, 0, 0, 0, 0)
				)
			);
		}
		
		else
		{
			Info << "Species  " << thermodynamicsMapXML->NamesOfSpecies()[i] << 
			"not present at time  " << runTime.timeName() << endl;
			abort();
		}
		
		Dmix.set
		(
			j*thermodynamicsMapXML->NumberOfSpecies()+i,
			new volScalarField
			(
				IOobject
				(
					"Dmix_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
					runTime.timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				fluidRegions[j],
				dimensionSet(0, 2, -1, 0, 0)
			)
		);	

		HFluidSpecies.set
		(
			j*thermodynamicsMapXML->NumberOfSpecies()+i,
			new volScalarField
			(
				IOobject
				(
					"gas::H_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
					runTime.timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
				),
				fluidRegions[j],
				dimensionSet(0, 2, -2, 0, 0)
			)
		);	
		
		CpSpecies.set
		(
			j*thermodynamicsMapXML->NumberOfSpecies()+i,
			new volScalarField
			(
			    IOobject
			    (
				"CpSpecies_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
			    ),
			    fluidRegions[j],
			    dimensionSet(0, 2, -2, -1, 0)
			)
	    );
	}
	
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
				j*thermodynamicsMapXML->NumberOfSpecies()+i,
				new volScalarField
				(
					IOobject
					(
						"gas::Dsoret_"+ thermodynamicsMapXML->NamesOfSpecies()[index-1],
						runTime.timeName(),
						fluidRegions[j],
						IOobject::NO_READ,
						IOobject::AUTO_WRITE
					),
					fluidRegions[j],
					dimensionSet(0, 2, -1, 0, 0)
				)
			);
		}
	}
}

// Create fluxes fields
#include "createFluxes.H"

