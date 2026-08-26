//---------------------------------------------------------------------------
#include "circleHough.h"
#include "edge.h"

int KCircleHough::Init(void* pInfo)
{
	_oInfo = *(KCIRCLEHOUGH_INFO*)pInfo;

	//create voting table
    _lllVotes.resize(_oInfo.nNumGridCy);
    for(auto& itemY : _lllVotes)
    {
        itemY.resize(_oInfo.nNumGridCx);
        for(auto& itemX : itemY)
            itemX.resize(_oInfo.nNumGridR);
    }

	//equal ratio for quantization of center positon and radius
    _dSx = (double)(_oInfo.nRight - _oInfo.nLeft) / (double)(_oInfo.nNumGridCx-1); //Cx = nLeft  + k*_dSx
    _dSy = (double)(_oInfo.nBottom- _oInfo.nTop)  / (double)(_oInfo.nNumGridCy-1); //Cy = nTop   + k*_dSy
    _dSr = (double)(_oInfo.nMaxR  - _oInfo.nMinR) / (double)(_oInfo.nNumGridR -1); //R  = nMinR  + k*_dSr

    return 1;
}

void* KCircleHough::Run(void* pEdge, void* pFrom,void* pTo,void* pVote, void* p5)
{
	int			nGridCx,nGridCy,nGridR;
	double		dRadius;
	double		dAngle;
	KPoint		ptCen;
	KEdge* 		opEdge = (KEdge*)pEdge;
	KCircle*	ccpFrom = (KCircle*)pFrom;
	KCircle*	ccpTo   = (KCircle*)pTo;
    vector<KCircle>* dCircles = new vector<KCircle>();
    int nMAX = 10;
    int dCount = 0;

	//reset the voting table
    _lllVotes.resize(_oInfo.nNumGridCy);
    for(auto& itemY : _lllVotes)
    {
        itemY.resize(_oInfo.nNumGridCx);
        for(auto& itemX : itemY)
            itemX.resize(_oInfo.nNumGridR);
    }

	//Hough Transform
    for(auto& oEdgePxl : *opEdge)
    {
		//check if the edge is in effective region
        if(ccpTo && !ccpTo->InCircle((double)oEdgePxl.u,(double)oEdgePxl.v))
			continue;
        if(ccpFrom && ccpFrom->InCircle((double)oEdgePxl.u,(double)oEdgePxl.v))
			continue;

		//votes for radius
		nGridR 	= 1;
		dRadius = (double)_oInfo.nMinR;
		do{
			//compute circle centers for voting
            dAngle 	  =	_RADIAN(oEdgePxl.wAng);

			if(_oInfo.nDirEdge == _OUTWARD_EDGE)
			{
                ptCen._dX = oEdgePxl.u - dRadius*cos(dAngle);
                ptCen._dY = oEdgePxl.v - dRadius*sin(dAngle);
			}
            else //_INWARD_EDGE
			{
                ptCen._dX = oEdgePxl.u + dRadius*cos(dAngle);
                ptCen._dY = oEdgePxl.v + dRadius*sin(dAngle);
			}

			//compute grid coordinates
			nGridCx	  = (int)( (ptCen._dX - (double)_oInfo.nLeft)/_dSx );
			if(nGridCx < 2 || nGridCx > _oInfo.nNumGridCx-3)
				continue;
			nGridCy	  = (int)( (ptCen._dY - (double)_oInfo.nTop)/_dSy );
			if(nGridCy < 2 || nGridCy > _oInfo.nNumGridCy-3)
				continue;

			//votes to avoid edge noises
			for(int i=-2; i<3; i++)
				for(int j=-2; j<3; j++)
				{
                    _lllVotes[nGridCy+i][nGridCx+j][nGridR-1] += 0.7;
                    _lllVotes[nGridCy+i][nGridCx+j][nGridR]   += 0.7;
                    _lllVotes[nGridCy+i][nGridCx+j][nGridR+1] += 0.7;
				}
            _lllVotes[nGridCy][nGridCx][nGridR] += 0.3;

			//increment
			dRadius += _dSr;

		}while(++nGridR < _oInfo.nNumGridR-1);
	}

	//select peaks
    while(dCount < nMAX){
	int	nGmax=0;
	for(int i=0; i<_oInfo.nNumGridCy; i++)
	{
		for(int j=0; j<_oInfo.nNumGridCx; j++)
		{
			for(int k=0; k<_oInfo.nNumGridR; k++)
                if(_lllVotes[i][j][k] > nGmax)
				{
                    nGmax 	= (int)(_lllVotes[i][j][k]);
					nGridCx	= j;
					nGridCy	= i;
					nGridR	= k;
				}
		}
	}


    int nThresh = (_oInfo.nThreshVote == 0 ? _PI*(_oInfo.nMinR + nGridR*_dSr)*0.5 : _oInfo.nThreshVote);
    if(nGmax < nThresh) break;

    KCircle dCircle(
        _oInfo.nLeft + nGridCx * _dSx,
        _oInfo.nTop + nGridCy * _dSy,
        _oInfo.nMinR + nGridR * _dSr
        );
    dCircles->push_back(dCircle);
    dCount++;

    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
        for (int k = -1; k <= 1; k++) { // 반지름 범위 내 제거
            int x = nGridCx + i;
            int y = nGridCy + j;
            int r = nGridR + k;
            if (x >= 0 && x < _oInfo.nNumGridCx &&
                y >= 0 && y < _oInfo.nNumGridCy &&
                r >= 0 && r < _oInfo.nNumGridR) {
                _lllVotes[y][x][r] = 0; // 투표값 제거
            }
        }
        }
    }
    }


	//중심 및 반지름의 그리드 값으로 부터 실제 픽셀 단위로 환산하여 반환한다
    _oDetected.Create(_oInfo.nLeft+nGridCx*_dSx,_oInfo.nTop+nGridCy*_dSy,_oInfo.nMinR + nGridR*_dSr);

//    //voting 결과를 이미지로 저장함
//    if(pVote)
//    {
//        KImageGray& igVote = *((KImageGray*)pVote);
//        igVote =
//    }
    return (void*)(dCircles);
}

void* KCircleHough::Run1(void* pEdge){
    KEdge* opEdge = (KEdge*) pEdge;
    vector<KPOINT>* maskVectors = new vector<KPOINT>();
    vector<vector<double>> accumulator;

    for(auto& _Rp : *maskVectors)
    {
        double angle = atan2(_Rp.nY - _oInfo.nTop, _Rp.nX - _oInfo.nLeft);
        int angleIndex = (int)(angle * 180.0 / M_PI);
        maskVectors->push_back({angleIndex, (int)(sqrt(pow(_Rp.nX, 2)+pow(_Rp.nY, 2)))});
    }

    for(auto& edgePixel : *opEdge){
        int angleIndex = edgePixel.wAng % 360;
        for(auto& rEntry : *maskVectors){
            int dx = rEntry.nX * cos(angleIndex);
            int dy = rEntry.nY * sin(angleIndex);

            int cx = edgePixel.u + dx;
            int cy = edgePixel.v + dy;

            if(cx >= 0 && cx < _oInfo.nNumGridCx && cy >= 0 && cy < _oInfo.nNumGridCy)
                accumulator[cy][cx]++;
        }
    }

    double maxVotes = 0;
    int bx=0 , by =0;

    for(int y = 0; y < accumulator.size(); ++y){
        for(int x = 0; x < accumulator[0].size(); ++x){
            if(accumulator[y][x] > maxVotes){
                maxVotes = accumulator[y][x];
                bx = x;
                by = y;
            }
        }
    }
    if(maxVotes < _oInfo.nThreshVote){
    KPOINT* dCenter = new KPOINT(bx, by);
    return dCenter;
    }
}
//---------------------------------------------------------------------------
