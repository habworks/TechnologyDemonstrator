#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
// HAB ADD
#include "MainSupport.h"
#include "cmsis_os2.h"
extern osSemaphoreId_t BinarySemAnalogDigitalHandle;

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
	if (BinarySemAnalogDigitalHandle != NULL)  // Simple test to make sure the semaphore was created
	{
		if (osSemaphoreAcquire(BinarySemAnalogDigitalHandle, 10U) == osOK)
		{
			analogDigitalUpdate();
		}
	}
}



// HAB ADDED FUNCIONS FOR MODEL TICK
void Model::analogDigitalUpdate(void)
{
	if (TechDemoStatus.ActiveScreen == AD_SCREEN)
		modelListener->analogDigitalUpdate_Presenter(TechDemoStatus.AnalogDigitalScreen.UserInterfaceVoltage, TechDemoStatus.AnalogDigitalScreen.HexSwitchChar);
}
