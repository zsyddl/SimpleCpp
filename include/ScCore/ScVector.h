#pragma once

#include <vector>
#include <math.h>
#include "ScCoreDefine.h"
using namespace std;
//
//空间的矢量点.

template<class T>
class SCCORE_API CVector: public vector<T>
{
public:
	CVector(void);
	virtual ~CVector(void);

// 	template<class T1>
// 	float Distance(T1* pSample, int nSize);
	template<class T1>
	void SetValue(T1* pSample, int nSize)
	{
		resize(nSize);
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			pValue[i] = pSample[i];
		}
	}
	template<class T1>
	void SetValue(CVector<T1>& other)
	{
		int nSize = other.size();
		resize(nSize);
		T* pValue = &at(0);
		T1* pOtherValue = &other.at(0);
		for (int i=0; i<nSize; i++)
		{
			pValue[i] = pOtherValue[i];
		}
	}
	template<class T1>
	int GetValue(T1* pSample, int nSize)
	{
		if (size() != nSize)
		{
			return -1;
		}
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			pSample[i] = pValue[i];
		}
		return 0;
	}
	//将一个区域设置为一个数值 
	template<class T1>
	void SetValue(T1 nValue, int nW, int nH, int nRectX, int nRectY, int nRectW, int nRectH)
	{
		int nSize = nW*nH;
		resize(nSize);

		T* pValue = &at(0);

		int nY1 = nRectY+nRectH;
		int nX1 = nRectX+nRectW;
		int nLinePos = 0;
		int nPos = 0;
		T diffTmp(0);
		for (int y=nRectY; y<nY1; y++)
		{
			nLinePos = y*nW;
			for (int x=nRectX; x<nX1; x++)
			{
				nPos = nLinePos + x;
				
				pValue[nPos] = nValue;
			}
		}

	}


	template<class T1>
	int Update(T1* pSample, int nSize, int nFactorSample, int nFactorSelf)
	{
		if (size() != nSize)
		{
			resize(nSize);
		}
		int nFactorTotal = nFactorSample+nFactorSelf;
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			pValue[i] = (pSample[i]*nFactorSample + pValue[i]*nFactorSelf)/(float)nFactorTotal;
		}
		return 0;
	}

	template<class T1>
	float Distance(T1* pSample, int nSize)
	{
		if (size() != nSize)
		{
			return -1;
		}
		float fDistance = 0;
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			float fTmp = pValue[i] - pSample[i];
			fDistance =  fDistance + fTmp*fTmp;
		}
		fDistance = sqrt(fDistance);

		return fDistance;
	}
	template<class T1, class T2>
	float Distance(T1* pSample, int nSize, T2 *pMask)
	{
		if (size() != nSize)
		{
			return -1;
		}
		float fDistance = 0;
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			float fTmp = pValue[i] - pSample[i];
			fDistance =  fDistance + fTmp*fTmp*pMask[i];
		}
		fDistance = sqrt(fDistance);

		return fDistance;
	}
	template<class T1>
	float Distance(CVector<T1>& other)
	{
		if (size() != other.size())
		{
			return -1;
		}
		int nSize = size();
		float fDistance = 0;
		T* pValue = &at(0);
		T1* pValueOther = &other.at(0);
		for (int i=0; i<nSize; i++)
		{
			float fTmp = pValue[i] - pValueOther[i];
			fDistance =  fDistance + fTmp*fTmp;
		}
		fDistance = sqrt(fDistance);

		return fDistance;
	}
	template<class T1>
	float AngleDistance(T1* pValue, int nSize)
	{
		if (size() != nSize)
		{
			return -1;
		}
		//fDistance = sqrt(fDistance);
		float fR0(0), fR1(0), fPValue(0);
		float fSim(0);
		T*		pLocalValue = &at(0);

		for (int i=0; i<nSize; i++)
		{
			int nTmp = pLocalValue[i];
			fR0 += nTmp*nTmp;
		}
		fR0 = sqrt(fR0);

		for (int i=0; i<nSize; i++)
		{
			int nTmp = pValue[i];
			fR1 += nTmp*nTmp;
		}
		fR1 = sqrt(fR1);
		//计算投影

		for (int i=0; i<nSize; i++)
		{
			int nTmp = pValue[i]*pLocalValue[i];
			fPValue += nTmp;
		}
		//cos 越大相似度越大
		fSim = fPValue/(fR0*fR1);
		//反过来，角度越小越好
		return 1.0/fSim;
	}

	template<class T1, class T2>
	void Sub(CVector<T1>& other, CVector<T2>& diff){
		if (size() != other.size())
		{
			return ;
		}
		int nSize = size();
		diff.resize(nSize);

		float fDistance = 0;
		T* pValue = &at(0);
		T1* pValueOther = &other.at(0);
		T2* pDiff = &diff.at(0);
		for (int i=0; i<nSize; i++)
		{
			pDiff[i] = pValue[i] - pValueOther[i];
		}
		
	
	};
	//
	float Mod(){
		float fMod = 0;
		int nSize = size();
		T* pValue = &at(0);
		 
		for (int i=0; i<nSize; i++)
		{
			fMod = fMod + pValue[i]*pValue[i];
		}
		fMod = sqrt(fMod);

		return fMod;
	};
	void Normalize(float fValue);
	//计算 x y w h区域里面的diff, 返回diff数值
	template<class T1>
	T	Distance(CVector<T1>& other, int nW, int nH, int nRectX, int nRectY, int nRectW, int nRectH)
	{
		T diff(0);
		T* pValue = &at(0);
		T1* pValue1 = &other.at(0);
		int nY1 = nRectY+nRectH;
		int nX1 = nRectX+nRectW;
		int nLinePos = 0;
		int nPos = 0;
		T diffTmp(0);
		for (int y=nRectY; y<nY1; y++)
		{
			nLinePos = y*nW;
			for (int x=nRectX; x<nX1; x++)
			{
				nPos = nLinePos + x;
				diffTmp = pValue[nPos] - pValue1[nPos];
				diff = diff + diffTmp*diffTmp;
			}
		}

		return diff;
	}
	template<class T1>
	T	Distance(T1* pValue1, int nW, int nH, int nRectX, int nRectY, int nRectW, int nRectH)
	{
		T diff(0);
		T* pValue = &at(0);
		//T1* pValue1 = &other.at(0);
		int nY1 = nRectY+nRectH;
		int nX1 = nRectX+nRectW;
		int nLinePos = 0;
		int nPos = 0;
		T diffTmp(0);
		for (int y=nRectY; y<nY1; y++)
		{
			nLinePos = y*nW;
			for (int x=nRectX; x<nX1; x++)
			{
				nPos = nLinePos + x;
				diffTmp = pValue[nPos] - pValue1[nPos];
				diff = diff + diffTmp*diffTmp;
			}
		}

		return diff;
	}
	//点积
	template<class T1>
	T	Dot(CVector<T1>& other)
	{
		T ret(0);
		T* pValue = &at(0);
		T1* pValue1 = &other.at(0);
		int nSize = size();
		for (int i=0; i<nSize; i++)
		{
			ret = ret + pValue[i]*pValue1[i];
		}

		return ret;
	}

	template<class T1>
	T	Dot(T1* pValue1, int nSize)
	{
		T ret(0);
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			ret = ret + pValue[i]*pValue1[i];
		}

		return ret;
	}
	//乘法
	template<class T1>
	void	Mul(T1* pValue1, int nSize)
	{
		T* pValue = &at(0);
		for (int i=0; i<nSize; i++)
		{
			 pValue[i] = pValue[i]*pValue1[i];
		}

	}

// 	template<class T1>
// 	T	Distance(T1* pValue1, int nSize)
// 	{
// 		T diff(0);
// 		T* pValue = &at(0);
// 		//T1* pValue1 = &other.at(0);
// 		T diffTmp(0);
// 
// 		for (int i=0; i<nSize; i++)
// 		{
// 			diffTmp = pValue[i] - pValue1[i];
// 			diff = diff + diffTmp*diffTmp;
// 		}
// 
// 		return diff;
// 	}
	//
	//
	
	//vector<T> m_arrValue;
};

 
template<class T>
void CVector<T>::Normalize(float fValue)
{
	float fMod = Mod();
	int nSize = size();
	T* pValue = &at(0);

	for (int i=0; i<nSize; i++)
	{
		pValue[i] = pValue[i]*fValue/fMod;
	}

}


template<class T>
CVector<T>::CVector(void)
{

}

template<class T>
CVector<T>::~CVector(void)
{

}
