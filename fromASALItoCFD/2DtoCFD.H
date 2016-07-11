Info << endl;

forAll(fluidRegions, j)
{
    Info << "ASALI --> CFD for region: ";
    #include "setRegionFluidFields.H"

    const volVectorField& centers          = mesh.C();

    std::vector<double> dzVector(Na);
    std::vector<double> drVector(Nr);
    double minZ;
    double minR;
    forAll(T.internalField(), celli)
    {
        for (unsigned int i=0;i<Na;i++)
        {
            dzVector[i] = std::fabs(centers[celli][directionIndex] - zasali[i]);
        }

        minZ = dzVector[0];
        for (unsigned int i=1;i<Na;i++)
        {
            minZ = std::min(dzVector[i],minZ);
        }

        for (unsigned int i=0;i<Na;i++)
        {
            if ( std::fabs(centers[celli][directionIndex] - zasali[i]) == minZ)
            {
                for (unsigned int k=0;k<Nr;k++)
                {
                    drVector[k] = std::fabs(std::pow(centers[celli][radialIndex1],2.) + std::pow(centers[celli][radialIndex2],2.) - std::pow(rasali[k],2.));
                }

                minR = dzVector[0];
                for (unsigned int k=1;k<Nr;k++)
                {
                    minR = std::min(drVector[k],minR);
                }
                
                for (unsigned int k=0;k<Nr;k++)
                {
                    double dist = std::fabs(std::pow(centers[celli][radialIndex1],2.) + std::pow(centers[celli][radialIndex2],2.) - std::pow(rasali[k],2.));

                    if ( dist == minR)
                    {
                        if ( energyFluid[j] == true )
                        {
                            T.internalField()[celli] = Tasali[i][k];
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    if ( energyFluid[j] == true )
    {
        T.write();
    }
    Info << fluidRegions[j].name() << nl << endl;
}


forAll(solidRegions, j)
{
    Info << "\nASALI --> CFD for region: ";
    #include "setRegionSolidFields.H"

    const volVectorField& centers          = mesh.C();

    std::vector<double> dzVector(Na);
    std::vector<double> drVector(Nr);
    double minZ;
    double minR;
    forAll(T.internalField(), celli)
    {
        for (unsigned int i=0;i<Na;i++)
        {
            dzVector[i] = std::fabs(centers[celli][directionIndex] - zasali[i]);
        }

        minZ = dzVector[0];
        for (unsigned int i=1;i<Na;i++)
        {
            minZ = std::min(dzVector[i],minZ);
        }

        for (unsigned int i=0;i<Na;i++)
        {
            if ( std::fabs(centers[celli][directionIndex] - zasali[i]) == minZ)
            {
                for (unsigned int k=0;k<Nr;k++)
                {
                    drVector[k] = std::fabs(std::pow(centers[celli][radialIndex1],2.) + std::pow(centers[celli][radialIndex2],2.) - std::pow(rasali[k],2.));
                }

                minR = dzVector[0];
                for (unsigned int k=1;k<Nr;k++)
                {
                    minR = std::min(drVector[k],minR);
                }
                
                for (unsigned int k=0;k<Nr;k++)
                {
                    double dist = std::fabs(std::pow(centers[celli][radialIndex1],2.) + std::pow(centers[celli][radialIndex2],2.) - std::pow(rasali[k],2.));

                    if ( dist == minR)
                    {
                        if ( energySolid[j] == true )
                        {
                            T.internalField()[celli] = Tasali[i][k];
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    if ( energySolid[j] == true )
    {
        T.write();
    }

    Info << solidRegions[j].name() << nl << endl;
}
