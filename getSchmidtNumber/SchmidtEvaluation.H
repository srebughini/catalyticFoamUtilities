Info << "Schmidt number calculation..." << endl;
{
    {
        const scalarField& muCells   = mu.internalField();
        const scalarField& rhoCells  = rho.internalField();

        forAll(T.internalField(), celli)
        {
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                Sc[i].internalField()[celli] = muCells[celli]/(Dmix[i].internalField()[celli]*rhoCells[celli]);
            }
        }
    }

    forAll(T.boundaryField(), patchi)
    {
        fvPatchScalarField& pmu   = mu.boundaryField()[patchi];
        fvPatchScalarField& prho  = rho.boundaryField()[patchi];
        
        forAll(T.boundaryField()[patchi], facei)
        {
            for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
            {
                Sc[i].boundaryField()[patchi][facei] = pmu[facei]/(Dmix[i].boundaryField()[patchi][facei]*prho[facei]);
            }
        }
    }
}

{
    mu.write();
    rho.write();

    for(int i=0;i<thermodynamicsMapXML->NumberOfSpecies();i++)
    {
        Dmix[i].write();
        Sc[i].write();
    }
}
