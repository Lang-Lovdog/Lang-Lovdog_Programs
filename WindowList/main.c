#include <xcb/xcb.h>
#include <stdio.h>
#include <stdlib.h>

void get_active_window_list(xcb_connection_t *connection);

int main(int argc, char *argv[]) {
  xcb_connection_t *connection = xcb_connect(NULL, NULL);
  if (connection == NULL) {
    fprintf(stderr, "Failed to connect to X server\n");
    return 1;
  }
  return 0;
}

// Get active window list
void get_active_window_list(xcb_connection_t *connection) {
  // Get root window
  xcb_window_t root = xcb_setup_roots_iterator(xcb_get_setup(connection)).data->root;
  xcb_window_t theWindow = 0;
  // Get active window list
  while (theWindow) {
    theWindow = xcb_query_tree(connection, root).first_child;
  }
  // Close connection
  xcb_disconnect(connection);
  return;
}
