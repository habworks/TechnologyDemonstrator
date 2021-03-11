#ifndef SCREEN_PAVIEW_HPP
#define SCREEN_PAVIEW_HPP

#include <gui_generated/screen_pa_screen/Screen_PAViewBase.hpp>
#include <gui/screen_pa_screen/Screen_PAPresenter.hpp>
#include "Driver_LSI3DH.h"

class Screen_PAView : public Screen_PAViewBase
{
public:
    Screen_PAView();
    virtual ~Screen_PAView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    // HAB ADDED:
    virtual void updateScreen_PA(void);
    virtual void PWM_AccelerometerUpdate_View(Type_PWM_AccelerometerScreen);
    virtual void redSliderValueChanged(int);
    virtual void greenSliderValueChanged(int);
protected:
};

#endif // SCREEN_PAVIEW_HPP
