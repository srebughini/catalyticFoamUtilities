PtrList<volVectorField>     UFluid(fluidRegions.size());
PtrList<volScalarField>     pFluid(fluidRegions.size());
PtrList<volScalarField>     TFluid(fluidRegions.size());
PtrList<volScalarField>     MWmixFluid(fluidRegions.size());
PtrList<volScalarField>     psiFluid(fluidRegions.size());
PtrList<volScalarField>     muFluid(fluidRegions.size());
PtrList<volScalarField>     lambdaFluid(fluidRegions.size());
PtrList<volScalarField>     cpFluid(fluidRegions.size());
PtrList<volScalarField>     rhoFluid(fluidRegions.size());
PtrList<volScalarField>     sourceImplicitTempFluid(fluidRegions.size());
PtrList<volScalarField>     sourceExplicitTempFluid(fluidRegions.size());
PtrList<surfaceScalarField> phiFluid(fluidRegions.size());

PtrList<dimensionedScalar>  rhoMax(fluidRegions.size());
PtrList<dimensionedScalar>  rhoMin(fluidRegions.size());

PtrList<volScalarField>     YFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     XFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     DmixFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     rDFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     MWFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     sourceImplicitSpecieFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField>     sourceExplicitSpecieFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());

PtrList<surfaceScalarField> JFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<surfaceScalarField> JcFluid(fluidRegions.size());
PtrList<volScalarField>     sumDiffusionCorrectionsFluid(fluidRegions.size());

List<scalar>                initialMassFluid(fluidRegions.size());
List<label>                 pRefCellFluid(fluidRegions.size(), 0);
List<scalar>                pRefValueFluid(fluidRegions.size(), 0.0);

forAll(fluidRegions, j)
{
    Info<< "Reading region " << fluidRegions[j].name() << "\n" << endl;

    Info<< "Reading field U\n" << endl;
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
        
    Info<< "Reading field p\n" << endl;
    pFluid.set
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
        
    Info<< "Reading field T\n" << endl;
    TFluid.set
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

    MWmixFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "MW",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionSet(1, 0, 0, 0, -1)
        )
    );
        
    psiFluid.set
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

    muFluid.set
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
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0), 1.8e-5)
        )
    );

    lambdaFluid.set
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

    cpFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "cp",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionSet(0, 2, -2, -1, 0)
        )
    );

    JcFluid.set
    (
        j,
        new surfaceScalarField
        (
            IOobject
            (
				"Jc",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("J::Jc", dimensionSet(1, 0, -1, 0, 0), 0.0)
        )
    );

    sumDiffusionCorrectionsFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
				"sumCorrections",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionedScalar("zero", dimensionSet(1, -3, -1, 0, 0), 0.0)
        )
    );

    sourceImplicitTempFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
				"SI::T",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            fluidRegions[j],
			dimensionedScalar("SI::T", dimensionSet(1, -1, -3, -1, 0), 0.0)
        )
    );

    sourceExplicitTempFluid.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
				"SI::T",
				runTime.timeName(),
				fluidRegions[j],
				IOobject::NO_READ,
				IOobject::NO_WRITE
            ),
            fluidRegions[j],
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
            fluidRegions[j],
            IOobject::NO_READ
        );
        
        Info << thermodynamicsMapXML->NamesOfSpecies()[i] << endl;
        YFluid.set
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

        DmixFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "Dmix::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(0, 2, -1, 0, 0),1.8e-05)
            )
        );

        rDFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "rD::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0),1.8e-06)
            )
        );

        XFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "X::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(0, 0, 0, 0, 0),0.0)
            )
        );

        MWFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "MW::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, 0, 0, 0, -1),scalar(thermodynamicsMapXML->MW()[i+1]))
            )
        );

        JFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new surfaceScalarField
            (
                IOobject
                (
                    "J::"+ thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("zero", dimensionSet(1, 0, -1, 0, 0),0.0)
            )
        );

        sourceImplicitSpecieFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
					"SI::" + thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
                ),
                fluidRegions[j],
                dimensionedScalar("SI", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );

        sourceExplicitSpecieFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
					"SE::" + thermodynamicsMapXML->NamesOfSpecies()[i],
                    runTime.timeName(),
					fluidRegions[j],
					IOobject::NO_READ,
					IOobject::NO_WRITE
                ),
                fluidRegions[j],
				dimensionedScalar("SE", dimensionSet(1, -3, -1, 0, 0), 0.0)
            )
        );

    }

    const dictionary& simpleDict = fluidRegions[j].solutionDict().subDict("SIMPLE");

    setRefCell
    (
        pFluid[j],
        simpleDict,
        pRefCellFluid[j],
        pRefValueFluid[j]
    );

    rhoMax.set(j, new dimensionedScalar(simpleDict.lookup("rhoMax")));
    rhoMin.set(j, new dimensionedScalar(simpleDict.lookup("rhoMin")));

}

Info << endl;
