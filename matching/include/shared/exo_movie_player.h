// exo_movie_player.h -- CExoMoviePlayer
//
// One declaration for every unit (CODE_RULES.md, R1), from the fullest copy
// (include/o/exo_movie_player.h); the other copies' members fall on its members.
#ifndef SHARED_EXO_MOVIE_PLAYER_H
#define SHARED_EXO_MOVIE_PLAYER_H

#include "shared/exo_string.h"

template <class T> class CExoArrayList;
class CExoMoviePlayerInternal;
class CExoStringMovieList;

class CExoMoviePlayer {
public:
    CExoMoviePlayer();
    ~CExoMoviePlayer();
    void CancelMovie(int nSkip, int bForce);
    int IsMoviePlaying();
    void PlayMoviesAsync(const CExoArrayList<CExoString>& lstMovies, int nFlags,
                         const CExoArrayList<int>* pSkippable);
    void ShowFadePanel();
    void HideFadePanel();
    int GetNumberOfMoviesToPlay();
    void RestartMoviesAsync();

    CExoMoviePlayerInternal* m_pInternal;           // 0x00

    // declared by other copies
    int GetState();
    void PlayMoviesAsync(const CExoStringMovieList& lstMovies, int a, int b);
    void PlayMoviesAsync(CExoArrayList<CExoString>& lstMovies, unsigned long nFlags, CExoArrayList<int>* pMovieFlags);
};

#endif
