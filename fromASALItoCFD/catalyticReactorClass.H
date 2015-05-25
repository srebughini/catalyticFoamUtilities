class catalyticReactorClass
{
    public:

        catalyticReactorClass()
        {
            globalIndex     = -1;
            volume          =  0;
            numberOfFaces   =  0;
        }

        int         globalIndex;
        double      volume;
        int         numberOfFaces;

        OpenSMOKE::OpenSMOKEVectorInt       patchIndex;
        OpenSMOKE::OpenSMOKEVectorInt       faceIndex;
        OpenSMOKE::OpenSMOKEVectorDouble    faceArea;
};
