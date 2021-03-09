#include <gui/screen_ad_screen/Screen_ADView.hpp>
#include "MainSupport.h"
#include "AnalogDigitalTaskSupport.h"

Screen_ADView::Screen_ADView()
{

}

void Screen_ADView::setupScreen()
{
    Screen_ADViewBase::setupScreen();
}

void Screen_ADView::tearDownScreen()
{
    Screen_ADViewBase::tearDownScreen();
}


// HAB ADDED:
void Screen_ADView::analogDigitalUpdate_View(float AnalogValue, char DigitalValueChar)
{
	// ANALOG VOLTAGE DISPLAY
	memset(&textAreaAnalogBuffer, 0, TEXTAREAANALOG_SIZE);
	Unicode::snprintfFloat(textAreaAnalogBuffer, sizeof(textAreaAnalogBuffer), "%1.3f", AnalogValue);
	textAreaAnalog.invalidate();

	// ANALOG VOLTAGE GRAPH
//	if (TechDemoStatus.AnalogDigitalScreen.RefreshGraph == TRUE)
//	{
//		dynamicGraph1.setGraphRange(0, 100, 0, 100);
//		dynamicGraph1.clear();
//		dynamicGraph1.invalidate();
//	}

	uint8_t YRange = dynamicGraph1.getGraphRangeYMaxAsInt();
	float ADC_Value = (AnalogValue / ADC_VREF) * YRange;
	dynamicGraph1.addDataPoint(ADC_Value);


	// HEX SWITCH DISPLAY
	memset(&textAreaDigitalBuffer, 0, TEXTAREADIGITAL_SIZE);
	Unicode::strncpy(textAreaDigitalBuffer, &DigitalValueChar, 1);
	textAreaDigital.invalidate();

}


void Screen_ADView::updateScreen_AD(void)
{
	TechDemoStatus.ActiveScreen = AD_SCREEN;
}
