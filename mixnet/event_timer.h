#ifndef EVENT_TIMER_H
#define EVENT_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque timer type
typedef struct event_timer event_timer_t;

/**
 * @brief Allocate and initialize a new event timer.
 * 
 * The timer is initialized with the current time as the timestamp for the 
   "last event" that occured.
 * 
 * @return Pointer to the allocated event_timer_t, or NULL on failure.
 */
event_timer_t* event_timer_init(void);

/**
 * @brief Get the time elapsed since the last event (in ms).
 * 
 * Does NOT update the last event timestamp.
 * 
 * @param timer Pointer to the event timer
 * @return uint32_t Time elapsed in milliseconds
 */
uint32_t event_timer_get_time_elapsed_since_last_event(const event_timer_t* timer);

/**
 * @brief Get the time elapsed since the last event (in ms) AND update the last event timestamp.
 * 
 * This should be used if you have received a new event and want to refresh the timer.
 * 
 * @param timer Pointer to the event timer
 * @return uint32_t Time elapsed in milliseconds
 */
uint32_t event_timer_get_time_elapsed_since_last_event_and_log_event(event_timer_t* timer);

/**
 * @brief Reset the timer by logging the current time as the last event.
 * 
 * @param timer Pointer to the event timer
 */
void event_timer_log_event(event_timer_t* timer);

/**
 * @brief Free any memory associated with the event timer.
 * 
 * @param timer Pointer to the event timer to free
 */
void event_timer_free(event_timer_t* timer);

#ifdef __cplusplus
}
#endif

#endif // EVENT_TIMER_H
