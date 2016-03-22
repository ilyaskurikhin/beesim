#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <SFML/Graphics.hpp>



enum class Kind : short { Grass, Water, Rock };



class World
{
    public:
        
        
		void
		reloadConfig();
        
		void
		reloadCacheStructure();

        void
        drawOn(sf::RenderTarget& target);

		void 
		updateCache();
		
		void 
		reset();
		

    private:
        int numberColumns_;
        float cellSize_;
        std::vector<Kind> cells_;
        
        sf::RenderTexture renderingCache_;
        std::vector<sf::Vertex> grassVertexes_;
        std::vector<sf::Vertex> waterVertexes_;
        std::vector<sf::Vertex> rockVertexes_;
        
        bool regenerate_;
};
#endif
