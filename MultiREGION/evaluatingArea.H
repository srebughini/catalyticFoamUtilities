// Evaluate Inlet patch area
scalar PatchInletArea  = 0.;
for (int k=0;k<listInletBoundaries.size();k++)
{
	PatchInletArea +=  PatchArea(fluidRegions[j], listInletBoundaries[k]);
}

// Evaluate Oulet patch area
scalar PatchOutletArea  = 0.;
for (int k=0;k<listOutletBoundaries.size();k++)
{
	PatchOutletArea +=  PatchArea(fluidRegions[j], listOutletBoundaries[k]);
}


// Evaluate catalyticArea patch area
scalar PatchCatArea  = 0.;
for (int k=0;k<listCatalyticBoundaries.size();k++)
{
	PatchCatArea +=  PatchArea(fluidRegions[j], listCatalyticBoundaries[k]);
}

