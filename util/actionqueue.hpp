#ifndef __ACTIONQUEUE_HPP__
#define __ACTIONQUEUE_HPP__

#include <mutex>
#include <deque>
#include <vector>
#include <string>
#include "action.hpp"

class ActionQueue
{
	private:
		std::mutex acess;
		std::deque<Action> actions;

	public:
		ActionQueue(void){};
		void pushFront(std::vector<Action> newActions);
		void pushFront(Action newAction);
		void pushBack(Action newAction);
		Action pop(void);
		bool isEmpty(void);
};

#endif
