#ifndef SCREEN_MAINMENUPRESENTER_HPP
#define SCREEN_MAINMENUPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen_MainMenuView;

class Screen_MainMenuPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen_MainMenuPresenter(Screen_MainMenuView& v);

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

    virtual ~Screen_MainMenuPresenter() {};

private:
    Screen_MainMenuPresenter();

    Screen_MainMenuView& view;
};

#endif // SCREEN_MAINMENUPRESENTER_HPP
