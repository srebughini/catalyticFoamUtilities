tmp<volScalarField> Rho(volScalarField& p, volScalarField& psi)
{
    return p*psi;
}

volScalarField normalized(const volScalarField x)
{
    dimensionedScalar one("one",x.dimensions() ,1.0);
    dimensionedScalar max("max",x.dimensions() ,gMax(x));
    dimensionedScalar min("min",x.dimensions() ,gMin(x));

    volScalarField    n = x/one;

    if ( max.value() != min.value() )
    {
        n = (x - min)/(max - min);
    }
    else if ( max.value() == min.value() && min.value() == 0.)
    {
        n = 0.0*x;
    }
    else
    {
        n = x/x;
    }

    return n;
}

double average(const scalarField y, const scalarField rho, const scalarField V, const std::vector<int> cells)
{
    double num = 0.;
    double den = 0.;
    for(unsigned int i=0;i<cells.size();i++)
    {
        num += y[cells[i]]*rho[cells[i]]*V[cells[i]];
        den += rho[cells[i]]*V[cells[i]];
    }

    return num/den;
}

double average(const scalarField y, const scalarField V, const std::vector<int> cells)
{
    double num = 0.;
    double den = 0.;
    for(unsigned int i=0;i<cells.size();i++)
    {
        num += y[cells[i]]*V[cells[i]];
        den += V[cells[i]];
    }

    return num/den;
}

double average(const scalarField y, const scalarField rho, const scalarField V, const scalarField cp, const std::vector<int> cells)
{
    double num = 0.;
    double den = 0.;
    for(unsigned int i=0;i<cells.size();i++)
    {
        num += y[cells[i]]*rho[cells[i]]*V[cells[i]]*cp[cells[i]];
        den += rho[cells[i]]*V[cells[i]]*cp[cells[i]];
    }

    return num/den;
}

void linearization(scalarField& z, const double y0, const double yf, const std::vector<int> cells)
{
    for(unsigned int i=0;i<cells.size();i++)
    {
        if ( (z[cells[i]] + yf - y0) < 0. )
        {
            z[cells[i]] = yf;
        }
        else
        {
            z[cells[i]] = z[cells[i]] + yf - y0;
        }
    }
}
