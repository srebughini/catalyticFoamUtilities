PtrList<volScalarField>     TSolid(solidRegions.size());
PtrList<volScalarField>     lambdaSolid(solidRegions.size());
PtrList<volScalarField>     cpSolid(solidRegions.size());
PtrList<volScalarField>     rhoSolid(solidRegions.size());
PtrList<volScalarField>     pSolid(solidRegions.size());
PtrList<volScalarField>     MWmixSolid(solidRegions.size());
PtrList<volScalarField>     psiSolid(solidRegions.size());
PtrList<volScalarField>     muSolid(solidRegions.size());
PtrList<volScalarField>     sourceImplicitTempSolid(solidRegions.size());
PtrList<volScalarField>     sourceExplicitTempSolid(solidRegions.size());

PtrList<volScalarField>     lambdaMaterial(solidRegions.size());
PtrList<volScalarField>     cpMaterial(solidRegions.size());
PtrList<volScalarField>     rhoMaterial(solidRegions.size());

PtrList<volScalarField>     YSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volScalarField>     RSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volScalarField>     XSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volSymmTensorField> DmixSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volSymmTensorField> rDSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volScalarField>     MWSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volScalarField>     sourceImplicitSpecieSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<volScalarField>     sourceExplicitSpecieSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());

PtrList<volScalarField>     ZSolid(surfaceThermodynamicsMapXML->number_of_site_species()*solidRegions.size());

PtrList<surfaceScalarField> JSolid(thermodynamicsMapXML->NumberOfSpecies()*solidRegions.size());
PtrList<surfaceScalarField> JcSolid(solidRegions.size());
PtrList<volScalarField>     sumDiffusionCorrectionsSolid(solidRegions.size());

forAll(solidRegions, j)
{
    Info<< "Reading region " << solidRegions[j].name() << "\n" << endl;   

    rhoMaterial.set
    (   
        j,
        new volScalarField
        (
            IOobject
            (
                "rho",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
			solidRegions[j],
			dimensionedScalar("rho", dimensionSet(1, -3, 0, 0, 0),rhoM[j])
        )
    ); 

    rhoSolid.set
    (   j,
        new volScalarField
        (
            IOobject
            (
                "gas::rho",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[j],
			dimensionedScalar("gas::rho", dimensionSet(1, -3, 0, 0, 0),0.0)
        )
    );

    Info<< "Reading field T\n" << endl;
    TSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "T",
                runTime.timeName(),
                solidRegions[j],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[j]
        )
    );     

    MWmixSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "MW",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionSet(1, 0, 0, 0, -1)
        )
    );
        
    psiSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "psi",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionSet(0, -2, 2, 0, 0)
        )
    ); 
        
    muSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "mu",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionSet(1, -1, -1, 0, 0)
        )
    );


    Info<< "Reading field p\n" << endl;
    pSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "p",
                runTime.timeName(),
                solidRegions[j],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            solidRegions[j]
        )
    );     


    lambdaMaterial.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "K",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionedScalar("K", dimensionSet(1, 1, -3, -1, 0),lambdaM[j])
        )
    );

    lambdaSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "gas::K",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionSet(1, 1, -3, -1, 0)
        )
    );


    cpMaterial.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "cp",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionedScalar("cp", dimensionSet(0, 2, -2, -1, 0),cpM[j])
        )
    );

    cpSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "gas::cp",
                runTime.timeName(),
                solidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionSet(0, 2, -2, -1, 0)
        )
    );

    JcSolid.set
    (
        j,
        new surfaceScalarField
        (
            IOobject
            (
				"Jc",
				runTime.timeName(),
				solidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionedScalar("J::Jc", dimensionSet(1, 0, -1, 0, 0), 0.0)
        )
    );

    sumDiffusionCorrectionsSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
				"sumCorrections",
				runTime.timeName(),
				solidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            solidRegions[j],
            dimensionedScalar("zero", dimensionSet(1, -3, -1, 0, 0), 0.0)
        )
    );

    sourceImplicitTempSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
				"SI::T",
				runTime.timeName(),
				solidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            solidRegions[j],
			dimensionedScalar("SI::T", dimensionSet(1, -1, -3, -1, 0), 0.0)
        )
    );

    sourceExplicitTempSolid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
				"SI::T",
				runTime.timeName(),
				solidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            solidRegions[j],
			dimensionedScalar("SE::T", dimensionSet(1, -1, -3, 0, 0), 0.0)
        )
    );


    for(unsigned int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        Info << "Reading species: " << thermodynamicsMapXML->NamesOfSpecies()[i] << " > ";
        IOobject header
        (
            thermodynamicsMapXML->NamesOfSpecies()[i],
            runTime.timeName(),
            solidRegions[j],
            IOobject::NO_READ
        );
        
        Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
        YSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[j]
            )
        );

        DmixSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volSymmTensorField
            (
                IOobject
                (
                    "Dmix::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j],
                dimensionedSymmTensor("zero", dimensionSet(0, 2, -1, 0, 0),1.8e-05*symmTensor::I)
            )
        );

        rDSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volSymmTensorField
            (
                IOobject
                (
                    "rD::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j],
                dimensionedSymmTensor("zero", dimensionSet(1, -1, -1, 0, 0),1.8e-05*symmTensor::I)
            )
        );

        XSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "X::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j],
                dimensionedScalar("zero", dimensionSet(0, 0, 0, 0, 0),0.0)
            )
        );

        MWSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "MW::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, 0, 0, 0, -1),scalar(thermodynamicsMapXML->MW()[i+1]))
            )
        );
 
        JSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new surfaceScalarField
            (
                IOobject
                (
                    "J::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, 0, -1, 0, 0),0.0)
            )
        );

        RSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "R::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );

        sourceImplicitSpecieSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
					"SI::" + thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
					solidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
                ),
                solidRegions[j],
                dimensionedScalar("SI", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );

        sourceExplicitSpecieSolid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
					"SE::" + thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
					solidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
                ),
                solidRegions[j],
				dimensionedScalar("SE", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );
 

    }

	Info << endl;

    for(int i=0;i<surfaceThermodynamicsMapXML->number_of_site_species();i++)
    {
        std::string name = surfaceThermodynamicsMapXML->NamesOfSpecies()[i+surfaceThermodynamicsMapXML->number_of_gas_species()];
        Info << "Reading adsorbed species: " << name << " > ";
        IOobject header
        (
            name,
            runTime.timeName(),
            solidRegions[j],
            IOobject::NO_READ
        );

        if (header.headerOk())
        {
            Info << name << endl;
            ZSolid.set
            (
                j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
                new volScalarField
                (
                    IOobject
                    (
                        name,
                        runTime.timeName(),
                        solidRegions[j],
                        IOobject::MUST_READ,
                        IOobject::AUTO_WRITE
                    ),
                    solidRegions[j]
                )
            );
        }
        else
        {
			Info << "thetaDefault" << endl;
            volScalarField thetaDefault
            (
                IOobject
                (
                    "thetaDefault",
                    runTime.timeName(),
                    solidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                solidRegions[j]
            );

            ZSolid.set
            (
                j*surfaceThermodynamicsMapXML->number_of_site_species()+i,
                new volScalarField
                (
                    IOobject
                    (
                        name,
                        runTime.timeName(),
                        solidRegions[j],
                        IOobject::NO_READ,
                        IOobject::AUTO_WRITE
                    ),
                    thetaDefault
                )
            );
        }
    }
}

Info << endl;
