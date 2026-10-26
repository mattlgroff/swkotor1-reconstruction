#include "c/client_exo_app.h"

// FUNCTION: SWKOTOR 0x00602D40
void CClientExoAppInternal::PlayMovieQueue(int b)
{
    if (reinterpret_cast<CExoArrayList<CExoString>*>(&m_lstQueuedMovies)->count(
            CExoString(".\\movies\\55.bik")) != 0) {
        m_bEvilEndingHackInProgress = 1;
        SetInputState(0x80);
    }
    PlayMoviesInExoArrayList(reinterpret_cast<CExoArrayList<CExoString>*>(&m_lstQueuedMovies),
                             &b, 0, (unsigned long)b, &m_lstQueuedMovieFlags);
    m_lstQueuedMovies.num = 0;
    m_lstQueuedMovieFlags.num = 0;
}
