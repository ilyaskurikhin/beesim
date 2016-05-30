#include <Env/Env.hpp>

#include <Env/Flower.hpp>
#include <Env/FlowerGenerator.hpp>

bool ENV_VERBOSE(true);

Env::Env() :
    world_(new World()), flower_generator_(new FlowerGenerator)
{
    logEvent("Env", "generating environment");
    reloadConfig();
}

Env::~Env()
{
    logEvent("Env", "destroying environment");
    delete world_;

    for (size_t i = 0; i < flowers_.size(); ++i) {
        delete flowers_[i];
    }
    flowers_.clear();

    for (size_t i = 0; i < hives_.size(); ++i) {
        delete hives_[i];
    }
    hives_.clear();

}

void
Env::update(sf::Time dt)
{
    // update du generateur
    if ((flowers_.size() < max_flowers_)
        && (getAppConfig()["simulation"]["flower generator"]["active"].toBool())) {
        flower_generator_->update(dt);
    }

    // iterate through flowers
    for (size_t i = 0; i < flowers_.size(); ++i) {
        // flowers are updates
        // new flowers are drawn in next draw cycle
        flowers_[i]->update(dt);

        // check if flower is dead
        if (flowers_[i]->getPollen() <= 0) {
            // remove dead flower
            delete flowers_[i];
            flowers_[i] = nullptr;
        }
    }
    // remove empty locations
    flowers_.erase(std::remove(flowers_.begin(), flowers_.end(), nullptr),
                   flowers_.end());

    for (size_t i = 0; i < hives_.size(); ++i) {
        hives_[i]->update(dt);

        if (hives_[i]->getNectar() == 0
            && hives_[i]->getNumberBees() == 0) {
            delete hives_[i];
            hives_[i] = nullptr;
        }
    }

    hives_.erase(std::remove(hives_.begin(), hives_.end(), nullptr),
                 hives_.end());

}

void
Env::drawOn(sf::RenderTarget& target) const
{
    world_->drawOn(target);
    for (size_t i = 0; i < flowers_.size(); ++i) {
        flowers_[i]->drawOn(target);
    }

    for (size_t i = 0; i < hives_.size(); ++i) {
        hives_[i]->drawOn(target);

    }

    // if debug is on, show values
    if (isDebugOn()) {
        // get cursor position
        Vec2d position = getApp().getCursorPositionInView();
        if (world_->isInWorld(position)) {
            bool isFlower(false);
            std::string valueString("empty");
            sf::Color color(sf::Color::White);

            // check for flowers
            for (size_t i = 0; i < flowers_.size(); ++i) {
                if (*(flowers_[i]) > position) {
                    isFlower = true;
                    valueString = to_nice_string(flowers_[i]->getPollen());
                    color = sf::Color::Yellow;
                }
            }

            // otherwise show ambient humidity
            if (!isFlower) {
                valueString = to_nice_string(world_->getHumidity(position));
                color = sf::Color::Red;
            }

            sf::Text text = buildText(valueString, position, getAppFont(),
                                      debug_text_size_, color);
            target.draw(text);

        }
    }

}

void
Env::reset()
{
    logEvent("Env", "resetting environment");

    world_->reset(true);
    for (size_t i = 0; i < flowers_.size(); ++i) {
        delete flowers_[i];
    }
    flowers_.clear();
    flower_generator_->reset();

    for (size_t i = 0; i < hives_.size(); ++i) {
        delete hives_[i];
    }
    hives_.clear();
}

void
Env::reloadConfig()
{
    // get variables from configuration
    flower_manual_radius_ =
        getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble();
    flower_max_nectar_ =
        getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toDouble();
    flower_min_nectar_ =
        getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toDouble();

    // get max number of Flower from configuration
    max_flowers_ = getAppConfig()["simulation"]["env"]["max flowers"].toInt();

    hive_manual_radius_ =
        getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble();

    debug_text_size_ = 10
                       * (getAppConfig()["simulation"]["world"]["size"].toDouble()
                          / getAppConfig()["simulation"]["world"]["cells"].toDouble());

    hiveable_factor_ =
        getAppConfig()["simulation"]["env"]["initial"]["hive"]["hiveable factor"].toDouble();
}

void
Env::loadWorldFromFile()
{
    world_->loadFromFile();
}

void
Env::saveWorldToFile() const
{
    world_->saveToFile();
}

const Vec2d&
Env::getWorldSize() const
{
    return world_->getWorldSize();
}

double
Env::getHumidity(const Vec2d& position) const
{
    return world_->getHumidity(position);
}

bool
Env::isGrowable(const Vec2d& position) const
{
    return world_->isGrass(position);
}

bool
Env::isFlyable(const Vec2d& position) const
{
    return world_->isFlyable(position);
}

bool
Env::isPlaceable(const Vec2d& position, double radius) const
{
    if (world_->isGrass(position)) {
        Collider object(position, radius);

        // check if object can be made at position
        if (getCollidingHive(object) == nullptr) {
            return true;
        }
    }
    return false;
}

bool
Env::canAddFlower()
{
    if (flowers_.size() < max_flowers_)
        return true;
    else
        return false;
}

bool
Env::addFlowerAt(const Vec2d& position)
{
    // check if flower can be made at position
    if ((flowers_.size() < max_flowers_)
        && (isPlaceable(position, flower_manual_radius_))) {
        // set a random number of pollen
        double pollen = uniform(flower_min_nectar_, flower_max_nectar_);

        flowers_.push_back(new Flower(position, flower_manual_radius_, pollen));
        return true;
    } else {
        return false;
    }
}

void
Env::drawFlowerZone(sf::RenderTarget& target, const Vec2d& position)
{
    if (world_->isGrass(position)) {
        sf::CircleShape shape = buildAnnulus(position, flower_manual_radius_,
                                             sf::Color::Green, 5.0);
        target.draw(shape);
    } else {
        sf::CircleShape shape = buildAnnulus(position, flower_manual_radius_,
                                             sf::Color::Red, 5.0);
        target.draw(shape);
    }
}

bool
Env::addHiveAt(const Vec2d& position)
{

    if (world_->isGrass(position) && (isPlaceable(position, hive_manual_radius_))) {
        hives_.push_back(new Hive(position, hive_manual_radius_));
        return true;
    } else {
        return false;
    }
}

void
Env::drawHiveableZone(sf::RenderTarget& target, const Vec2d& position)
{
    // TODO implement

    Vec2d worldSize = getApp().getWorldSize();

    sf::Color color;
    sf::Color fillColor;
    fillColor.a = 0;
    double left, right, top, bottom;
    double h_left(-5), h_right(-5); // horizontal
    double v_top(-5), v_bottom(-5); // vertical

    if (!world_->isInWorld(position))
        return;

    left = position.x - hive_manual_radius_;
    if (left < 0) {
        h_right = left + worldSize.x;
        h_left = worldSize.x;
    }

    right = position.x + hive_manual_radius_;
    if (right > worldSize.x) {
        h_right = right - worldSize.x;
        h_left = 0;
    }

    top = position.y - hive_manual_radius_;
    if (top < 0) {
        v_bottom = worldSize.y;
        v_top = top + worldSize.y;
    }

    bottom = position.y + hive_manual_radius_;
    if (bottom > worldSize.y) {
        v_bottom = 0;
        v_top = bottom - worldSize.y;
    }

    if (!world_->isGrassArea(Vec2d(left, top), Vec2d(right, bottom))
        || !world_->isGrassArea(Vec2d(h_left, top), Vec2d(h_right, bottom))
        || !world_->isGrassArea(Vec2d(left, v_top), Vec2d(right, v_bottom))
        || !world_->isGrassArea(Vec2d(h_left, v_top), Vec2d(h_right, v_bottom))) {
        color = sf::Color::Blue;
    } else if (!isPlaceable(position, hive_manual_radius_)) {
        color = sf::Color::Red;
    } else {
        color = sf::Color::Green;
    }

    sf::RectangleShape shape(
        buildRectangle(Vec2d(left, top), Vec2d(right, bottom), color, 5.0,
                       fillColor));
    target.draw(shape);
    sf::RectangleShape h_shape(
        buildRectangle(Vec2d(h_left, top), Vec2d(h_right, bottom), color, 5.0,
                       fillColor));
    target.draw(h_shape);
    sf::RectangleShape v_shape(
        buildRectangle(Vec2d(left, v_top), Vec2d(right, v_bottom), color, 5.0,
                       fillColor));
    target.draw(v_shape);
    sf::RectangleShape d_shape(
        buildRectangle(Vec2d(h_left, v_top), Vec2d(h_right, v_bottom), color, 5.0,
                       fillColor));
    target.draw(d_shape);
}

Hive*
Env::getCollidingHive(const Collider& body) const
{
    for (size_t i(0); i < hives_.size(); ++i) {
        Collider collidingHive(hives_[i]->getPosition(),
                               (hive_manual_radius_ * hiveable_factor_));

        if (collidingHive.isColliding(body)) {
            return hives_[i];
        }
    }
    return nullptr;
}

Flower*
Env::getCollidingFlower(const Collider& body) const
{
    for (size_t i(0); i < flowers_.size(); ++i) {
        Collider collidingFlower(flowers_[i]->getPosition(),
                                 flowers_[i]->getRadius());

        if (collidingFlower.isColliding(body)) {
            return flowers_[i];
        }
    }
    return nullptr;

}

Bee*
Env::getBeeAt(const Vec2d& position) const
{
    for (size_t i = 0; i < hives_.size(); ++i) {
        Bee* bee = hives_[i]->getBeeAt(position);
        if (bee) {
            return bee;
        }
    }
    return nullptr;
}

double
Env::getTextSize()
{
    return debug_text_size_;
}