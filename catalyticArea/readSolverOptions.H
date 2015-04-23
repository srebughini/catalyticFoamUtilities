Info<< "Reading Solver options\n" << endl;
IOdictionary solverOptions
(
	IOobject
	(
		"solverOptions",
		runTime.constant(),
		runTime.db(),
		IOobject::MUST_READ,
		IOobject::NO_WRITE
	)
);

List<word>  listInletBoundaries;
List<word>  listOutletBoundaries;
List<word>  listCatalyticBoundaries;
const dictionary& postProcessing = solverOptions.subDict("PostProcessing");
{
	// List of inlet boundaries
	listInletBoundaries = List<word>(postProcessing.lookup("inletBoundaries"));
	// List of outlet boundaries
	listOutletBoundaries = List<word>(postProcessing.lookup("outletBoundaries"));
	// List of catalytic boundaries
	listCatalyticBoundaries = List<word>(postProcessing.lookup("catalyticBoundaries"));
}
