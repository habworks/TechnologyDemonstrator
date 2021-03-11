#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
// HAB ADD
#include "MainSupport.h"
#include "cmsis_os2.h"
extern osSemaphoreId_t BinarySemAnalogDigitalHandle;
extern osSemaphoreId_t BinarySemPWM_AccelerometerHandle;

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
	// ANALOG DIGITAL SCREEN
	if (BinarySemAnalogDigitalHandle != NULL)  // Simple test to make sure the semaphore was created
	{
		if (osSemaphoreAcquire(BinarySemAnalogDigitalHandle, 10U) == osOK)
		{
			analogDigitalUpdate();
		}
	}

	// PWM ACCELEROMETER SCREEN
	if (BinarySemPWM_AccelerometerHandle != NULL)  // Simple test to make sure the semaphore was created
	{
		if (osSemaphoreAcquire(BinarySemPWM_AccelerometerHandle, 10U) == osOK)
		{
			PWM_AccelerometerUpdate();
		}
	}
}



// HAB ADDED FUNCIONS FOR MODEL TICK
void Model::analogDigitalUpdate(void)
{
	if (TechDemoStatus.ActiveScreen == AD_SCREEN)
		modelListener->analogDigitalUpdate_Presenter(TechDemoStatus.AnalogDigitalScreen.UserInterfaceVoltage, TechDemoStatus.AnalogDigitalScreen.HexSwitchChar);
}

// HAB ADDED FUNCIONS FOR MODEL TICK
void Model::PWM_AccelerometerUpdate(void)
{
	if (TechDemoStatus.ActiveScreen == PA_SCREEN)
		modelListener->PWM_AccelerometerUpdate_Presenter(TechDemoStatus.PWM_AccelerometerScreen);
}
