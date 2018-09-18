/*
James Eckler
CS3240
2/1/2018
A0
*/

/*
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct to hold song data
typedef struct _song_t {
	char *artist;
	char *title;
	char *album;
	float duration;
	int year;
	double hotttness;
} song_t;

song_t binarySearch(char *searchTitle, song_t *array[], int start, int end);

int main(int argc, char *argv[]) {

	// Buffer to read from file
	char buf[1024];

	// Opens file using a file stream
	FILE *csv = fopen("./SongCSV.csv", "r");

	// Counter that will be used to determine size of song array
	int songs = 0;	
	
	// Skip first entry of file
	char *result = fgets(buf, 1024, csv);

	// Checks that result is not NULL
	if (NULL == result) { exit(-1); }

	// A loop that will count the number of entries in the csv file until it hits the end of the file
	while (feof(csv) == 0) {

		// Reads file one line at a time		
		result = fgets(buf, 1024, csv);

		// Increments song count only if the entry isn't empty
		if (strcmp(result, "") != 0) {
			songs++;
		}
	}
	
	// Clears EOF flag
	clearerr(csv);

	// Set pointer back to beginning of file and skip first entry
	fseek(csv, 0, SEEK_SET);
	fgets(buf, 1024, csv);

	// Array of pointers to song_t structs
	song_t *arr[songs];

	// Read file into song array, keeping them in alphabetical order via insertion sort
	int i = 0;
	while (feof(csv) == 0) {

		

		// Looks at next line of file
		fgets(buf, 1024, csv);
		
		// Only goes through process of looking at tokens and allocating memory if buffer isn't empty
		if (strcmp(buf, "") != 0) {

			// A struct to hold the song we are currently looking at
			song_t *currentSong = malloc(sizeof(song_t));

			char *token = strtok(buf, ",\"");
			int tokCounter = 0;
			int length;

			// Looks through tokens until there are none left
			while (NULL != token) {

				// Gets next token and increments token counter
				token = strtok(NULL, ",\"");
				tokCounter++;			

				// If current token is looking at right data, puts that in currentSong struct
				switch (tokCounter) {
					// Album
					case 3:
						// Gets length of album name, allocates memory and places it in struct
						length = strlen(token);
						currentSong->album = malloc(length + 1);
						strncpy(currentSong->album, token, length+1);
						break;
					// Artist
					case 8:
						// Gets length of artist name, allocates memory and places it in struct
						length = strlen(token);
						currentSong->artist = malloc(length + 1);
						strncpy(currentSong->artist, token, length+1);
						break;
					// Duration
					case 10:
						// Converts token to float and places it in struct
						currentSong->duration = atof(token);
						break;
					// Hotttness
					case 14:
						// Converts token to float and places it in struct
						currentSong->hotttness = atof(token);
						break;
					// Title
					case 17:
						// Gets length of song title, allocates memory and places it in struct
						length = strlen(token);
						currentSong->title = malloc(length + 1);
						strncpy(currentSong->title, token, length+1);
						break;
					// Year
					case 18:
						// Converts token to int and places it in struct
						currentSong->year = atoi(token);
						break;
				}
			
			}

		

			// Allocate space for each song entry in array
			arr[i] = malloc(sizeof(song_t));

			// If nothing has been added to array, set current song to first index
			if (i == 0) {
				arr[0] = currentSong;
			}

			// Runs insertion sort on songs in array			
			else {
				// Checks song_t at current index currently in array to see if current song belongs there
				int current = 0;
				while (current <= i) {
					// Checks if current song comes before song at index
					if (strcmp(currentSong->title, arr[current]->title) < 0) {
						// If so, moves all songs below current song down one
						for (int mover = i; mover > current; mover--) {
							arr[mover] = arr[mover-1];
						}
						// Place current song at index
						arr[current] = currentSong;

						// Current song is where it needs to be, set current so it exits while loop
						current = i;
					}
					// Current song belongs at end of list
					else if (current == i) {
						arr[current] = currentSong;
					}
					// Increment current index to next song_t
					current++;
				}

			}

			// Increments array counter
			i++;
		}
	}

	// Close SongCSV.csv
	fclose(csv);

	

}

// Recursive method that searches the sorted song array
song_t binarySearch(char *searchTitle, song_t *array[], int start, int end) {

	int middle = start + (end-start) / 2;

	// Base case, song not found	
	if (start > end) {
		return song_t;
	}

	// Base case, song found
	if (strcmp(searchTitle, array[middle]->title) == 0) {
		return *array[middle];
	}

	// Search term is before middle song
	if (strcmp(searchTitle, array[middle]->title) < 0) {
		return binarySearch(searchTitle, array, start, middle - 1);
	}

	// Search term is after middle song
	if (strcmp(searchTitle, array[middle]->title) > 0) {
		return binarySearch(searchTitle, array, middle + 1, end);
	}
	return song_t;
}
