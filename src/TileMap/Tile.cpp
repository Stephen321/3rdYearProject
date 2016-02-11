#include "TileMap\Tile.h"

class Tile : public sf::Drawable {
public:
	Tile(sf::Texture texture, sf::Vector2f position, int id, int weight, bool walkable);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

private:
	sf::Sprite m_sprite;
	int m_height;
	int m_weight;
	int m_id;
	bool m_walkable;
};
