/*
 * Bear.cpp
 *
 *  Created on: May 13, 2016
 *      Author: ilya
 */

#include <Env/Bear.hpp>

#include <Env/Env.hpp>

Bear::Bear(Cave* cave, const Vec2d& position) :
    Movable(position), CFSM(std::vector<State>(
{
    HIBERNATION, SEARCH_HIVE, EAT_HONEY, RETURN_CAVE
})), cave_(cave), debug_thickness_random_(
    5), debug_thickness_target_(3), vision_range_(position), move_state_(
        AT_REST)
{
    reloadConfig();
    loadTexture();
}

Bear::~Bear()
{
}

j::Value const&
Bear::getConfig()
{
    return getAppConfig()["simulation"]["bear"];
}

void
Bear::loadTexture()
{
    current_texture_index_ = 0;
    int number_files = getConfig()["textures"]["walking"]["number files"].toInt();
    for (int i=0; i < number_files; ++i) {
        std::string file_name = getConfig()["textures"]["walking"]["file name"].toString();
        file_name.append(std::to_string(i));
        file_name.append(".png");
        walking_textures_.push_back(getAppTexture(file_name));
    }
}

void
Bear::chooseTexture()
{
    if (isWalking() && texture_counter_.getElapsedTime() > texture_delay_) {
        texture_counter_.restart();
        if (current_texture_index_ < walking_textures_.size() - 1) {
            ++current_texture_index_;
        } else {
            current_texture_index_ = 0;
        }
    }
    //  if ((texture_counter_.getElapsedTime() > texture_delay_))
}

void
Bear::move(sf::Time dt)
{
    if (move_state_ == AT_REST) {
        energy_ = energy_ - energy_rate_idle_ * dt.asSeconds();
    } else if (move_state_ == RANDOM) {
        randomMove(dt);
        energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    } else if (move_state_ == TARGET) {
        targetMove(dt);
        energy_ = energy_ - energy_rate_moving_ * dt.asSeconds();
    }
    vision_range_.setPosition(this->getPosition());
}

bool
Bear::isMovablePosition(const Vec2d& position) const
{
    return getAppEnv().isWalkable(position);
}


bool
Bear::isDead()
{
    if (energy_ <= 0)
        return true;
    else
        return false;
}

bool
Bear::isInCave() const
{
    if (this->getState() == HIBERNATION)
        return true;
    else
        return false;
}

bool
Bear::isWalking() const
{
    if (this->getState() == SEARCH_HIVE || this->getState() == RETURN_CAVE)
        return true;
    else
        return false;
}

Hive*
Bear::findVisibleHive() const
{
    return getAppEnv().getCollidingHive(vision_range_);
}


double
Bear::eatHoney(Hive* hive, sf::Time dt)
{
    double eaten(honey_eating_rate_ * dt.asSeconds());
    if (hive) {
        eaten = hive->takeNectar(eaten);
    } else {
        eaten = 0;
    }
    energy_ += eaten;

    return eaten;
}

void
Bear::update(sf::Time dt)
{
    this->action(dt);
    this->move(dt);
}

void
Bear::drawOn(sf::RenderTarget& target) const
{
    Vec2d position(this->getPosition());
    double radius(this->getRadius());

    auto bearSprite = buildSprite(position, radius, walking_textures_[current_texture_index_]);
    double angle(this->getMoveVec().Vec2d::angle());
    if ((angle >= PI / 2) or (angle <= -PI / 2)) {
        bearSprite.scale(1, -1);
    }
    bearSprite.rotate(angle / DEG_TO_RAD);

    if (!isInCave()) {
        target.draw(bearSprite);
    }

    if (isDebugOn()) {
        double thickness(0);
        if (move_state_ == RANDOM) {
            thickness = debug_thickness_random_;
            sf::CircleShape shape = buildAnnulus(position, radius,
                                                 sf::Color::Black, thickness);
            target.draw(shape);
        } else if (move_state_ == TARGET) {
            thickness = debug_thickness_target_;
            sf::CircleShape shape = buildAnnulus(position, radius,
                                                 sf::Color::Blue, thickness);
            target.draw(shape);
        }

        std::string valueString;
        sf::Color color(sf::Color::Red);
        Vec2d position;
        double text_size(getAppEnv().getTextSize());

        position = this->getPosition();
        position.y += text_size;

        valueString = "Bear: energy " + to_nice_string(this->getEnergy())
                      + " Bear: hibernation length "
                      + to_nice_string(this->getHibernationLength());
        sf::Text text = buildText(valueString, position, getAppFont(), text_size,
                                  color);
        target.draw(text);

        position.y += text_size;
        sf::Text status = buildText(this->getDebugStatus(), position,
                                    getAppFont(), text_size, color);
        target.draw(status);
    }
}

void
Bear::reloadConfig()
{
    // configure Collider
    this->setRadius(getConfig()["size"].toDouble());

    // configure Moveable
    this->setDelay(
        sf::seconds(
            getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble()));
    this->setSpeed(getConfig()["speed"].toDouble());
    this->setMoveVec(Vec2d::fromRandomAngle() * this->getSpeed());
    this->setMaxAngle(
        getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
    this->setRotationProbability(
        getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());

    energy_ = getConfig()["energy"]["initial"].toDouble();
    energy_rate_idle_ =
        getConfig()["energy"]["consumption rates"]["idle"].toDouble();
    energy_leave_cave_ = getConfig()["energy"]["to leave cave"].toDouble();
    energy_rate_moving_ =
        getConfig()["energy"]["consumption rates"]["moving"].toDouble();
    energy_rate_eating_ =
        getConfig()["energy"]["consumption rates"]["eating"].toDouble();
    energy_seek_hives_ =
        getConfig()["energy"]["consumption rates"]["seeking hive"].toDouble();

    texture_delay_ = sf::seconds(
                         static_cast<float>(getConfig()["textures"]["delay"].toDouble()));
    texture_counter_.restart();

    honey_eating_rate_ = getConfig()["honey eating rate"].toDouble();

    max_hibernation_ =
        sf::seconds(
            static_cast<float>(getConfig()["hibernation"]["maximum time"].toDouble()));

    visibility_ = getConfig()["visibility range"].toDouble();
    vision_range_.setRadius(visibility_ + this->getRadius());
}

void
Bear::onState(State state, sf::Time dt)
{
    // first state
    if (state == HIBERNATION) {
        if (hibernation_length_ < max_hibernation_
            && this->getEnergy() > energy_leave_cave_) {
            std::string status = "in_cave_hibernating_";
            this->setDebugStatus(status);
            hibernation_length_ += dt;
        }

        if (hibernation_length_ >= max_hibernation_
            && this->getEnergy() > energy_leave_cave_) {
            this->setDebugStatus("in_cave_leaving");
            this->nextState();
        }
    }

    // second state
    else if (state == SEARCH_HIVE) {
        this->setDebugStatus("seeking_hives");
        Hive* hive = this->findVisibleHive();
        if (this->getEnergy() > energy_seek_hives_ && hive != nullptr) {
            this->setMoveTarget(hive->getPosition());
            this->setMoveState(TARGET);
            if (this->isColliderInside(hive->getCollider()))
                this->nextState();
        }
    }

    // third state
    else if (state == EAT_HONEY) {
        this->setDebugStatus("eating_honey");
        Hive* hive(getAppEnv().getCollidingHive(this->getCollider()));
        if (hive != nullptr) {
            eatHoney(hive, dt);
            if (hive->getNectar() <= 0)
                this->nextState();
        } else {
            this->nextState();
        }
    }

    // fourth state
    else if (state == RETURN_CAVE) {
        this->setDebugStatus("back_to_cave");
        this->setMoveTarget(this->getCave()->getPosition());
        if (this->getCave()->isColliderInside(this->getCollider())) {
            this->nextState();
        }
    }

    // choose texture appropriate for state
    chooseTexture();
}

void
Bear::onEnterState(State state)
{
    if (state == HIBERNATION or state == EAT_HONEY) {
        this->setMoveState(AT_REST);
    } else if (state == SEARCH_HIVE) {
        this->setMoveState(RANDOM);
        hibernation_length_ = sf::Time::Zero;
    } else if (state == RETURN_CAVE) {
        this->setMoveState(TARGET);
    }
}

Cave*
Bear::getCave() const
{
    return cave_;
}

void
Bear::setCave(Cave* cave)
{
    cave_ = cave;
}

void
Bear::setDebugStatus(const std::string& status)
{
    debug_status_ = status;
}

const std::string&
Bear::getDebugStatus() const
{
    return debug_status_;
}

double
Bear::getEnergy() const
{
    return energy_;
}

void
Bear::setEnergy(double energy)
{
    energy_ = energy;
}

double
Bear::getHibernationLength() const
{
    return hibernation_length_.asSeconds();
}

const Vec2d&
Bear::getHiveLocation() const
{
    return hive_location_;
}

void
Bear::setHiveLocation(const Vec2d& hiveLocation)
{
    hive_location_ = hiveLocation;
}

State
Bear::getMoveState() const
{
    return move_state_;
}

void
Bear::setMoveState(const State& state)
{
    move_state_ = state;
}

double
Bear::getVisibility() const
{
    return visibility_;
}

const Collider&
Bear::getVisionRange() const
{
    return vision_range_;
}

State const Bear::HIBERNATION = createUid();
State const Bear::SEARCH_HIVE = createUid();
State const Bear::EAT_HONEY = createUid();
State const Bear::RETURN_CAVE = createUid();
