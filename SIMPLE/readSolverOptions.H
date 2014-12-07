/***************************************************************************
 *   catalyticFOAM                                                         *
 *                                                                         *
 *   Copyright (C) 2011 by Alberto Cuoci and Matteo Maestri                *
 *                                                                         *
 *   Authors                                                               *
 *   alberto.cuoci@polimi.it                                               *
 *   matteo.maestri@polimi.it                                              *
 *   sandro.goisis@mail.polimi.it                                          *
 *   alessandra.osio@mail.polimi.it                                        *
 *                                                                         *
 *   Last modified: 12/23/2011                                             *
 ***************************************************************************/
Info<< "Reading Solver options\n" << endl;
IOdictionary solverOptions
(
	IOobject
	(
		"solverOptions",
		U.time().constant(),
		U.db(),
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


