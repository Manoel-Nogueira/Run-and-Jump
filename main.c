#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>

//constants
const int SCREEN_W = 600;
const int SCREEN_H = 400;
const float FPS = 100.0;
float PARALLAX_SPEED_X = 1.5;
const int JUMP_HEIGHT = 60;
const int JUMP_SPEED = 2;
const int FALL_SPEED = 4;

//function to assemble the scene
void setScene(int width_x, int height_y, char image_address[300], int clear_screen){

    if(clear_screen == 1){

        //changing the screen color
        al_clear_to_color(al_map_rgb(143,211,255));

    }

    //allegro variables
    ALLEGRO_BITMAP *Image = NULL;

    //load the image
    Image = al_load_bitmap(image_address);

    //put image on screen
    al_draw_bitmap(Image, width_x, height_y, 0);

    //destroy a variable image
    al_destroy_bitmap(Image);

}

int main (int argc, char **argv){

    float parallax_position_x = 0;
    float parallax_end = 0;
    float frame_pass_run = 0.0 ;
    int start_jump = 362;
    boolean max_jump_point = false;
    boolean pressed_jump = false;
    boolean pressed_esc = false;
    int obstacle_generation = 0;
    int difficulty = 0;
    int limit_number_cacti = 20;
    char cactus_name[150];
    FILE *cactus_file;
    int cactus_number;
    int cactus_x = 650;

    //allegro variables
    ALLEGRO_DISPLAY *Display = NULL;
    ALLEGRO_EVENT_QUEUE *EventQueue = NULL;
    ALLEGRO_TIMER *Timer = NULL;
    ALLEGRO_BITMAP *RunnerJumpSheet = NULL;
    ALLEGRO_BITMAP *RunnerRunSheet = NULL;
    ALLEGRO_BITMAP *Title = NULL;
    ALLEGRO_BITMAP *Cacti = NULL;
    
    //initialize the Allegro
	al_init();

    //initializes the module that loads images
    al_init_image_addon();

    //install the keyboard
    al_install_keyboard();
    
    //create a screen
    Display = al_create_display(SCREEN_W, SCREEN_H);

    //renaming the window
    al_set_window_title(Display, "Run and Jump");

    //creates a timer that increments one unit every 1.0/FPS seconds
    Timer = al_create_timer(1.0/FPS);

    //create the event queue
    EventQueue = al_create_event_queue();

    //records screen events in the queue
    al_register_event_source(EventQueue, al_get_display_event_source(Display));
    //records time events in the queue
    al_register_event_source(EventQueue, al_get_timer_event_source(Timer));
    //records keyboard events in the queue
    al_register_event_source(EventQueue, al_get_keyboard_event_source());

    //start the timer
    al_start_timer(Timer);

    //main game loop
    int playing = 1;
    while(playing){
        
        //allegro variables
        ALLEGRO_EVENT Ev;

        //waits for an event and stores it in the ev variable
        al_wait_for_event(EventQueue, &Ev);

        //runner jump
        if(Ev.type == ALLEGRO_EVENT_KEY_DOWN  && Ev.keyboard.keycode == ALLEGRO_KEY_UP && pressed_jump == false){

            pressed_jump = true;

        }

        //if the event type is a timer event
        if(Ev.type == ALLEGRO_EVENT_TIMER){

            //parallax
            if(parallax_end > -200/PARALLAX_SPEED_X){

                setScene(parallax_position_x, 386, "C:\\Users\\Teste\\Desktop\\Run and Jump\\Sprites\\Grass_Sheet.png", 1);

                parallax_position_x -= PARALLAX_SPEED_X;
                parallax_end --;

            }else{

                parallax_position_x = 0;
                parallax_end = 0;

            }

            if(pressed_jump == true){

                start_jump -= JUMP_SPEED;

                if(start_jump <= 362 - JUMP_HEIGHT){

                    pressed_jump = false;
                    start_jump = 362 - JUMP_HEIGHT;
                    max_jump_point = true;

                }

            }

            if(max_jump_point == true){

                start_jump += FALL_SPEED;

                if(start_jump >= 362){

                    start_jump = 362;
                    max_jump_point = false;

                }
            
            }

            srand(time(NULL));
           
            //selecting the cactus randomly
            if(obstacle_generation % 450 == 0){

                cactus_file = fopen("cac_file.txt", "w");

                fprintf(cactus_file, "%s", "C:\\Users\\Teste\\Desktop\\Run and Jump\\Sprites\\Cactus\\Cactus_");

                cactus_number = rand() % limit_number_cacti;

                fprintf(cactus_file, "%d", cactus_number);
                fprintf(cactus_file, "%s", ".png");
                fclose(cactus_file);

                cactus_file = fopen("cac_file.txt", "r");

                fgets(cactus_name, 150, cactus_file);

                printf("%s\n", cactus_name);

                fclose(cactus_file);

            }

            //zeroing the generation of obstacles
            if(obstacle_generation == 450){

                obstacle_generation = 0;
                difficulty ++;
            }
            
            //cactus movement
            if(cactus_x > - 200/PARALLAX_SPEED_X){

                setScene(cactus_x, 354, cactus_name, 0);

                cactus_x -= PARALLAX_SPEED_X;

            }else{

                cactus_x = 650;

            }

            obstacle_generation ++; 
          
            //load the Runner Run Sheet image
            RunnerRunSheet = al_load_bitmap("C:\\Users\\Teste\\Desktop\\Run and Jump\\Sprites\\Runner_Run_Sheet.png");

            //frame rate run
            frame_pass_run += 0.3;
            if(frame_pass_run > 3){

                frame_pass_run  -= 3;

            }

            //animating the Runner
            al_draw_bitmap_region(RunnerRunSheet, 21 * (int)frame_pass_run , 0, 21, 24, 200, start_jump, 0);

            //destroy a variable image
            al_destroy_bitmap(RunnerRunSheet);

            //refresh the screen
            al_flip_display();

        }

        //if the event type is screen closing
        else if(Ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){

            playing = 0;

        }
    }

    

    //end-game procedures
    al_destroy_display(Display);
    al_destroy_event_queue(EventQueue);
    
    return 0;
}
