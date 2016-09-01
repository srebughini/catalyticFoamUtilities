const dictionary& simple = fluidRegions[j].solutionDict().subDict("SIMPLE");

const int nNonOrthCorr =
    simple.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);

const double rF =
    simple.lookupOrDefault<double>("reactionUpdate", 0);

