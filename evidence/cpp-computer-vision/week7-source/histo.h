#ifndef KHISTO_H
#define KHISTO_H

#include "kfc.h"

struct KBINARIZATION_OUTPUT
{
    int     nThresh;
    double  dMyuG;  //전역 평균
    double	dMyu1;  //영역 1 평균
    double	dMyu2;  //영역 2 평균 ( > 영역 1 평균 )
    double  dQ1;
    double  dQ2;
};


class KHisto : public KVector  //tag: h
{
    KBINARIZATION_OUTPUT	_oOtsuOutput;

public:
    KHisto(){ }
    KHisto(const int& nSize){ KVector::Create(nSize); }
    KHisto(const KImageGray& igImg){ Histogram(igImg); }
    KHisto(const KHisto& oHisto){ KVector::Create(oHisto.Size(),oHisto.Pointer()); }
    virtual ~KHisto(){ }

    //operations
    void        	HistogramAdded(const KImageGray& igImg);
    void        	Histogram(const KImageGray& igImg,const int& nType=_GRAY,const int& nDir=-1,const int& nS=0,const int& nE=0);
    void        	Histogram(const KPtrList<KImageGray*>& plImg);
    void        	HEQ(KImageGray&  igImg, bool bSetHisto = false);
    void        	HEQ(KImageColor& icImg, bool bSetHisto = false){ }
    KImageColor     HistogramMatching(const KImageColor& icRef, const KImageColor& icIn);
    KImageGray      HistogramMatching(const KImageGray& igIn);
    KImageColor     HistogramMatching(const KVector& vLookupR, const KVector& vLookupG, const KVector& vLookupB, const KImageColor& icSrc);
    KVector         LookupTable(const KHisto& hRef, const KHisto& hSrc);

    double                  Mean();
    KBINARIZATION_OUTPUT*	Otsu(const KImageGray* igpImg=0,KImageGray* igpOut=0);

    //	KHisto&     operator = (const double& dSet);
};

#endif // KHISTO_H
