#include "histo.h"


double KHisto::Mean()
{
    if(KVector::Pointer() == 0)
        return 0.0;

    double dSum = 0.0;
    double dMean = 0.0;

    for(int i=0,ii=KVector::Dim(); ii; i++,ii--)
    {
        dMean 	+= (double)(i)*_ppA[i][0];
        dSum 	+= _ppA[i][0];
    }

    return (dMean /= dSum);
}



KBINARIZATION_OUTPUT* KHisto::Otsu(const KImageGray* igpImg,KImageGray* igpOut)
{
    //histogram
    if(igpImg)
        Histogram(*igpImg);
    else
        assert(KVector::Pointer());

    //apply Ostu' thresholding
    double      dpQ1[256],dpMyu1[256],dpMyu2[256];
    double		dMyu 	= Mean();
    KVector		vP 	 	= KVector::Normalize(_UNITSUM_NORMALIZE);
    int			nThresh = 1;
    double		dMax 	= 0.0, dSigmaB;

    dpQ1[0]   	= vP[0];
    dpMyu1[0] 	= 0.0;

    for(int i=0; i<254; i++)
    {
        dpQ1[i+1] 	= dpQ1[i] + vP[i+1];
        if(_ABS(1.0-dpQ1[i+1]) < 0.000000001)
            break;

        if(dpQ1[i+1]  == 0.0)
        {
            dpMyu1[i+1] = 0.0;
            continue;
        }

        dpMyu1[i+1]	= (dpQ1[i]*dpMyu1[i] + (double)(i+1)*vP[i+1])/dpQ1[i+1];
        dpMyu2[i+1] =  (dMyu-dpQ1[i+1]*dpMyu1[i+1]) / (1.0 - dpQ1[i+1]);

        //between-class variance
        dSigmaB = dpQ1[i+1]*(1.0-dpQ1[i+1])*_SQR(dpMyu1[i+1] - dpMyu2[i+1]);

        //search the maximum
        if(dSigmaB > dMax)
        {
            dMax 	 	= dSigmaB;
            nThresh  	= i+1;
        }
    }

    //output image
    if(igpOut)
    {
        if(igpOut != igpImg)
            igpOut->Create(igpImg->Row(),igpImg->Col());
        for(int i=0,ii=igpImg->Row(); ii; i++,ii--)
            for(int j=0,jj=igpImg->Col(); jj; j++,jj--)
                (*igpOut)[i][j] = ((*igpImg)[i][j] > nThresh ? 0xFF : 0);
    }

    //return the threshold
    _oOtsuOutput.nThresh  = nThresh;
    _oOtsuOutput.dMyuG    = dMyu;
    _oOtsuOutput.dMyu1    = dpMyu1[nThresh];
    _oOtsuOutput.dMyu2    = dpMyu2[nThresh];
    _oOtsuOutput.dQ1      = dpQ1[nThresh];
    _oOtsuOutput.dQ2      = 1.0-dpQ1[nThresh];

    return &_oOtsuOutput;
}



void KHisto::Histogram(const KImageGray& igImg,const int& nType,const int& nDir,const int& nS,const int& nE)
{
    int 	nRow 	= igImg.Row();
    int 	nCol 	= igImg.Col();

    if(nType == _GRAY && nDir == -1)
    {
        //init.
        KVector::Create(256);

        //histograming according to intensities
        double* dpHisto = (double*)KVector::Pointer();

        for(int ic=nRow,i=0; ic; ic--, i++)
            for(int jc=nCol,j=0; jc; jc--, j++)
                dpHisto[igImg[i][j]] ++;
    }
    else if(nType == _GRAY && nDir == _X_AXIS)
    {
        //init.
        int	nSt = (nS ? nS : 0);
        int nEn = (nE ? nE+1 : nCol);

        KVector::Create(nEn-nSt);
        double* dpHisto = (double*)KVector::Pointer();

        //compute the mean intensities according to X coordinates
        for(int i=0,ic=this->Dim(); ic; i++,ic--)
        {
            for(int jc=nRow,j=0; jc; jc--, j++)
                dpHisto[i] += igImg[j][i];
            dpHisto[i] /= (double)(nRow);
        }
    }
    else if(nType == _BINARY && nDir == _X_AXIS)
    {
        //init.
        KVector::Create(nCol);

        //histograming according to X coordinates
        double* dpHisto = (double*)KVector::Pointer();
        int     nSt = (nS ? nS : 0);
        int     nEn = (nE ? nE+1 : nCol);

        for(int i=nSt; i<nEn; i++)
            for(int jc=nRow,j=0; jc; jc--, j++)
                if(igImg[j][i])
                    dpHisto[i] ++;
    }
    else if(nType == _BINARY && nDir == _Y_AXIS)
    {
        //init.
        KVector::Create(nRow);

        //histograming according to X coordinates
        double* dpHisto = (double*)KVector::Pointer();
        int     nSt = (nS ? nS : 0);
        int     nEn = (nE ? nE+1 : nRow);


        for(int i=nSt; i<nEn; i++)
            for(int jc=nCol,j=0; jc; jc--, j++)
                if(igImg[i][j])
                    dpHisto[i] ++;
    }
}

void KHisto::HistogramAdded(const KImageGray& igImg)
{
    if(KVector::Address() == 0)
        KVector::Create(256);

    //histograming according to intensities
    double* dpHisto = (double*)KVector::Address();

    for(int i=0; i<igImg.Row(); i++)
        for(int j=0; j<igImg.Col(); j++)
            dpHisto[igImg[i][j]] ++;
}

void KHisto::Histogram(const KPtrList<KImageGray*>& plImg)
{
    //init.
    KVector::Create(256);

    //histograming according to intensities
    double* dpHisto	= (double*)KVector::Pointer();

    for(int n=0; n<plImg.Count(); n++)
    {
        KImageGray& igImg 	= *plImg[n];
        int               nRow 	= igImg.Row();
        int               nCol 	= igImg.Col();

        for(int ic=nRow,i=0; ic; ic--, i++)
            for(int jc=nCol,j=0; jc; jc--, j++)
                dpHisto[igImg[i][j]] ++;
    }
}


void KHisto::HEQ(KImageGray& igImg, bool bSetHisto)
{
    //histograming according to intensities
    Histogram(igImg);

    //accumulated histogram
    double*  dpHisto = KVector::Address();

    for(int i=1, ic=KVector::Size()-1; ic; ic--, i++)
        dpHisto[i] += dpHisto[i-1];

    //Histogram Equalization
    for(int i=0, ic=KVector::Size(); ic; ic--, i++)
        dpHisto[i] = dpHisto[i] / (double)(igImg.Size());

    //transform the original image
    for(int ic=igImg.Row(),i=0; ic; ic--,i++)
        for(int jc=igImg.Col(),j=0; jc; jc--,j++)
            igImg[i][j] = (unsigned char)(dpHisto[igImg[i][j]]*255.0 + 0.5);

    //re-histograming
    if(!bSetHisto)
        return;

    memset(dpHisto,0,sizeof(double)*KVector::Size());
    for(int ic=igImg.Row(),i=0; ic; ic--,i++)
        for(int jc=igImg.Col(),j=0; jc; jc--,j++)
            dpHisto[igImg[i][j]] ++;
}

KImageGray KHisto::HistogramMatching(const KImageGray& igSrc)
{
    //cumulative histogram
    KVector vRef(256, this->Address());

    vRef.Normalized(_UNITSUM_NORMALIZE);

    for(int i=1,ic=255; ic; ic--, i++)
        vRef[i] += vRef[i-1];

    //similarily
    KHisto  hSrc(igSrc);
    double* dpHisto = hSrc.Address();

    hSrc.Normalized(_UNITSUM_NORMALIZE);

    for(int i=1, ic=255; ic; ic--, i++)
        dpHisto[i] += dpHisto[i-1];


    //lookup table
    int npLookup[256] = { 0,};

    for(int i=0; i<256; i++)
    {
        if(hSrc[i] != 0.0)
            vRef.Nearest(hSrc[i], &npLookup[i]);
    }

    //transform
    KImageGray igMatched(igSrc.Row(), igSrc.Col());
    for(int ic=igSrc.Row(),i=0; ic; ic--,i++)
        for(int jc=igSrc.Col(),j=0; jc; jc--,j++)
            igMatched[i][j] = (unsigned char)(npLookup[igSrc[i][j]]);

    return igMatched;

}

KImageColor KHisto::HistogramMatching(const KImageColor& icRef, const KImageColor& icSrc)
{
    //color split
    KImageGray igpRef[3], igpSrc[3], igpMatched[3];

    icRef.SplitInto(igpRef[0], igpRef[1], igpRef[2]);//0: Red, 1:Green, 2:Blue
    icSrc.SplitInto(igpSrc[0], igpSrc[1], igpSrc[2]); //0: Red, 1:Green, 2:Blue

    //cumulative histogram
    KVector vpRefC[3], vpSrcC[3];
    double* dpHisto;
    int     npLookup[256];


    for(int n=0; n<3; n++)
    {
        //init.
        vpRefC[n].Create(256);
        vpSrcC[n].Create(256);

        //histograming according to intensities
        Histogram(igpRef[n]);

        //normalizing
        dpHisto = KVector::Address();

        for(int i=0, ic=KVector::Size(); ic; ic--, i++)
            dpHisto[i] = dpHisto[i] / (double)(igpRef[n].Size());

        //cumulated histogram
        for(int i=1, ic=255; ic; ic--, i++)
            dpHisto[i] += dpHisto[i-1];
        memcpy(vpRefC[n].Address(), dpHisto, sizeof(double) * KVector::Size());

        //similarily
        Histogram(igpSrc[n]);

        dpHisto = KVector::Address();

        for(int i=0, ic=KVector::Size(); ic; ic--, i++)
            dpHisto[i] = dpHisto[i] / (double)(igpSrc[n].Size());

        for(int i=1, ic=255; ic; ic--, i++)
            dpHisto[i] += dpHisto[i-1];

        memcpy(vpSrcC[n].Address(), dpHisto, sizeof(double) * KVector::Size());


        //lookup table
        memset(npLookup,0, 256*sizeof(int));

        for(int i=0; i<256; i++)
        {
            if(vpSrcC[n][i] != 0.0)
                vpRefC[n].Nearest(vpSrcC[n][i], &npLookup[i]);
        }

        //transform
        igpMatched[n].Create(icSrc.Row(), icSrc.Col());

        for(int ic=igpSrc[n].Row(),i=0; ic; ic--,i++)
            for(int jc=igpSrc[n].Col(),j=0; jc; jc--,j++)
                igpMatched[n][i][j] = (unsigned char)(npLookup[igpSrc[n][i][j]]);
    }


    //output
    KImageColor icMatched;
    icMatched.FromRGB(igpMatched[_RED], igpMatched[_GREEN], igpMatched[_BLUE]);
    return icMatched;
}

KImageColor KHisto::HistogramMatching(const KVector& vLookupR, const KVector& vLookupG, const KVector& vLookupB, const KImageColor& icSrc)
{
    //transform
    KImageColor icMatched(icSrc.Row(), icSrc.Col());
    for(int ic=icSrc.Row(),i=0; ic; ic--,i++)
        for(int jc=icSrc.Col(),j=0; jc; jc--,j++)
        {
            icMatched[i][j].r = (unsigned char)(vLookupR[icSrc[i][j].r]);
            icMatched[i][j].g = (unsigned char)(vLookupG[icSrc[i][j].g]);
            icMatched[i][j].b = (unsigned char)(vLookupB[icSrc[i][j].b]);
        }

    return icMatched;
}

KVector KHisto::LookupTable(const KHisto& hRef, const KHisto& hSrc)
{
    //cumulative histogram
    KVector  vRefC, vSrcC;
    KVector  vLookup(256);

    //init.
    vRefC = hRef;
    vSrcC = hSrc;

    //normalizing
    vRefC.Normalized(_UNITSUM_NORMALIZE);
    vSrcC.Normalized(_UNITSUM_NORMALIZE);

    //cumulation
    for(int i=1; i<256; i++)
    {
        vRefC[i] += vRefC[i-1];
        vSrcC[i] += vSrcC[i-1];
    }

    //lookup table
    int idx;
    for(int i=0; i<256; i++)
        if(vSrcC[i] != 0.0)
        {
            vRefC.Nearest(vSrcC[i], &idx);
            vLookup[i] = idx;
        }

    return vLookup;
}
