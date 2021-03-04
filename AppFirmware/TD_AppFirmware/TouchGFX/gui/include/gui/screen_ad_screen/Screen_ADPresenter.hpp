#ifndef SCREEN_ADPRESENTER_HPP
#define SCREEN_ADPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_ADView;

class Screen_ADPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_ADPresenter(Screen_ADView& v);

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

    virtual ~Screen_ADPresenter() {};

private:
    Screen_ADPresenter();

    Screen_ADView& view;
};

#endif // SCREEN_ADPRESENTER_HPP
