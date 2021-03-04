#ifndef SCREEN_OPENINGPRESENTER_HPP
#define SCREEN_OPENINGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_OpeningView;

class Screen_OpeningPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_OpeningPresenter(Screen_OpeningView& v);

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

    virtual ~Screen_OpeningPresenter() {};

private:
    Screen_OpeningPresenter();

    Screen_OpeningView& view;
};

#endif // SCREEN_OPENINGPRESENTER_HPP
