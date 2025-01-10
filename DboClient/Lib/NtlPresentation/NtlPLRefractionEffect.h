#pragma once

struct WaveElement
{
    RwV2d   v2Pos;          //Refracted wave generation point
    RwReal  fRound;         //The maximum radius at which waves spread
    RwReal  fMaxHeight;     //Initial height of the wave (z value)
    RwReal  fDelta;         //Current travel time (and radius of current wave)
};

/**
 * \ingroup NtlPresentation
 * \brief Screen refraction reflection effect class
 *
 * \date 2008-08-20
 * \author agebreak
 */
class CNtlPLRefractionEffect
{
public:
    CNtlPLRefractionEffect(void);
    ~CNtlPLRefractionEffect(void);

    void Init(RwIm2DVertex* pVertices, RwInt32 nVertexCount);
    void Update(RwReal fElapsedTime);

    void InsertWave(RwReal fX, RwReal fY, RwReal fRound, RwReal fMaxHeight);

protected:

protected:
    std::list<WaveElement*> m_listWaveElement;           ///< List of wave sources
    RwInt32     m_nVertexCnt;    
    RwIm2DVertex* m_pVertices;
    RwV2d*      m_pUVVerticesOrg;
};
