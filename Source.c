#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include <stdio.h>
#include <allegro5/allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#define WIDTH 1200
#define HEIGHT 850

int x = 0, y = 0;// location of mouse
int show[8][8] = { 0 }; //map that show which tiles have penguin
int c = 0;//counter
int index;//index of array
int allowable_tiles[21][2];//possible choices for moving penguin
int turn = 1, k = 1; // show turn of plyaers

/* define images & musics for game */
ALLEGRO_BITMAP* image1;
ALLEGRO_BITMAP* image2;
ALLEGRO_BITMAP* image3;
ALLEGRO_BITMAP* wallpaper;
ALLEGRO_BITMAP* blue;
ALLEGRO_BITMAP* red;
ALLEGRO_BITMAP* green;
ALLEGRO_BITMAP* yellow;
ALLEGRO_SAMPLE* sample1;
ALLEGRO_SAMPLE* sample2;

typedef struct { // moshakhasat bazikon
	int Number_Of_Fish;
	int Number_Of_Tile;
	int Number_Of_Deleted_Penguins;
	struct { // location of penguins
		int x;
		int y;
	} location[4];
} Player;

/* PROTOTYPES */
void makemap(int map[8][8]);
void display_map(int map[8][8], ALLEGRO_BITMAP* image1, ALLEGRO_BITMAP* image2, ALLEGRO_BITMAP* image3); // display & update map
void menu(ALLEGRO_FONT* font);// show starting menu
void two_player(int map[8][8], ALLEGRO_BITMAP* blue, ALLEGRO_BITMAP* red, ALLEGRO_MOUSE_STATE mouse_state, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font); //preparing condition for starting game with 2 players
void three_player(int map[8][8], ALLEGRO_BITMAP* blue, ALLEGRO_BITMAP* red, ALLEGRO_BITMAP* green, ALLEGRO_MOUSE_STATE mouse_state, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font); //preparing condition for starting game with 3 players
void four_player(int map[8][8], ALLEGRO_BITMAP* blue, ALLEGRO_BITMAP* red, ALLEGRO_BITMAP* green, ALLEGRO_BITMAP* yellow, ALLEGRO_MOUSE_STATE mouse_state, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font); //preparing condition for starting game with 4 players
void send(int map[8][8], int x, int y, Player* player); // sending x & y to find where user's penguin can go
void up_right(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i); // gozinehaye pisheroo baraye harekat dar rastaye shomale sharghi
void down_right(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i);// gozinehaye pisheroo baraye harekat dar rastaye jonoobe sharghi
void up_left(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i);// gozinehaye pisheroo baraye harekat dar rastaye shomale gharbi
void down_left(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i);// gozinehaye pisheroo baraye harekat dar rastaye jonoobe gharbi
int delete_penguin(int map[8][8], int i, int j);// if penguin can not move delete it
void win2(int map[8][8], Player player1, Player player2, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue);//show winner of game with 2 player
void win3(int map[8][8], Player player1, Player player2, Player player3, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue);//show winner of game with 3 player
void win4(int map[8][8], Player player1, Player player2, Player player3, Player player4, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue);//show winner of game with 4 player
void mouse_location(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT *event); // control location of mouse
void display_turn(ALLEGRO_FONT* font);// show turn on screen
void close_game(ALLEGRO_EVENT_QUEUE* event_queue);//close game by click on close display(on top right)


int main(void) {
	if (!al_init()) {
		return -1;
	}
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_acodec_addon();
	al_install_mouse();
	al_install_keyboard();
	al_install_audio();

	ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
	image1 = al_load_bitmap("1.png");
	image2 = al_load_bitmap("2.png");
	image3 = al_load_bitmap("3.png");
	blue = al_load_bitmap("blue.png");
	red = al_load_bitmap("red.png");
	green = al_load_bitmap("green.png");
	yellow = al_load_bitmap("yellow.png");
	wallpaper = al_load_bitmap("ice.jpg");
	sample1 = al_load_sample("sample1.mp3");
	sample2 = al_load_sample("sample2.mp3");
	al_reserve_samples(1);
	ALLEGRO_MOUSE_STATE mouse_state;
	ALLEGRO_KEYBOARD_STATE keyboard_state;
	ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", 30, 0);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

	if (!display) {
		return -1;
	}

	int map[8][8] = { 0 };
	makemap(map);

	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	int number_of_player = 0;
	ALLEGRO_EVENT event;
	while (1) {
		al_play_sample(sample2, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		menu(font);
		al_flip_display();
		al_wait_for_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return;
		al_get_mouse_state(&mouse_state);
		if (mouse_state.buttons == 1 && mouse_state.x > 550 && mouse_state.y > 200 && mouse_state.x < 650 && mouse_state.y < 240) {
			number_of_player = 2;
			break;
		}
		else if (mouse_state.buttons == 1 && mouse_state.x > 550 && mouse_state.y > 400 && mouse_state.x < 650 && mouse_state.y < 440) {
			number_of_player = 3;
			break;
		}
		else if (mouse_state.buttons == 1 && mouse_state.x > 550 && mouse_state.y > 600 && mouse_state.x < 650 && mouse_state.y < 640) {
			number_of_player = 4;
			break;
		}
	}
	al_destroy_sample(sample2);
	if (number_of_player == 2) 
		two_player(map, blue, red, mouse_state, event_queue, font);
	else if (number_of_player == 3) 
		three_player(map, blue, red, green, mouse_state, event_queue, font);
	else if (number_of_player == 4) 
		four_player(map, blue, red, green, yellow, mouse_state, event_queue, font);
	al_destroy_sample(sample1);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	return 0;
}

void menu(ALLEGRO_FONT* font) {
	al_clear_to_color(al_map_rgb(103, 227, 198));
	al_draw_bitmap(wallpaper, 0, 0, 0);
	al_draw_text(font, al_map_rgb(47, 79, 79), 430, 70, 0, "Please choose number of players");
	al_draw_text(font, al_map_rgb(139, 0, 139), 550, 200, 0, "2 players");
	al_draw_text(font, al_map_rgb(139, 0, 139), 550, 400, 0, "3 players");
	al_draw_text(font, al_map_rgb(139, 0, 139), 550, 600, 0, "4 players");
}

void makemap(int map[8][8]) {
	int n = 0;
	map[0][7] = map[2][7] = map[4][7] = map[6][7] = 4; // kashihaye ezafi

	srand(time(NULL));

	// 10 random spot for tiles with 3 fish
	while (n != 10) {
		int x = rand() % 8 + 0;
		int y = rand() % 8;

		if (map[x][y] == 0) {
			map[x][y] = 3;
			n++;
		}
	}

	// 20 random spot for tiles with 2 fish
	n = 0;
	while (n != 20) {
		int x = rand() % 8 + 0;
		int y = rand() % 8;

		if (map[x][y] == 0) {
			map[x][y] = 2;
			n++;
		}
	}

	// 30 random spot for tiles with 1 fish
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (map[i][j] == 0) {
				map[i][j] = 1;
			}
		}
	}

}

void display_map(int map[8][8], ALLEGRO_BITMAP* image1, ALLEGRO_BITMAP* image2, ALLEGRO_BITMAP* image3) {
	al_draw_bitmap(wallpaper, 0, 0, 0);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (j % 2 != 0) {
				if (map[j][i] == 1) {
					al_draw_bitmap(image1, (i * 100), (j * 100), 0);
				}
				else if (map[j][i] == 2) {
					al_draw_bitmap(image2, (i * 100), (j * 100), 0);
				}
				else if (map[j][i] == 3) {
					al_draw_bitmap(image3, (i * 100), (j * 100), 0);
				}
			}
			else {
				if (map[j][i] == 1) {
					al_draw_bitmap(image1, (i * 100) + 50, (j * 100), 0);
				}
				else if (map[j][i] == 2) {
					al_draw_bitmap(image2, (i * 100) + 50, (j * 100), 0);
				}
				else if (map[j][i] == 3) {
					al_draw_bitmap(image3, (i * 100) + 50, (j * 100), 0);
				}
			}
		}
	}
	// show penguins
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (j % 2 != 0) {
				if (show[j][i] == 1) {
					al_draw_bitmap(blue, (i * 100) + 10, (j * 100) + 10, 0);
				}
				else if (show[j][i] == 2) {
					al_draw_bitmap(red, (i * 100) + 20, (j * 100) + 10, 0);
				}
				else if (show[j][i] == 3) {
					al_draw_bitmap(green, (i * 100) + 20, (j * 100) + 10, 0);
				}
				else if (show[j][i] == 4) {
					al_draw_bitmap(yellow, (i * 100) + 20, (j * 100) + 10, 0);
				}
			}
			else {
				if (show[j][i] == 1) {
					al_draw_bitmap(blue, (i * 100) + 50 + 10, (j * 100) + 10, 0);
				}
				else if (show[j][i] == 2) {
					al_draw_bitmap(red, (i * 100) + 50 + 20, (j * 100) + 10, 0);
				}
				else if (show[j][i] == 3) {
					al_draw_bitmap(green, (i * 100) + 50 + 20, (j * 100) + 10, 0);
				}
				else if (show[j][i] == 4) {
					al_draw_bitmap(yellow, (i * 100) + 50 + 20, (j * 100) + 10, 0);
				}
			}
		}
	}
}

void two_player(int map[8][8], ALLEGRO_BITMAP* blue, ALLEGRO_BITMAP* red, ALLEGRO_MOUSE_STATE mouse_state, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font) {
	Player player1;
	Player player2;
	player1.Number_Of_Fish = player2.Number_Of_Fish = 4;
	player1.Number_Of_Tile = player2.Number_Of_Tile = 4;
	player1.Number_Of_Deleted_Penguins = player2.Number_Of_Deleted_Penguins = 0;
	ALLEGRO_EVENT event;

	int check = 0; // to start to call deleted_penguin() when 8 penguins are on the board (bad az chideman avalie)
	while (player1.Number_Of_Deleted_Penguins != 4 || player2.Number_Of_Deleted_Penguins != 4) {
		al_play_sample(sample1, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		if (al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(103, 227, 198));
			display_map(map, image1, image2, image3);
			if (c >= 8) {//show scores
				al_draw_text(font, al_map_rgb(139, 0, 0), 900, 100, 0, "statistics");
				al_draw_textf(font, al_map_rgb(0, 0, 255), 900, 200, 0, "player1 fish: %d", player1.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(0, 0, 255), 900, 250, 0, "player1 tiles: %d", player1.Number_Of_Tile);
				al_draw_textf(font, al_map_rgb(255, 0, 0), 900, 350, 0, "player2 fish: %d", player2.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(255, 0, 0), 900, 400, 0, "player2 tiles: %d", player2.Number_Of_Tile);
				display_turn(font);
			}
			al_flip_display();
		}
		al_wait_for_event(event_queue, &event);
		switch (event.type) {
		case	ALLEGRO_EVENT_DISPLAY_CLOSE: //exit from game
			return;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if (event.mouse.button == 2 && c < 8) { // kashtan avaliye
				mouse_location(event_queue, &event);
				if (k == 1 && player1.Number_Of_Deleted_Penguins != 4 && map[x][y] == 1 && show[x][y] == 0) {
					show[x][y] = 1;
					if (player2.Number_Of_Deleted_Penguins != 4)
						k = 2;
					player1.location[c / 2].x = x;
					player1.location[c / 2].y = y;
					c++;
				}
				else if (k == 2 && player2.Number_Of_Deleted_Penguins != 4 && map[x][y] == 1 && show[x][y] == 0) {
					show[x][y] = 2;
					if (player1.Number_Of_Deleted_Penguins != 4)
						k = 1;
					player2.location[(c - 1) / 2].x = x;
					player2.location[(c - 1) / 2].y = y;
					c++;
					if (c == 8)
						check = 1; // jaygozarie avaliye tamam shod
				}
			}
			else if (event.mouse.button == 1 && c >= 8 && c % 2 == 0) { //(c%2==0) -> if user grab a penguin she/he must put it back then grab another penguin again
				mouse_location(event_queue, &event);
				if (show[x][y] != 0 && delete_penguin(map, x, y) == 0) { // if there is a penguin in this location and it can move 
					if (turn == 1 && player1.Number_Of_Deleted_Penguins != 4) {
						if (show[x][y] == 1) { //penguin for player 1
							send(map, x, y, &player1); // compute which tiles this penguin can move to it
							if (player2.Number_Of_Deleted_Penguins != 4)
								turn = 2;
							for (int i = 0; i < 4; i++) {
								if (player1.location[i].x == x && player1.location[i].y == y)
									index = i;
							}
							show[x][y] = 5; // user can not use this tile again
							map[x][y] = 4; //delete tile from map
							c++;
						}
					}
					else if (turn == 2 && player2.Number_Of_Deleted_Penguins != 4) {
						if (show[x][y] == 2) { //pengin for player 2
							send(map, x, y, &player2); // compute which tiles this penguin can move to it
							if (player1.Number_Of_Deleted_Penguins != 4)
								turn = 1;
							for (int i = 0; i < 4; i++) { //save index of penguin that choose
								if (player2.location[i].x == x && player2.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
						}
					}
				}
			}
			else if (event.mouse.button == 2 && c % 2 != 0) { // replace (jaygozari)
				mouse_location(event_queue, &event);
				if (k == 1 && player1.Number_Of_Deleted_Penguins != 4) {
					for (int i = 0; i < 21; i++) {
						if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
							if (show[x][y] == 0) {
								show[x][y] = 1;
								if (player2.Number_Of_Deleted_Penguins != 4)
									k = 2;
								player1.location[index].x = x; //save new location
								player1.location[index].y = y;
								player1.Number_Of_Fish += map[x][y]; //update points
								player1.Number_Of_Tile++;
								c++;
							}
						}
					}
				}
				else if (k == 2 && player2.Number_Of_Deleted_Penguins != 4) {
					for (int i = 0; i < 21; i++) {
						if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
							if (show[x][y] == 0) {
								show[x][y] = 2;
								if (player1.Number_Of_Deleted_Penguins != 4)
									k = 1;
								player2.location[index].x = x;
								player2.location[index].y = y;
								player2.Number_Of_Fish += map[x][y];
								player2.Number_Of_Tile++;
								c++;
							}
						}
					}
				}
			}
			break;
		}
		if (check) { // agar jaygozarie avaliye tamam shode bood -> barrasi tedad panguan haye hazf shode
			player1.Number_Of_Deleted_Penguins = 0;
			player2.Number_Of_Deleted_Penguins = 0;
			for (int a = 0; a < 4; a++) {
				if (delete_penguin(map, player1.location[a].x, player1.location[a].y) == 1)  //if penguin can not move to another tile
					player1.Number_Of_Deleted_Penguins++;
				if (delete_penguin(map, player2.location[a].x, player2.location[a].y) == 1)
					player2.Number_Of_Deleted_Penguins++;
			}
			if (player1.Number_Of_Deleted_Penguins == 4) {
				turn = 2;
				k = 2;
			}
			if (player2.Number_Of_Deleted_Penguins == 4) {
				turn = 1;
				k = 1;
			}
		}
	}
	al_rest(1);
	win2(map, player1, player2, font,event_queue);
}

void send(int map[8][8], int x, int y, Player* player) {
	int i, j; // rikhtan location kashihaye mojaz baraye maghsad dar yek araye 2D

	for (i = 0; i < 21; i++) {
		for (j = 0; j < 2; j++)
			allowable_tiles[i][j] = -1;
	}

	i = 0;
	if (x % 2 != 0) { // odd row
		// Right
		for (j = y + 1; j != 8; j++) {
			if (map[x][j] != 4 && show[x][j] == 0) {
				allowable_tiles[i][0] = x;
				allowable_tiles[i][1] = j;
				i++;
			}
			else {
				break;
			}
		}

		// Left
		for (j = y - 1; j != -1; j--) {
			if (map[x][j] != 4 && show[x][j] == 0) {
				allowable_tiles[i][0] = x;
				allowable_tiles[i][1] = j;
				i++;
			}
			else {
				break;
			}
		}

		// Up Right
		if (x != 0) {
			//x - 1, y cause it won't print & it's the end of the chain
			allowable_tiles[i][0] = x - 1;
			allowable_tiles[i][1] = y;
			i++;
			up_right(map, x - 1, y, allowable_tiles, &i); // sending x - 1, y
		}

		// Down Right
		if (x != 7) {
			allowable_tiles[i][0] = x + 1;
			allowable_tiles[i][1] = y;
			i++;
			down_right(map, x + 1, y, allowable_tiles, &i);
		}

		// Up Left
		if (x != 0) {
			up_left(map, x, y, allowable_tiles, &i);
		}

		// Down Left
		if (x != 7) {
			down_left(map, x, y, allowable_tiles, &i);
		}
	}

	else { // even row
		// Right
		for (j = y + 1; j != 7; j++) {
			if (map[x][j] != 4 && show[x][j] == 0) {
				allowable_tiles[i][0] = x;
				allowable_tiles[i][1] = j;
				i++;
			}
			else {
				break;
			}
		}

		// Left
		for (j = y - 1; j != -1; j--) {
			if (map[x][j] != 4 && show[x][j] == 0) {
				allowable_tiles[i][0] = x;
				allowable_tiles[i][1] = j;
				i++;
			}
			else {
				break;
			}
		}

		// Up Right
		if (x != 0) {
			up_right(map, x, y, allowable_tiles, &i); // sending x, y themselves
		}

		// Down Right
		if (x != 7) {
			down_right(map, x, y, allowable_tiles, &i);
		}

		// Up Left
		if (x != 0) {
			allowable_tiles[i][0] = x - 1;
			allowable_tiles[i][1] = y;
			i++;
			up_left(map, x - 1, y, allowable_tiles, &i);
		}

		// Down Left
		if (x != 7) {
			allowable_tiles[i][0] = x + 1;
			allowable_tiles[i][1] = y;
			i++;
			down_left(map, x + 1, y, allowable_tiles, &i);
		}
	}
}


void up_right(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i) { // odd & even are different in y ----> ignore 4 (recursive functions)
	if (x == 0 || y == 7) {
		return;
	}

	if (map[x - 1][y + 1] != 4 && show[x - 1][y + 1] == 0) {
		allowable_tiles[*i][0] = x - 1;
		allowable_tiles[*i][1] = y + 1;
		(*i)++;
	}

	else
		return;
	if (map[x - 2][y + 1] != 4 && show[x - 2][y + 1] == 0) {
		allowable_tiles[*i][0] = x - 2;
		allowable_tiles[*i][1] = y + 1;
		(*i)++;
	}
	else
		return;

	up_right(map, x - 2, y + 1, allowable_tiles, i);
}

void down_right(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i) {
	if (x == 8 || y == 7) {
		return;
	}

	if (map[x + 1][y + 1] != 4 && show[x + 1][y + 1] == 0) {
		allowable_tiles[(*i)][0] = x + 1;
		allowable_tiles[*i][1] = y + 1;
		(*i)++;
	}
	else
		return;

	if (x + 2 == 8 || y == 7) {
		return;
	}

	if (map[x + 2][y + 1] != 4 && show[x + 2][y + 1] == 0) {
		allowable_tiles[*i][0] = x + 2;
		allowable_tiles[*i][1] = y + 1;
		(*i)++;
	}
	else
		return;

	down_right(map, x + 2, y + 1, allowable_tiles, i);
}

void up_left(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i) {
	if (x == 0 || y == 0) {
		return;
	}

	if (map[x - 1][y - 1] != 4 && show[x - 1][y - 1] == 0) {
		allowable_tiles[*i][0] = x - 1;
		allowable_tiles[*i][1] = y - 1;
		(*i)++;
	}
	else
		return;

	if (x - 2 == -1 || y == 0)
		return;

	if (map[x - 2][y - 1] != 4 && show[x - 2][y - 1] == 0) {
		allowable_tiles[*i][0] = x - 2;
		allowable_tiles[*i][1] = y - 1;
		(*i)++;
	}
	else
		return;

	up_left(map, x - 2, y - 1, allowable_tiles, i);
}

void down_left(int map[8][8], int x, int y, int allowable_tiles[21][2], int* i) {
	if (x == 7 || y == 0) {
		return;
	}

	if (map[x + 1][y - 1] != 4 && show[x + 1][y - 1] == 0) {
		allowable_tiles[*i][0] = x + 1;
		allowable_tiles[*i][1] = y - 1;
		(*i)++;
	}
	else
		return;

	if (map[x + 2][y - 1] != 4 && show[x + 2][y - 1] == 0) {
		allowable_tiles[*i][0] = x + 2;
		allowable_tiles[*i][1] = y - 1;
		(*i)++;
	}
	else
		return;

	down_left(map, x + 2, y - 1, allowable_tiles, i);
}

int delete_penguin(int map[8][8], int i, int j) {
	if (i % 2 != 0) {
		if ((map[i][j + 1] == 4 || show[i][j + 1] != 0 || i > 7 || i < 0 || j + 1 > 7 ) && (map[i][j - 1] == 4 || show[i][j - 1] != 0 || i > 7 || i < 0  || j - 1 < 0) && (map[i - 1][j] == 4 || show[i - 1][j] != 0 || i - 1 > 7 || i - 1 < 0 || j > 7 || j < 0) && (map[i + 1][j] == 4 || show[i + 1][j] != 0 || i + 1 > 7 || i + 1 < 0 || j > 7 || j < 0) && (map[i - 1][j - 1] == 4 || show[i - 1][j - 1] != 0 || i - 1 > 7 || i - 1 < 0 || j - 1 > 7 || j - 1 < 0) && (map[i + 1][j - 1] == 4 || show[i + 1][j - 1] != 0 || i + 1 > 7 || i + 1 < 0  || j - 1 < 0)) {
			return 1;
		}
	}
	else {
		if ((map[i][j + 1] == 4 || show[i][j + 1] != 0 || i < 0 || j + 1>6 || j + 1 < 0) && (map[i][j - 1] == 4 || show[i][j - 1] != 0 || i < 0 || j - 1>6 || j - 1 < 0) && (map[i - 1][j + 1] == 4 || show[i - 1][j + 1] != 0 || i - 1 < 0 || j + 1 > 7 || j + 1 < 0) && (map[i + 1][j + 1] == 4 || show[i + 1][j + 1] != 0 || i + 1 > 7 || j + 1 > 7 ) && (map[i - 1][j] == 4 || show[i - 1][j] != 0 || i - 1 < 0 || j > 7 || j < 0) && (map[i + 1][j] == 4 || show[i + 1][j] != 0 || i + 1 < 0 || j > 7 || j < 0)) {
			return 1;
		}
	}
	return 0;
}

void win2(int map[8][8], Player player1, Player player2, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue) {
	al_clear_to_color(al_map_rgb(103, 227, 198));
	al_draw_bitmap(wallpaper, 0, 0, 0);
	al_draw_text(font, al_map_rgb(139, 0, 0), 500, 200, 0, "THE END");
	al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 300, 0, "player1 fish: %d", player1.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 350, 0, "player1 tiles: %d", player1.Number_Of_Tile);
	al_draw_textf(font, al_map_rgb(255, 0, 0), 500, 450, 0, "player2 fish: %d", player2.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(255, 0, 0), 500, 500, 0, "player2 tiles: %d", player2.Number_Of_Tile);
	//show result
	if (player1.Number_Of_Fish > player2.Number_Of_Fish)
		al_draw_text(font, al_map_rgb(128, 0, 128), 500, 600, 0, "player1 won!!!");
	else if (player1.Number_Of_Fish < player2.Number_Of_Fish)
		al_draw_text(font, al_map_rgb(128, 0, 128), 500, 600, 0, "player2 won!!!");
	else {
		if (player1.Number_Of_Tile > player2.Number_Of_Tile)
			al_draw_text(font, al_map_rgb(128, 0, 128), 600, 700, 0, "player1 won!!!");
		else if (player1.Number_Of_Tile < player2.Number_Of_Tile)
			al_draw_text(font, al_map_rgb(128, 0, 128), 600, 700, 0, "player2 won!!!");
		else
			al_draw_text(font, al_map_rgb(128, 0, 128), 600, 700, 0, "both of the players won!!!");
	}
	al_flip_display();
	close_game(event_queue);
}

void three_player(int map[8][8], ALLEGRO_BITMAP* blue, ALLEGRO_BITMAP* red, ALLEGRO_BITMAP* green, ALLEGRO_MOUSE_STATE mouse_state, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font) {
	Player player1;
	Player player2;
	Player player3;
	player1.Number_Of_Fish = player2.Number_Of_Fish = player3.Number_Of_Fish = 3;
	player1.Number_Of_Tile = player2.Number_Of_Tile = player3.Number_Of_Tile = 3;
	player1.Number_Of_Deleted_Penguins = player2.Number_Of_Deleted_Penguins = player3.Number_Of_Deleted_Penguins = 0;
	ALLEGRO_EVENT event;
	int check = 0; // to start to call deleted_penguin() when 9 penguins are on the board (bad az chideman avalie)
	while (player1.Number_Of_Deleted_Penguins != 3 || player2.Number_Of_Deleted_Penguins != 3 || player3.Number_Of_Deleted_Penguins != 3) {
		al_play_sample(sample1, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		if (al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(103, 227, 198));
			display_map(map, image1, image2, image3);
			if (c >= 9) {
				al_draw_text(font, al_map_rgb(139, 0, 0), 900, 100, 0, "statistics");
				al_draw_textf(font, al_map_rgb(0, 0, 255), 900, 200, 0, "player1 fish: %d", player1.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(0, 0, 255), 900, 250, 0, "player1 tiles: %d", player1.Number_Of_Tile);
				al_draw_textf(font, al_map_rgb(255, 0, 0), 900, 350, 0, "player2 fish: %d", player2.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(255, 0, 0), 900, 400, 0, "player2 tiles: %d", player2.Number_Of_Tile);
				al_draw_textf(font, al_map_rgb(0, 128, 0), 900, 500, 0, "player3 fish: %d", player3.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(0, 128, 0), 900, 550, 0, "player3 tiles: %d", player3.Number_Of_Tile);
				display_turn(font);
			}
			al_flip_display();
		}
		al_wait_for_event(event_queue, &event);

		switch (event.type) {
		case	ALLEGRO_EVENT_DISPLAY_CLOSE:
			return;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if (event.mouse.button == 2 && c < 9) { // kashtan avaliye
				mouse_location(event_queue, &event);
				if (k == 1 && player1.Number_Of_Deleted_Penguins != 3) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 1;
						if (player2.Number_Of_Deleted_Penguins != 3)
							k = 2;
						else if (player3.Number_Of_Deleted_Penguins != 3)
							k = 3;
						player1.location[c / 3].x = x;
						player1.location[c / 3].y = y;
						c++;
					}
				}
				else if (k == 2 && player2.Number_Of_Deleted_Penguins != 3) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 2;
						if (player3.Number_Of_Deleted_Penguins != 3)
							k = 3;
						else if (player1.Number_Of_Deleted_Penguins != 3)
							k = 1;
						player2.location[(c - 1) / 3].x = x;
						player2.location[(c - 1) / 3].y = y;
						c++;
					}
				}
				else if (k == 3 && player3.Number_Of_Deleted_Penguins != 3) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 3;
						if (player1.Number_Of_Deleted_Penguins != 3)
							k = 1;
						else if (player2.Number_Of_Deleted_Penguins != 3)
							k = 2;
						player3.location[(c - 2) / 3].x = x;
						player3.location[(c - 2) / 3].y = y;
						c++;
						if (c == 9)
							check = 1; // jaygozarie avaliye tamam shod
					}
				}
			}
			else if (event.mouse.button == 1 && c >= 9 && c % 2 != 0) { //(c%2==0) -> if user grab a penguin she/he must put it back then grab another penguin again
				mouse_location(event_queue, &event);
				if (show[x][y] != 0 && delete_penguin(map, x, y) == 0) { // if there is a penguin in this location and it can move
					if (turn == 1) {
						if (player1.Number_Of_Deleted_Penguins == 3) {
							if (player2.Number_Of_Deleted_Penguins != 3)
								turn = 2;
							else if (player3.Number_Of_Deleted_Penguins != 3)
								turn = 3;
						}
						else if (show[x][y] == 1) {
							send(map, x, y, &player1);
							for (int i = 0; i < 3; i++) {
								if (player1.location[i].x == x && player1.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player2.Number_Of_Deleted_Penguins != 3)
								turn = 2;
							else if (player3.Number_Of_Deleted_Penguins != 3)
								turn = 3;
						}
					}
					else if (turn == 2) {
						if (player2.Number_Of_Deleted_Penguins == 3) {
							if (player3.Number_Of_Deleted_Penguins != 3)
								turn = 3;
							else if (player1.Number_Of_Deleted_Penguins != 3)
								turn = 1;
						}
						else if (show[x][y] == 2) {
							send(map, x, y, &player2);
							for (int i = 0; i < 3; i++) {
								if (player2.location[i].x == x && player2.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player3.Number_Of_Deleted_Penguins != 3)
								turn = 3;
							else if (player1.Number_Of_Deleted_Penguins != 3)
								turn = 1;
						}
					}
					else if (turn == 3) {
						if (player3.Number_Of_Deleted_Penguins == 3) {
							if (player1.Number_Of_Deleted_Penguins != 3)
								turn = 1;
							else if (player2.Number_Of_Deleted_Penguins != 3)
								turn = 2;
						}
						else if (show[x][y] == 3) {
							send(map, x, y, &player3);
							for (int i = 0; i < 3; i++) {
								if (player3.location[i].x == x && player3.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player1.Number_Of_Deleted_Penguins != 3)
								turn = 1;
							else if (player2.Number_Of_Deleted_Penguins != 3)
								turn = 2;
						}
					}
				}
			}
			else if (event.mouse.button == 2 && c % 2 == 0) { // replace(jaygozari)
				mouse_location(event_queue, &event);
				if (k == 1) {
					if (player1.Number_Of_Deleted_Penguins == 3) {
						if (player2.Number_Of_Deleted_Penguins != 3)
							k = 2;
						else if (player3.Number_Of_Deleted_Penguins != 3)
							k = 3;
					}
					else {
						for (int i = 0; i < 21; i++) {
							if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
								if (show[x][y] == 0) {
									show[x][y] = 1;
									player1.location[index].x = x;
									player1.location[index].y = y;
									player1.Number_Of_Fish += map[x][y];
									player1.Number_Of_Tile++;
									c++;
									if (player2.Number_Of_Deleted_Penguins != 3)
										k = 2;
									else if (player3.Number_Of_Deleted_Penguins != 3)
										k = 3;
								}
							}
						}
					}
				}
				else if (k == 2) {
					if (player2.Number_Of_Deleted_Penguins == 3) {
						if (player3.Number_Of_Deleted_Penguins != 3)
							k = 3;
						else if (player1.Number_Of_Deleted_Penguins != 3)
							k = 1;
					}
					else {
						for (int i = 0; i < 21; i++) {
							if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
								if (show[x][y] == 0) {
									show[x][y] = 2;
									player2.location[index].x = x;
									player2.location[index].y = y;
									player2.Number_Of_Fish += map[x][y];
									player2.Number_Of_Tile++;
									c++;
									if (player3.Number_Of_Deleted_Penguins != 3)
										k = 3;
									else if (player1.Number_Of_Deleted_Penguins != 3)
										k = 1;
								}
							}
						}
					}
				}
				else if (k == 3) {
					if(player3.Number_Of_Deleted_Penguins == 3){
						if (player1.Number_Of_Deleted_Penguins != 3)
							k = 1;
						else if (player2.Number_Of_Deleted_Penguins != 3)
							k = 2;
					}
					for (int i = 0; i < 21; i++) {
						if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
							if (show[x][y] == 0) {
								show[x][y] = 3;
								player3.location[index].x = x;
								player3.location[index].y = y;
								player3.Number_Of_Fish += map[x][y];
								player3.Number_Of_Tile++;
								c++;
								if (player1.Number_Of_Deleted_Penguins != 3)
									k = 1;
								else if (player2.Number_Of_Deleted_Penguins != 3)
									k = 2;
							}
						}
					}
				}
			}
			break;
		}
		if (check) {
			player1.Number_Of_Deleted_Penguins = 0;
			player2.Number_Of_Deleted_Penguins = 0;
			player3.Number_Of_Deleted_Penguins = 0;
			for (int a = 0; a < 3; a++) {
				if (delete_penguin(map, player1.location[a].x, player1.location[a].y) == 1) 
					player1.Number_Of_Deleted_Penguins++;
				if (delete_penguin(map, player2.location[a].x, player2.location[a].y) == 1) 
					player2.Number_Of_Deleted_Penguins++;
				if (delete_penguin(map, player3.location[a].x, player3.location[a].y) == 1) 
					player3.Number_Of_Deleted_Penguins++;
			}
		}
	}
	al_rest(1);
	win3(map, player1, player2, player3, font,event_queue);
}

void win3(int map[8][8], Player player1, Player player2, Player player3, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue) {
	al_clear_to_color(al_map_rgb(103, 227, 198));
	al_draw_bitmap(wallpaper, 0, 0, 0);
	al_draw_text(font, al_map_rgb(139, 0, 0), 500, 100, 0, "THE END");
	al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 300, 0, "player1 fish: %d", player1.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 350, 0, "player1 tiles: %d", player1.Number_Of_Tile);
	al_draw_textf(font, al_map_rgb(255, 0, 0), 500, 450, 0, "player2 fish: %d", player2.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(255, 0, 0), 500, 500, 0, "player2 tiles: %d", player2.Number_Of_Tile);
	al_draw_textf(font, al_map_rgb(0, 128, 0), 500, 600, 0, "player3 fish: %d", player3.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(0, 128, 0), 500, 650, 0, "player3 tiles: %d", player3.Number_Of_Tile);

	// find max fish
	int max_fish = player1.Number_Of_Fish;
	if (player2.Number_Of_Fish > max_fish) {
		max_fish = player2.Number_Of_Fish;
	}
	if (player3.Number_Of_Fish > max_fish) {
		max_fish = player3.Number_Of_Fish;
	}

	// find max tile for players who got max fish
	int max_tile = 0;
	if (player1.Number_Of_Fish == max_fish) {
		if (player1.Number_Of_Tile > max_tile) {
			max_tile = player1.Number_Of_Tile;
		}
	}
	if (player2.Number_Of_Fish == max_fish) {
		if (player2.Number_Of_Tile > max_tile) {
			max_tile = player2.Number_Of_Tile;
		}
	}
	if (player3.Number_Of_Fish == max_fish) {
		if (player3.Number_Of_Tile > max_tile) {
			max_tile = player3.Number_Of_Tile;
		}
	}

	int cnt[3] = {0};//moshakhas shodan barande
	if (player1.Number_Of_Fish == max_fish && player1.Number_Of_Tile == max_tile)
		cnt[0] = 1;
	if (player2.Number_Of_Fish == max_fish && player2.Number_Of_Tile == max_tile)
		cnt[1] = 1;
	if (player3.Number_Of_Fish == max_fish && player3.Number_Of_Tile == max_tile)
		cnt[2] = 1;

	if (cnt[0] == 1 && cnt[1] == 0 && cnt[2] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "player1 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 1 && cnt[2] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "player2 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 0 && cnt[2] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "player3 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 1 && cnt[2] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "player1 & player2 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 0 && cnt[2] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "player1 & player3 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 1 && cnt[2] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "player2 & player3 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 1 && cnt[2] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 200, 0, "All players won!!!");

	al_flip_display();
	close_game(event_queue);
}




void four_player(int map[8][8], ALLEGRO_BITMAP* blue, ALLEGRO_BITMAP* red, ALLEGRO_BITMAP* green, ALLEGRO_BITMAP* yellow, ALLEGRO_MOUSE_STATE mouse_state, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font) {
	Player player1;
	Player player2;
	Player player3;
	Player player4;
	player1.Number_Of_Fish = player2.Number_Of_Fish = player3.Number_Of_Fish = player4.Number_Of_Fish = 2;
	player1.Number_Of_Tile = player2.Number_Of_Tile = player3.Number_Of_Tile = player4.Number_Of_Tile = 2;
	player1.Number_Of_Deleted_Penguins = player2.Number_Of_Deleted_Penguins = player3.Number_Of_Deleted_Penguins = player4.Number_Of_Deleted_Penguins = 0;
	ALLEGRO_EVENT event;
	int check = 0; // to start to call delted_penguin() when 9 penguins are on the board (bad az chideman avalie)
	while (player1.Number_Of_Deleted_Penguins != 2 || player2.Number_Of_Deleted_Penguins != 2 || player3.Number_Of_Deleted_Penguins != 2 || player4.Number_Of_Deleted_Penguins != 2) {
		al_play_sample(sample1, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
		if (al_is_event_queue_empty(event_queue)) {
			al_clear_to_color(al_map_rgb(103, 227, 198));
			display_map(map, image1, image2, image3);
			if (c >= 8) {
				al_draw_text(font, al_map_rgb(139, 0, 0), 900, 100, 0, "statistics");
				al_draw_textf(font, al_map_rgb(0, 0, 255), 900, 200, 0, "player1 fish: %d", player1.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(0, 0, 255), 900, 250, 0, "player1 tiles: %d", player1.Number_Of_Tile);
				al_draw_textf(font, al_map_rgb(255, 0, 0), 900, 350, 0, "player2 fish: %d", player2.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(255, 0, 0), 900, 400, 0, "player2 tiles: %d", player2.Number_Of_Tile);
				al_draw_textf(font, al_map_rgb(0, 128, 0), 900, 500, 0, "player3 fish: %d", player3.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(0, 128, 0), 900, 550, 0, "player3 tiles: %d", player3.Number_Of_Tile);
				al_draw_textf(font, al_map_rgb(255, 215, 0), 900, 650, 0, "player4 fish: %d", player4.Number_Of_Fish);
				al_draw_textf(font, al_map_rgb(255, 215, 0), 900, 700, 0, "player4 tiles: %d", player4.Number_Of_Tile);
				display_turn(font);
			}
			al_flip_display();
		}
		al_wait_for_event(event_queue, &event);
		switch (event.type) {
		case	ALLEGRO_EVENT_DISPLAY_CLOSE:
			return;
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			if (event.mouse.button == 2 && c < 8) { // kashtan avaliye
				mouse_location(event_queue, &event);
				if (k == 1 && player1.Number_Of_Deleted_Penguins != 2) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 1;
						if (player2.Number_Of_Deleted_Penguins != 2)
							k = 2;
						else if (player3.Number_Of_Deleted_Penguins != 2)
							k = 3;
						else if (player4.Number_Of_Deleted_Penguins != 2)
							k = 4;
						player1.location[c / 4].x = x;
						player1.location[c / 4].y = y;
						c++;
					}
				}
				else if (k == 2 && player2.Number_Of_Deleted_Penguins != 2) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 2;
						if (player3.Number_Of_Deleted_Penguins != 2)
							k = 3;
						else if (player4.Number_Of_Deleted_Penguins != 2)
							k = 4;
						else if (player1.Number_Of_Deleted_Penguins != 2)
							k = 1;
						player2.location[(c - 1) / 4].x = x;
						player2.location[(c - 1) / 4].y = y;
						c++;
					}
				}
				else if (k == 3 && player3.Number_Of_Deleted_Penguins != 2) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 3;
						if (player4.Number_Of_Deleted_Penguins != 2)
							k = 4;
						else if (player1.Number_Of_Deleted_Penguins != 2)
							k = 1;
						else if (player2.Number_Of_Deleted_Penguins != 2)
							k = 2;
						player3.location[(c - 2) / 4].x = x;
						player3.location[(c - 2) / 4].y = y;
						c++;
					}
				}
				else if (k == 4 && player4.Number_Of_Deleted_Penguins != 2) {
					if (map[x][y] == 1 && show[x][y] == 0) {
						show[x][y] = 4;
						if (player1.Number_Of_Deleted_Penguins != 3)
							k = 1;
						else if (player2.Number_Of_Deleted_Penguins != 2)
							k = 2;
						else if (player3.Number_Of_Deleted_Penguins != 2)
							k = 3;
						player4.location[(c - 3) / 4].x = x;
						player4.location[(c - 3) / 4].y = y;
						c++;
						if (c == 8) {
							check = 1; // jaygozarie avaliye tamam shod
						}
					}
				}
			}
			else if (event.mouse.button == 1 && c >= 8 && c % 2 == 0) { //(c%2==0) -> if user grab a penguin she/he must put it back then grab another penguin again
				mouse_location(event_queue, &event);
				if (show[x][y] != 0 && delete_penguin(map, x, y) == 0) {
					if (turn == 1) {
						if (player1.Number_Of_Deleted_Penguins == 2) {
							if (player2.Number_Of_Deleted_Penguins != 2)
								turn = 2;
							else if (player3.Number_Of_Deleted_Penguins != 2)
								turn = 3;
							else if (player4.Number_Of_Deleted_Penguins != 2)
								turn = 4;
						}
						else if (show[x][y] == 1) {
							send(map, x, y, &player1);
							for (int i = 0; i < 2; i++) {
								if (player1.location[i].x == x && player1.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player2.Number_Of_Deleted_Penguins != 2)
								turn = 2;
							else if (player3.Number_Of_Deleted_Penguins != 2)
								turn = 3;
							else if (player4.Number_Of_Deleted_Penguins != 2)
								turn = 4;
						}
					}
					else if (turn == 2) {
						if (player2.Number_Of_Deleted_Penguins == 2) {
							if (player3.Number_Of_Deleted_Penguins != 2)
								turn = 3;
							else if (player4.Number_Of_Deleted_Penguins != 2)
								turn = 4;
							else if (player1.Number_Of_Deleted_Penguins != 2)
								turn = 1;
						}
						else if (show[x][y] == 2) {
							send(map, x, y, &player2);
							for (int i = 0; i < 2; i++) {
								if (player2.location[i].x == x && player2.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player3.Number_Of_Deleted_Penguins != 2)
								turn = 3;
							else if (player4.Number_Of_Deleted_Penguins != 2)
								turn = 4;
							else if (player1.Number_Of_Deleted_Penguins != 2)
								turn = 1;
						}
					}
					else if (turn == 3) {
						if (player3.Number_Of_Deleted_Penguins == 2) {
							if (player4.Number_Of_Deleted_Penguins != 2)
								turn = 4;
							else if (player1.Number_Of_Deleted_Penguins != 2)
								turn = 1;
							else if (player2.Number_Of_Deleted_Penguins != 2)
								turn = 2;
						}
						else if (show[x][y] == 3) {
							send(map, x, y, &player3);
							for (int i = 0; i < 2; i++) {
								if (player3.location[i].x == x && player3.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player4.Number_Of_Deleted_Penguins != 2)
								turn = 4;
							else if (player1.Number_Of_Deleted_Penguins != 2)
								turn = 1;
							else if (player2.Number_Of_Deleted_Penguins != 2)
								turn = 2;
						}
					}
					else if (turn == 4) {
						if (player4.Number_Of_Deleted_Penguins == 2) {
							if (player1.Number_Of_Deleted_Penguins != 2)
								turn = 1;
							else if (player2.Number_Of_Deleted_Penguins != 2)
								turn = 2;
							else if (player3.Number_Of_Deleted_Penguins != 2)
								turn = 3;
						}
						else if (show[x][y] == 4) {
							send(map, x, y, &player4);
							for (int i = 0; i < 2; i++) {
								if (player4.location[i].x == x && player4.location[i].y == y)
									index = i;
							}
							show[x][y] = 5;
							map[x][y] = 4;
							c++;
							if (player1.Number_Of_Deleted_Penguins != 2)
								turn = 1;
							else if (player2.Number_Of_Deleted_Penguins != 2)
								turn = 2;
							else if (player3.Number_Of_Deleted_Penguins != 2)
								turn = 3;
						}
					}
				}
			}
			else if (event.mouse.button == 2 && c % 2 != 0) { // replace
				mouse_location(event_queue, &event);
				if (k == 1) {
					if (player1.Number_Of_Deleted_Penguins == 2) {
						if (player2.Number_Of_Deleted_Penguins != 2)
							k = 2;
						else if (player3.Number_Of_Deleted_Penguins != 2)
							k = 3;
						else if (player4.Number_Of_Deleted_Penguins != 2)
							k = 4;
					}
					else {
						for (int i = 0; i < 21; i++) {
							if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
								if (show[x][y] == 0) {
									show[x][y] = 1;
									if (player2.Number_Of_Deleted_Penguins != 2)
										k = 2;
									else if (player3.Number_Of_Deleted_Penguins != 2)
										k = 3;
									else if (player4.Number_Of_Deleted_Penguins != 2)
										k = 4;
									player1.location[index].x = x;
									player1.location[index].y = y;
									player1.Number_Of_Fish += map[x][y];
									player1.Number_Of_Tile++;
									c++;
								}
							}
						}
					}
				}
				else if (k == 2) {
					if (player2.Number_Of_Deleted_Penguins == 2) {
						if (player3.Number_Of_Deleted_Penguins != 2)
							k = 3;
						else if (player4.Number_Of_Deleted_Penguins != 2)
							k = 4;
						else if (player1.Number_Of_Deleted_Penguins != 2)
							k = 1;
					}
					else {
						for (int i = 0; i < 21; i++) {
							if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
								if (show[x][y] == 0) {
									show[x][y] = 2;
									if (player3.Number_Of_Deleted_Penguins != 2)
										k = 3;
									else if (player4.Number_Of_Deleted_Penguins != 2)
										k = 4;
									else if (player1.Number_Of_Deleted_Penguins != 2)
										k = 1;
									player2.location[index].x = x;
									player2.location[index].y = y;
									player2.Number_Of_Fish += map[x][y];
									player2.Number_Of_Tile++;
									c++;
								}
							}
						}
					}
				}
				else if (k == 3) {
					if (player3.Number_Of_Deleted_Penguins == 2) {
						if (player4.Number_Of_Deleted_Penguins != 2)
							k = 4;
						else if (player1.Number_Of_Deleted_Penguins != 2)
							k = 1;
						else if (player2.Number_Of_Deleted_Penguins != 2)
							k = 2;
					}
					else {
						for (int i = 0; i < 21; i++) {
							if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
								if (show[x][y] == 0) {
									show[x][y] = 3;
									if (player4.Number_Of_Deleted_Penguins != 2)
										k = 4;
									else if (player1.Number_Of_Deleted_Penguins != 2)
										k = 1;
									else if (player2.Number_Of_Deleted_Penguins != 2)
										k = 2;
									player3.location[index].x = x;
									player3.location[index].y = y;
									player3.Number_Of_Fish += map[x][y];
									player3.Number_Of_Tile++;
									c++;
								}
							}
						}
					}
				}
				else if (k == 4) {
					if (player4.Number_Of_Deleted_Penguins == 2) {
						if (player1.Number_Of_Deleted_Penguins != 2)
							k = 1;
						else if (player2.Number_Of_Deleted_Penguins != 2)
							k = 2;
						else if (player3.Number_Of_Deleted_Penguins != 2)
							k = 3;
					}
					else {
						for (int i = 0; i < 21; i++) {
							if (allowable_tiles[i][0] == x && allowable_tiles[i][1] == y) {
								if (show[x][y] == 0) {
									show[x][y] = 4;
									if (player1.Number_Of_Deleted_Penguins != 2)
										k = 1;
									else if (player2.Number_Of_Deleted_Penguins != 2)
										k = 2;
									else if (player3.Number_Of_Deleted_Penguins != 2)
										k = 3;
									player4.location[index].x = x;
									player4.location[index].y = y;
									player4.Number_Of_Fish += map[x][y];
									player4.Number_Of_Tile++;
									c++;
								}
							}
						}
					}
				}
			}
			break;
		}
		if (check) {
			player1.Number_Of_Deleted_Penguins = 0;
			player2.Number_Of_Deleted_Penguins = 0;
			player3.Number_Of_Deleted_Penguins = 0;
			player4.Number_Of_Deleted_Penguins = 0;
			for (int a = 0; a < 2; a++) {
				if (delete_penguin(map, player1.location[a].x, player1.location[a].y) == 1)
					player1.Number_Of_Deleted_Penguins++;
				if (delete_penguin(map, player2.location[a].x, player2.location[a].y) == 1)
					player2.Number_Of_Deleted_Penguins++;
				if (delete_penguin(map, player3.location[a].x, player3.location[a].y) == 1)
					player3.Number_Of_Deleted_Penguins++;
				if (delete_penguin(map, player4.location[a].x, player4.location[a].y) == 1)
					player4.Number_Of_Deleted_Penguins++;
			}
		}
	}
	al_rest(1);
	win4(map, player1, player2, player3, player4, font,event_queue);
}

void win4(int map[8][8], Player player1, Player player2, Player player3, Player player4, ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* event_queue) {
	al_clear_to_color(al_map_rgb(103, 227, 198));
	al_draw_bitmap(wallpaper, 0, 0, 0);
	al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 200, 0, "player1 fish: %d", player1.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(0, 0, 255), 500, 250, 0, "player1 tiles: %d", player1.Number_Of_Tile);
	al_draw_textf(font, al_map_rgb(255, 0, 0), 500, 350, 0, "player2 fish: %d", player2.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(255, 0, 0), 500, 400, 0, "player2 tiles: %d", player2.Number_Of_Tile);
	al_draw_textf(font, al_map_rgb(0, 128, 0), 500, 500, 0, "player3 fish: %d", player3.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(0, 128, 0), 500, 550, 0, "player3 tiles: %d", player3.Number_Of_Tile);
	al_draw_textf(font, al_map_rgb(255, 255, 0), 500, 650, 0, "player4 fish: %d", player4.Number_Of_Fish);
	al_draw_textf(font, al_map_rgb(255, 255, 0), 500, 700, 0, "player4 tiles: %d", player4.Number_Of_Tile);
	// find max fish
	int max_fish = player1.Number_Of_Fish;
	if (player2.Number_Of_Fish > max_fish) {
		max_fish = player2.Number_Of_Fish;
	}
	if (player3.Number_Of_Fish > max_fish) {
		max_fish = player3.Number_Of_Fish;
	}
	if (player4.Number_Of_Fish > max_fish) {
		max_fish = player4.Number_Of_Fish;
	}

	// find max tile for players who got max fish
	int max_tile = 0;
	if (player1.Number_Of_Fish == max_fish) {
		if (player1.Number_Of_Tile > max_tile) {
			max_tile = player1.Number_Of_Tile;
		}
	}
	if (player2.Number_Of_Fish == max_fish) {
		if (player2.Number_Of_Tile > max_tile) {
			max_tile = player2.Number_Of_Tile;
		}
	}
	if (player3.Number_Of_Fish == max_fish) {
		if (player3.Number_Of_Tile > max_tile) {
			max_tile = player3.Number_Of_Tile;
		}
	}
	if (player4.Number_Of_Fish == max_fish) {
		if (player4.Number_Of_Tile > max_tile) {
			max_tile = player4.Number_Of_Tile;
		}
	}

	int cnt[4] = {0}; //moshakhas shodan barande 
	if (player1.Number_Of_Fish == max_fish && player1.Number_Of_Tile == max_tile)
		cnt[0] = 1;
	if (player2.Number_Of_Fish == max_fish && player2.Number_Of_Tile == max_tile)
		cnt[1] = 1;
	if (player3.Number_Of_Fish == max_fish && player3.Number_Of_Tile == max_tile)
		cnt[2] = 1;
	if (player4.Number_Of_Fish == max_fish && player4.Number_Of_Tile == max_tile)
		cnt[3] = 1;

	if (cnt[0] == 1 && cnt[1] == 0 && cnt[2] == 0 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player1 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 1 && cnt[2] == 0 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player2 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 0 && cnt[2] == 1 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player3 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 0 && cnt[2] == 0 && cnt[3] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player4 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 1 && cnt[2] == 0 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player1 & player2 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 0 && cnt[2] == 1 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player1 & player3 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 0 && cnt[2] == 0 && cnt[3] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player1 & player4 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 1 && cnt[2] == 1 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player2 & player3 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 1 && cnt[2] == 0 && cnt[3] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player2 & player4 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 0 && cnt[2] == 1 && cnt[3] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player3 & player4 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 1 && cnt[2] == 1 && cnt[3] == 0)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player1 & player2 & player3 won!!!");
	else if (cnt[0] == 1 && cnt[1] == 1 && cnt[2] == 0 && cnt[3] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player1 & player2 & player4 won!!!");
	else if (cnt[0] == 0 && cnt[1] == 1 && cnt[2] == 1 && cnt[3] == 1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "player2 & player3 & player4 won!!!");
	else
		al_draw_text(font, al_map_rgb(0, 0, 0), 500, 100, 0, "All players won!!!");

	al_flip_display();
	close_game(event_queue);
}

void mouse_location(ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_EVENT* event) { 
	while (1) { //control location of mouse
		if ((*event).mouse.x <= 800)
			break;
		al_wait_for_event(event_queue, event);
	}
	// tabdil location be deraye matris
	y = (((*event).mouse.x) - 50) / 100;
	x = ((*event).mouse.y) / 100;
	if (x % 2 != 0) {
		y = (((*event).mouse.x)) / 100;
	}
}

void display_turn(ALLEGRO_FONT* font) {
	if(turn==1 && k==1)
		al_draw_text(font, al_map_rgb(0, 0, 0), 900, 50, 0, "turn: player1");
	if (turn == 2 && k == 2)
		al_draw_text(font, al_map_rgb(0, 0, 0), 900, 50, 0, "turn: player2");
	if (turn == 3 && k == 3)
		al_draw_text(font, al_map_rgb(0, 0, 0), 900, 50, 0, "turn: player3");
	if (turn == 4 && k == 4)
		al_draw_text(font, al_map_rgb(0, 0, 0), 900, 50, 0, "turn: player4");
}

void close_game(ALLEGRO_EVENT_QUEUE* event_queue) {
	ALLEGRO_EVENT event;
	while (1) {
		al_wait_for_event(event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			return;
	}
}