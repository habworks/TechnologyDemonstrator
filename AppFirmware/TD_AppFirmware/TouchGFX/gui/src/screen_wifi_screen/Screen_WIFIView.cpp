#include <gui/screen_wifi_screen/Screen_WIFIView.hpp>
#include "MainSupport.h"

Screen_WIFIView::Screen_WIFIView()
{

}

void Screen_WIFIView::setupScreen()
{
    Screen_WIFIViewBase::setupScreen();
}

void Screen_WIFIView::tearDownScreen()
{
    Screen_WIFIViewBase::tearDownScreen();
}


// HAB ADDED:
void Screen_WIFIView::updateScreen_WIFI(void)
{
	TechDemoStatus.ActiveScreen = WIFI_SCREEN;
}


void Screen_WIFIView::click_WIFI_UI_Up1(void)
{
	TechDemoStatus.WifiScreen.UpdateCount1++;
}

void Screen_WIFIView::click_WIFI_UI_Down1(void)
{
	TechDemoStatus.WifiScreen.UpdateCount1--;
}

void Screen_WIFIView::click_WIFI_UI_Up2(void)
{
	TechDemoStatus.WifiScreen.UpdateCount2++;
}

void Screen_WIFIView::click_WIFI_UI_Down2(void)
{
	TechDemoStatus.WifiScreen.UpdateCount2--;
}
