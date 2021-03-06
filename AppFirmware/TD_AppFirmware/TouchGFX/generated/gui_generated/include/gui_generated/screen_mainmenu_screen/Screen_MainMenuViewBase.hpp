/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN_MAINMENUVIEWBASE_HPP
#define SCREEN_MAINMENUVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_mainmenu_screen/Screen_MainMenuPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/mixins/ClickListener.hpp>

class Screen_MainMenuViewBase : public touchgfx::View<Screen_MainMenuPresenter>
{
public:
    Screen_MainMenuViewBase();
    virtual ~Screen_MainMenuViewBase() {}
    virtual void setupScreen();
    virtual void afterTransition();

    /*
     * Virtual Action Handlers
     */
    virtual void updateScreen_MainMenu()
    {
        // Override and implement this function in Screen_MainMenu
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::ScalableImage scalableImage1;
    touchgfx::TextArea textArea1;
    touchgfx::ClickListener< touchgfx::Image > image_AD;
    touchgfx::ButtonWithIcon buttonRestart;
    touchgfx::Button button_AD;
    touchgfx::Button button_PA;
    touchgfx::Button button_WIFI;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen_MainMenuViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // SCREEN_MAINMENUVIEWBASE_HPP
