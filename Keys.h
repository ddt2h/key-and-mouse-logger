#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <dinput.h>

#include <thread>
struct Position //both key is pressed, also on default timing
{
	double time;
	int x, y;
	sf::Keyboard::Key key;


	double hold_time;
	bool key_state = false;
};


void keytest_2(int code, int holdtime_ms)
{




	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = KEYEVENTF_SCANCODE;
	ip.ki.wScan = code;
	SendInput(1, &ip, sizeof(INPUT));
	Sleep(holdtime_ms);
	
	
	ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // set the flag so the key goes up so it doesn't repeat keys
	SendInput(1, &ip, sizeof(INPUT)); // Resend the input
}


class Replicator
{
private:
	std::vector<Position> positions;

	sf::Clock clock;

	int current_position;

	double rec_current;

	bool isReplicating = false;

	bool key_lock;

	sf::Keyboard::Key start_key;

	std::vector<std::pair<sf::Keyboard::Key, int>> key_map;
	void map()
	{
		key_map.push_back(std::make_pair(sf::Keyboard::Key::A, DIKEYBOARD_A));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::B, DIKEYBOARD_B));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::C, DIKEYBOARD_C));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::D, DIKEYBOARD_D));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::E, DIKEYBOARD_E));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::F, DIKEYBOARD_F));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::G, DIKEYBOARD_G));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::H, DIKEYBOARD_H));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::I, DIKEYBOARD_I));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::J, DIKEYBOARD_J));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::K, DIKEYBOARD_K));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::L, DIKEYBOARD_L));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::M, DIKEYBOARD_M));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::N, DIKEYBOARD_N));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::O, DIKEYBOARD_O));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::P, DIKEYBOARD_P));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::Q, DIKEYBOARD_Q));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::R, DIKEYBOARD_R));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::S, DIKEYBOARD_S));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::T, DIKEYBOARD_T));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::U, DIKEYBOARD_U));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::V, DIKEYBOARD_V));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::W, DIKEYBOARD_W));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::X, DIKEYBOARD_X));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::Y, DIKEYBOARD_Y));
		key_map.push_back(std::make_pair(sf::Keyboard::Key::Z, DIKEYBOARD_Z));
	}
	void unpackData()
	{
		SetCursorPos(positions[current_position].x, positions[current_position].y);
		
		if (positions[current_position].key_state)
		{
			
			for (int i = 0; i < key_map.size(); i++)
			{
			//	keyDown(key_map[i].second);
			//	keyUp(key_map[i].second);

				if (key_map[i].first == positions[current_position].key)
				{
					
					keytest_2(key_map[i].second, positions[current_position].hold_time);
				}
				
			}
		}
	
	
		
	}
	void replicate()
	{

		rec_current = clock.getElapsedTime().asSeconds();

		if (current_position < positions.size() && positions.size() != 0)
		{
			if (rec_current >= positions[current_position].time)
			{
				//std::cout << current_position << std::endl;
				std::thread execute_thread(&Replicator::unpackData, this);
				execute_thread.detach();
			//	unpackData();
				current_position++;
			}
		}
		else
		{
			isReplicating = false;
		}

	
		////////////////


	}
	void keyboard()
	{
		if (sf::Keyboard::isKeyPressed(start_key) && !key_lock)
		{
			isReplicating = !isReplicating;
			key_lock = true;

			if (isReplicating)
			{
				current_position = 0;
				clock.restart();
			}
			if (!isReplicating)
			{
				clock.restart();
			}
		}
		if (!sf::Keyboard::isKeyPressed(start_key))
		{
			key_lock = false;
		}
	}

public:
	Replicator()
	{
		map();
	}
	void update()
	{

		keyboard();
		if (isReplicating)
			replicate();
	}
	void feedData(std::vector<Position>& data)
	{
		positions = data;
	}
	void setStartingKey(sf::Keyboard::Key key)
	{
		start_key = key;
	}
};
class LoggingKey
{
public:
	sf::Clock hold_clock;
	std::pair<sf::Keyboard::Key, bool> key_down; //make a vec
	double current_hold_time = 0;
	LoggingKey()
	{

	}
};
class Logger
{
private:
	std::vector<Position> positions;

	bool isRecording;

	sf::Clock clock;

	double rec_interval = 0.006f;

	double rec_interval_dd;

	double rec_current;


	sf::Keyboard::Key start_key;

	bool key_lock;

	//temp mb
	Replicator replicator;

	std::vector<bool> key_locks;

	//rebuild
	std::vector<LoggingKey> loggs;


	//sf::Clock hold_clock;
	//std::pair<sf::Keyboard::Key, bool> key_down; //make a vec
	//double current_hold_time = 0;
	void init_locks()
	{
		for (sf::Keyboard::Key i = sf::Keyboard::Key::Unknown; i < sf::Keyboard::Key::Return; i = sf::Keyboard::Key(i + 1))
		{
			key_locks.push_back(true);
		}
		
		
	}
	void pushDataDefault()
	{
		POINT pnt;
		GetCursorPos(&pnt);
		Position position;

		position.x = pnt.x;
		position.y = pnt.y;

		position.time = rec_current;

		positions.push_back(position);

		//interpret key
	}
	void pushDataKey(sf::Keyboard::Key key, int time_ms)
	{
		POINT pnt;
		GetCursorPos(&pnt);
		Position position;
		position.x = pnt.x;
		position.y = pnt.y;

		position.time = rec_current;
	
		position.key = key;
		position.key_state = true;
		position.hold_time = time_ms; /////////////////////////

		positions.push_back(position);
		//keyboard key push;
	}

	void debugInput()
	{
		int cnt = positions.size() - 1;
		if (cnt >= 0)
		{

			std::cout <<  "num of: " << positions.size() <<  "x: " << positions[cnt].x << "y: " << positions[cnt].y << " timing: " << positions[cnt].time << " key state: " << positions[cnt].key_state << std::endl;
		}
	}
	void record()
	{
		rec_current = clock.getElapsedTime().asSeconds();

		bool isExcessive = false;

		

		int cycle_id = 0;
		for (sf::Keyboard::Key i = sf::Keyboard::Key::Unknown; i < sf::Keyboard::Key::Return; i = sf::Keyboard::Key(i + 1)) //checking for input [any timing]
		{
			
			if (sf::Keyboard::isKeyPressed(i) && i != start_key) //problem 
			{
				if (key_locks[cycle_id])
				{
					LoggingKey lg;
					lg.current_hold_time = 0;
					lg.hold_clock.restart();
					lg.key_down.first = i;
					lg.key_down.second = true;


					loggs.push_back(lg);
					
					
					isExcessive = true; //so the input isn't doubled

					key_locks[cycle_id] = false;
				}
				
				
			}
			if (!sf::Keyboard::isKeyPressed(i))
			{
				key_locks[cycle_id] = true;
			}
			cycle_id++;
		}
		
		if (!isExcessive)
		{
			if (rec_current >= rec_interval_dd) //checking for mouse pos [default timing]
			{
				pushDataDefault();
				rec_interval_dd += rec_interval;
			}
		}
		//on finished recording a key
		for (int i = 0; i < loggs.size(); i++)
		{
			loggs[i].current_hold_time = loggs[i].hold_clock.getElapsedTime().asMilliseconds();
			if (!sf::Keyboard::isKeyPressed(loggs[i].key_down.first))
			{

				pushDataKey(loggs[i].key_down.first, loggs[i].current_hold_time); //set this key to be always down, then the default state is every key is up {?}
				std::cout << "pushed a key, hold time: " << loggs[i].current_hold_time << std::endl;
				


				loggs.erase(loggs.begin() + i);
			}
		}
		
		




	}
	void clearRecord()
	{
		positions.clear();
		clock.restart();
		rec_interval_dd = rec_interval;
	}
	void keyboard()
	{
		if (sf::Keyboard::isKeyPressed(start_key) && !key_lock)
		{
			isRecording = !isRecording;
			key_lock = true;
			start_key = sf::Keyboard::Key::Escape;
			if (isRecording)
			{
				clearRecord();

			}
			if (!isRecording)
			{
				clock.restart();
				//replicator
				replicator.feedData(positions);
			}
		}
		if (!sf::Keyboard::isKeyPressed(start_key))
		{
			key_lock = false;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
		//	exit(0);
		}
	}
	void cycle()
	{
		
		for (;;)
		{
			keyboard();
			replicator.update();

			if (isRecording)
				record();

			//	debugInput();

		}
	}
public:
	Logger()
	{
		rec_interval_dd = rec_interval;


	}
	void startCycle()
	{
		init_locks();
		replicator.setStartingKey(sf::Keyboard::Key::LControl);
		cycle();
	}
	void setStartingKey(sf::Keyboard::Key key)
	{
		start_key = key;
	}



};