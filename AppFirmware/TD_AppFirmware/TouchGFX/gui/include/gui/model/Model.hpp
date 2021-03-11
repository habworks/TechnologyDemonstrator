#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    // HAB ADDED:
    void analogDigitalUpdate(void);
    void PWM_AccelerometerUpdate(void);
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
