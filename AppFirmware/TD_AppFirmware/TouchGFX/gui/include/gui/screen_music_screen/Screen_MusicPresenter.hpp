#ifndef SCREEN_MUSICPRESENTER_HPP
#define SCREEN_MUSICPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_MusicView;

class Screen_MusicPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_MusicPresenter(Screen_MusicView& v);

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

    virtual ~Screen_MusicPresenter() {};

private:
    Screen_MusicPresenter();

    Screen_MusicView& view;
};

#endif // SCREEN_MUSICPRESENTER_HPP
