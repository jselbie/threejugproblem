#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <string>

using namespace std;

struct JugState
{
	int jugs[3];
};

JugState getJugStateFromIndex(int index)
{
	int jug2 = index % 10;
	index = index / 10;

	int jug1 = index % 10;
	index = index / 10;

	int jug0 = index;
	return { jug0, jug1, jug2};
}

std::string getStringFromIndex(int index)
{
	auto js = getJugStateFromIndex(index);
	std::string s = "{";
	s += std::to_string(js.jugs[0]) + "," + std::to_string(js.jugs[1]) +  "," + std::to_string(js.jugs[2]);
	s += "}";
	return s;
}

int getIndexFromJugState(const JugState& jugState)
{
	return jugState.jugs[0] * 100 + jugState.jugs[1] * 10 + jugState.jugs[2];
}

int moveWater(int index, int srcJug, int dstJug)
{
	if (srcJug == dstJug)
	{
		return index;
	}

	JugState js = getJugStateFromIndex(index);

	// how much water is in the source jug
	int sourceAmount = js.jugs[srcJug];

	// how much water is in the destination jug
	int dstAmount = js.jugs[dstJug];

	// how much more water can the destination jug take
	int maxTransfer = (dstJug == 0) ? 12 : ((dstJug == 1) ? 8 : 5) - dstAmount;

	// how much to actually move
	int transfer = (sourceAmount >= maxTransfer) ? maxTransfer : sourceAmount;

	js.jugs[srcJug] -= transfer;
	js.jugs[dstJug] += transfer;

	return getIndexFromJugState(js);
}

bool canTransitionExist(int startIndex, int endIndex, unordered_set<int>& visited)
{
	if (startIndex == endIndex)
	{
		return true;
	}

	// visit the start state
	visited.insert(startIndex);

	// there's 6 possible candidate states to transition to
	int candidates[6];
	candidates[0] = moveWater(startIndex, 0, 1);
	candidates[1] = moveWater(startIndex, 0, 2);
	candidates[2] = moveWater(startIndex, 1, 0);
	candidates[3] = moveWater(startIndex, 1, 2);
	candidates[4] = moveWater(startIndex, 2, 0);
	candidates[5] = moveWater(startIndex, 2, 1);

	// let's try visiting each one of these if we haven't visited before
	for (int i = 0; i < 6; i++)
	{
		// don't visit nodes we've seen before
		if (visited.count(candidates[i]) == 0)
		{
			bool result = canTransitionExist(candidates[i], endIndex, visited);
			if (result)
			{
				return true;
			}
		}
	}
	return false;
}

bool canTransitionExist(int startIndex, int endIndex)
{
	std::unordered_set<int> visited;
	return canTransitionExist(startIndex, endIndex, visited);
}

void getValidJugStates(int targetAmount, vector<int>& validStates)
{
	for (int i = 0; i <= 12; i++)
	{
		for (int j = 0; j <= 8; j++)
		{
			for (int k = 0; k <= 5; k++)
			{
				if ((i + j + k) == targetAmount)
				{
					JugState js = { i,j,k };
					validStates.push_back(getIndexFromJugState(js));
				}
			}
		}
	}
}

int main()
{
	vector<int> validStates;

	const int capacityAmount = 12;
	getValidJugStates(capacityAmount, validStates);

	std::cout << "there are: " << validStates.size() << " initial starting states of jugs with " << capacityAmount << " units total\n";

	size_t testIndex = 0;

	for (size_t i = 0; i < validStates.size(); i++)
	{
		for (size_t j = 0; j < validStates.size(); j++)
		{
			if (i == j)
			{
				continue;
			}

			int start = validStates[i];
			int end = validStates[j];

			bool success = canTransitionExist(start, end);

			std::cout << "Test number " << testIndex << ": ";
			testIndex++;

			if (success)
			{
				std::cout << "valid path from " << getStringFromIndex(start) << " to " << getStringFromIndex(end) << endl;
			}
			else
			{
				std::cout << "there is not a path from " << getStringFromIndex(start) << " to " << getStringFromIndex(end) << endl;
			}
		}
	}

	return 0;
}