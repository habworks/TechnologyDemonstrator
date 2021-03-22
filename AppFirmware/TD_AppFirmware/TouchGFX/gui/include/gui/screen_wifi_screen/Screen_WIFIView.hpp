#ifndef SCREEN_WIFIVIEW_HPP
#define SCREEN_WIFIVIEW_HPP

#include <gui_generated/screen_wifi_screen/Screen_WIFIViewBase.hpp>
#include <gui/screen_wifi_screen/Screen_WIFIPresenter.hpp>

class Screen_WIFIView : public Screen_WIFIViewBase
{
public:
    Screen_WIFIView();
    virtual ~Screen_WIFIView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // HAB ADDED:
    virtual void updateScreen_WIFI(void);
    virtual void click_WIFI_UI_Up1(void);
    virtual void click_WIFI_UI_Down1(void);
    virtual void click_WIFI_UI_Up2(void);
    virtual void click_WIFI_UI_Down2(void);
protected:
};

#endif // SCREEN_WIFIVIEW_HPP
