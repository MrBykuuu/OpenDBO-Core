////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//This is custom real-time type information.
//First of all, template, multiple inheritance has not been verified. General inheritance is fully implemented.
//The reference book is Game Coding Complete.
//Since it is applied by comparing strings, do not do it in real time.
//See the implementation below.
//
//First of all, in the header, it is similar to MFC.
//   
//
/*
class CHodong
{
	NTL_RTTI_SYSTEM_DECL;
};

class CHyungseok: public CHodong
{
	NTL_RTTI_SYSTEM_DECL;

	void HodongIsTheBest();
};

class CWoody: public CHodong
{
	NTL_RTTI_SYSTEM_DECL;

	void UdongdongIsFool();
};

class CJuncheol: public CHodong
{
	NTL_RTTI_SYSTEM_DECL;

	void WhyAlwaysNagOnlyMe();
};
*/
// The implementation department does the following:
//
// It is said that we inherited Hodong from our parents.
//
// NTL_RTTI_SYSTEM_IMPL_NOPARENT (CHodong);
// NTL_RTTI_SYSTEM_IMPL(CHyungseok, CHodong)
// NTL_RTTI_SYSTEM_IMPL(Cwoody, CHodong);
// NTL_RTTI_SYSTEM_IMPL(CJuncheol, CHodong);


// Do it like this
// CHodong *pATM1 = new CHyungseok;
// CHodong *pATM2 = new CWoody;
// CHodong *pATM3 = new CJuncheol;
//
// It would roughly go like this. Then, if a random pointer were to find out the dust in real time,
// It is as follows:
//
// void OnHodongCmdLBtnClick ()
//{
//
//	// If you want to know if the current Hodong pointer is Juncheol pointer,
//	if(pATM->GetCRTTIS().DerivesFrom(Cjuncheol::s_CRTTIS))
//  {
//      // And here, we type cast and call Juncheol¡¯s function.
//      reinterpret_cast<CJuncheol*>(pATM3)->WhyAlwaysNagOnlyMe();
//	    
//      // It goes like this.
//  }
//
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define NTL_RTTI_SYSTEM_DECL \
public:\
	virtual const CNtlRTTI& GetCRTTIS() const { return s_CRTTIS; }\
	static const CNtlRTTI s_CRTTIS;

#define NTL_RTTI_SYSTEM_IMPL_NOPARENT(name) \
const CNtlRTTI name::s_CRTTIS(#name);

#define NTL_RTTI_SYSTEM_IMPL(name, parent) \
const CNtlRTTI name::s_CRTTIS(#name, parent::s_CRTTIS);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlRTTI
{
public:
	CNtlRTTI(const std::string& className);
	CNtlRTTI(const std::string& className, const CNtlRTTI& baseRTTI);

public:
    const std::string& GetClassName() const;
    bool IsExactly(const CNtlRTTI& rtti) const;
    bool DerivesFrom(const CNtlRTTI& rtti) const;

private:
    CNtlRTTI(const CNtlRTTI& obj);
    CNtlRTTI& operator = (const CNtlRTTI& obj);
	
private:
    const std::string m_className;
    const CNtlRTTI *m_pBaseRTTI;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////