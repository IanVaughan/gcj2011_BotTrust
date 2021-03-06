#include <stdio.h>
bool debug = false;

class Robot
{
public:
	Robot()
	{
		Reset();
	}

	void Name(char *_name)
	{
		name = _name;
	}

	void Push(int button, int order)
	{
		target_buttons[number_of_buttons].target = button;
		target_buttons[number_of_buttons++].order = order;
		if (debug) printf("%s Robot::Push->%d, button:%d, order:%d\n", name, number_of_buttons-1, button, order);
	}

	int Pop()
	{
		if (debug) printf("%s Robot::Pop->Push button:%d (at index:%d/%d)\n", 
			name, target_buttons[current_index].target, current_index, number_of_buttons);
		if (number_of_buttons)
		{
			current_index++;
			return (current_index == number_of_buttons);
		}
		else
		{
			return -1;
		}
	}

	int Move()
	{
		if (current_button < target_buttons[current_index].target)
		{
			current_button++;
			if (debug) printf("%s Robot::Move up to button %d (target:%d)\n", 
				name, current_button, target_buttons[current_index-1].target);
			return -1;
		}
		else if (current_button > target_buttons[current_index].target)
		{
			current_button--;
			if (debug) printf("%s Robot::Move down to button %d (target:%d)\n", 
				name, current_button, target_buttons[current_index-1].target);
			return -1;
		}
		else
		{
			if (debug) printf("%s Robot::Stay at button %d (index:%d order:%d)\n", 
				name, current_button, current_index, target_buttons[current_index].order);
			return target_buttons[current_index].order;
		}
	}

	void Reset()
	{
		number_of_buttons = 0;
		current_index = 0;
		current_button = 1;
		for (int i=0; i<100; i++)
		{
			target_buttons[i].order = 0;
			target_buttons[i].target = 1;
		}
	}

private:
	struct SButtons
	{
		int target;
		int order;
	};

	int number_of_buttons;
	int current_index;
	int current_button;
	SButtons target_buttons[100];
	char *name;
};

int get_robot(char type, bool first = false)
{
	static char first_type;
	if (first || type == first_type)
	{
		first_type = type;
		return 0;
	}
	else
	{
		return 1;
	}
}

int main(void)
{
	int number_of_test_cases, number_of_buttons, button_to_press;
	char robot_char;
	scanf("%d", &number_of_test_cases);
	//printf("main:number_of_test_cases:%d\n", number_of_test_cases);
	
	Robot Robots[2];

	for (int test=0; test<number_of_test_cases; test++)
	{
		scanf("%d ", &number_of_buttons);
		if (debug) printf("*** Load->test:%d, number_of_buttons:%d\n", test, number_of_buttons);
		
		for (int button=0; button<number_of_buttons; button++)
		{
			scanf("%c %d ", &robot_char, &button_to_press);

			if (!button)
			{
				get_robot(robot_char, !button);
				Robots[get_robot('O')].Name("Orange");
				Robots[get_robot('B')].Name("Blue");
			}

			if (debug) printf("set robot->%c button_to_press:%d\n", robot_char, button_to_press);
			Robots[get_robot(robot_char)].Push(button_to_press, button);
		}
		
		if (debug) printf("*** Start...\n");
		int time=0;
		bool waiting0 = false;
		bool waiting1 = false;
		int ready0 = 0, ready1 = 0;
		int order = 0;
		bool inc=false;
		while (!waiting0 || !waiting1)
		{
			time++;

			if (debug) printf("Time:%d\n", time);
			ready0 = Robots[0].Move();
			ready1 = Robots[1].Move();
			
			if (ready0 == order)
			{
				int r = Robots[0].Pop();
				if (r > -1) {
					inc=true;
				}
				if (r != 0) {
					waiting0 = true;
				}
			}
			if (ready1 == order) 
			{
				int r = Robots[1].Pop();
				if (r > -1)
				{
					inc=true;
				}
				if (r != 0) {
					waiting1 = true;
				}
			}
			if (inc)
			{
				order++;
				inc=false;
			}
			if (debug) printf("%d, %d %d, %d %d\n", order, ready0, ready1, waiting0, waiting1);
		}

		printf("Case #%d: %d\n", test+1, time);
		waiting0 = waiting1 = true;
		time = 0;
		Robots[0].Reset();
		Robots[1].Reset();
	}

	return 0;
}
