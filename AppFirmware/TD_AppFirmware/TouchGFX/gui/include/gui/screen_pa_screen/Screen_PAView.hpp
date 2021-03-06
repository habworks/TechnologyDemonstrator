#ifndef SCREEN_PAVIEW_HPP
#define SCREEN_PAVIEW_HPP

#include <gui_generated/screen_pa_screen/Screen_PAViewBase.hpp>
#include <gui/screen_pa_screen/Screen_PAPresenter.hpp>

class Screen_PAView : public Screen_PAViewBase
{
public:
    Screen_PAView();
    virtual ~Screen_PAView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // SCREEN_PAVIEW_HPP
