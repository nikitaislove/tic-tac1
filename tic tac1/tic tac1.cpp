#include <iostream>
#include <chrono>
#include <random>

using namespace std;

enum Cell 
{ 
	EMPTY = '_',
	CROSS = 'X',
	ZERO = '0' 
};

enum Progress { IN_PROGRES, WIN_HUMAN, WIN_AI, DRAW };

struct Game
{
	Cell** pField;
	Cell human;
	Cell ai;
	Progress progress;
	bool humanFirst = true;
	const size_t size = 3;
	size_t turn;
};

struct Coord
{
	size_t x;
	size_t y;
};

int32_t getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

void initGame(Game& g)
{
	g.pField = new Cell * [g.size];
	for (size_t i = 0; i < g.size; ++i)
		g.pField[i] = new Cell[g.size];

	g.progress = IN_PROGRES;

	for (size_t x = 0; x < g.size; x++)
		for (size_t y = 0; y < g.size; y++)
			g.pField[x][y] = EMPTY;

	if (getRandomNum(0, 100) > 50)
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.humanFirst = true;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.humanFirst = false;
		g.turn = 1;
	}
}

void deintGame(Game& g)
{
	for (size_t i = 0; i < g.size; i++)
		delete[] g.pField[i];

	delete[] g.pField;

	g.pField = nullptr;
}

void drawField(Game& g)
{
	system("cls");
	cout << "    1  2  3" << endl;
	for (size_t i = 0; i < g.size; ++i)
	{
		cout << i + 1 << " |";
		for (size_t j = 0; j < g.size; ++j)
		{
			cout << " " << (char)g.pField[i][j] << " ";
		}
		cout << " |" << endl;
	}

	cout << endl << " Human: " << (char)g.human << endl << " AI: " << (char)g.ai << endl;

}

Progress isWon(const Game& g)
{
	for (size_t i = 0; i < g.size; i++)
	{
		if (g.pField[0][i] == g.pField[1][i] && g.pField[1][i] == g.pField[2][i])
		{
			if (g.pField[1][i] == g.human)
				return WIN_HUMAN;
			else if (g.pField[1][i] == g.ai)
				return WIN_AI;
		}

		if (g.pField[i][0] == g.pField[i][1] && g.pField[i][1] == g.pField[i][2])
		{
			if (g.pField[i][1] == g.human)
				return WIN_HUMAN;
			else if (g.pField[i][1] == g.ai)
				return WIN_AI;
		}

		if (g.pField[0][0] == g.pField[1][1] && g.pField[1][1] == g.pField[2][2])
		{
			if (g.pField[1][1] == g.human)
				return WIN_HUMAN;
			else if (g.pField[1][1] == g.ai)
				return WIN_AI;
		}

		if (g.pField[0][2] == g.pField[1][1] && g.pField[1][1] == g.pField[2][0])
		{
			if (g.pField[1][1] == g.human)
				return WIN_HUMAN;
			else if (g.pField[1][1] == g.ai)
				return WIN_AI;
		}
	}

	bool empty = false;
	for (size_t i = 0; i < g.size; i++)
	{
		for (size_t j = 0; j < g.size; j++)
		{
			if (g.pField[i][j] == EMPTY)
				empty = true;
		}
	}

	if (empty)
		return IN_PROGRES;
	else	return DRAW;

}

Coord getCoordHuman(Game& g)
{
	Coord c = { 0 };
	do
	{
		cout << " Введите координату х фигуры: " << endl;
		cin >> c.x;
		cout << " Введите координату у фигуры: " << endl;
		cin >> c.y;
	} 	while ((c.x > g.size) || (c.y > g.size) || (g.pField[c.x - 1][c.y - 1] != EMPTY));
	--c.x;
	--c.y;

	return c;
}

Coord getCoordAI(Game& g)
{
	size_t sumEmpty = 0;
	for (size_t i = 0; i < g.size; ++i)
	{
		for (size_t j = 0; j < g.size; ++j)
		{
			if (g.pField[i][j] == EMPTY)
			{
				++sumEmpty;
				g.pField[i][j] = g.ai;
				if (isWon(g) == WIN_AI)
				{
					g.pField[i][j] = EMPTY;
					return { i,j };
				}
				else
				{
					g.pField[i][j] = g.human;
					if (isWon(g) == WIN_HUMAN)
					{
						g.pField[i][j] = EMPTY;
						return { i,j };
					}
					g.pField[i][j] = EMPTY;
				}
			}

		}
	}

	if (g.pField[1][1] == EMPTY)
		return { 1, 1 };
	if (g.pField[0][0] == EMPTY)
		return { 0, 0 };
	if (g.pField[0][2] == EMPTY)
		return { 0, 2 };
	if (g.pField[2][2] == EMPTY)
		return { 2, 2 };
	if (g.pField[2][0] == EMPTY)
		return { 2, 0 };
	if (g.pField[0][1] == EMPTY)
		return { 0, 1 };
	if (g.pField[1][0] == EMPTY)
		return { 1, 0 };
	if (g.pField[1][2] == EMPTY)
		return { 1, 2 };
	if (g.pField[2][1] == EMPTY)
		return { 2, 1 };
}

int main()
{
	setlocale(LC_ALL, "ru");
	Game game;
	Coord c = { 0,0 };
	initGame(game);
	drawField(game);

	while (game.progress == IN_PROGRES)
	{
		if (game.turn % 2 == 0)
		{
			Coord c = getCoordHuman(game);
			game.pField[c.x][c.y] = game.human;
		}
		else
		{
			Coord c = getCoordAI(game);
			game.pField[c.x][c.y] = game.ai;
		}
		++game.turn;
		drawField(game);
		game.progress = isWon(game);
	};

	if (game.progress == WIN_HUMAN)
	{
		cout << " Поздравляю вы выйграли!!! " << endl;
	}
	else if (game.progress == WIN_AI)
	{
		cout << " Выиграл компьютер. " << endl;
	}
	else if (game.progress == DRAW)
	{
		cout << " Ничья. " << endl;
	}

	deintGame(game);
	return 0;
}