#include <gui/screen_music_screen/Screen_MusicView.hpp>
#include "MainSupport.h"
#include "cmsis_os2.h"

extern osMessageQueueId_t AudioQueueHandle;

Screen_MusicView::Screen_MusicView()
{

}

void Screen_MusicView::setupScreen()
{
    Screen_MusicViewBase::setupScreen();
}

void Screen_MusicView::tearDownScreen()
{
    Screen_MusicViewBase::tearDownScreen();
}

// HAB ADDED:
void Screen_MusicView::updateScreen_Music(void)
{
	TechDemoStatus.ActiveScreen = MUSIC_SCREEN;
}

void Screen_MusicView::musicPlay(void)
{
	Type_AudioMsg *Msg = &TechDemoStatus.MusicScreen.AudioMsg;

	TechDemoStatus.MusicScreen.AudioMsg.IsMusic = TRUE;
	osMessageQueuePut(AudioQueueHandle, Msg, 0U, 0U);
}
