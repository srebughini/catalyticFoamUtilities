forAll(fluidRegions, j)
{
    const scalarField& TCells   = TFluid[j].internalField();
    const scalarField& pCells   = pFluid[j].internalField();

    const unsigned int NC  = thermodynamicsMapXML->NumberOfSpecies();
    const unsigned int NEQ = NC + 1;

    OpenSMOKE::OpenSMOKEVectorDouble Source(NEQ);
    OpenSMOKE::OpenSMOKEVectorDouble y(NEQ);

    forAll(TCells, celli)
    {
        for(unsigned int i=0;i<NC;i++)
        {
            y[i+1] = YFluid[j*thermodynamicsMapXML->NumberOfSpecies()+i].internalField()[celli];
        }

        y[NEQ] = TCells[celli];

        hom.reactionSourceTerms(*thermodynamicsMapXML,*kineticsMapXML,y,pCells[celli],Source);

        for(unsigned int i=0;i<NC;i++)
        {
            RFluid[j*NC+i].internalField()[celli]   = Source[i+1];
        }

        QFluid[j].internalField()[celli] = Source[NEQ];
    }
}
