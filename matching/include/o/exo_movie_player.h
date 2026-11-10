// Worker O matching headers: CExoMoviePlayer, the pimpl front end over
// CExoMoviePlayerInternal (bucket d, 0x00404BB0-0x00405E50).
#ifndef O_EXO_MOVIE_PLAYER_H
#define O_EXO_MOVIE_PLAYER_H

class CExoString;

template <class T>
class CExoArrayList {
public:
    T* element;
    int num;
    int array_size;
};

class CExoMoviePlayerInternal {
public:
    CExoMoviePlayerInternal();                      // 0x00405D70
    ~CExoMoviePlayerInternal();                     // 0x00405A50
    void CancelMovie(int nSkip, int bForce);        // 0x00404C40
    int IsMoviePlaying();                           // 0x00404DD0
    void ShowFadePanel();                           // 0x00404E00
    void HideFadePanel();                           // 0x00404F00
    int GetNumberOfMoviesToPlay();                  // 0x00405040
    void Resume();                                  // 0x00405050
    void PlayMoviesAsync(const CExoArrayList<CExoString>& lstMovies, int nFlags,
                         const CExoArrayList<int>* pSkippable); // 0x00405E50

    char pad_0x0[0xa0];
};

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
};

#endif
