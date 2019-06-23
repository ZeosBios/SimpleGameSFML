#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 25;
const int WIDTH_MAP = 40; 


sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
    "0                                      0",
	"0vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv0",
	"0dddddddddddddddddddddddddddddddddddddd0",
	"0000000000000000000000000000000000000000",
};
void randomMapGenerate() 
{
	int randomElementX = 0;
	int randomElementY = 23;
	srand(time(0));
	int countStone = 2;
	int countMine = 3;
	int countMedicine = 1;
	int countCartridges = 3;
	while (countStone > 0)
	{
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);
		//randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

		if (TileMap[randomElementY][randomElementX] == ' ')
		{
			TileMap[randomElementY][randomElementX] = 's';
			//std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
			countStone--;
		}
	}
		while (countMine > 0)
		{
			randomElementX = 1 + rand() % (WIDTH_MAP - 1);
			//randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

			if (TileMap[randomElementY][randomElementX] == ' ')
			{
				TileMap[randomElementY][randomElementX] = 'm';
				//std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
				countMine--;
			}
		}
			while (countMedicine > 0)
			{
				randomElementX = 1 + rand() % (WIDTH_MAP - 1);
				//randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

				if (TileMap[randomElementY][randomElementX] == ' ')
				{
					TileMap[randomElementY][randomElementX] = 'a';
					//std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
					countMedicine--;
				}
			}
				while (countCartridges>0)
				{
					randomElementX = 1 + rand() % (WIDTH_MAP - 1);
					//randomElementY = 1 + rand() % (HEIGHT_MAP - 1);

					if (TileMap[randomElementY][randomElementX] == ' ')
					{
						TileMap[randomElementY][randomElementX] = 'b';
						//std::cout << "coordinate of Stone X:" << randomElementX << "\n" << "coordinate of Stone Y:" << randomElementY << "\n\n";
						countCartridges--;
					}
	             }
}
