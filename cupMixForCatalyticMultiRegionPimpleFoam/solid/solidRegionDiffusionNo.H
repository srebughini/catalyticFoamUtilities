    scalar DiffNum = -GREAT;

    label nregions = 0;
    forAll(solidRegions, j)
	nregions++;

    OpenSMOKE::OpenSMOKEVectorDouble Diff_i ( thermodynamicsMapXML->NumberOfSpecies()*nregions );

    forAll(solidRegions, j)
    {
	#include "setRegionSolidFields.H"
	if (solveSolidRegions[j] == true)
	{
		for (unsigned int i=j*thermodynamicsMapXML->NumberOfSpecies(); i<thermodynamicsMapXML->NumberOfSpecies()*(j+1); i++)
        	{
			Diff_i[i+1]=max(DmixSolid[i]).value();	
		}
        
		double Diff_max = Diff_i.Max();
		
		DiffNum = max
		(
		    solidRegionDiffNo
		    (
		        solidRegions[j],
		        runTime,
		        Diff_max,
		        solidDimension[j+1]
		    ),
		    DiffNum
		);
	}
	else
		DiffNum = -GREAT;
    }
