{
    for (label i=j*thermodynamicsMapXML->NumberOfSpecies(); i<(j+1)*thermodynamicsMapXML->NumberOfSpecies(); i++)
    {
        volScalarField& Yi = YFluid[i];
        volScalarField& Dmixi = DmixFluid[i];

        J[i] = ( fvc::interpolate ( -rho*Dmixi ) ) * ( fvc::interpolate ( fvc::grad(Yi) ) & fluidRegions[j].Sf() );
    }

    surfaceScalarField& Jcj = Jc[j];

    Jcj *= 0.;
    for (label i=j*thermodynamicsMapXML->NumberOfSpecies(); i<(j+1)*thermodynamicsMapXML->NumberOfSpecies(); i++)
    {
        Jcj -= J[i];
    }

    for (label i=j*thermodynamicsMapXML->NumberOfSpecies(); i<(j+1)*thermodynamicsMapXML->NumberOfSpecies(); i++)
    {
        J[i] += Jcj*fvc::interpolate(YFluid[i]);
    }
}
