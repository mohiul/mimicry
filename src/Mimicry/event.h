/**
 * \file event.h
 * \author Peter Grogono and others
 */

#ifndef EVENT_H
#define EVENT_H

class Agent;
class Cell;

/**
 * The kinds of event that may occur.
 */
enum EventKind
{
   DUMMY,      /**< The state of a new event (overwritten before event is used). */
   MOVE,       /**< An agent moves from one cell to another. */
   BIRTH,      /**< An agent is born and is added to the cell. */
   DEATH       /**< An agent dies and is removed from the cell. */
};

/**
 * An instance is a move, birth, or death event that must be processed
 * by the model but cannot be processed in the main loop.
 */
class Event
{
public:

   Event();
   void setDummy();
   void setMove(Agent *h, Cell *from, Cell *to);
   void setBirth(Agent *pa, Cell *pc);
   void setDeath(Agent *h, Cell *place);
   void act();
   friend Event *getEvent(Event *first, Event * & last);
   friend void processEvents(Event *first, Event *last);

private:

   /**
    * The kind of this event.
    */
   EventKind kind;

   /**
    * Pointer to the agent affected by this event.
    */
   Agent *pH;

   /**
    * Pointer to a cell used by this event.
    */
   Cell *cFrom;

   /**
    * Pointer to another cell used by this event.
    */
   Cell *cTo;

   /**
    * Pointer to the next event in the event list.
    */
   Event *next;
};

#endif
