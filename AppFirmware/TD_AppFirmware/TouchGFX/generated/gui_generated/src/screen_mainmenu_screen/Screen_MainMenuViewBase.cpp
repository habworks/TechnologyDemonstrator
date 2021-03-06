/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen_mainmenu_screen/Screen_MainMenuViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

Screen_MainMenuViewBase::Screen_MainMenuViewBase() :
    buttonCallback(this, &Screen_MainMenuViewBase::buttonCallbackHandler)
{

    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    scalableImage1.setBitmap(touchgfx::Bitmap(BITMAP_BACKGROUND2_ID));
    scalableImage1.setPosition(-2, 0, 600, 444);
    scalableImage1.setScalingAlgorithm(touchgfx::ScalableImage::NEAREST_NEIGHBOR);

    textArea1.setXY(147, 6);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(57, 224, 23));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID6));

    image_AD.setXY(266, 193);
    image_AD.setBitmap(touchgfx::Bitmap(BITMAP_ADI_R_ID));

    buttonRestart.setXY(405, 203);
    buttonRestart.setBitmaps(touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_DARK_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_REFRESH_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_REFRESH_32_ID));
    buttonRestart.setIconXY(15, 16);
    buttonRestart.setAction(buttonCallback);

    button_AD.setXY(32, 60);
    button_AD.setBitmaps(touchgfx::Bitmap(BITMAP_ADI_G_ID), touchgfx::Bitmap(BITMAP_ADI_G_ID));
    button_AD.setAction(buttonCallback);

    button_PA.setXY(134, 60);
    button_PA.setBitmaps(touchgfx::Bitmap(BITMAP_PA_G_ID), touchgfx::Bitmap(BITMAP_PA_G_ID));
    button_PA.setAction(buttonCallback);

    button_WIFI.setXY(240, 60);
    button_WIFI.setBitmaps(touchgfx::Bitmap(BITMAP_WIFI_G_ID), touchgfx::Bitmap(BITMAP_WIFI_G_ID));
    button_WIFI.setAction(buttonCallback);

    add(__background);
    add(scalableImage1);
    add(textArea1);
    add(image_AD);
    add(buttonRestart);
    add(button_AD);
    add(button_PA);
    add(button_WIFI);
}

void Screen_MainMenuViewBase::setupScreen()
{

}

//Called when the screen transition ends
void Screen_MainMenuViewBase::afterTransition()
{
    //Interaction5
    //When screen transition ends call virtual function
    //Call updateScreen_MainMenu
    updateScreen_MainMenu();
}

void Screen_MainMenuViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &buttonRestart)
    {
        //Interaction2
        //When buttonRestart clicked change screen to Screen_Opening
        //Go to Screen_Opening with screen transition towards West
        application().gotoScreen_OpeningScreenCoverTransitionWest();
    }
    else if (&src == &button_AD)
    {
        //Interaction3
        //When button_AD clicked change screen to Screen_AD
        //Go to Screen_AD with screen transition towards East
        application().gotoScreen_ADScreenSlideTransitionEast();
    }
    else if (&src == &button_PA)
    {
        //Interaction4
        //When button_PA clicked change screen to Screen_PA
        //Go to Screen_PA with screen transition towards East
        application().gotoScreen_PAScreenSlideTransitionEast();
    }
    else if (&src == &button_WIFI)
    {
        //Interaction6
        //When button_WIFI clicked change screen to Screen_WIFI
        //Go to Screen_WIFI with screen transition towards East
        application().gotoScreen_WIFIScreenSlideTransitionEast();
    }
}
