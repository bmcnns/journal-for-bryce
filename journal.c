#include <ncurses.h>
#include <ncurses.h>
#include <string.h>
#include <stdio.h>

#define DIALOG_BOX_WIDTH (COLS - (COLS / 8))
#define DIALOG_BOX_HEIGHT (LINES - (LINES / 8))

#define DIALOG_BOX_Y (LINES - DIALOG_BOX_HEIGHT) / 2
#define DIALOG_BOX_X (COLS - DIALOG_BOX_WIDTH) / 2

#define TODO_WINDOW_HEIGHT DIALOG_BOX_HEIGHT - 3 - 1
#define TODO_WINDOW_WIDTH (DIALOG_BOX_WIDTH / 3)
#define TODO_WINDOW_Y DIALOG_BOX_Y + 3
#define TODO_WINDOW_X DIALOG_BOX_X + 1

#define NOTES_BOX_WIDTH (DIALOG_BOX_WIDTH - TODO_WINDOW_WIDTH) - 2
#define NOTES_BOX_HEIGHT DIALOG_BOX_HEIGHT - 3 - 1
#define NOTES_BOX_Y DIALOG_BOX_Y + 3 
#define NOTES_BOX_X TODO_WINDOW_WIDTH + TODO_WINDOW_X

#define DATE_MAX_LENGTH 20
#define TODO_MAX_LENGTH 50
#define NOTE_MAX_LENGTH 500

#define MAX_TODO_ITEMS 20

WINDOW* create_dialog_box()
{
     WINDOW* window = newwin(DIALOG_BOX_HEIGHT, DIALOG_BOX_WIDTH, DIALOG_BOX_Y, DIALOG_BOX_X);

     wattron(window, A_BOLD);
     box(window, 0, 0);
     mvwprintw(window, 0, (DIALOG_BOX_WIDTH - 17) / 2, "JOURNAL OF BRYCE");
     wattroff(window, A_BOLD);

     wbkgd(window, COLOR_PAIR(2));

     return window;
}

WINDOW* create_notes_window()
{
     WINDOW* window = newwin(NOTES_BOX_HEIGHT, NOTES_BOX_WIDTH, NOTES_BOX_Y, NOTES_BOX_X);
     wbkgd(window, COLOR_PAIR(4));
     return window;
}

void draw_headers(WINDOW* window, const char* date)
{
     wattron(window, A_BOLD);
     mvwprintw(window, 2, DIALOG_BOX_WIDTH - (TODO_WINDOW_WIDTH / 2) - 2, "DATE %s", date);
     mvwprintw(window, 2, TODO_WINDOW_WIDTH + TODO_WINDOW_X - DIALOG_BOX_X, "NOTES");
     mvwprintw(window, 2, 2, "TO DO");
     wattroff(window, A_BOLD);
     wrefresh(window);
}

WINDOW* create_todo_window()
{
     WINDOW* window = newwin(TODO_WINDOW_HEIGHT, TODO_WINDOW_WIDTH, TODO_WINDOW_Y, TODO_WINDOW_X);

     wbkgd(window, COLOR_PAIR(3));

     return window;
}

WINDOW* draw_todo_window(WINDOW* window, char* todo_items[], int num_items, int selected_item)
{
     for (int i = 0; i < num_items; i++)
     {
	  if (i == selected_item)
	  {
	       wattron(window, A_STANDOUT);
	  }

	  mvwprintw(window, 2 + i, 0, "%-*s", TODO_MAX_LENGTH, todo_items[i]);
	  wattroff(window, A_STANDOUT);
     }
     
     wrefresh(window);
     return window;
}

void draw_notes_column(WINDOW* window, const char* note)
{
     mvwprintw(window, 1, 0,  "%-*s", NOTE_MAX_LENGTH, note);
     wrefresh(window);
}

void write_to_file(char* date, char* note, char* todo_items[])
{
     char filename[DATE_MAX_LENGTH];
     sprintf(filename, "%s.txt", date);

     FILE* file = fopen(filename, "w");

     fprintf(file, "%s\n", date);

     for (int i = 0; i < MAX_TODO_ITEMS; i++)
     {
	  fprintf(file, "%s\n", todo_items[i]);
     }
     
     fprintf(file, "%s\n", note);

     fclose(file);
}

int main()
{
     initscr();
     start_color();
     curs_set(0);
     cbreak();
     noecho();

     keypad(stdscr, TRUE);

     init_pair(1, COLOR_BLUE, COLOR_BLUE);
     init_pair(2, COLOR_BLACK, COLOR_WHITE);
     init_pair(3, COLOR_BLACK, COLOR_WHITE);
     init_pair(4, COLOR_BLACK, COLOR_WHITE);

     wbkgd(stdscr, COLOR_PAIR(1));

     refresh();

     char date[DATE_MAX_LENGTH] = "2024-01-01";
     char note[NOTE_MAX_LENGTH] = "[ INSERT NOTES ]";

     char *todo_items[MAX_TODO_ITEMS] = {
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
	  strdup("[ ]"),
     };

     int num_todo_items = 20;
     int selected_item = 0;
     
     WINDOW *dialog_box = create_dialog_box();
     WINDOW *notes_window = create_notes_window();
     WINDOW *todo_window = create_todo_window();

     while (true)
     {
	  //clear();
	  //refresh();
	  

	  wrefresh(dialog_box);
	  draw_headers(dialog_box, date);
	  draw_notes_column(notes_window, note);
	  draw_todo_window(todo_window, todo_items, num_todo_items, selected_item);

	  int ch = wgetch(dialog_box);

	  if (ch == 'q')
	  {
	       break;
	  }

	  else if (ch == 'i')
	  {
	       selected_item = (selected_item + 1) % num_todo_items;
	       draw_todo_window(todo_window, todo_items, num_todo_items, 2);
	  }

	  else if (ch == 'u')
	  {
	       selected_item = (selected_item - 1 + num_todo_items) % num_todo_items;
	       draw_todo_window(todo_window, todo_items, num_todo_items, selected_item);
	  }
	  
	  else if (ch == 't')
	  {
	       int todo_length = strlen(todo_items[selected_item]);
	       int new_ch;

	       while ((new_ch = wgetch(todo_window)) != 10 && todo_length < TODO_MAX_LENGTH)
	       {
		    if (new_ch == 127)
		    {
			 todo_length--;
			 todo_items[selected_item][todo_length] = '\0';
			 draw_todo_window(todo_window, todo_items, num_todo_items, selected_item);
		    }
		    else
		    {
			 todo_items[selected_item][todo_length] = (char)new_ch;
			 todo_length++;
			 draw_todo_window(todo_window, todo_items, num_todo_items, selected_item);
		    }
	       }

	       todo_items[selected_item][todo_length] = '\0';
	  }

	  else if (ch == 'd')
	  {
	       int new_ch;
	       int date_length = 0;

	       while ((new_ch = wgetch(dialog_box)) != 10 && date_length < DATE_MAX_LENGTH)
	       {
		    if (new_ch == 127)
		    {
			 date_length--;
			 date[date_length] = '\0';
			 draw_headers(dialog_box, date);
		    }
		    else if (date_length < DATE_MAX_LENGTH - 1)
		    {
			 date[date_length] = (char)new_ch;
			 date_length++;
			 date[date_length] = '\0';
			 draw_headers(dialog_box, date);
		    }
	       }
	  }
	  
	  else if (ch == 'n')
	  {
	       int new_ch;
	       int note_length = strlen(note);

	       while ((new_ch = wgetch(notes_window)) != 10 && note_length < NOTE_MAX_LENGTH)
	       {
		    if (new_ch == 127)
		    {
			 note_length--;
			 note[note_length] = '\0';
			 draw_notes_column(notes_window, note);
		    }
		    else if (note_length < NOTE_MAX_LENGTH - 1)
		    {
			 note[note_length] = (char)new_ch;
			 note_length++;
			 note[note_length] = '\0';
			 draw_notes_column(notes_window, note);
		    }
	       }
	  }

	  else if (ch == 's')
	  {
	       write_to_file(date, note, todo_items);
	  }

	  
     }

     delwin(notes_window);
     delwin(dialog_box);
     endwin();
     return 0;
}

