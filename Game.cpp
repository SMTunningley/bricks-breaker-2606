#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	int brickAmount = 5;
	for (int i = 0; i < brickAmount; ++i)
	{
		Box tempBrick;
		tempBrick.width = 10;
		tempBrick.height = 2;
		tempBrick.x_position = 0 + (i * 10);
		tempBrick.y_position = 5;
		tempBrick.doubleThick = true;
		tempBrick.color = ConsoleColor::DarkGreen;

		brick.push_back(tempBrick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < brick.size(); ++i)
	{
		brick[i].Draw();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < brick.size(); ++i)
	{
		if (brick[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			brick[i].color = ConsoleColor(brick[i].color - 1);
			ball.y_velocity *= -1;

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if ((int)brick[i].color == Black)
			{
				brick.erase(brick.begin() + i);
				break;
			}
		}
	}


	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (brick.size() <= 0)
	{
		ball.moving = false;

		std::string winText = "You won! Press \"R\" to reset.";

		std::cout << std::string(WINDOW_HEIGHT / 5, '\n') // Puts the text in the vertical middle
			<< std::string((WINDOW_WIDTH - winText.length()) / 2, ' ') // Puts the text in the horizontal middle
			<< winText;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1 && ball.y_velocity >= 0) // Velocity check stops the game from ending instantly
	{
		ball.moving = false;

		std::string lossText = "You lost... Press \"R\" to reset.";

		std::cout << std::string(WINDOW_HEIGHT / 5, '\n') // Puts the text in the vertical middle
			<< std::string((WINDOW_WIDTH - lossText.length()) / 2, ' ') // Puts the text in the horizontal middle
			<< lossText;
	}
}
