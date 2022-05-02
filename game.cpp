#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "Graphics.h"
#include "Point.h"
#include "Polygon.h"

using namespace std;

const int FPS = 100;

const int MOUSE_SENS_X = 1000;
const int MOUSE_SENS_Y = 1500;

// border margin for warping the pointer
const int marginSize = 1;

vector<Polygon> polygons = {
	Polygon(
		{
			Point(100, 100, 1000),
			Point(300, 100, 1000),
			Point(300, 300, 1000),
			Point(100, 300, 1000)
		}
	)
};

vector<Polygon> screenPolygons(polygons.size());

double angleX = 0, angleY = 0;

int mouseX = -1, mouseY = -1;

int windowWidth, windowHeight;

Graphics g(polygons, FPS);

int main() {
  // Open the server connection
  Display* display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  // Get the default screen of the display (needed for getting the root window)
  int screen = DefaultScreen(display);

  // set some color constants
  unsigned long black = BlackPixel(display, screen);
  unsigned long white = WhitePixel(display, screen);

  // Get the root window (neeeded for opening a window)
  Window root = RootWindow(display, screen);


  // Create the window (does not appear on the screen yet)
  Window window = XCreateSimpleWindow(display, root, 0, 0, 200, 100, 0, black, white);

  /* process window close event through event handler so XNextEvent does not fail */
  //Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
  //XSetWMProtocols(display, window, &del_window, 1);

  // Set the types of events we are interested in
  // - StructureNotifyMask: for when the window first appears on screen (MapNotify event)
  // - KeyPressMask: for getting keyboard pressed events (KeyPress)
  // - KeyReleaseMask: for getting keyboard released events (KeyRelease)
  // - PointerMotionMask: for getting mouse pointer movement events (MotionNotify)
  XSelectInput(display, window, StructureNotifyMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);

  // Make the window appear on the screen
  XMapWindow(display, window);

  // Set parameters for creating the Graphics Context
  XGCValues values;
  values.line_width = 4;
  values.foreground = black;
  unsigned long valuemask = GCLineWidth | GCForeground;

  // Create a Graphics Context (set of reusable drawing parameters)
  GC gc = XCreateGC(display, window, valuemask, &values);

  // Initialize the game state
  int velX = 0;
  int velY = 0;

  // Set up a sleep data structure (needed for calling sleep() later)
  struct timespec ts;
  ts.tv_sec = 0;
  //ts.tv_nsec = 33333333;
  ts.tv_nsec = 1000000000/FPS;

  // Wait for MapNotify event (when the window appears on the screen)
  while (1) {
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == MapNotify)
      break;
  }

  // Set minimum and initial window sizes

  // Pointer to the size hints structure
  XSizeHints* win_size_hints = XAllocSizeHints();
  if (!win_size_hints) {
    fprintf(stderr, "XAllocSizeHints - out of memory\n");
    exit(1);
  }

  // initialize the structure appropriately.
  // first, specify which size hints we want to fill in.
  // in our case - setting the minimal size as well as the initial size.
  win_size_hints->flags = PSize | PMinSize;

  // next, specify the desired limits.
  // in our case - make the window's size at least 300x200 pixels.
  // and make its initial size 400x250.
  win_size_hints->min_width = 300;
  win_size_hints->min_height = 200;
  win_size_hints->base_width = 400;
  win_size_hints->base_height = 250;

  // pass the size hints to the window manager.
  XSetWMNormalHints(display, window, win_size_hints);

  // finally, we can free the size hints structure.
  XFree(win_size_hints);

  // Initialize window size variables
  XWindowAttributes winAttributes;
  XGetWindowAttributes(display, window, &winAttributes);
  windowWidth = winAttributes.width;
  windowHeight = winAttributes.height;

  // Set up a tiny 1x1 cursor

  // Create a 1x1 pixmap width a depth of 1
  Pixmap cursorMask = XCreatePixmap(display, window, 1, 1, 1);

  // Set up a Graphics Context for the cursorMask that only writes 0's (GXclear)
  XGCValues xgc;
  xgc.function = GXclear;
  GC clearGc =  XCreateGC(display, cursorMask, GCFunction, &xgc);

  // Draw a blank 1x1 rectangle on the cursorMask pixmap (which is already only a 1x1 in size)
  XFillRectangle(display, cursorMask, clearGc, 0, 0, 1, 1);

  // Create a white color for the cursor .
  // The cursor will only be 1x1 pixel large so
  // it doesn't really matter which color we use.
  XColor whiteColor;
  whiteColor.pixel = 0;
  whiteColor.red = 65535;
  whiteColor.green = 65535;
  whiteColor.blue = 65535;
  whiteColor.flags = 0;

  // Create the cursor with the cursorMask
  Cursor cursor = XCreatePixmapCursor(display, cursorMask, cursorMask,
                                      &whiteColor,&whiteColor, 0,0);
  XFreePixmap(display,cursorMask);
  XFreeGC(display,clearGc);

  // Grab pointer focus and warp pointer to center of window
  XGrabPointer(display, window, true, PointerMotionMask, GrabModeAsync, GrabModeAsync, window, cursor, CurrentTime);

	// Set up polygons

  vector<Polygon> morePolygons;
	for (int x = -1000; x < 1000; x += 200) {
		morePolygons.push_back(
				Polygon({
					Point(x, 1000, -10000),
					Point(x+50, 1000, -10000),
					Point(x+50, 1000, 10000),
					Point(x, 1000, 10000)
					})
			);
	}

  g.addPolygons(morePolygons);

  // Key pressed states
  bool leftArrowKeyIsPressed = 0;
  bool rightArrowKeyIsPressed = 0;
  bool upArrowKeyIsPressed = 0;
  bool downArrowKeyIsPressed = 0;
  bool shiftIsPressed = 0;
  bool spaceIsPressed = 0;
  int dx = 0, dy = 0; // change in mouse position

  // Game loop
  while (1) {

    // If there are any events waiting, handle them.
    //
    // If the event queue is not empty at this moment
    if (XEventsQueued(display, QueuedAlready)) {
      // Get the next event
      XEvent event;
      XNextEvent(display, &event);

      // Handle events
      if (event.type == MotionNotify) {
        XMotionEvent xMotionEvent = event.xmotion;
        if (mouseX == -1 && mouseY == -1) {
          // First event; init mouse pos variables
          mouseX = xMotionEvent.x;
          mouseY = xMotionEvent.y;
        } else if (xMotionEvent.x < marginSize
                   || xMotionEvent.x > windowWidth - marginSize
                   || xMotionEvent.y < marginSize
                   || xMotionEvent.y > windowHeight - marginSize) {
          // If the pointer is near the edge of the window, warp it back to the center
          // Warp the pointer to the center
          XWarpPointer(display, None, window, 0, 0, 0, 0, windowWidth / 2, windowHeight / 2);

          // Ignore the next few events of the pointer moving in the border
          do {
            XNextEvent(display, &event);
            xMotionEvent = event.xmotion;
          } while (event.type == MotionNotify
                   && (xMotionEvent.x < marginSize
                       || xMotionEvent.x > windowWidth - marginSize
                       || xMotionEvent.y < marginSize
                       || xMotionEvent.y > windowHeight - marginSize));

          // Ignore the next event because
          // XWarpPointer produces a mouse movement event
          // as if the user had suddenly moved the pointer
          // to the destination location
          XNextEvent(display, &event);

          mouseX = windowWidth / 2;
          mouseY = windowHeight / 2;
        } else {
          dx = xMotionEvent.x - mouseX;
          dy = xMotionEvent.y - mouseY;
          mouseX = xMotionEvent.x;
          mouseY = xMotionEvent.y;
        }
      } else if (event.type == ConfigureNotify) {
        // If window is resized, update local window size variables
        XConfigureEvent xConfEvent = event.xconfigure;
        if (xConfEvent.width != windowWidth) {
          windowWidth = xConfEvent.width;
        }
        if (xConfEvent.height != windowHeight) {
          windowHeight = xConfEvent.height;
        }
      } else if (event.type == KeyPress) {
        // Get the key symbol (XK_Left, XK_Up, etc.) of the KeyPressed event
        KeySym keySymbol = XKeycodeToKeysym(display, event.xkey.keycode, 0);

        // Handle event depending on which key was pressed
        switch (keySymbol) {
        // Set appropriate key state
        case XK_a:
        case XK_Left:
          leftArrowKeyIsPressed = 1;
          break;
        case XK_d:
        case XK_e: // for dvorak layout
        case XK_Right:
          rightArrowKeyIsPressed = 1;
          break;
        case XK_w:
        case XK_comma: // for dvorak layout
        case XK_Up:
          upArrowKeyIsPressed = 1;
          break;
        case XK_s:
        case XK_o: // for dvorak layout
        case XK_Down:
          downArrowKeyIsPressed = 1;
          break;
        case XK_Shift_L:
          shiftIsPressed = 1;
          break;
        case XK_space:
          spaceIsPressed = 1;
          break;

        // If 'q' is pressed, exit
        case XK_q:
          goto breakout;
          break;
        }
      } else if (event.type == KeyRelease) {
        // Check if a KeyPress event for the same key is queued next.
        // If so, discard this event and the next event.
        // This eliminates auto-repeated keypresses.
        //
        // If there are events in the queue at this moment (check without flushing the queue)
        if (XEventsQueued(display, QueuedAlready)) {
          // Get the event
          XEvent nextEvent;
          XPeekEvent(display, &nextEvent);

          // If the next event is a KeyPress AND is of the same keycode, then discard the two events
          if (nextEvent.type == KeyPress && nextEvent.xkey.keycode == event.xkey.keycode) {
            XNextEvent(display, &event);
            continue;
          }
        }

        // Get the key symbol (XK_Left, XK_Up, etc.) of the KeyPressed event
        //
        KeySym keySymbol = XKeycodeToKeysym(display, event.xkey.keycode, 0);

        // Handle event depending on which key was released
        switch (keySymbol) {
        // Unset appropriate key state
        case XK_a:
        case XK_Left:
          leftArrowKeyIsPressed = 0;
          break;
        case XK_d:
        case XK_e: // for dvorak layout
        case XK_Right:
          rightArrowKeyIsPressed = 0;
          break;
        case XK_w:
        case XK_comma: // for dvorak layout
        case XK_Up:
          upArrowKeyIsPressed = 0;
          break;
        case XK_s:
        case XK_o: // for dvorak layout
        case XK_Down:
          downArrowKeyIsPressed = 0;
          break;
        case XK_Shift_L:
          shiftIsPressed = 0;
          break;
        case XK_space:
          spaceIsPressed = 0;
          break;
        }
      }
    }

    // Update window size variables
    XGetWindowAttributes(display, window, &winAttributes);
    windowWidth = winAttributes.width;
    windowHeight = winAttributes.height;

    // Update game state

    // Translate each polygon
    if (leftArrowKeyIsPressed) {
      g.moveLeft(angleX);
    }
    if (rightArrowKeyIsPressed) {
      g.moveRight(angleX);
    }
    if (upArrowKeyIsPressed) {
      g.moveForward(angleX, angleY);
    }
    if (downArrowKeyIsPressed) {
      g.moveBackward(angleX, angleY);
    }
    if (spaceIsPressed) {
      g.moveUp();
    }
    if (shiftIsPressed) {
      g.moveDown();
    }

    // Set up rotation angles
    angleX += (double) -dx / MOUSE_SENS_X;
    angleY += (double) dy / MOUSE_SENS_Y;

    // Rotate the polygons
    g.turn(angleX, angleY);

    // Clear the window
    XClearWindow(display, window);

    // Clip and project the polygons onto the viewing plane
    g.render(screenPolygons);

    // Display them on the screen
    for (int i = 0; i < screenPolygons.size(); ++i) {

			int numVertices = screenPolygons[i].vertices.size();
			XPoint xPoints[numVertices];

      // Set up the XPoints array (for drawing on screen)
			for (int j = 0; j < numVertices; ++j) {

        // Copy over the coordinates
				xPoints[j].x = screenPolygons[i].vertices[j].x;
				xPoints[j].y = screenPolygons[i].vertices[j].y;

				// Translate from game coordinates to screen coordinates (for XFillPolygon)
				xPoints[j].x += windowWidth / 2;
				xPoints[j].y += windowHeight / 2;
			}

			// Fill in a polygon connecting each of the points
			XFillPolygon(display, window, gc, xPoints, 4, Complex, CoordModeOrigin);

			// Flush all events (send them to the server)
			XFlush(display);
		}

    // Reset rotation variables
    dx = 0;
    dy = 0;

    // Pause for a bit
    nanosleep(&ts, NULL);
  }

// Cleanup and exit
breakout:

  // Detroy the window
  XDestroyWindow(display, window);

  // Free the memory allocated for the cursor
  XFreeCursor(display, cursor);

  // Close server connection
  XCloseDisplay(display);

  return 0;
}

