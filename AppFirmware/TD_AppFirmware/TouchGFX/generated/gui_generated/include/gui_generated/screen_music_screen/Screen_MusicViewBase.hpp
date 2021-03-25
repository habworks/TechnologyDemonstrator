/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN_MUSICVIEWBASE_HPP
#define SCREEN_MUSICVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen_music_screen/Screen_MusicPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/ScalableImage.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <touchgfx/containers/Slider.hpp>

class Screen_MusicViewBase : public touchgfx::View<Screen_MusicPresenter>
{
public:
    Screen_MusicViewBase();
    virtual ~Screen_MusicViewBase() {}
    virtual void setupScreen();
    virtual void afterTransition();

    /*
     * Virtual Action Handlers
     */
    virtual void updateScreen_Music()
    {
        // Override and implement this function in Screen_Music
    }

    virtual void musicPause()
    {
        // Override and implement this function in Screen_Music
    }

    virtual void musicPlay()
    {
        // Override and implement this function in Screen_Music
    }

    virtual void musicStop()
    {
        // Override and implement this function in Screen_Music
    }

    virtual void musicVolume(int value)
    {
        // Override and implement this function in Screen_Music
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
    touchgfx::ButtonWithIcon buttonWithIconMusic_Home;
    touchgfx::Box box_Highlight1;
    touchgfx::BoxProgress boxProgress_Music;
    touchgfx::Box box_Highlight2;
    touchgfx::ButtonWithIcon buttonWithIcon_Play;
    touchgfx::ButtonWithIcon buttonWithIcon_Pause;
    touchgfx::ButtonWithIcon buttonWithIcon_Stop;
    touchgfx::Slider slider_Volume;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen_MusicViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<Screen_MusicViewBase, const touchgfx::Slider&, int> sliderValueChangedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);

};

#endif // SCREEN_MUSICVIEWBASE_HPP
