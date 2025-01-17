#include "precomp_ntlpresentation.h"
#include "NtlPLRefractionEffect.h"
#include "NtlMath.h"
#include "NtlDebug.h"

CNtlPLRefractionEffect::CNtlPLRefractionEffect(void)
{
    m_pUVVerticesOrg = NULL;
    m_nVertexCnt = 0;
    m_pVertices = NULL;    
}

CNtlPLRefractionEffect::~CNtlPLRefractionEffect(void)
{
	NTL_ARRAY_DELETE( m_pUVVerticesOrg );

    m_pVertices = NULL;           

    for each(WaveElement* pWave in m_listWaveElement)
    {
        NTL_DELETE(pWave);
    }

    m_listWaveElement.clear();
}

void CNtlPLRefractionEffect::Init(RwIm2DVertex* pVertices, RwInt32 nVertexCount)
{
    //Initialization of wave source list    
    m_listWaveElement.clear();

    //Initializing global variables    
    m_pVertices = pVertices;
    m_nVertexCnt = nVertexCount;    

    m_pUVVerticesOrg = NTL_NEW RwV2d[nVertexCount];    
    for(int i = 0; i < nVertexCount; ++i)
    {
        m_pUVVerticesOrg[i].x = pVertices[i].u;
        m_pUVVerticesOrg[i].y = pVertices[i].v;
    }
}

void CNtlPLRefractionEffect::InsertWave( RwReal fX, RwReal fY, RwReal fRound, RwReal fMaxHeight ) 
{
    WaveElement* wave = NTL_NEW WaveElement();
    wave->v2Pos.x = fX;
    wave->v2Pos.y = fY;
    wave->fRound = fRound;
    wave->fMaxHeight = fMaxHeight;
    wave->fDelta = 0.0f;

    m_listWaveElement.push_back(wave);
}

void CNtlPLRefractionEffect::Update( RwReal fElapsedTime ) 
{
    RwReal fDistance, fHeight;    
    RwV2d  v2PosVertex, v2UV;

    std::list<WaveElement*>::iterator it = m_listWaveElement.begin();
    while(it != m_listWaveElement.end())
    {
        WaveElement* pWave = *it;
        
        pWave->fDelta += fElapsedTime;
        
        for(int i = 0; i < m_nVertexCnt; ++i)
        {
            v2PosVertex.x = m_pVertices[i].x;
            v2PosVertex.y = m_pVertices[i].y;            
            fDistance = CNtlMath::GetLength(pWave->v2Pos, v2PosVertex);                                
            if(fDistance > pWave->fRound) // Skip if the distance from the wave source exceeds the critical point.
                continue;

            fDistance *= 0.01f;

            // Gaussian function
            fHeight = max(exp(-(fDistance - pWave->fDelta) * (fDistance - pWave->fDelta)),
                          exp(-(fDistance + pWave->fDelta) * (fDistance + pWave->fDelta)));            
            fHeight *= pWave->fMaxHeight * (pWave->fDelta / pWave->fRound + 1);            
            
            RwV2d vTrans = v2PosVertex - pWave->v2Pos;
            RwV2dNormalize(&vTrans, &vTrans);            
            v2UV = m_pUVVerticesOrg[i] + vTrans * fHeight;
            m_pVertices[i].u = v2UV.x;
            m_pVertices[i].v = v2UV.y;
        }

        // Waves that have passed the critical time are deleted.
        if(pWave->fDelta > pWave->fRound)
        {
            it = m_listWaveElement.erase(it);
            NTL_DELETE(pWave);
        }
        else
        {
            ++it;
        }
    }
}