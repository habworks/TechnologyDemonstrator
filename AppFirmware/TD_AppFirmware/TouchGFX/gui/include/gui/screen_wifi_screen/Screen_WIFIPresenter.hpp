#ifndef SCREEN_WIFIPRESENTER_HPP
#define SCREEN_WIFIPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_WIFIView;

class Screen_WIFIPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_WIFIPresenter(Screen_WIFIView& v);

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

    virtual ~Screen_WIFIPresenter() {};

private:
    Screen_WIFIPresenter();

    Screen_WIFIView& view;
};

#endif // SCREEN_WIFIPRESENTER_HPP
