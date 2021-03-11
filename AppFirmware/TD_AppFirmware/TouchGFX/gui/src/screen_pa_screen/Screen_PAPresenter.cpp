#include <gui/screen_pa_screen/Screen_PAView.hpp>
#include <gui/screen_pa_screen/Screen_PAPresenter.hpp>

Screen_PAPresenter::Screen_PAPresenter(Screen_PAView& v)
    : view(v)
{

}

void Screen_PAPresenter::activate()
{

}

void Screen_PAPresenter::deactivate()
{

}


// HAB ADDED:
void Screen_PAPresenter::PWM_AccelerometerUpdate_Presenter(Type_PWM_AccelerometerScreen ScreenStatus)
{
	view.PWM_AccelerometerUpdate_View(ScreenStatus);
}
