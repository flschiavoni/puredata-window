/* Plugin structure generated by Schiavoni Pure Data external Generator */
#include "m_pd.h"
#include "g_canvas.h"
#include "g_all_guis.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static t_class *colorwindow_class;

typedef struct _colorwindow {
   t_object x_obj;
   t_symbol *sp_name;
   t_symbol *sp_color;
   t_glist *glist;
   t_canvas *x_canvas;
} t_colorwindow;

t_glist * colorwindow_find_parent(t_glist * glist);
void colorwindow_recursive_search(t_colorwindow *x, t_glist *c, t_symbol *sp_name);
void colorwindow_do(t_colorwindow *x, t_canvas * c);

void colorwindow_process(t_colorwindow *x){
  t_glist *c = x->x_canvas;
   if(c == NULL){
      return;
   }

   if(strcmp(x->sp_name->s_name, "") == 0){
      colorwindow_do(x, (t_canvas*) x->x_canvas);
      return;
   }
   colorwindow_recursive_search(x, c, x->sp_name);
}


void colorwindow_color(t_colorwindow *x, t_symbol * color){
   x->sp_color = color;
   colorwindow_process(x);
}

void colorwindow_set(t_colorwindow *x, t_symbol * sp_name){
   x->sp_name = sp_name;
   if(strcmp(sp_name->s_name, "") == 0){
      t_canvas *canvas=(t_canvas*)glist_getcanvas(x->glist);
      x->x_canvas = canvas;
      return;
   }
   //It was empty. Now we need to find the parent
   if(strcmp(x->sp_name->s_name, "") != 0){
      t_glist * glist = colorwindow_find_parent(x->x_canvas);
      t_canvas *canvas=(t_canvas*)glist_getcanvas(glist);
      x->x_canvas = canvas;
   }
}

void colorwindow_do(t_colorwindow *x, t_canvas * c){
   if(x->sp_color != NULL)
      sys_vgui(".x%lx.c configure -bg \"%s\";\n", c, x->sp_color->s_name);
}



void colorwindow_recursive_search(t_colorwindow *x, t_glist *c, t_symbol *sp_name){
   t_gobj*obj = NULL;
   for(obj = c->gl_list; obj; obj = obj->g_next) {
      if(pd_class(&obj->g_pd) != canvas_class)
         continue;
      //if subpatchname is empty, don't go recursively
      colorwindow_recursive_search(x, ((t_glist *)obj) ,sp_name);
      //look for subpatch name
      t_binbuf *bz = binbuf_new();
      t_symbol *patchsym;
      binbuf_addbinbuf(bz, ((t_canvas *)obj)->gl_obj.ob_binbuf);
      patchsym = atom_getsymbolarg(1, binbuf_getnatom(bz), binbuf_getvec(bz));
      binbuf_free(bz);
      if(strcmp(sp_name->s_name, patchsym->s_name) == 0
         || strcmp(sp_name->s_name, "") == 0)
         colorwindow_do(x, (t_canvas *)obj);
   }
}

t_glist * colorwindow_find_parent(t_glist * glist){
   while(glist->gl_owner){
      glist = glist->gl_owner;
   }
   return glist;
}

// Constructor of the class
void * colorwindow_new(t_symbol *sp_name_arg) {
   t_colorwindow *x = (t_colorwindow *) pd_new(colorwindow_class);
   t_glist *glist=(t_glist *)canvas_getcurrent();
   x->glist = glist;
   x->sp_name = sp_name_arg;
   // if no name, we use the actual canvas
   if(strcmp(x->sp_name->s_name, "") != 0)
      glist = colorwindow_find_parent(glist);
   t_canvas *canvas=(t_canvas*)glist_getcanvas(glist);
   x->x_canvas = canvas;
   x->sp_color = NULL;
   return (void *) x;
}

// Destroy the class
void colorwindow_destroy(t_colorwindow *x) {
   (void) x;
//   post("You say good bye and I say hello");
}


void colorwindow_setup(void) {
   colorwindow_class = class_new(gensym("colorwindow"),
      (t_newmethod) colorwindow_new, // Constructor
      (t_method) colorwindow_destroy, // Destructor
      sizeof (t_colorwindow),
      CLASS_DEFAULT,
      A_DEFSYMBOL,
      0);//Must always ends with a zero

   class_addmethod(colorwindow_class, (t_method) colorwindow_color, gensym("color"),A_DEFSYMBOL,  0);
   class_addmethod(colorwindow_class, (t_method) colorwindow_set, gensym("set"),A_DEFSYMBOL,  0);
}