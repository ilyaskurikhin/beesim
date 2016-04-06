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
        reset(bool regenerate);
        
        void
        loadFromFile();

        void
        step();
        
        void
        steps(int repeats, bool b=false);
        
        void 
        moveSeeds(Kind texture);
        
        void
        smooth ();
        
        void 
        smooths();
		
		
    private:
        
        size_t numberColumns_;
        float cellSize_;
        std::vector<Kind> cells_;

        sf::RenderTexture renderingCache_;
        std::vector<sf::Vertex> grassVertexes_;
        std::vector<sf::Vertex> waterVertexes_;
        std::vector<sf::Vertex> rockVertexes_;

	
        struct Seed {
            sf::Vector2i coords_;
            Kind Seedtexture_;
        };
        
        std::vector<Seed> seeds_;
        int nbWaterSeeds_;
        int nbGrassSeeds_;
       
        bool regenerate_;
};
#endif
