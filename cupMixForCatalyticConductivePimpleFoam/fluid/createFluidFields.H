// Initialise fluid field pointer lists
PtrList<volVectorField> UFluid(fluidRegions.size());
PtrList<volScalarField> pFluid(fluidRegions.size());
PtrList<volScalarField> TFluid(fluidRegions.size());
PtrList<volScalarField> MWmixFluid(fluidRegions.size());
PtrList<volScalarField> cTotFluid(fluidRegions.size());
PtrList<volScalarField> psiFluid(fluidRegions.size());
PtrList<volScalarField> muFluid(fluidRegions.size());
PtrList<volScalarField> lambdaFluid(fluidRegions.size());
PtrList<volScalarField> cpFluid(fluidRegions.size());
PtrList<volScalarField> massFluidDiffusionInEnergyEquation(fluidRegions.size());
PtrList<volScalarField> rhoFluid(fluidRegions.size());

PtrList<fv::IOoptionList> fluidFvOptions(fluidRegions.size());

List<bool> chemistryEquationFluid(fluidRegions.size());

// Populate fluid field pointer lists
forAll(fluidRegions, j)
{
    Info<< "*** Reading region " << fluidRegions[j].name() << "\n" << endl;

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
        
    Info<< "Reading field T\n" << endl;
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

    Info<< "Creating field MW\n" << endl;
    MWmixFluid.set
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
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionSet(1, 0, 0, 0, -1)
        )
    );
        
    Info<< "Creating field cTot\n" << endl;
    cTotFluid.set
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
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionSet(0, -3, 0, 0, 1)
        )
    );    
        
    Info<< "Creating field psi\n" << endl;
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
        
    Info<< "Creating field mu\n" << endl;
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
            dimensionSet(1, -1, -1, 0, 0)
        )
    );

    Info<< "Creating field lambda\n" << endl;
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

    Info<< "Creating field cp\n" << endl;
    cpFluid.set
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

    Info<< "Creating massDiffusionInEnergyEquation\n" << endl;
    massFluidDiffusionInEnergyEquation.set
    (
        j,
        new volScalarField
        (
            IOobject
            (
                "gas::massFluidDiffusionInEnergyEquation",
                runTime.timeName(),
                fluidRegions[j],
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            fluidRegions[j],
            dimensionSet(1, -1, -3, 0, 0)
        )
    );

    massFluidDiffusionInEnergyEquation[j] = dimensionedScalar("zero", dimensionSet(1, -1, -3, 0, 0), 0.);

    Info<< "Adding fvOptions\n" << endl;
    fluidFvOptions.set
    (
        j,
        new fv::IOoptionList(fluidRegions[j])
    );
}

PtrList<volScalarField> YFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> DmixFluid(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());
PtrList<volScalarField> HFluidSpecies(thermodynamicsMapXML->NumberOfSpecies()*fluidRegions.size());

forAll (fluidRegions,j)
{
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
        
        // check if field exists and can be read
        if (header.headerOk())
        {
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
        }
        else
        {
            Info << "Ydefault" << endl;

            volScalarField Ydefault
            (
                IOobject
                (
                    "Ydefault",
                    runTime.timeName(),
                    fluidRegions[j],
                    IOobject::MUST_READ,
                    IOobject::NO_WRITE
                ),
                fluidRegions[j]
            );

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
                        IOobject::NO_READ,
                        IOobject::AUTO_WRITE
                    ),
                    Ydefault
                )
            );

        }

        DmixFluid.set
        (
            j*thermodynamicsMapXML->NumberOfSpecies()+i,
            new volScalarField
            (
                IOobject
                (
                    "gas::Dmix_"+ thermodynamicsMapXML->NamesOfSpecies()[i],
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
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
    //    Viscosity and mass diffusion coefficients initialization
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
    // The fields of viscosity and mass diffusion coefficients are initialized according to
    // the values suggested by the user

    muFluid [j] = dimensionedScalar("zero", dimensionSet(1, -1, -1, 0, 0), 1.8e-5);

    for(int i=j*thermodynamicsMapXML->NumberOfSpecies();i<thermodynamicsMapXML->NumberOfSpecies()*(j+1);i++)
    {
        DmixFluid[i] = dimensionedScalar("zero", dimensionSet(0, 2, -1, 0, 0), 1.8e-5);
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *//
}

#include "createFluidFluxes.H"

Info << endl;
