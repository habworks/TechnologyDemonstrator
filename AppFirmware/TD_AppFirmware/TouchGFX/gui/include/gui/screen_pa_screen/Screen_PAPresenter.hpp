#ifndef SCREEN_PAPRESENTER_HPP
#define SCREEN_PAPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include "Driver_LSI3DH.h"

using namespace touchgfx;

class Screen_PAView;

class Screen_PAPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_PAPresenter(Screen_PAView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Screen_PAPresenter() {};

    // HAB ADDED:
    virtual void PWM_AccelerometerUpdate_Presenter(Type_PWM_AccelerometerScreen ScreenStatus);

private:
    Screen_PAPresenter();

    Screen_PAView& view;
};

#endif // SCREEN_PAPRESENTER_HPP
