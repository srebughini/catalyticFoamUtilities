scalar DiNum = -GREAT;

forAll(solidRegions, j)
{
    //- Note: do not use setRegionSolidFields.H to avoid double registering Cp
    //#include "setRegionSolidFields.H"

    tmp<volScalarField> magKappa  = lambdaSolid[j];

    tmp<volScalarField> tcp       = cpSolid[j];
    const volScalarField& cp      = tcp();

    tmp<volScalarField> trho      = rhoSolid[j];
    const volScalarField& rho     = trho();

    DiNum = max
    (
        solidRegionDiffNo
        (
            solidRegions[j],
            runTime,
            rho*cp,
            magKappa()
        ),
        DiNum
    );
}
