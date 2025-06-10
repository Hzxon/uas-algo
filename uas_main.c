#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 


// =================================================================================
// BAGIAN 1: DEFINISI STRUCT & KONSTANTA
// =================================================================================

#define MAX_NAME_LEN 100
#define MAX_GENRE_LEN 50
#define MAX_AGE_RATING_LEN 10
#define MAX_LINE_BUFFER 2048
#define MAX_DATETIME_LEN 30


#define MAX_LOCATIONS 20
#define MAX_CINEMAS 300
#define MAX_MOVIES 100
#define MAX_SHOWTIMES 200


typedef struct {
    int location_id; 
    char name[MAX_NAME_LEN];
} Location; 

typedef struct {
    int cinema_id; 
    int location_id; 
    char name[MAX_NAME_LEN];
} Cinema; 

typedef struct {
    int movie_id; 
    char title[MAX_NAME_LEN];
    char genre[MAX_GENRE_LEN];
    int duration_minutes; 
    char age_rating[MAX_AGE_RATING_LEN];
    int upcoming; 
} Movie;

typedef struct {
    int show_id; 
    int movie_id; 
    int cinema_id; 
    char show_time[MAX_DATETIME_LEN];
    int price; 
} Showtime; 

typedef struct tbst {
    char judul[100];
    struct tbst *left, *right;  
} tbst; 



// =================================================================================
// BAGIAN 2: DEKLARASI FUNGSI (PROTOTYPES)
// =================================================================================


// --- Utilitas ---
void clear_screen();
void press_enter_to_continue();
void clean_input_buffer();

// --- File I/O untuk file .txt ---
int load_locations_from_txt(const char* filename, Location locations[], int max_items);
int load_cinemas_from_txt(const char* filename, Cinema cinemas[], int max_items);
int load_movies_from_txt(const char* filename, Movie movies[], int max_items);
int load_showtimes_from_txt(const char* filename, Showtime showtimes[], int max_items);

// --- Logika Aplikasi & Menu ---
void load_all_data(Location all_locations[], Cinema all_cinemas[], Movie all_movies[], Showtime all_showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count);
void main_menu(Location locations[], Cinema cinemas[], Movie movies[], Showtime showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count);
int change_location(Location locations[], int location_count);
int find_cinemas_index(Cinema cinemas[], int cinema_count, int location_id);
void print_cinemas(Cinema cinemas[], int index);
void print_movies(Movie movies[], int movie_count, int upcoming, int current_film);
void film_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count);


void buy_ticket_flow(int selected_movie_id, int selected_cinema_id, Showtime showtimes[], int showtime_count);
int select_cinema(Showtime showtimes[], int showtime_count, Cinema cinemas[], int cinema_count, int selected_movie_id);


// =================================================================================
// BAGIAN 4: IMPLEMENTASI FUNGSI UTAMA & MENU
// =================================================================================

int main() {
    Location all_locations[MAX_LOCATIONS];
    Cinema all_cinemas[MAX_CINEMAS];
    Movie all_movies[MAX_MOVIES];
    Showtime all_showtimes[MAX_SHOWTIMES];

    int location_count = 0; 
    int cinema_count = 0; 
    int movie_count = 0;
    int showtime_count = 0; 

    load_all_data(all_locations, all_cinemas, all_movies, all_showtimes, &location_count, &cinema_count, &movie_count, &showtime_count);
    main_menu(all_locations, all_cinemas, all_movies, all_showtimes, &location_count, &cinema_count, &movie_count, &showtime_count);

    return 0; 
}

void main_menu(Location locations[], Cinema cinemas[], Movie movies[], Showtime showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count) {
    int choice; 
    int current_location_id = 1;
    int current_location_index = 0;
    int current_cinemas_index; 
    do {
        printf("====================================================\n");
        printf("                 CINEMA XXI - GROUP 2              \n");
        printf("====================================================\n");
        printf("Location: %s\n", locations[current_location_index].name);
        printf("[0] Exit\n");
        printf("[1] Bioskop\n");
        printf("[2] Film\n");
        printf("[3] m.food\n");
        printf("[4] Pesanan Saya\n");
        printf("[5] Change location\n");
        printf("----------------------------------------------------\n");
        printf("Pilihan Anda: ");

        if(scanf("%d", &choice) != 1) {
            choice = -1; 
            clean_input_buffer();
        }

        switch(choice) {
            case 0:
                clear_screen();
                printf("Thankyou, see you next time!\n");
                break; 
            case 1: 
                clear_screen();
                current_cinemas_index = find_cinemas_index(cinemas, *cinema_count, current_location_id); 
                print_cinemas(cinemas, current_cinemas_index);
                break; 
            case 2:
                clear_screen();
                film_menu(movies, *movie_count, cinemas, *cinema_count, showtimes, *showtime_count);
                break; 
            case 3: 
                clear_screen();
                break; 
            case 4: 
                clear_screen();
                break;
            case 5: 
                clear_screen();
                current_location_index = change_location(locations, *location_count);
                current_location_id = locations[current_location_index].location_id;
                break; 
            default: 
                printf("Input tidak valid!\n");
                press_enter_to_continue();
                break; 
        } 
    } while(choice != 0);
}

void film_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count) {
    int choice;
    int current_film = 1;
    int selected_cinema_id;
    int selected_movie_id = movies[current_film-1].movie_id;
    do {
        print_movies(movies, movie_count, 0, current_film);
        printf("====================================================\n");
        printf("                   FILM MAIN MENU                   \n");
        printf("====================================================\n");
        printf("[0] Exit\n");
        printf("[1] Akan tayang\n");
        printf("[2] Pilih film\n");
        printf("[3] Beli tiket\n");
        printf("[4] \n");
        printf("----------------------------------------------------\n");
        printf("Pilihan Anda: ");

        if(scanf("%d", &choice) != 1) {
            choice = -1; 
            clean_input_buffer();
        }

        switch (choice) {
            case 0: 
                clear_screen();
                break; 
            case 1:
                clear_screen();
                print_movies(movies, movie_count, 1, 0);
                press_enter_to_continue();
                clear_screen();
                break; 
            case 2:
                printf("Masukkan nomor film: ");
                scanf("%d", &current_film);
                clean_input_buffer();
                clear_screen();
                break;
            case 3:
                clear_screen();
                selected_cinema_id = select_cinema(showtimes, showtime_count, cinemas, cinema_count, selected_movie_id);
                buy_ticket_flow(selected_movie_id, selected_cinema_id, showtimes, showtime_count);
                break;

            default: 
                printf("Input tidak valid!\n");
                press_enter_to_continue();
                break; 
        } 
    } while(choice != 0);
}

void load_all_data(Location all_locations[], Cinema all_cinemas[], Movie all_movies[], Showtime all_showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count) {
    printf("Fetching all data from .txt...\n");
    *location_count = load_locations_from_txt("locations.txt", all_locations, MAX_LOCATIONS);
    *cinema_count = load_cinemas_from_txt("cinemas.txt", all_cinemas, MAX_CINEMAS);
    *movie_count = load_movies_from_txt("movies.txt", all_movies, MAX_MOVIES);
    *showtime_count = load_showtimes_from_txt("showtimes.txt", all_showtimes, MAX_SHOWTIMES);
    printf("Successfully load all data. (Location: %d, Cinema: %d, Movie: %d, Showtime: %d)\n", *location_count, *cinema_count, *movie_count, *showtime_count);
}

// =================================================================================
// BAGIAN 5: IMPLEMENTASI FILE I/O UNTUK .TXT
// =================================================================================
int load_locations_from_txt(const char* filename, Location locations[], int max_items)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0; 
    int count = 0; 
    while(count < max_items && fscanf(fp, "%d, %[^\n]\n", &locations[count].location_id, locations[count].name) == 2) {
        count++; 
    }

    fclose(fp);
    return count; 
}

int load_cinemas_from_txt(const char* filename, Cinema cinemas[], int max_items)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0; 
    int count = 0; 
    while(count < max_items && fscanf(fp, "%d, %d, %[^\n]\n", &cinemas[count].cinema_id, &cinemas[count].location_id, cinemas[count].name) == 3) {
        count++;
    }

    fclose(fp);
    return count; 
}

// --- Film ---
int load_movies_from_txt(const char* filename, Movie movies[], int max_items) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    int count = 0;
    char line_buffer[MAX_LINE_BUFFER];
    while(count < max_items && fgets(line_buffer, sizeof(line_buffer), fp)) {
        sscanf(line_buffer, "%d,%[^,],%[^,],%d,%[^,],%d",
               &movies[count].movie_id, movies[count].title, movies[count].genre,
               &movies[count].duration_minutes, movies[count].age_rating, &movies[count].upcoming);
        count++;
    }
    fclose(fp);
    return count;
}

int load_showtimes_from_txt(const char* filename, Showtime showtimes[], int max_items)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0; 
    int count = 0; 
    while(count < max_items && fscanf(fp, "%d,%d,%d,%[^,],%d", &showtimes[count].show_id, &showtimes[count].movie_id, &showtimes[count].cinema_id, showtimes[count].show_time, &showtimes[count].price) == 5) {
        count++;
    }

    fclose(fp);
    return count; 
}
 
// =================================================================================
// BAGIAN 6: IMPLEMENTASI LOGIKA APLIKASI
// =================================================================================

void buy_ticket_flow(int selected_movie_id, int selected_cinema_id, Showtime showtimes[], int showtime_count) {
    clear_screen();
    printf("=== Jadwal Tayang Tersedia ===\n");

    int available_shows[MAX_SHOWTIMES];
    int show_count = 0;

    // 1. Cari semua jadwal yang cocok
    for (int i = 0; i < showtime_count; i++) {
        if (showtimes[i].movie_id == selected_movie_id && showtimes[i].cinema_id == selected_cinema_id) {
            available_shows[show_count] = i;
            show_count++;
        }
    }

    if (show_count == 0) {
        printf("Maaf, tidak ada jadwal tayang untuk film ini di bioskop yang dipilih.\n");
        press_enter_to_continue();
        return;
    }

    // 2. Tampilkan jadwal yang ditemukan
    for (int i = 0; i < show_count; i++) {
        int show_index = available_shows[i];
        printf("[%d] Waktu: %s - Harga: Rp %d\n", i + 1, showtimes[show_index].show_time, showtimes[show_index].price);
    }
    printf("[0] Batal\n");

    // Lanjutkan ke langkah b...
}

int select_cinema(Showtime showtimes[], int showtime_count, Cinema cinemas[], int cinema_count, int selected_movie_id) {

    int k = 1;
    int choice;
    int cinemas_index[MAX_CINEMAS];

    printf("Film tayang di: \n");
    for(int i = 0; i < showtime_count; i++) {
        if (selected_movie_id == showtimes[i].movie_id) {
            for(int j = 0; j < cinema_count; j++) {
                if (cinemas[j].cinema_id == showtimes[i].cinema_id) {
                    cinemas_index[k] = cinemas[j].cinema_id; 
                    printf("%d. %s\n", k, cinemas[j].name);
                    k++;
                } 
            }
        }
    }


    printf("Pilih bioskop: ");
    scanf("%d", &choice);
    clean_input_buffer();

    printf("K : %d, C ID : %d\n", k, cinemas_index[k]);

    return cinemas_index[choice];
}

int change_location(Location locations[], int location_count) {

    int index; 

    printf("Location count %d\n", location_count);

    for(int i = 0; i < location_count; i++) {
        printf("[%d] %s\n", i+1, locations[i].name);
    }

    printf("Select location (number): ");
    scanf("%d", &index);

    return index-1; 
}

int find_cinemas_index(Cinema cinemas[], int cinema_count, int location_id) {
    int index; 

    for(int i = 0; i < cinema_count; i++)
        if (cinemas[i].location_id == location_id) {
            index = i;
            break;
        }

    return index; 
}

void print_cinemas(Cinema cinemas[], int index) {

    int j = 1;
    for(int i = index; cinemas[i].location_id == cinemas[index].location_id; i++) {
        printf("%d. %s\n", j, cinemas[i].name);
        j++;
    }

}

void print_movies(Movie movies[], int movie_count, int upcoming, int current_film) {
    
    if (movie_count <= 0) return;

    if (upcoming)
        printf("Akan tayang\n");
    else 
        printf("Lagi tayang\n");

    for(int i = 0; i < movie_count; i++) {
        if (upcoming && movies[i].upcoming)
            printf("%d. %s\n", (i-20)+1, movies[i].title);
        else if (!upcoming && !movies[i].upcoming) {
            if (current_film == i+1) printf("-->");
            printf("%d. %s\n", i+1, movies[i].title);
        }
    }
}


// =================================================================================
// BAGIAN 7: UTILITAS INTERNAL & INISIALISASI DATA
// =================================================================================



void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void press_enter_to_continue() {
    printf("\nTekan Enter untuk melanjutkan...");
    clean_input_buffer();
    getchar();
}
