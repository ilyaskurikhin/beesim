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
        drawOn(sf::RenderTarget& target);


    private:
        int nbColumns_;
        float cellSize_;
        vector<Kind> cells_;
        sf::RenderTexture renderingCache_;
};
#endif
