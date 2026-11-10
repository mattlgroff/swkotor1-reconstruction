// CExoMoviePlayer, 0x0070C170-0x0070C250. Each method forwards to
// CExoMoviePlayerInternal.
#include "o/exo_movie_player.h"

void operator delete(void* p);

// FUNCTION: SWKOTOR 0x0070C170
CExoMoviePlayer::CExoMoviePlayer()
{
    m_pInternal = new CExoMoviePlayerInternal();
}

// FUNCTION: SWKOTOR 0x0070C1E0
void CExoMoviePlayer::CancelMovie(int nSkip, int bForce)
{
    m_pInternal->CancelMovie(nSkip, bForce);
}

// FUNCTION: SWKOTOR 0x0070C1F0
int CExoMoviePlayer::IsMoviePlaying()
{
    return m_pInternal->IsMoviePlaying();
}

// FUNCTION: SWKOTOR 0x0070C200
void CExoMoviePlayer::PlayMoviesAsync(const CExoArrayList<CExoString>& lstMovies, int nFlags,
                                      const CExoArrayList<int>* pSkippable)
{
    m_pInternal->PlayMoviesAsync(lstMovies, nFlags, pSkippable);
}

// FUNCTION: SWKOTOR 0x0070C210
void CExoMoviePlayer::ShowFadePanel()
{
    m_pInternal->ShowFadePanel();
}

// FUNCTION: SWKOTOR 0x0070C220
void CExoMoviePlayer::HideFadePanel()
{
    m_pInternal->HideFadePanel();
}

// FUNCTION: SWKOTOR 0x0070C230
int CExoMoviePlayer::GetNumberOfMoviesToPlay()
{
    if (m_pInternal) {
        return m_pInternal->GetNumberOfMoviesToPlay();
    }
    return 0;
}

// FUNCTION: SWKOTOR 0x0070C240
void CExoMoviePlayer::RestartMoviesAsync()
{
    m_pInternal->Resume();
}

// FUNCTION: SWKOTOR 0x0070C250
CExoMoviePlayer::~CExoMoviePlayer()
{
    if (m_pInternal) {
        delete m_pInternal;
        m_pInternal = 0;
    }
}
