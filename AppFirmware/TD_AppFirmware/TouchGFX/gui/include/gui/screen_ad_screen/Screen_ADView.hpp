#ifndef SCREEN_ADVIEW_HPP
#define SCREEN_ADVIEW_HPP

#include <gui_generated/screen_ad_screen/Screen_ADViewBase.hpp>
#include <gui/screen_ad_screen/Screen_ADPresenter.hpp>

class Screen_ADView : public Screen_ADViewBase
{
public:
    Screen_ADView();
    virtual ~Screen_ADView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // HAB ADDED:
    virtual void analogDigitalUpdate_View(float, char);
    virtual void updateScreen_AD(void);
protected:
};

#endif // SCREEN_ADVIEW_HPP
