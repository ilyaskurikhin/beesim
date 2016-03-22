#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <SFML/Graphics.hpp> 

enum class Kind : short { Grass, Water, Rock };

class World
{
    public:
        
        // 
        void
		reloadConfig();
        
        void
        reloadCacheStructure();

        void
        draw(sf::RenderTarget& target);

        
    private:
    
        int nbCells_;
        float cellSize_;
        std::vector<Kind> cells_;
        sf::RenderTexture renderingCache_;
};

#endif
