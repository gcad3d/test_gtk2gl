// test_gtk2gl.c
// disp all callbacks

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

#include <GL/gl.h>
// #include <GL/glu.h>


GtkWidget *window, *glarea;

int   cPosX, cPosY;  // position of mouse
float quat[4];        // orientation of object
float zoom;           // field of view in degrees

// Pointf cenPt;         // view-center in user-coords
// // Modelsize; box from 2 points; all coords must be inside this box.
// Pointf mdlMax = {30.0, 30.0,  30.0};
// Pointf mdlMin = {-30.0, -30.0,  -30.0};



// prototypes:
int cb_init (GtkWidget *widget);
int cb_draw (GtkWidget *widget, GdkEventExpose *event);
int cb_reshape (GtkWidget *widget, GdkEventConfigure *event);
int cb_motion (GtkWidget *widget, GdkEventMotion *event);
int cb_key_press (GtkWidget *widget, void *event);
int tst_CB_b1 (GtkWidget *parent, void *data);
int tst_CB_b2 (GtkWidget *parent, void *data);



//================================================================
  int cb_init (GtkWidget *widget) {
//================================================================

  GtkAllocation allocation;

  printf("cb_init \n");


  // OpenGL functions can be called only if make_current returns true
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
      gtk_widget_get_allocation (widget, &allocation);
      glViewport(0, 0, allocation.width, allocation.height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0,100, 100,0, -1,1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
    }
  return TRUE;
}


//================================================================
  int cb_draw (GtkWidget *widget, GdkEventExpose *event) {
//================================================================
// When widget is exposed it's contents are redrawn.

  // Draw only last expose.
  if (event->count > 0) return TRUE;

  printf(" cb_draw\n");


  // OpenGL functions can be called only if make_current returns true
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {

      // Draw simple triangle
      glClearColor(0,0,0,1);
      glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(1,1,1);
      glBegin(GL_TRIANGLES);
      glVertex2f(10,10);
      glVertex2f(10,90);
      glVertex2f(90,90);
      glEnd();

      // Swap backbuffer to front
      gtk_gl_area_swap_buffers(GTK_GL_AREA(widget));

    }

  return TRUE;
}


//================================================================
  int cb_reshape (GtkWidget *widget, GdkEventConfigure *event) {
//================================================================
// When glarea widget size changes, viewport size is set to match the new size
// OpenGL functions can be called only if make_current returns true

  GtkAllocation allocation;

  printf(" cb_reshape\n");


  if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) {
      gtk_widget_get_allocation (widget, &allocation);
      glViewport(0, 0, allocation.width, allocation.height);
    }

  return TRUE;
}


//====================================================================
  int cb_button_press (GtkWidget *widget, void *ev) {
//====================================================================
// callback mousebutton

  int      typ, ikey, iTyp, i1;



  typ  = ((GdkEventAny*)ev)->type;

  if(typ == GDK_BUTTON_PRESS) {
    iTyp = 1; // TYP_EventPress;
    ikey = ((GdkEventButton*)ev)->button;

  } else if(typ == GDK_BUTTON_RELEASE) {
    iTyp = 0; // TYP_EventRelease;
    ikey = ((GdkEventButton*)ev)->button;

  } else if(typ == GDK_SCROLL) {
    iTyp = 2; // TYP_EventPress;
    if(((GdkEventScroll*)ev)->direction == GDK_SCROLL_UP) ikey = 1; // GUI_MouseScUp;
    else                                                  ikey = -1; // GUI_MouseScDown;

  } //else return FALSE;  // TRUE=1   FALSE=0



  printf(" cb_button_press iTyp=%d ikey=%d pos = %d %d\n",iTyp,ikey,cPosX,cPosY);

  return TRUE;
}


//====================================================================
  int cb_motion (GtkWidget *widget, GdkEventMotion *event) {
//====================================================================
// callback mousemove

  GdkModifierType state;
  GtkAllocation allocation;




  // if (event->is_hint) {
    gtk_widget_get_pointer (widget, &cPosX, &cPosY);
    state = event->state;         // state of mouse-buttons
  // }


  gtk_widget_get_allocation (widget, &allocation);

      printf(" cb_motion state=%d pos %d %d siz %d %d\n",
              state, cPosX, cPosY, allocation.width, allocation.height);

/*
  if (state & GDK_BUTTON1_MASK) {
    // drag in progress, simulate trackball
    trackball_rot (quat, beginx, beginy, x, y, area.width, area.height, &cenPt);
    glarea_expose (NULL, NULL);
  }

  if (state & GDK_BUTTON2_MASK) {
    // zooming drag
    trackball_zoom (&zoom, beginx, beginy, x, y, area.width, area.height);
    glarea_expose (NULL, NULL);
  }

  if (state & GDK_BUTTON3_MASK) {
    // zooming drag
    trackball_pan (&cenPt, beginx, beginy, x, y, area.width, area.height);
    glarea_expose (NULL, NULL);
  }
*/

  return TRUE;
}


//================================================================
  int cb_key_press (GtkWidget *widget, void *event) {
//================================================================
 
  int    i1, typ, iTyp = -1, ikey, imod;


  typ  = ((GdkEventAny*)event)->type;

  if(typ == GDK_KEY_PRESS) {
    iTyp = 1; // TYP_EventPress;
  } else if(typ == GDK_KEY_RELEASE) {
    iTyp = 0; // TYP_EventRelease;
  } // else return FALSE;  // TRUE=1   FALSE=0

  ikey = ((GdkEventKey*)event)->keyval;
  imod = ((GdkEventKey*)event)->state;
    printf("GUI_gl_key key=%d ev=%d state=%d\n",ikey,iTyp,imod);

  // get state of modifier-keys (shift ..)
  gtk_get_current_event_state ((GdkModifierType*)&i1);
    printf(" _keys_mod %d\n",i1);


  return TRUE; // TRUE=1: do no defaultOperations
}



//====================================================================
int main (int argc, char **argv) {
//====================================================================


  GtkWidget    *box1, *box2, *bt1, *bt2;
  gchar        *info_str;
  int attrlist[] = {
    GDK_GL_RGBA,
    GDK_GL_RED_SIZE,1,
    GDK_GL_GREEN_SIZE,1,
    GDK_GL_BLUE_SIZE,1,
    GDK_GL_DOUBLEBUFFER,
    GDK_GL_NONE
  };




  printf("tst_GL_gtk_2.c\n");

  // initialize gtk
  gtk_init( &argc, &argv );


  // Check if OpenGL is supported.
  if (gdk_gl_query() == FALSE) {
    g_print("OpenGL not supported\n");
    return 0;
  }


  // Create new top level window.
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "test_gtkgl");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  // define events to handle
  gtk_widget_set_events(GTK_WIDGET (window),
                        GDK_ENTER_NOTIFY_MASK |
                        GDK_KEY_PRESS_MASK |
                        GDK_KEY_RELEASE_MASK);

  // Quit form main if got delete event
  g_signal_connect(G_OBJECT(window), "delete-event",
                   G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect (window, "enter-notify-event",
                    G_CALLBACK(gtk_widget_grab_focus), (void*)TRUE);


  // box with 2 buttons on top; OpenGL-window below.
  box1 = gtk_vbox_new (FALSE, 0);   // TRUE=expand, FALSE=min-space
  gtk_container_add (GTK_CONTAINER (window), box1);

  box2 = gtk_hbox_new (FALSE, 0);
  gtk_container_add (GTK_CONTAINER (box1), box2);

  bt1 = gtk_button_new_with_label (" Button-1 ");
  g_signal_connect (bt1, "clicked", G_CALLBACK(tst_CB_b1), NULL);
  gtk_container_add (GTK_CONTAINER (box2), bt1);

  bt2 = gtk_button_new_with_label (" Button-2 ");
  g_signal_connect (bt2, "clicked", G_CALLBACK(tst_CB_b2), NULL);
  gtk_container_add (GTK_CONTAINER (box2), bt2);


  // connect key-press
  g_signal_connect (window, "key-press-event",
          G_CALLBACK(cb_key_press), NULL);

  // connect delete ("X"-button)
  gtk_quit_add_destroy(1, GTK_OBJECT(window));


  // Create new OpenGL widget.
  glarea = GTK_WIDGET(gtk_gl_area_new(attrlist));


  // Events for widget must be set before X Window is created
  gtk_widget_set_events(GTK_WIDGET (glarea),
                        GDK_EXPOSURE_MASK |
                        GDK_SCROLL_MASK |
                        GDK_BUTTON_PRESS_MASK |
                        GDK_BUTTON_RELEASE_MASK |
                        GDK_POINTER_MOTION_MASK |
                        GDK_POINTER_MOTION_HINT_MASK);


  // Connect signal handlers
  // Redraw image when exposed. gtk3: "draw"
  g_signal_connect(G_OBJECT(glarea), "expose-event",
                   G_CALLBACK(cb_draw), NULL);

  // When window is resized viewport needs to be resized also.
  g_signal_connect(G_OBJECT(glarea), "configure-event",
                   G_CALLBACK(cb_reshape), NULL);

  // do init when GL-win has been realized.
  g_signal_connect(G_OBJECT(glarea), "realize",
                   G_CALLBACK(cb_init), NULL);

  // connect GL-win mouse-move
  g_signal_connect (glarea, "motion_notify_event",
          G_CALLBACK(cb_motion), NULL);

  // connect GL-win mouse-button-press
  g_signal_connect (glarea, "button_press_event",
                    G_CALLBACK(cb_button_press), NULL);

  g_signal_connect (glarea, "button_release_event",
                    G_CALLBACK(cb_button_press), NULL);

  g_signal_connect (glarea, "scroll_event",
                    G_CALLBACK(cb_button_press), NULL);


  // set minimum size for GL-win
  gtk_widget_set_size_request(GTK_WIDGET(glarea), 400,400);


  // put glarea into window and show it all
  gtk_container_add (GTK_CONTAINER (box1), glarea);

  gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_MOUSE);

  gtk_widget_show_all (window);


  // vendor dependent version info string
  info_str = gdk_gl_get_info();
  g_print(info_str);
  g_free(info_str);


  // showObjects();

  gtk_main();

  return 0;
}


//================================================================
  int tst_CB_b1 (GtkWidget *parent, void *data) {
//================================================================
// callback-function

  printf("tst_CB_b1 \n");

  return 0;

}


//================================================================
  int tst_CB_b2 (GtkWidget *parent, void *data) {
//================================================================
// callback-function

  printf("tst_CB_b2 \n");

  return 0;

}


//================== EOF ==================================
