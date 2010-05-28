/**
 * \file event.cpp
 * \author Peter Grogono and others
 */

#include "event.h"
#include "agent.h"
#include "cell.h"
#include "model.h"

#include <iostream>

/**
 * Construct a dummy event with all pointers null.
 */
Event::Event()
: kind(DUMMY), pH(0), cFrom(0), cTo(0)
{ }

Event::~Event()
{
}

/**
 * Make this \e Event a dummy event.
 */
void Event::setDummy()
{
   kind = DUMMY;
   pH = 0;
   cFrom = 0;
   cTo = 0;
}

/**
 * Make this \a Event a move event.
 * \param h is a pointe to the \a Agent to e moved.
 * \param from is a pointer to the \a Cell that 
 * currently contains the \a Agent.
 * \param to is a pointer to the destination \a Cell.
 */
void Event::setMove(Agent *h, Cell *from, Cell *to)
{
   pH = h;
   cFrom = from;
   cTo = to;
   kind = MOVE;
}

/**
 * Make this event record the birth of a new \a Agent.
 * \param pa is a pointer to the new \a Agent.
 * \param pc is a pointer to the \a Cell that will contain the \a Agent.
 */
void Event::setBirth(Agent *pa, Cell *pc)
{
   pH = pa;
   cFrom = pc;
   kind = BIRTH;
}

/**
 * Make this event record the death of an \a Agent.
 * \param h is a pointer to the dead \a Agent.
 * \param place is the \a Cell that contains the \a Agent.
 */
void Event::setDeath(Agent *h, Cell *place)
{
   pH = h;
   cFrom = place;
   kind = DEATH;
}

/**
 * Perform the action described by this \a Event.
 */
void Event::act()
{
   switch (kind)
   {
   case DUMMY:
      break;
   case MOVE:
      cFrom->remove(pH);
      cTo->insert(pH);
      pH->setCell(cTo);
      break;
   case BIRTH:
      cFrom->insert(pH);  
      break;
   case DEATH:
	  cFrom->remove(pH);
	  //delete pH;
      break;
   }
}

/**
 * Return a free event.
 * Events are stored on a linked list and are never deleted from this list.
 * The list is extended whenever necessary.  After the simulation has run
 * for a while, the list will be long enough and new events will be created
 * only occasionally.
 */
Event *getEvent(std::list<Event*> *eventList)
{
	Event* eventToReturn;
	bool foundDummy = false;

	if (eventList->size() > 0) {
		std::list<Event*>::iterator eventIter = eventList->begin();
		while( eventIter != eventList->end() ) {
			if ((*eventIter)->getKind() == DUMMY) {
				eventToReturn = *eventIter;
				foundDummy = true;
				break;
			}
			eventIter++;
		}
	}

	if (eventList->size() == 0 || !foundDummy)
	{
		eventToReturn = new Event;
		eventList->push_back(eventToReturn);
	}

	return eventToReturn;
}

/**
 * Process the events in an event list.
 * \param first is a pointer to the first event in the list.
 * \param last is a pointer to the last event in the list.
 */
void processEvents(std::list<Event*> *eventList)
{
	std::list<Event*>::iterator eventIter = eventList->begin();
	while( eventIter != eventList->end() ) {
		std::list<Event*>::iterator iter = eventIter;
		eventIter++;
		(*iter)->act();
		eventList->erase(iter);
//		delete (*iter);
	}
}

