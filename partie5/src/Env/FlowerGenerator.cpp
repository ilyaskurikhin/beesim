#include <Env/FlowerGenerator.hpp>

FlowerGenerator::FlowerGenerator() :
    counter_(sf::Time::Zero)
{
    delay_ =
        sf::seconds(
            (float) getAppConfig()["simulation"]["flower generator"]["delay"].toDouble());
}

void
FlowerGenerator::update(sf::Time dt)
{
    counter_ += dt;

    // if generation is active and a delay has passed
    // generate flowers at random locations
    if (counter_ > delay_) {
        // reset the counter
        counter_ = sf::Time::Zero;
        Vec2d position;
        position.x = uniform((float) 0, (float) getApp().getWorldSize().x);
        position.y = uniform((float) 0, (float) getApp().getWorldSize().y);
        getAppEnv().addFlowerAt(position);
    }

}

void
FlowerGenerator::reset()
{
    counter_ = sf::Time::Zero;
}
