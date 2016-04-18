#include <Env/FlowerGenerator.hpp>

Flower::FlowerGenerator() :
(counter_(Sf::Time::Zero)) {}    


void
FlowerGenerator::update(sf::Time dt){

  counter_ += dt;
    if (counter_ > sf::seconds(getAppConfig()["simulation"]["flower generator"]["delay"].toDouble())
    && (["simulation"]["flower"]["active"].toBool()) {
      counter_=0;
      Vec2d position;
      position.x= uniform(0, getApp().getWorldSize());
      position.y= uniform(0, getApp().getWorldSize());
      (getAppEnv()).addFlowerAt(position);
    }

}

void 
FlowerGenerator::reset{
  counter_ = sf::Time::Zero;
}
 

sf::Time
FlowerGenerator::counter_;
}



#endif

