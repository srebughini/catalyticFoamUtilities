{
    // Mass diffusion
    forAll (Y,i)
    {
        volScalarField& Yi = Y[i];
        volScalarField& Dmixi = Dmix[i];

        J[i] = ( fvc::interpolate ( -rho*Dmixi ) ) * ( fvc::interpolate ( fvc::grad(Yi) ) & mesh.Sf() );
    }

    // Correction flux
    Jc *= 0.;
    forAll (Y,i)
    {
        Jc -= J[i];
    }

    // Corrected fluxes
    forAll (Y,i)
    {
        J[i] += Jc*fvc::interpolate(Y[i]);
    }
}
