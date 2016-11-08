//- Internal Field
Info << "ASALI --> CFD ";
{
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
                        if ( energy == true )
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
}
Info << "internal fields" << nl << endl;

Info << "ASALI --> CFD ";
{
    forAll(T.boundaryField(), patchi)
    {
        if ( Y[0].boundaryField()[patchi].type() == "compressible::catalyticWall" )
        {
            const fvsPatchVectorField& centers          = mesh.Cf().boundaryField()[patchi];
			std::vector<double> dzVector(Na);
			std::vector<double> drVector(Nr);
			double minZ;
			double minR;

            forAll(T.boundaryField()[patchi], facei)
            {
                for (unsigned int i=0;i<Na;i++)
                {
                    dzVector[i] = std::fabs(centers[facei][directionIndex] - zasali[i]);
                }

                minZ = dzVector[0];
                for (unsigned int i=1;i<Na;i++)
                {
                    minZ = std::min(dzVector[i],minZ);
                }

                for (unsigned int i=0;i<Na;i++)
                {
                    if ( std::fabs(centers[facei][directionIndex] - zasali[i]) == minZ)
                    {
                        for (unsigned int k=0;k<Nr;k++)
                        {
                            drVector[k] = std::fabs(std::pow(centers[facei][radialIndex1],2.) + std::pow(centers[facei][radialIndex2],2.) - std::pow(rasali[k],2.));
                        }

                        minR = dzVector[0];
                        for (unsigned int k=1;k<Nr;k++)
                        {
                            minR = std::min(drVector[k],minR);
                        }
                        
                        for (unsigned int k=0;k<Nr;k++)
                        {
                            double dist = std::fabs(std::pow(centers[facei][radialIndex1],2.) + std::pow(centers[facei][radialIndex2],2.) - std::pow(rasali[k],2.));

                            if ( dist == minR)
                            {
                                if ( energy == true )
                                {
                                    T.boundaryField()[patchi][facei] = Tasali[i][k];
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}
Info << "boundary fields" << nl << endl;

//- Write
{
    if ( energy == true )
    {
        T.write();
    }
}
