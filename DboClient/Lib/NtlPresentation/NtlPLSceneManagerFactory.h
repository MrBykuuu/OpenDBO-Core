/*****************************************************************************
 *
 * File			: NtlPLSceneManagerFactroy.h
 * Author		: HyungSuk, Jang
 * Copyright	: (?)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation scene manager factroy class 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLSCENEMANAGER_FACTROY_H__
#define __NTL_PLSCENEMANAGER_FACTROY_H__

class CNtlPLSceneManager;

/**
 * \ingroup NtlPresentation
 * 
 *
 */

class CNtlPLSceneManagerFactory
{
private:

	static CNtlPLSceneManager *m_pSceneManager;		/**< activeµÈ scene manager instance pointer */
    	
public:

	/**
    *Function to obtain a scene manager instance.
	*
    *  \return Returns the scene manager instance pointer.
    */
	static CNtlPLSceneManager* GetInstance();
	
	/**
    *Register the active scene manager pointer.
	*
    *  \param scene manager pointer.
    */
	static void ActiveSceneManager(CNtlPLSceneManager *pSceneManager);
};


#endif
