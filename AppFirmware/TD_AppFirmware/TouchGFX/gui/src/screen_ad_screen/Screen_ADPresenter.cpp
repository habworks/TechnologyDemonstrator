#include <gui/screen_ad_screen/Screen_ADView.hpp>
#include <gui/screen_ad_screen/Screen_ADPresenter.hpp>

Screen_ADPresenter::Screen_ADPresenter(Screen_ADView& v)
    : view(v)
{

}

void Screen_ADPresenter::activate()
{

}

void Screen_ADPresenter::deactivate()
{

}


// HAB ADDED:
void Screen_ADPresenter::analogDigitalUpdate_Presenter(float AnalogValue, char DigitalValueChar)
{
	view.analogDigitalUpdate_View(AnalogValue, DigitalValueChar);
}
