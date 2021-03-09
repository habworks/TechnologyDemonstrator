/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <new>
#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/transitions/NoTransition.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/hal/HAL.hpp>
#include<platform/driver/lcd/LCD24bpp.hpp>
#include <gui/screen_opening_screen/Screen_OpeningView.hpp>
#include <gui/screen_opening_screen/Screen_OpeningPresenter.hpp>
#include <gui/screen_mainmenu_screen/Screen_MainMenuView.hpp>
#include <gui/screen_mainmenu_screen/Screen_MainMenuPresenter.hpp>
#include <gui/screen_pa_screen/Screen_PAView.hpp>
#include <gui/screen_pa_screen/Screen_PAPresenter.hpp>
#include <gui/screen_ad_screen/Screen_ADView.hpp>
#include <gui/screen_ad_screen/Screen_ADPresenter.hpp>

using namespace touchgfx;

FrontendApplicationBase::FrontendApplicationBase(Model& m, FrontendHeap& heap)
    : touchgfx::MVPApplication(),
      transitionCallback(),
      frontendHeap(heap),
      model(m)
{
    touchgfx::HAL::getInstance()->setDisplayOrientation(touchgfx::ORIENTATION_LANDSCAPE);
    touchgfx::Texts::setLanguage(GB);
    reinterpret_cast<touchgfx::LCD24bpp&>(touchgfx::HAL::lcd()).enableTextureMapperAll();
}

/*
 * Screen Transition Declarations
 */

// Screen_Opening

void FrontendApplicationBase::gotoScreen_OpeningScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_OpeningScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_OpeningScreenNoTransitionImpl()
{
    touchgfx::makeTransition<Screen_OpeningView, Screen_OpeningPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplicationBase::gotoScreen_OpeningScreenSlideTransitionWest()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_OpeningScreenSlideTransitionWestImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_OpeningScreenSlideTransitionWestImpl()
{
    touchgfx::makeTransition<Screen_OpeningView, Screen_OpeningPresenter, touchgfx::SlideTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplicationBase::gotoScreen_OpeningScreenCoverTransitionWest()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_OpeningScreenCoverTransitionWestImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_OpeningScreenCoverTransitionWestImpl()
{
    touchgfx::makeTransition<Screen_OpeningView, Screen_OpeningPresenter, touchgfx::CoverTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Screen_MainMenu

void FrontendApplicationBase::gotoScreen_MainMenuScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_MainMenuScreenNoTransitionImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_MainMenuScreenNoTransitionImpl()
{
    touchgfx::makeTransition<Screen_MainMenuView, Screen_MainMenuPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplicationBase::gotoScreen_MainMenuScreenSlideTransitionWest()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_MainMenuScreenSlideTransitionWestImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_MainMenuScreenSlideTransitionWestImpl()
{
    touchgfx::makeTransition<Screen_MainMenuView, Screen_MainMenuPresenter, touchgfx::SlideTransition<WEST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Screen_PA

void FrontendApplicationBase::gotoScreen_PAScreenSlideTransitionEast()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_PAScreenSlideTransitionEastImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_PAScreenSlideTransitionEastImpl()
{
    touchgfx::makeTransition<Screen_PAView, Screen_PAPresenter, touchgfx::SlideTransition<EAST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

// Screen_AD

void FrontendApplicationBase::gotoScreen_ADScreenSlideTransitionEast()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, &FrontendApplication::gotoScreen_ADScreenSlideTransitionEastImpl);
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplicationBase::gotoScreen_ADScreenSlideTransitionEastImpl()
{
    touchgfx::makeTransition<Screen_ADView, Screen_ADPresenter, touchgfx::SlideTransition<EAST>, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
