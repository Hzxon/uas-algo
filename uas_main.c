#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>


// =================================================================================
// BAGIAN 1: DEFINISI STRUCT & KONSTANTA
// =================================================================================

#define MAX_NAME_LEN 100
#define MAX_GENRE_LEN 50
#define MAX_AGE_RATING_LEN 10
#define MAX_LINE_BUFFER 2048
#define MAX_DATETIME_LEN 30
#define MAX_COLS 16
#define MAX_ROWS 8
#define MAX_SEATS_PER_BOOKING 10


#define MAX_LOCATIONS 20
#define MAX_CINEMAS 300
#define MAX_MOVIES 100
#define MAX_SHOWTIMES 500
#define MAX_BOOKINGS 500


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
    char seats[MAX_ROWS][MAX_COLS];
} Showtime; 

typedef struct {
    int booking_id;
    char user[MAX_NAME_LEN];
    int show_id;        // Foreign key ke Showtime.show_id
    char selected_seats[MAX_SEATS_PER_BOOKING][5];
    int num_selected_seats;
    int total_price;
} Booking;

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
void print_header(const char* text);
void print_divider();
//void get_current_datetime_string(char *buffer, int buffer_size);

// --- File I/O untuk file .txt ---
int load_locations_from_txt(const char* filename, Location locations[], int max_items);
int load_cinemas_from_txt(const char* filename, Cinema cinemas[], int max_items);
int load_movies_from_txt(const char* filename, Movie movies[], int max_items);
int load_showtimes_from_txt(const char* filename, Showtime showtimes[], int max_items);

// --- Logika Aplikasi & Menu ---
void load_all_data(Location all_locations[], Cinema all_cinemas[], Movie all_movies[], Showtime all_showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count);
void main_menu(Location locations[], Cinema cinemas[], Movie movies[], Showtime showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count, Booking bookings[], int *booking_count);
int change_location(Location locations[], int location_count);
int find_cinemas_index(Cinema cinemas[], int cinema_count, int location_id);
void print_cinemas(Cinema cinemas[], int index);
void print_movies(Movie movies[], int movie_count, int upcoming, int current_film, int sort_ascending);
void film_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count, int location_id, Booking bookings[], int *booking_count);


void buy_ticket_flow(int selected_movie_id, int selected_cinema_id, Showtime showtimes[], int showtime_count, Booking bookings[], int *booking_count, Movie movies[], int movie_count);
int select_cinema(Showtime showtimes[], int showtime_count, Cinema cinemas[], int cinema_count, int selected_movie_id, int location_id);


void seat_selection_flow(Showtime *selected_showtime, int seat_number, char *seat_temp);
void display_seat_map(Showtime selected_showtime);


void merge_sort_movies(Movie arr[], int l, int r, int ascending);

// =================================================================================
// BAGIAN 4: IMPLEMENTASI FUNGSI UTAMA & MENU
// =================================================================================

int main() {
    Location all_locations[MAX_LOCATIONS];
    Cinema all_cinemas[MAX_CINEMAS];
    Movie all_movies[MAX_MOVIES];
    Showtime all_showtimes[MAX_SHOWTIMES];
    Booking all_bookings[MAX_BOOKINGS];

    int location_count = 0; 
    int cinema_count = 0; 
    int movie_count = 0;
    int showtime_count = 0;
    int booking_count = 0; 

    load_all_data(all_locations, all_cinemas, all_movies, all_showtimes, &location_count, &cinema_count, &movie_count, &showtime_count);
    main_menu(all_locations, all_cinemas, all_movies, all_showtimes, &location_count, &cinema_count, &movie_count, &showtime_count, all_bookings, &booking_count);

    return 0; 
}

void main_menu(Location locations[], Cinema cinemas[], Movie movies[], Showtime showtimes[], int *location_count, int *cinema_count, int *movie_count, int *showtime_count, Booking bookings[], int *booking_count) {
    int choice; 
    int current_location_id = 1;
    int current_location_index = 0;
    int current_cinemas_index; 
    do {
        print_header("CINEMA XXI - GROUP 2");
        printf("Location -> %s\n", locations[current_location_index].name);
        printf("[0] Exit\n");
        printf("[1] Bioskop\n");
        printf("[2] Film\n");
        printf("[3] m.food\n");
        printf("[4] Pesanan Saya\n");
        printf("[5] Change location\n");
        print_divider();
        printf("Pilihan Anda: ");

        if(scanf("%d", &choice) != 1) {
            choice = -1; 
            clean_input_buffer();
        }

        int temp;
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
                film_menu(movies, *movie_count, cinemas, *cinema_count, showtimes, *showtime_count, current_location_id, bookings, booking_count);
                break; 
            case 3: 
                clear_screen();
                break; 
            case 4: 
                clear_screen();
                break;
            case 5: 
                clear_screen();
                print_header("Pilih lokasi kamu");
                printf("Current Location -> %s\n", locations[current_location_index].name);

                if ((temp = change_location(locations, *location_count)) != -1) {
                    current_location_index = temp;
                    current_location_id = locations[current_location_index].location_id;
                    break;
                }
                break; 
            default: 
                printf("Input tidak valid!\n");
                press_enter_to_continue();
                break; 
        } 
    } while(choice != 0);
}

void film_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count, int location_id, Booking bookings[], int *booking_count) {
    int choice;
    int current_film = 1;
    int selected_cinema_id;
    int selected_movie_id = movies[current_film-1].movie_id;
    int sort_ascending = 1; 
    do {
        print_movies(movies, movie_count, 0, current_film, sort_ascending);
        print_header("FILM MAIN MENU");
        printf("[0] Back\n");
        printf("[1] Akan tayang\n");
        printf("[2] Pilih film\n");
        printf("[3] Beli tiket\n");
        printf("[4] Urutkan %s\n", sort_ascending ? "descending (Z-A)" : "ascending (A-Z)");
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
                print_movies(movies, movie_count, 1, 0, sort_ascending);
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
                selected_cinema_id = select_cinema(showtimes, showtime_count, cinemas, cinema_count, selected_movie_id, location_id);
                buy_ticket_flow(selected_movie_id, selected_cinema_id, showtimes, showtime_count, bookings, booking_count, movies, movie_count);
                break;
            case 4: 
                sort_ascending = !sort_ascending; 
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
    printf("Successfully load all data.\n(Location: %d, Cinema: %d, Movie: %d, Showtime: %d)\n", *location_count, *cinema_count, *movie_count, *showtime_count);
}

// =================================================================================
// BAGIAN 5: IMPLEMENTASI FILE I/O UNTUK .TXT
// =================================================================================
int load_locations_from_txt(const char* filename, Location locations[], int max_items)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0; 
    int count = 0; 
    while(count < max_items && fscanf(fp, "%d, %[^\n]\n", &locations[count].location_id, locations[count].name) == 2) { count++; }

    fclose(fp);
    return count; 
}

int load_cinemas_from_txt(const char* filename, Cinema cinemas[], int max_items)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0; 
    int count = 0; 
    while(count < max_items && fscanf(fp, "%d, %d, %[^\n]\n", &cinemas[count].cinema_id, &cinemas[count].location_id, cinemas[count].name) == 3) { count++; }

    fclose(fp);
    return count; 
}

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
    while(count < max_items && fscanf(fp, "%d,%d,%d,%[^,],%d", &showtimes[count].show_id, &showtimes[count].movie_id, &showtimes[count].cinema_id, showtimes[count].show_time, &showtimes[count].price) == 5) { count++; }

    fclose(fp);
    return count; 
}
 
// =================================================================================
// BAGIAN 6: IMPLEMENTASI LOGIKA APLIKASI
// =================================================================================

int select_cinema(Showtime showtimes[], int showtime_count, Cinema cinemas[], int cinema_count, int selected_movie_id, int location_id) {

    int k = 1;
    int choice;
    int cinemas_id[MAX_CINEMAS];


    print_header("Film tayang di");
    for(int i = 0; i < showtime_count; i++) {
        if (selected_movie_id == showtimes[i].movie_id) {
            for(int j = 0; j < cinema_count; j++) {
                if (cinemas[j].cinema_id == showtimes[i].cinema_id && cinemas[j].location_id == location_id) {
                    int flag = 0;
                    for (int p = 1; p <= k; p++)
                        if (cinemas[j].cinema_id == cinemas_id[p]) { flag = 1; break; }     
                    if (!flag) {
                        cinemas_id[k] = cinemas[j].cinema_id; 
                        printf("[%d] %s\n", k, cinemas[j].name);
                        k++;
                    }
                } 
            }
        }
    }
    print_divider();
    printf("Pilih bioskop: ");
    scanf("%d", &choice);
    clean_input_buffer();

    return cinemas_id[choice];
}

void buy_ticket_flow(int selected_movie_id, int selected_cinema_id, Showtime showtimes[], int showtime_count, Booking bookings[], int *booking_count, Movie movies[], int movie_count) {
    clear_screen();

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
    print_header("Jadwal tayang tersedia");
    for (int i = 0; i < show_count; i++) {
        int show_index = available_shows[i];
        printf("[%d] Waktu: %s - Harga: Rp%d\n", i + 1, showtimes[show_index].show_time, showtimes[show_index].price);
    }
    printf("[0] Batal\n");

    // 3. Validasi input user 
    int choice;
    print_divider();
    printf("Pilih jadwal: ");
    scanf("%d", &choice);

    if (choice == 0) return; 

    if (choice < 0 || choice > show_count) {
        printf("Pilihan tidak valid.\n");
        return;
    }

    Showtime *selected_showtime = &showtimes[available_shows[choice - 1]];

    print_header("How many seats needed?");
    printf("Cinema XXI - GROUP 2\n");
    printf("%s\n", selected_showtime->show_time);
    
    int jumlah_tiket;
    printf("Butuh berapa tiket? (number): ");
    clean_input_buffer();
    scanf("%d", &jumlah_tiket);

    press_enter_to_continue();


    // 4. proses pemilihan kursi & booking 
    Booking new_booking;
    new_booking.booking_id = *booking_count + 1;
    new_booking.show_id = selected_showtime->show_id;
    new_booking.num_selected_seats = jumlah_tiket;
    new_booking.total_price = jumlah_tiket * selected_showtime->price;
    // copy user name to booking
    //strcpy(new_booking.user, "user_name"); 

    for (int i = 0; i < jumlah_tiket; i++) {
        char seat_temp[5]; 
        seat_selection_flow(selected_showtime, i + 1, seat_temp);
        strcpy(new_booking.selected_seats[i], seat_temp);
    }

    // 5. Konfirmasi pesanan
    clear_screen();
    print_header("KONFIRMASI PEMESANAN");
    printf("ID Pemesanan: %d\n", new_booking.booking_id);

    int selected_movie_idx;
    for(int i = 0; i < movie_count; i++) {
        if(movies[i].movie_id == selected_movie_id) {
            selected_movie_idx = i; 
            break;
        }
    }
    printf("Film: %s\n", movies[selected_movie_idx].title); 
    
    printf("Jadwal: %s\n", selected_showtime->show_time);
    printf("Jumlah Tiket: %d\n", new_booking.num_selected_seats);
    printf("Kursi yang Dipilih: ");
    for(int i = 0; i < new_booking.num_selected_seats; i++){
        printf("%s ", new_booking.selected_seats[i]);
    }
    printf("\n");
    printf("Total Harga: Rp%d\n", new_booking.total_price);
    print_divider();
    printf("Konfirmasi pemesanan? (y/n): ");
    
    char confirmation;
    clean_input_buffer();
    scanf(" %c", &confirmation);

    if (tolower(confirmation) == 'y') {
        bookings[*booking_count] = new_booking;
        (*booking_count)++; 
        printf("\nSelamat! Pemesanan berhasil dibuat.\n");
    } else {
        printf("\nPemesanan dibatalkan.\n");
    }
    press_enter_to_continue();
}

void seat_selection_flow(Showtime *selected_showtime, int seat_number, char *seat_temp) {

    char rowChar; 
    int row, col; 
    bool validSeat = false;
    
    while(!validSeat) {
        display_seat_map(*selected_showtime);   
        printf("Select seat %d (ex. A5): ", seat_number);
        scanf(" %c %d", &rowChar, &col);
        
        row = toupper(rowChar) - 'A';
        col--;
        
        // validate user select
        if(row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
            printf("Invalid seat! Please select between A-%c and 1-%d.\n", 
                  'A' + MAX_ROWS - 1, MAX_COLS);
            press_enter_to_continue();
        } else if(selected_showtime->seats[row][col] == 1) {
            printf("Seat %c%d is already taken! Please choose another seat.\n", 
                  'A' + row, col + 1);
            press_enter_to_continue();
        } else { validSeat = true; }
    }
    
    // mark seat as taken
    selected_showtime->seats[row][col] = 1;
    sprintf(seat_temp, "%c%d", 'A' + row, col + 1);
}


void display_seat_map(Showtime selected_showtime) {

    clear_screen();
    printf("============================== PETA KURSI =============================\n\n");
    for(int row = 0; row < MAX_ROWS; row++) {
        printf("%c    ", 'A' + row);
        for(int col = 0; col < MAX_COLS; col++) {
            if(selected_showtime.seats[row][col] == 0) {
                printf("[_] ");
            } else {
                printf("[X] ");
            }
        }
        printf("\n");
    }
    printf("\n");

    printf("\n\n");
    printf("-------------------------------------------------------------------------\n");
    printf("                             LAYAR BIOSKOP\n");
    printf("-----------------------------------------------------------------------\n\n");
    printf("Legenda: [X] Dipesan  [_] Tersedia\n\n");
}



int change_location(Location locations[], int location_count) {

    int index; 

    for(int i = 0; i < location_count; i++) {
        printf("[%d] %s\n", i+1, locations[i].name);
    }
    printf("[0] Batal\n");

    printf("Select location (number): ");
    scanf("%d", &index);
    clean_input_buffer();

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

    print_header("Bioskop");

    int j = 1;
    for(int i = index; cinemas[i].location_id == cinemas[index].location_id; i++) {
        printf("[%2d] %s\n", j, cinemas[i].name);
        j++;
    }

}

void print_movies(Movie movies[], int movie_count, int upcoming, int current_film, int ascending) {

    print_header("Film");
    
    if (movie_count <= 0) return;
    /*
    if (upcoming)
        printf("Akan tayang\n\n");
    else 
        printf("Lagi tayang\n\n");

    for(int i = 0; i < movie_count; i++) {
        if (upcoming && movies[i].upcoming)
            printf("%-3s [%2d] %-20s\n", "   ", (i-20)+1, movies[i].title);
        else if (!upcoming && !movies[i].upcoming) {
            if (current_film == i+1) printf("%-3s [%2d] %-20s\n", "-->", i+1, movies[i].title);
            else 
                printf("%-3s [%2d] %-20s\n", "   ", i+1, movies[i].title);
        }
    }
    */

    int temp_count = 0;
    for (int i = 0; i < movie_count; i++) {
        if ((upcoming && movies[i].upcoming) || (!upcoming && !movies[i].upcoming))
            temp_count++;
    }

    if (temp_count == 0) {
        printf("Tidak ada film untuk ditampilkan.\n");
        return;
    }

    Movie temp_movies[temp_count];
    int idx = 0;
    for (int i = 0; i < movie_count; i++) {
        if ((upcoming && movies[i].upcoming) || (!upcoming && !movies[i].upcoming)) {
            temp_movies[idx++] = movies[i];
        }
    }

    merge_sort_movies(temp_movies, 0, temp_count - 1, ascending);

    if (upcoming)
        printf("== Akan Tayang ==\n");
    else
        printf("== Lagi Tayang ==\n");

    for (int i = 0; i < temp_count; i++) {
        if (!upcoming && current_film == i + 1) printf("-->");
        else printf("   ");
        printf("%d. %s\n", i + 1, temp_movies[i].title);
    }
}



void merge(Movie arr[], int l, int m, int r, int ascending) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Movie L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        int cmp = strcmp(L[i].title, R[j].title);
        if ((ascending && cmp <= 0) || (!ascending && cmp > 0)) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort_movies(Movie arr[], int l, int r, int ascending) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_movies(arr, l, m, ascending);
        merge_sort_movies(arr, m + 1, r, ascending);
        merge(arr, l, m, r, ascending);
    }
}

// =================================================================================
// BAGIAN 7: UTILITAS INTERNAL & INISIALISASI DATA
// =================================================================================

void print_header(const char* text) {
    printf("====================================================\n");
    printf("\t\t%s\t\t\n", text);
    printf("====================================================\n");
}

void print_divider() {
    printf("----------------------------------------------------\n");
}

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


