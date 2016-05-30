#include <Env/Cave.hpp>

Cave::Cave(const Vec2d& position, double radius) :
    Collider(position, radius)

{
    reloadConfig();
    addBear();
}

void
Cave::reloadConfig()
{
    this->setRadius(getAppConfig()["simulation"]["cave"]["size"].toDouble());
    texture_ = getAppTexture(
                   getAppConfig()["simulation"]["cave"]["texture"].toString());
}

Cave::~Cave()
{
    delete bear_;
}

Bear*
Cave::addBear()
{
    bear_ = new Bear(this, this->getPosition());
    bear_->reloadConfig();
    return bear_;
}

Bear*
Cave::getBearAt(const Vec2d& position)
{

    if (bear_->isPointInside(position)) {
        return bear_;
    }
    return nullptr;
}

Bear*
Cave::getBear()
{
    return bear_;
}

void
Cave::update(sf::Time dt)
{
    getBear()->update(dt);

    if (bear_->isDead()) {
        delete bear_;
        bear_ = nullptr;
    }
}

void
Cave::drawOn(sf::RenderTarget& target) const
{
    auto caveSprite = buildSprite(this->getPosition(), this->getRadius(),
                                  texture_);
    target.draw(caveSprite);
    bear_->drawOn(target);
}

