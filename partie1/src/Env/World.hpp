#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <JSON/JSON.hpp>



enum class Kind : short { Grass, Water, Rock };



class World
{
    public:
        
        j::Value
        simulationWorld();
		
        void
        reloadConfig();
        
        void
        reloadCacheStructure();

        void
        drawOn(sf::RenderTarget& target);

        void 
        updateCache();
                
        void 
        reset(bool regenerate);
        
        void
        loadFromFile();

    private:
        
        size_t numberColumns_;
        float cellSize_;
        std::vector<Kind> cells_;

        sf::RenderTexture renderingCache_;
        std::vector<sf::Vertex> grassVertexes_;
        std::vector<sf::Vertex> waterVertexes_;
        std::vector<sf::Vertex> rockVertexes_;

        bool regenerate_;
};

//surcharge de l'opérateur << permettant de retourner le std::runtime_error ayant été lancé
std::ostream&
operator<< (std::ostream& oss, const std::runtime_error& error);



#endif
