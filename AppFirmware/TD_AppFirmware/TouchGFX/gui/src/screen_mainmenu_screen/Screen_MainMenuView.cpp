#include <gui/screen_mainmenu_screen/Screen_MainMenuView.hpp>

Screen_MainMenuView::Screen_MainMenuView():
// Hab Add: Callback declaration and function to handle the event need to be bound to the view object, and bind which function to handle the event.
image_AD_ClickedCallback(this, &Screen_MainMenuView::image_AD_ClickHandler)
{

}

void Screen_MainMenuView::setupScreen()
{
    Screen_MainMenuViewBase::setupScreen();
    // Hab Add: Tell image widget which callback to use when touched
    image_AD.setClickAction(image_AD_ClickedCallback);
}

void Screen_MainMenuView::tearDownScreen()
{
    Screen_MainMenuViewBase::tearDownScreen();
}





// HAB ADD: Call back function for image_AD Clicked
void Screen_MainMenuView::image_AD_ClickHandler(const Image& i, const ClickEvent& evt )
{
	if (&i == &image_AD)
	{
		static int Count = 0;
		Count++;
	}
}
