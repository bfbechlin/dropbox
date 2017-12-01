#include "actionqueue.hpp"
#include <iostream>
void ActionQueue::pushFront(std::vector<Action> newActions)
{
	std::unique_lock<std::mutex> lck(this->acess);
	for (std::vector<Action>::reverse_iterator it = newActions.rbegin(); it != newActions.rend(); ++it)
	{
		this->actions.push_front(*it);
	}
}

void ActionQueue::pushFront(Action newAction)
{
	std::unique_lock<std::mutex> lck(this->acess);
	this->actions.push_front(newAction);
}
void ActionQueue::pushBack(Action newAction)
{
	std::unique_lock<std::mutex> lck(this->acess);
	this->actions.push_back(newAction);
}

Action ActionQueue::pop(void)
{
	std::unique_lock<std::mutex> lck(this->acess);
	if(!this->actions.empty())
	{
		Action action = this->actions.front();
		this->actions.pop_front();
		return action;
	}
	else
	{
		return Action();
	}
}

bool ActionQueue::isEmpty(void)
{
	std::unique_lock<std::mutex> lck(this->acess);
	return this->actions.empty();
}
