forAll(solidRegions, j)
{
    const scalarField& TCells   = TSolid[j].internalField();
    const scalarField& pCells   = pSolid[j].internalField();
    const scalarField& rhoCells = rhoSolid[j].internalField();

    const unsigned int NC       = thermodynamicsMapXML->NumberOfSpecies();
    const unsigned int SURF_NC  = surfaceThermodynamicsMapXML->number_of_site_species();
    const unsigned int NEQ      = NC + 1;


    OpenSMOKE::OpenSMOKEVectorDouble Source(NEQ);
    OpenSMOKE::OpenSMOKEVectorDouble y(NEQ);
    OpenSMOKE::OpenSMOKEVectorDouble Z(SURF_NC);

    het.setCatalyticLoad(alfaM[j]);
    het.setEnergyEquation(energySolid[j]);

    forAll(TCells, celli)
    {
        for(unsigned int i=0;i<NC;i++)
        {
            y[i + 1] = YSolid[j*NC+i].internalField()[celli];
        }

        y[NEQ] = TCells[celli];

        for(unsigned int i=0;i<SURF_NC;i++)
        {
            Z[i + 1] = ZSolid[j*SURF_NC+i].internalField()[celli];
        }

        het.setPressure(pCells[celli]);
        het.setCoverage(Z);
        het.reactionSourceTerms(y,Source);

        for(unsigned int i=0;i<NC;i++)
        {
            RSolid[j*NC+i].internalField()[celli] = Source[i+1];
        }


        QSolid[j].internalField()[celli] = Source[NEQ];
    }
}
