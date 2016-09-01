const dictionary& simple = solidRegions[j].solutionDict().subDict("SIMPLE");

const int nNonOrthCorr =
    simple.lookupOrDefault<int>("nNonOrthogonalCorrectors", 0);

const double rS =
    simple.lookupOrDefault<double>("reactionUpdate", 0);
