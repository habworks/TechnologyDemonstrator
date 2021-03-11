#include <gui/screen_pa_screen/Screen_PAView.hpp>
#include "MainSupport.h"
#include <touchgfx/Color.hpp>

Screen_PAView::Screen_PAView()
{

}

void Screen_PAView::setupScreen()
{
    Screen_PAViewBase::setupScreen();
}

void Screen_PAView::tearDownScreen()
{
    Screen_PAViewBase::tearDownScreen();
}


// HAB ADDED:
void Screen_PAView::updateScreen_PA(void)
{
	TechDemoStatus.ActiveScreen = PA_SCREEN;
}


void Screen_PAView::PWM_AccelerometerUpdate_View(Type_PWM_AccelerometerScreen ScreenStatus)
{
	// UPDATE TILT:
	// X Axis display
	memset(&textAreaXaxisBuffer, 0, TEXTAREAXAXIS_SIZE);
	Unicode::snprintfFloat(textAreaXaxisBuffer, sizeof(textAreaXaxisBuffer), "%3.1f", ScreenStatus.AccelerometerAngle.X_Degrees);
	textAreaXaxis.invalidate();
	// X Axis display
	memset(&textAreaYaxisBuffer, 0, TEXTAREAYAXIS_SIZE);
	Unicode::snprintfFloat(textAreaYaxisBuffer, sizeof(textAreaYaxisBuffer), "%3.1f", ScreenStatus.AccelerometerAngle.Y_Degrees);
	textAreaYaxis.invalidate();
	// X Axis display
	memset(&textAreaZaxisBuffer, 0, TEXTAREAZAXIS_SIZE);
	Unicode::snprintfFloat(textAreaZaxisBuffer, sizeof(textAreaZaxisBuffer), "%3.1f", ScreenStatus.AccelerometerAngle.Z_Degrees);
	textAreaZaxis.invalidate();

	// UDPDATE BOX FILED COLOR
	uint8_t RedColor = (uint8_t)(((255 * ScreenStatus.PWM_Percent.Red_LED ) / 100.0) + ROUNDING);
	uint8_t GreenColor = (uint8_t)(((255 * ScreenStatus.PWM_Percent.Green_LED) / 100.0) + ROUNDING);
	boxColor.setColor(touchgfx::Color::getColorFrom24BitRGB(RedColor, GreenColor, 0));
	boxColor.invalidate();
}


void Screen_PAView::greenSliderValueChanged(int Value)
{
	TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Green_LED = Value;
}


void Screen_PAView::redSliderValueChanged(int Value)
{
	TechDemoStatus.PWM_AccelerometerScreen.PWM_Percent.Red_LED = Value;
}
