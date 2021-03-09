#ifndef SCREEN_MAINMENUVIEW_HPP
#define SCREEN_MAINMENUVIEW_HPP

#include <gui_generated/screen_mainmenu_screen/Screen_MainMenuViewBase.hpp>
#include <gui/screen_mainmenu_screen/Screen_MainMenuPresenter.hpp>

class Screen_MainMenuView : public Screen_MainMenuViewBase
{
public:
    Screen_MainMenuView();
    virtual ~Screen_MainMenuView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // HAB ADDED:
    // Declaring callback Main Menu ICONs
    void image_AD_ClickHandler(const Image& i, const ClickEvent& e);
    virtual void updateScreen_MainMenu(void);

protected:
    // Hab Add: Declaring callback types
    Callback<Screen_MainMenuView, const Image&, const ClickEvent&> image_AD_ClickedCallback;
};

#endif // SCREEN_MAINMENUVIEW_HPP
