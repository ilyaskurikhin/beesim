#ifndef WORLD_H
#define WORLD_H

enum class Kind : short { Grass, Water, Rock };

class World
{
    public:
        
        // 
        void
        reloadCacheStructure()
        {

        }



        void
        draw(sf::RenderTarget& target)
        {

        }
    private:
        int nbColumns_;
        float cellSize_;
        vector<Kind> cells_;
        sf::RenderTexture renderingCache_;
};
#endif
