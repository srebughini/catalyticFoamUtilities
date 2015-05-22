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
