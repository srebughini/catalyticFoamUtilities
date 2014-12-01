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

// List of inlet boundaries
List<word>  listInletBoundaries(solverOptions.lookup("inletBoundaries"));

// List of outlet boundaries
List<word>  listOutletBoundaries(solverOptions.lookup("outletBoundaries"));

// List of catalytic boundaries
List<word>  listCatalyticBoundaries(solverOptions.lookup("catalyticBoundaries"));
