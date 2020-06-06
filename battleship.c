#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<time.h>

#define nr_col 10
#define nr_lin 10

//menu function - select New Game / Resume / Quit
void Menu(int *option) {

	curs_set(0);
	*option = -1;
	char list[3][12] = {"New Game", "Resume Game", "Quit"};
	int xmax, ymax, i;
	getmaxyx(stdscr, ymax, xmax);
	WINDOW *menu = newwin(5, xmax - 8, ymax/2 - 2, 5);
	box(menu, 0, 0);
	refresh();
	wrefresh(menu);

	keypad(menu, true);
	int highlight = 0;

	while(1) {

		for (i = 0; i < 3; i++) {
			if(i == highlight) {
				wattron(menu, A_STANDOUT);
			}
			mvwprintw(menu, i+1, (xmax - 8 - strlen(list[i]))/2, "%s", list[i]);
			wattroff(menu, A_STANDOUT);
		}
		refresh();
		wrefresh(menu);
		int c = wgetch (menu);
			switch (c) 
			{
				case KEY_DOWN:
					highlight++;
					if (highlight > 2) highlight = 2;
					break;
				case KEY_UP:
					highlight--;
					if(highlight < 0) highlight = 0;
					break;
				case '\n':
					*option = highlight;
					break;
				default:
					break;
			}
		 if(*option != -1)
		 	break;

	}
	wclear(menu);
	clear();	
}

//transpsing the integer matrix into the char matrix to be shown
void Transf_Map(char map[nr_lin][nr_col+12], int map_int[nr_lin][nr_col]) {
	int i, j;

	 for (i = 0; i < nr_lin; i++) {
	 	if(map_int[i][0] == 0)
	 		{
	 			strcpy((map)[i], "| ");
	 		}
	 	if(map_int[i][0] == 1) 
	 	{
	 		strcpy((map)[i], "|X");
	 		
	 	}
	 	for (j = 1; j < nr_col; j++) {
			if(map_int[i][j] == 0)
				strcat((map)[i], "| ");
			if(map_int[i][j] == 1)
				strcat((map)[i], "|X");
			
				
		}
		map[i][nr_col + 11] = '|';
		map[i][nr_col + 12] = '\0';
		
	}
}

//checks if the configuration rules are followed for horizontal ships
int Check_hor(int computer_map_int[][nr_col], int i , int j, int dim) {
	int k;

	for(k = j; k <= j + dim - 1; k++) {
		if(computer_map_int[i][k]!=-1)
			return 0;
		if(i == 0)
			if(computer_map_int[i+1][k]==1)
				return 0;
		if(i == nr_lin-1)
			if(computer_map_int[i-1][k]==1)
				return 0;
		if((i!=0)&&(i!=nr_lin-1))
			if((computer_map_int[i-1][k]==1)||(computer_map_int[i+1][k]==1))
				return 0;
	}
	if(j > 0)
	{
		if(i > 0 && computer_map_int[i-1][j-1] == 1)
			return 0;
		if(computer_map_int[i][j-1] == 1)
			return 0;
		if(i + 1 < nr_lin && computer_map_int[i+1][j-1] == 1)
			return 0;
	}
		
	if(j + dim < nr_col){
		if(i > 0 && computer_map_int[i-1][j+dim]==1)
			return 0;
		if(computer_map_int[i][j+dim]==1)
			return 0;
		if(i + 1 < nr_lin && computer_map_int[i+1][j+dim]==1)
		return 0;
	}
	return 1;
}

//checks if the configuration rules are followed for vertical ships
int Check_vert(int computer_map_int[][nr_col], int i, int j, int dim) {
	int k;
	for(k = i; k <= i + dim - 1; k++) {
		if(computer_map_int[k][j] != -1)
			return 0;
		if(j == 0)
			if(computer_map_int[k][j+1] == 1)
				return 0;
		if(j == nr_col - 1)
			if(computer_map_int[k][j-1] == 1)
				return 0;
		if(j != 0 && j != nr_col - 1)
			if((computer_map_int[k][j-1]==1)||(computer_map_int[k][j+1]==1))
				return 0;
	}
	if(i > 0) {
		if(j-1 >= 0 && computer_map_int[i-1][j-1] == 1)
			return 0;
		if(computer_map_int[i-1][j]==1)
			return 0;
		if(j+1 < nr_col && computer_map_int[i-1][j+1]==1)
			return 0;
	}
	if(i+dim < nr_lin) {
		if(j-1 >= 0 && computer_map_int[i+dim][j-1]==1)
			return 0;
		if(computer_map_int[i+dim][j]==1)
			return 0;
		if(j + 1 < nr_col && computer_map_int[i+dim][j+1]==1)
		return 0;
	}
	return 1;
}

//generates random map for the computer
void Generate_Map(int computer_map_int[][nr_col]) {
	int i, j, nr_ships, dim, hor_vert, k, cnr_ships;
	nr_ships = 1;
	dim = 4;

	for(i = 0; i < 10; i++) {
		 for(j = 0; j < 10; j++) {
			computer_map_int[i][j] = -1;
	 	}

	 }
	time_t t;
	srand(time(&t));
	for( nr_ships = 1, dim = 4; dim > 0 && nr_ships <=4 ; dim--,nr_ships++) {
		cnr_ships = nr_ships;
		while (cnr_ships) {
			i = rand() % 10;
			j = rand() % 10;
			hor_vert = rand() % 2;		

				if((hor_vert==1)&&(j + dim -1 < nr_col)&&Check_hor(computer_map_int,i,j,dim)==1) {
				
						for(k = j; k <= j + dim -1; k++) {
							computer_map_int[i][k] = 1;

							if(i - 1 >= 0) computer_map_int[i-1][k] = 0;
							if(i + 1 < nr_lin) computer_map_int[i+1][k] = 0;

						}
						if(j - 1 >= 0) {
							computer_map_int[i][j-1] =  0;
							if(i - 1 >= 0) 
								computer_map_int[i-1][j-1] = 0;
							if(i + 1 < nr_lin)
								computer_map_int[i+1][j-1] = 0;
						} 
						if(j + dim < nr_col) {
							computer_map_int[i][j+dim] = 0;
							if(i - 1 >= 0) 
								computer_map_int[i-1][j+dim] = 0;
							if(i + 1 < nr_lin)
								computer_map_int[i+1][j+dim] = 0;
						}
						cnr_ships--;

				} else
					if((hor_vert==0)&&(i + dim - 1 < nr_lin)&&(Check_vert(computer_map_int,i,j,dim)==1)) {

							for(k = i; k <= i + dim - 1; k++) {
								computer_map_int[k][j] = 1;
								if(j - 1 >= 0) computer_map_int[k][j-1] = 0;
								if(j + 1 < nr_col) computer_map_int[k][j+1] = 0;

							}
							if(i - 1 >= 0) {
								computer_map_int[i-1][j] = 0;
								if(j - 1 >= 0)
									computer_map_int[i-1][j-1] = 0;
								if(j + 1 < nr_col)
									computer_map_int[i-1][j+1] = 0;
							}
							if(i + dim  < nr_lin) {
								computer_map_int[i+dim][j] = 0;
								if(j - 1 >= 0)
									computer_map_int[i+dim][j-1] = 0;
								if(j + 1 < nr_col)
									computer_map_int[i+dim][j+1] = 0;
							}
							cnr_ships--;

					}
				
			
		}
		
	}
	for(i = 0; i < nr_lin; i++ )
		for(j = 0;j < nr_col; j++)
			if(computer_map_int[i][j] == -1)
				computer_map_int[i][j] = 0;
}


void Read_Player(FILE *debug, FILE *input, int player_map_int[nr_lin][nr_col]) {
	int i,j;

	for (i = 0; i < nr_lin; i++) {
		for(j = 0; j < nr_col; j++) {
			fscanf(input, "%d", &player_map_int[i][j]);
		}
	}
	fclose(input);
}


void Init_cmap(FILE *debug,char computer_map[nr_lin][nr_col+12] ) {
	int i, j;
	for(i = 0; i < nr_lin; i++) {
		strcpy((computer_map)[i],"| | | | | | | | | | |");
		computer_map[i][nr_col+12] = '\0';
	}
}


void Player_turn(int *ok, int *line, int *col,FILE *debug, WINDOW *computer, int *p_score, int computer_map_int[nr_lin][nr_col], char computer_map[nr_lin][nr_col+12], char player_map[nr_lin][nr_col+12], int comp_seen[nr_lin][nr_col]) {
	int i, j, c, ch;
	ch = 0;

	curs_set(1);
	keypad(computer, true);
	wmove(computer, *line, *col);
	wrefresh(computer);
	while(1) {
		curs_set(1);
		c = wgetch(computer);
		switch (c) 
		{
			case KEY_DOWN:
				(*line)++;
				if (*line > 9 ) *line = 9;
				wmove(computer, *line, *col);
				ch = 0;
				break;
			case KEY_UP:
				(*line)--;
				if(*line < 0) *line = 0;
				wmove(computer, *line, *col);
				ch = 0;
				break;
			case KEY_RIGHT:
				(*col)+=2;
				if (*col > 19 ) *col = 19;
				wmove(computer, *line, *col);
				ch = 0;
				break;
			case KEY_LEFT:
				(*col)-=2;
				if(*col < 1) *col = 1;
				wmove(computer, *line, *col);
				ch = 0;
				break;
			case '\n':
				ch = 1;
				break;
			case 'q':
				(*ok) = 0;
				break;

		}
		wrefresh(computer);
		if((*ok) == 1 && ch == 1 && comp_seen[*line][((*col)-1)/2] == 0) {
			comp_seen[*line][((*col)-1)/2] = 1;
			if(computer_map_int[*line][((*col)-1)/2] == 1) {
	
				computer_map[*line][*col] = 'X';
				
				wmove(computer, 0, 0);
				for(int i = 0; i < nr_lin; i++) {
					for(int j = 0; j < nr_col + 12; j++){
						 wprintw(computer, "%c", computer_map[i][j]);
						
					}
					wprintw(computer, "\n");
				
					}
					curs_set(0);
					wrefresh(computer);
					wmove(computer,*line,*col);
				(*p_score)++;
				
				
			}
			else {
				computer_map[*line][*col] = '-';
				wmove(computer, 0, 0);
				for(int i = 0; i < nr_lin; i++) {
					for(int j = 0; j < nr_col + 12; j++){
						 wprintw(computer, "%c", computer_map[i][j]);
				
					}
					wprintw(computer, "\n");
				
					}

				curs_set(0);
				wrefresh(computer);

				break;
			}
			

			
		}
		if((*ok) == 0)
			break;

	}
}	

void Comp_turn(FILE *debug, WINDOW *player, int *c_score, int player_map_int[nr_lin][nr_col], char computer_map[nr_lin][nr_col+12], char player_map[nr_lin][nr_col+12], int player_seen[nr_lin][nr_col]) {
	int xmax, ymax, i, j, p, k;
	curs_set(0);

	time_t t;
	srand(time(&t));
	while(1) {
		
		i = rand()%10;
		j = rand()%10;
		if(player_seen[i][j] == 0)
		{
			player_seen[i][j] = 1;
			if(player_map_int[i][j] == 0) {

				player_map[i][2*j+1] = '-';
				wclear(player);
				wrefresh(player);
				for(p = 0; p < nr_lin; p++) {
					for(k = 0; k < nr_lin+12; k++) {
						wprintw(player, "%c", player_map[p][k]);
						
					}
						
					wprintw(player, "\n");
					

				}
				break;
			}
			if(player_map_int[i][j] == 1) {
				player_map[i][2*j+1] = '#';
				wclear(player);
				wrefresh(player);
				for(p = 0; p < nr_lin; p++) {
					for(k = 0; k < nr_lin+12; k++) {
						wprintw(player, "%c", player_map[p][k]);
						
					}
					wprintw(player, "\n");
					}
				(*c_score)++;
				if((*c_score) == 20)
					break;	
				wrefresh(player);
			}
				
		}
		wrefresh(player);
	}
	
	wrefresh(player);
}	

void Init_seen(int map[nr_lin][nr_col]) {
	int i, j;
	for(i = 0; i < nr_lin; i++)
		for(j = 0; j< nr_col; j++)
			map[i][j] = 0;
}

void NewGame(int argc, char **argv, int comp_seen[nr_lin][nr_col], int player_seen[nr_lin][nr_col],
	FILE *debug,char computer_map[nr_lin][nr_col+12], int computer_map_int[nr_lin][nr_col], 
	char player_map[nr_lin][nr_col+12], int player_map_int[nr_lin][nr_col], int *option, int *c_score, int *p_score) {
	int i, j, line, col, ok, xmax, ymax;
	
	*c_score = 0;
	line = 0;
	col = 1;
	ok = 1;

	FILE *input;
	input=fopen(argv[1],"r");
	getmaxyx(stdscr, ymax, xmax);
	WINDOW *select = newwin(ymax, xmax, 0, 0);
	char str[100] = "Please select your grid";
	mvwprintw(select, ymax/2, xmax / 2 - strlen(str) / 2, "%s", str);
	keypad(select, 1);
	int current_option = 1;
	while(1) {
		for(i = 1; i < argc; i++) {
			if(i == current_option)
				wattron(select, A_STANDOUT);
			mvwprintw(select, ymax / 2 + 2, xmax / 2 - argc / 2 + i - 1, "%d", i);
			wattroff(select, A_STANDOUT);
			wrefresh(select);
		}
		int key_select = wgetch(select);
		if(key_select == KEY_RIGHT && current_option < argc - 1)
			current_option++;
		if(key_select == KEY_LEFT && current_option > 1)
			current_option--;
		if(key_select == 10) {
			wclear(select);
			wrefresh(select);
			break;
		}
	}

	WINDOW *game = newwin(ymax, xmax, 0, 0);
	mvwprintw(game, ymax/2-8, 8, "Opponent's grid");
	mvwprintw(game, ymax/2-8, xmax/2+10, "Your grid");

	WINDOW *computer = derwin(game, 10, nr_col+13, ymax/2-5, 5);
	WINDOW *player = derwin(game, 10, nr_col+13, ymax/2-5, xmax/2+5);


	Init_seen(comp_seen);
	Init_seen(player_seen);
	
	Generate_Map(computer_map_int);
	Init_cmap(debug, computer_map);
	Read_Player(debug, input, player_map_int);
	Transf_Map(player_map, player_map_int);

	keypad(game, TRUE);
	
	for (i = 0; i < nr_lin; i++) {
		for(j = 0; j < nr_col + 12; j++)
			wprintw(computer, "%c", computer_map[i][j]);
		wprintw(computer, "\n");
	}
	wrefresh(computer);
	for (i = 0; i < nr_lin; i++) {
		for(j = 0; j < nr_col + 12; j++)
			wprintw(player, "%c", player_map[i][j]);
		wprintw(player, "\n");
	}
	wrefresh(player);
	while((*c_score) < 20 && (*p_score) < 20) {
		int key = wgetch(game);
		if(key == 'q') {

			break;
		}
		Player_turn(&ok, &line, &col, debug, computer, p_score, computer_map_int, computer_map, player_map, comp_seen);
		if(ok == 1)
			Comp_turn(debug, player, c_score, player_map_int, computer_map, player_map, player_seen);
		else {
			wclear(game);
			wrefresh(game);
			break;
		}


	}
	clear();
	
	
}

void Resume(FILE *debug,int comp_seen[nr_lin][nr_col], int player_seen[nr_lin][nr_col], char computer_map[nr_lin][nr_col+12],
int computer_map_int[nr_lin][nr_col], char player_map[nr_lin][nr_col+12], int player_map_int[nr_lin][nr_col], int *option, int *c_score, int *p_score) {
	int i, j, line, col, ok, xmax, ymax;
	line = 0;
	col = 1;
	ok = 1;

	getmaxyx(stdscr, ymax, xmax);
	WINDOW *game = newwin(ymax, xmax, 0, 0);
	mvwprintw(game, ymax/2-8, 8, "Opponent's grid");
	mvwprintw(game, ymax/2-8, xmax/2+10, "Your grid");

	WINDOW *computer = derwin(game, 10, nr_col+13, ymax/2-5, 5);
	WINDOW *player = derwin(game, 10, nr_col+13, ymax/2-5, xmax/2+5);

	keypad(game, TRUE);
	for (i = 0; i < nr_lin; i++) {
		for(j = 0; j < nr_col + 12; j++)
				wprintw(computer, "%c", computer_map[i][j]);
		wprintw(computer, "\n");
	}
	wrefresh(computer);
	for (i = 0; i < nr_lin; i++) {
		for(j = 0; j < nr_col + 12; j++)
			wprintw(player, "%c", player_map[i][j]);
		wprintw(player, "\n");
	}
	wrefresh(player);

		while((*c_score) < 20 && (*p_score) < 20) {
		int key = wgetch(game);
		if(key == 'q') {
	
			break;
		}
		Player_turn(&ok, &line, &col, debug, computer, p_score, computer_map_int, computer_map, player_map, comp_seen);
		if(ok == 1)
			Comp_turn(debug, player, c_score, player_map_int, computer_map, player_map, player_seen);
		else {
			wclear(game);
			wrefresh(game);
			break;
		}

		}
		wclear(game);
		wrefresh(game);
}
int Check_first(int computer_seen[nr_lin][nr_col]) {
	int i, j;
	for(i = 0; i < nr_lin; i++)
		for(j = 0; j < nr_col; j++)
			if(computer_seen[i][j] == 1)
				return 0;
	return 1;

}
void Comp_score(FILE *debug, char computer_map[nr_lin][nr_col+12],int computer_map_int[nr_lin][nr_col], int *cships) {
	int i, j, x = 1, y = 0, dimc, dimi, vo, ok, d[3];
	(*cships) = 0;
	for(i = 0; i < nr_lin; i++) {
		for(j = 1; j < nr_col+12; j+=2) {
			ok = 1;
			if (computer_map[i][j] == 'X') {
				y = i;
				x = j;
				computer_map[i][j] == ' ';
				dimc = 1;
			if(j+2 < nr_col && computer_map[i][j+2] == 'X') {
				dimc++;
				vo = 0;
				computer_map[i][j+2] == ' ';
				if(j + 4 < nr_col && computer_map[i][j+4] == 'X') {
					dimc++;
					computer_map[i][j+4] == ' ';
					if(j + 6 < nr_col && computer_map[i][j+6] == 'X'){
						dimc++;
						computer_map[i][j+6] == ' ';
					}

				}
			}
			if(i+2 < nr_lin && computer_map[i+2][j] == 'X') {
				dimc++;
				vo = 1;
				computer_map[i+2][j] == ' ';
				if(i + 4 < nr_lin && computer_map[i+4][j] == 'X') {
					dimc++;
					computer_map[i+4][j] == ' ';
					if(i + 6 < nr_lin && computer_map[i+6][j] == 'X') {
						computer_map[i+6][j] == ' ';
						dimc++;
					}
				}
			}

		}
			
		dimi = 1;
		if( ( (x-1) / 2 >= 1 && (computer_map_int[y][(x-1)/2-1] == 1)) || ( (y-1) >= 0 && (computer_map_int[y - 1][(x - 1) / 2] == 1) ) ) {
			ok == 0;
		} else {
			if ( dimc != 1) {
				if(vo == 0) {
					while(computer_map_int[y][(x-1)/2+1] == 1){
						dimi++;
						x+=2;
					}
				} else {
					while(computer_map_int[++y][(x-1)/2] == 1)
						dimi++;
				}
				
			} 
		}
		if (ok == 1 && dimi == dimc )
			(*cships)++;
		}

	}
	
}

void Player_score(char player_map[nr_lin][nr_col+12],int player_map_int[nr_lin][nr_col], int *pships) {
	int i, j, x = 1, y = 0, dimc, dimi, vo, ok, d[3];
	*pships = 0;
	
	for(i = 0; i < nr_lin; i++) {
		for(j = 1; j < nr_col+12; j+=2) {
			ok = 1;
			if (player_map[i][j] == '#') {
				y = i;
				x = j;
				player_map[i][j] == ' ';
				dimc = 1;
			if(j+2 < nr_col && player_map[i][j+2] == '#') {
				dimc++;
				vo = 0;
				player_map[i][j+2] == ' ';
				if(j + 4 < nr_col && player_map[i][j+4] == '#') {
					dimc++;
					player_map[i][j+4] == ' ';
					if(j + 6 < nr_col && player_map[i][j+6] == '#'){
						dimc++;
						player_map[i][j+6] == ' ';
					}

				}
			}
			if(i+2 < nr_lin && player_map[i+2][j] == '#') {
				dimc++;
				vo = 1;
				player_map[i+2][j] == ' ';
				if(i + 4 < nr_lin && player_map[i+4][j] == '#') {
					dimc++;
					player_map[i+4][j] == ' ';
					if(i + 6 < nr_lin && player_map[i+6][j] == '#') {
						player_map[i+6][j] == ' ';
						dimc++;
					}
				}
			}

		}
			
		dimi = 1;
		if(((x-1)/2 >= 1 && player_map_int[y][(x-1)/2-1] == 1) || (y-1 >= 0 && player_map_int[y-1][(x-1)/2] == 1)) {
			ok == 0;
		} else {
			if ( dimc != 1) {
				if(vo == 0) {
					while(player_map_int[y][(x-1)/2+1] == 1){
						dimi++;
						x+=2;
					}
				} else {
					while(player_map_int[++y][(x-1)/2] == 1)
						dimi++;
				}
				
			} 
		}
		if (ok == 1 && dimi == dimc )
			(*pships)++;


		}

	}
	
}

void Score(FILE *debug, int p_score, int c_score, int *pships, int *cships, char computer_map[nr_lin][nr_col+12], char player_map[nr_lin][nr_col+12], int computer_map_int[nr_lin][nr_col], int player_map_int[nr_lin][nr_col]) {
	
	int xmax, ymax;
	getmaxyx(stdscr, ymax, xmax);
	getch();
	WINDOW *score = newwin(ymax, xmax, 0, 0);
	if (p_score == 20) {
		Comp_score(debug, computer_map, computer_map_int, cships);
		fprintf(debug, "%d ", *cships); 
		mvwprintw(score, ymax/2, xmax/2 - 3, "YOU WON!");
		mvwprintw(score, ymax/2 + 3, xmax/2 -3, "Your score:10");
		mvwprintw(score, ymax/2 + 4, xmax/2 -3, "Computer's score:%d", *cships);
		wrefresh(score);
	} else {
		Player_score(player_map, player_map_int, pships);
		fprintf(debug, "%d\n", *pships);
		mvwprintw(score, ymax/2, xmax/2 - 3, "YOU LOST!");
		mvwprintw(score, ymax/2 + 3, xmax/2 -3, "Your score:%d", *pships);
		mvwprintw(score, ymax/2 + 4, xmax/2 -3, "Computer's score:10");
		wrefresh(score);
	}
	getch();
	wclear(score);
	wrefresh(score);

}

int main(int argc, char **argv)
{
	FILE *debug;

	if(argc == 1) {
		printf("[Eroare]: Nu s-au dat argumente de comanda!\n");
		return 1;
	}

	char player_map[10][nr_col+12], computer_map[nr_lin][nr_col+12];
	int computer_map_int[10][10], player_map_int[10][10], computer_seen[10][10], player_seen[10][10];
	int option, i ,j, p_score, c_score, ymax, xmax, pships, cships;

	for(i = 1; i < argc; i++) {
		if(fopen(argv[i],"r") == NULL) {
			printf("[Eroare]: Fisierul %s nu s-a putut deschide!\n", argv[i]);
			return 1;
		}
	}

	initscr();
	cbreak();
	noecho();


	Menu(&option);	


	while(1) {

		
		if(option == 0){
			NewGame(argc, argv, computer_seen, player_seen, debug,computer_map, computer_map_int, player_map, player_map_int, &option, &c_score, &p_score);
			if (p_score == 20 || c_score == 20) {
				fprintf(debug, "%d %d", p_score, c_score);
				Score(debug, p_score, c_score, &pships, &cships, computer_map, player_map, computer_map_int, player_map_int);
			}
			Menu(&option);
		}
		if(option == 1 && Check_first(computer_seen) == 0) {
		 	Resume(debug,computer_seen, player_seen, computer_map, computer_map_int, player_map, player_map_int, &option, &c_score, &p_score);
		 	if (p_score == 20 || c_score == 20) {
				Score(debug, p_score, c_score, &pships, &cships, computer_map, player_map, computer_map_int, player_map_int);
			}
		 	Menu(&option);
		} else {
			if(option == 1 && Check_first(computer_seen) ) {
				NewGame(argc, argv, computer_seen, player_seen, debug,computer_map, computer_map_int, player_map, player_map_int, &option, &c_score, &p_score);
				if (p_score == 20 || c_score == 20) {
					Score(debug, p_score, c_score, &pships, &cships, computer_map, player_map, computer_map_int, player_map_int);
			}
			Menu(&option);
				Menu(&option);
			}
		}
		if(option == 2) {
			break;
		}
	}
	
	endwin();
	fclose(debug);

	return 0;
}