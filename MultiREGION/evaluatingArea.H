// Evaluate Inlet patch area
scalar PatchInletArea  = 0.;
for (int j=0;j<listInletBoundaries.size();j++)
{
	PatchInletArea +=  PatchArea(fluidRegions[j], listInletBoundaries[j]);
}
// Evaluate Oulet patch area
scalar PatchOutletArea  = 0.;
for (int j=0;j<listOutletBoundaries.size();j++)
{
	PatchOutletArea +=  PatchArea(fluidRegions[j], listOutletBoundaries[j]);
}
// Evaluate catalyticArea patch area
scalar PatchCatArea  = 0.;
for (int j=0;j<listCatalyticBoundaries.size();j++)
{
	PatchCatArea +=  PatchArea(fluidRegions[j], listCatalyticBoundaries[j]);
}
