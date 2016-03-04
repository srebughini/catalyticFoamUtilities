const dictionary& simple = fluidRegions[j].solutionDict().subDict("SIMPLE");

const int nNonOrthCorr =
    simple.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);

