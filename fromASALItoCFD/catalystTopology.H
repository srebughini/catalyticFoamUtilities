catalyticReactorClass  *catalyticReactors;
labelList               isNeighbourCell;
labelList               indexCatalyst;

label totalNumberOfNonCatalyticReactors = 0;
label totalNumberOfCatalyticFaces       = 0;
label totalNumberOfCatalyticReactors    = 0;
label totalNumberOfReactors             = 0;
label maxNumberOfCatalyticFaces         = 0;
label minNumberOfCatalyticFaces         = 0;

if(catalyst == true)
{
    volScalarField& T           = TFluid[0];
    fvMesh& mesh                = fluidRegions[0];

    labelList       catalyticWallSize;
    labelList       catalystNeighbour;
    labelList       catalyticPatchIndex;
    labelList       catalyticFaceIndex;
    scalarField     catalyticCellArea;
    scalarList      catalyticNeighbourCellVolume;

    for (label j=0;j<patchCatalytic.size();j++)
    {
        label patchID = mesh.boundaryMesh().findPatchID(patchCatalytic[j]);
        if (patchID==-1) 
        {
            Info << "Failure to find patch named " << patchCatalytic[j] << endl;
            abort();
        }
        else
        {
            indexCatalyst.append(patchID);
            catalyticWallSize.append(T.boundaryField()[patchID].size());
            totalNumberOfCatalyticFaces += T.boundaryField()[patchID].size();
        }
    }

    //- Look for cells near catalytic wall
    if(totalNumberOfCatalyticFaces != 0)
    {
        {
            OpenSMOKE::OpenSMOKEVectorInt           catalystNeighbourTemp(totalNumberOfCatalyticFaces);
            OpenSMOKE::OpenSMOKEVectorInt           catalyticPatchIndexTemp(totalNumberOfCatalyticFaces);
            OpenSMOKE::OpenSMOKEVectorInt           catalyticFaceIndexTemp(totalNumberOfCatalyticFaces);
            OpenSMOKE::OpenSMOKEVectorDouble        catalyticCellAreaTemp(totalNumberOfCatalyticFaces);
            OpenSMOKE::OpenSMOKEVectorDouble        catalyticNeighbourCellVolumeTemp(totalNumberOfCatalyticFaces);

            label counter = 1;
            for(int i=0; i<indexCatalyst.size(); i++)
            {
                double totalArea = 0.;
                double maxArea   = -1.e16;
                double minArea   =  1e16;

                const labelList& cellOwner = T.boundaryField()[indexCatalyst[i]].patch().faceCells();
                forAll(T.boundaryField()[indexCatalyst[i]] , facei)
                {

                    double area = mesh.magSf().boundaryField()[indexCatalyst[i]][facei];
                    
                    catalyticPatchIndexTemp[counter]          = indexCatalyst[i];
                    catalyticFaceIndexTemp[counter]           = facei;   
                    catalystNeighbourTemp[counter]            = cellOwner[facei];
                    catalyticNeighbourCellVolumeTemp[counter] = mesh.V()[cellOwner[facei]];
                    catalyticCellAreaTemp[counter]            = area;

                    totalArea += area;

                    if (area>maxArea)
                    {
                        maxArea = area;
                    }
                    else if (area < minArea)
                    {
                        minArea = area;
                    }

                    counter++;
                }
            }

            // Sort provisional vectors
            OpenSMOKE::OpenSMOKEVectorInt Indices(totalNumberOfCatalyticFaces);
            Sort(&catalystNeighbourTemp, &Indices);
            Reorder(&catalyticFaceIndexTemp, Indices);
            Reorder(&catalyticPatchIndexTemp, Indices);
            Reorder(&catalyticCellAreaTemp, Indices);
            Reorder(&catalyticNeighbourCellVolumeTemp, Indices);

            // Indices
            catalyticFaceIndex.resize(totalNumberOfCatalyticFaces);
            catalyticPatchIndex.resize(totalNumberOfCatalyticFaces);
            catalyticCellArea.resize(totalNumberOfCatalyticFaces);
            catalystNeighbour.resize(totalNumberOfCatalyticFaces);
            catalyticNeighbourCellVolume.resize(totalNumberOfCatalyticFaces);

            for (int j=1;j<=Indices.Size();j++)
            {
                catalyticFaceIndex[j-1]           = catalyticFaceIndexTemp[j];
                catalyticPatchIndex[j-1]          = catalyticPatchIndexTemp[j];
                catalyticCellArea[j-1]            = catalyticCellAreaTemp[j];
                catalystNeighbour[j-1]            = catalystNeighbourTemp[j];
                catalyticNeighbourCellVolume[j-1] = catalyticNeighbourCellVolumeTemp[j];
            }

            // NeighbourCell flag
            isNeighbourCell.resize(T.size());
            for (int j=0;j<T.size();j++)
            {
                isNeighbourCell[j] = 0;
            }

            int k=0;
            while(k<catalystNeighbour.size())
            {
                int counter = 1;
                for(int j=k+1; j<catalystNeighbour.size(); j++)
                {
                    if (catalystNeighbour[k] == catalystNeighbour[j])
                        counter++;
                    else
                        break;
                }
                isNeighbourCell[catalystNeighbour[k]] = counter;
                k+=counter;
            }

            // Count total number of catalytic reactors
            scalarField& TCells = T.internalField();
            forAll(TCells, celli)
            {
                if (isNeighbourCell[celli] > 0)
                {
                    totalNumberOfCatalyticReactors++;
                }
            }
        }

        catalyticReactors = new catalyticReactorClass[totalNumberOfCatalyticReactors];
    

        int k     = 0;
        int index = 0;
        while(k<catalystNeighbour.size())
        {
            int counter = 1;
            for(int j=k+1; j<catalystNeighbour.size(); j++)
            {
                if (catalystNeighbour[k] == catalystNeighbour[j])
                {
                    counter++;
                }
                else
                {
                    break;
                }
            }

            for(int j=k; j<k+counter; j++)
            {
                catalyticReactors[index].globalIndex = catalystNeighbour[j];
                catalyticReactors[index].volume      = catalyticNeighbourCellVolume[j];

                catalyticReactors[index].numberOfFaces++;

                catalyticReactors[index].patchIndex.Append(catalyticPatchIndex[j]);
                catalyticReactors[index].faceIndex.Append(catalyticFaceIndex[j]);
                catalyticReactors[index].faceArea.Append(catalyticCellArea[j]);    
            }

            k+=counter;
            index++;
        }

        totalNumberOfReactors = T.size();
        totalNumberOfNonCatalyticReactors = totalNumberOfReactors - totalNumberOfCatalyticReactors;
    }
}
else
{
    catalyticReactors = new catalyticReactorClass;
}
