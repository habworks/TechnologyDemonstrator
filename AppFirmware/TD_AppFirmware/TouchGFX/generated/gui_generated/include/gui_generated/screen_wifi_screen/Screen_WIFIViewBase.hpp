/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN_WIFIVIEWBASE_HPP
#define SCREEN_WIFIVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_wifi_screen/Screen_WIFIPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>

class Screen_WIFIViewBase : public touchgfx::View<Screen_WIFIPresenter>
{
public:
    Screen_WIFIViewBase();
    virtual ~Screen_WIFIViewBase() {}
    virtual void setupScreen();
    virtual void afterTransition();

    /*
     * Virtual Action Handlers
     */
    virtual void click_WIFI_UI_Up1()
    {
        // Override and implement this function in Screen_WIFI
    }

    virtual void click_WIFI_UI_Down1()
    {
        // Override and implement this function in Screen_WIFI
    }

    virtual void updateScreen_WIFI()
    {
        // Override and implement this function in Screen_WIFI
    }

    virtual void click_WIFI_UI_Up2()
    {
        // Override and implement this function in Screen_WIFI
    }

    virtual void click_WIFI_UI_Down2()
    {
        // Override and implement this function in Screen_WIFI
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
    touchgfx::TextArea textArea2;
    touchgfx::TextArea textArea2_1;
    touchgfx::TextArea textArea2_1_1;
    touchgfx::ButtonWithIcon buttonWithIcon_UI_Down1;
    touchgfx::ButtonWithIcon buttonWithIcon_UI_Up1;
    touchgfx::ButtonWithIcon buttonWithIcon_UI_Up2;
    touchgfx::ButtonWithIcon buttonWithIcon_UI_Down2;
    touchgfx::TextArea textArea2_2;
    touchgfx::TextArea textArea2_2_1;
    touchgfx::TextArea textArea2_2_1_1;
    touchgfx::TextArea textArea2_3;
    touchgfx::ButtonWithIcon buttonWithIconWIFI_Home;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen_WIFIViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // SCREEN_WIFIVIEWBASE_HPP
