const dictionary& simple = solidRegions[j].solutionDict().subDict("SIMPLE");

const int nNonOrthCorr =
    simple.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);

