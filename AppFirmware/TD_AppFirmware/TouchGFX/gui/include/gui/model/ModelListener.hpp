#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include "main.h"
#include "MainSupport.h"

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    // START HAB ADDED:
    virtual void analogDigitalUpdate_Presenter(float, char) {}
    virtual void PWM_AccelerometerUpdate_Presenter(Type_PWM_AccelerometerScreen) {}

    // END HAB ADDED

    void bind(Model* m)
    {
        model = m;
    }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
