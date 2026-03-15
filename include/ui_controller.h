#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include "ui_state.h"
#include "ui_view.h"

/**
 * Processes user interactions, updates the application state, and refreshes the view accordingly.
 * @param state Pointer to the application state structure that holds the current state of the application.
 * @param view Pointer to the application view structure that holds the UI components and rendering context.
 */
void process_user_interactions(ApplicationState* state, ApplicationView* view);

#endif // UI_CONTROLLER_H