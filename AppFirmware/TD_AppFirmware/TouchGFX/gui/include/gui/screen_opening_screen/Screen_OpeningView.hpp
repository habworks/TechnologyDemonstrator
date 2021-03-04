#ifndef SCREEN_OPENINGVIEW_HPP
#define SCREEN_OPENINGVIEW_HPP

#include <gui_generated/screen_opening_screen/Screen_OpeningViewBase.hpp>
#include <gui/screen_opening_screen/Screen_OpeningPresenter.hpp>

class Screen_OpeningView : public Screen_OpeningViewBase
{
public:
    Screen_OpeningView();
    virtual ~Screen_OpeningView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_OPENINGVIEW_HPP
