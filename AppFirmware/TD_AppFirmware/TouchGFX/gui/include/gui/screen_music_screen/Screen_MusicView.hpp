#ifndef SCREEN_MUSICVIEW_HPP
#define SCREEN_MUSICVIEW_HPP

#include <gui_generated/screen_music_screen/Screen_MusicViewBase.hpp>
#include <gui/screen_music_screen/Screen_MusicPresenter.hpp>

class Screen_MusicView : public Screen_MusicViewBase
{
public:
    Screen_MusicView();
    virtual ~Screen_MusicView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // HAB ADDED:
    virtual void updateScreen_Music(void);
    virtual void musicPlay(void);
protected:
};

#endif // SCREEN_MUSICVIEW_HPP
