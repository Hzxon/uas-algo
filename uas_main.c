#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>
#include <stdbool.h>
#include <strings.h> 


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
#define MAX_PASSWORD_LEN 20


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
    int show_id;
    char selected_seats[MAX_SEATS_PER_BOOKING][5];
    int num_selected_seats;
    int total_price;
} Booking;

typedef struct BookingNode {
    Booking booking_data;
    struct BookingNode *next;
} BookingNode;

typedef struct UserBookingsNode {
    char user[MAX_NAME_LEN];
    BookingNode *bookings_head; 
    struct UserBookingsNode *left, *right;
} UserBookingsNode;

typedef struct MaxHeap {
    Booking *array;
    int size;
    int capacity;
} MaxHeap;

// =================================================================================
// BAGIAN 2: DEKLARASI FUNGSI (PROTOTYPES)
// =================================================================================

// --- Utilitas ---
void clear_screen();
void press_enter_to_continue();
void clean_input_buffer();
void print_header(const char* text);
void print_divider();

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
void print_movies(Movie movies[], int movie_count, int upcoming, int current_film, int sort_ascending, Movie sorted_movies[]);
void film_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count, int location_id, Booking bookings[], int *booking_count, UserBookingsNode** bookings_root, MaxHeap *recent_bookings_heap, char username[], bool login);
void buy_ticket_flow(int selected_movie_id, int selected_cinema_id, Showtime showtimes[], int showtime_count, Booking bookings[], int *booking_count, Movie movies[], int movie_count, UserBookingsNode** bookings_root, MaxHeap *recent_bookings_heap, char username[], bool login);
int select_cinema(Showtime showtimes[], int showtime_count, Cinema cinemas[], int cinema_count, int selected_movie_id, int location_id);
void seat_selection_flow(Showtime *selected_showtime, int seat_number, char *seat_temp);
void display_seat_map(Showtime selected_showtime);
void merge_sort_movies(Movie arr[], int l, int r, int ascending);
void merge_sort_cinemas(Cinema arr[], int l, int r);

// --- BST ---
UserBookingsNode* addUserBooking(UserBookingsNode* root, Booking new_booking);
void view_my_bookings(UserBookingsNode** bookings_root_ptr, Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count);
UserBookingsNode* findMinNode(UserBookingsNode* node);
void deleteUserBooking(UserBookingsNode* userNode, int booking_index_to_delete, Showtime showtimes[], int showtime_count);
UserBookingsNode* deleteUserNode(UserBookingsNode* root, const char* user_key);
void freeUserBookingsTree(UserBookingsNode* root);

// --- Heap ---
MaxHeap* createHeap(int capacity);
void insertIntoHeap(MaxHeap* heap, Booking new_booking);
void viewRecentBookings(MaxHeap* heap, Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count);
void freeHeap(MaxHeap* heap);
void deleteFromHeap(MaxHeap* heap, int booking_id_to_delete);


// --- Binary Search ---
void search_menu(Movie sorted_movies_ascending[], int movie_count, Cinema sorted_cinemas_ascending[], int cinema_count, Location locations[], int location_count);
int binary_search_movie(Movie movies[], int count, const char* target);
int binary_search_cinema(Cinema cinemas[], int count, const char* target);

// =================================================================================
// BAGIAN 3: FUNGSI UTAMA & MENU
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

    UserBookingsNode *bookings_root = NULL;
    MaxHeap* recent_bookings_heap = createHeap(MAX_BOOKINGS);


    Movie sorted_movies_ascending[MAX_MOVIES];
    memcpy(sorted_movies_ascending, movies, sizeof(Movie) * (*movie_count));
    merge_sort_movies(sorted_movies_ascending, 0, *movie_count-1, 1);

    Cinema sorted_cinemas_ascending[MAX_CINEMAS];
    memcpy(sorted_cinemas_ascending, cinemas, sizeof(Cinema) * (*cinema_count));
    merge_sort_cinemas(sorted_cinemas_ascending, 0, *cinema_count-1);

    char username[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    bool login = false; 

    do {
        print_header("CINEMA XXI - GROUP 2");
        printf("Location -> %s\n", locations[current_location_index].name);
        printf("[0] Cari film atau bioskop\n");
        printf("[1] Bioskop\n");
        printf("[2] Film\n");
        printf("[3] Pesanan Saya\n");
        printf("[4] Change location\n");
        printf("[5] Pesanan saya\n");
        printf("[6] History Pesanan\n");
        printf("[7] %s\n", login ? "Logged" : "Login");
        printf("[8] Exit\n");
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
                search_menu(sorted_movies_ascending, *movie_count, sorted_cinemas_ascending, *cinema_count, locations, *location_count);
                break;

            case 1: 
                clear_screen();
                current_cinemas_index = find_cinemas_index(cinemas, *cinema_count, current_location_id); 
                print_cinemas(cinemas, current_cinemas_index);
                break; 
            case 2:
                clear_screen();
                film_menu(movies, *movie_count, cinemas, *cinema_count, showtimes, *showtime_count, current_location_id, bookings, booking_count, &bookings_root, recent_bookings_heap, username, login);
                break; 
            case 3: 
                clear_screen();
                break;
            case 4: 
                clear_screen();
                print_header("Pilih lokasi kamu");
                printf("Current Location -> %s\n", locations[current_location_index].name);

                if ((temp = change_location(locations, *location_count)) != -1) {
                    current_location_index = temp;
                    current_location_id = locations[current_location_index].location_id;
                    break;
                }
                break; 
            case 5: 
                clear_screen();
                view_my_bookings(&bookings_root, movies, *movie_count, cinemas, *cinema_count, showtimes, *showtime_count);
                break; 
            case 6: 
                clear_screen();
                viewRecentBookings(recent_bookings_heap, movies, *movie_count, cinemas, *cinema_count, showtimes, *showtime_count);
                break;
            case 7: 
                if (!login) {
                    printf("Username: "); 
                    scanf("%s", username);
                    clean_input_buffer();

                    printf("Password: ");
                    scanf("%s", password);
                    clean_input_buffer();

                    login = true; 
                    clear_screen();
                } else {
                    printf("Already logged!\n");
                } 
                break; 
            case 8:
                clear_screen();
                printf("Thankyou, see you next time!\n");
                break; 
            default: 
                printf("Input tidak valid!\n");
                press_enter_to_continue();
                break; 
        } 
    } while(choice != 8);

    freeUserBookingsTree(bookings_root);
    freeHeap(recent_bookings_heap);
}

void film_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count, int location_id, Booking bookings[], int *booking_count, UserBookingsNode **bookings_root, MaxHeap *recent_bookings_heap, char username[], bool login) {
    int choice;
    Movie sorted_movies[MAX_MOVIES];
    memcpy(&sorted_movies, movies, sizeof(Movie));
    int current_film = 9;
    int selected_cinema_id;
    int selected_movie_id = 1;
    int sort_ascending = 1;

    do {
        print_movies(movies, movie_count, 0, current_film, sort_ascending, sorted_movies);
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
                print_movies(movies, movie_count, 1, 0, sort_ascending, sorted_movies);
                press_enter_to_continue();
                clear_screen();
                break; 
            case 2:
                printf("Masukkan nomor film: ");
                scanf("%d", &current_film);
                clean_input_buffer();
                clear_screen();
                selected_movie_id = sorted_movies[current_film-1].movie_id;
                break;
            case 3:
                clear_screen();
                selected_cinema_id = select_cinema(showtimes, showtime_count, cinemas, cinema_count, selected_movie_id, location_id);
                if (selected_cinema_id == -1) { clear_screen(); break; } 
                buy_ticket_flow(selected_movie_id, selected_cinema_id, showtimes, showtime_count, bookings, booking_count, movies, movie_count, bookings_root, recent_bookings_heap, username, login);
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
// BAGIAN 4: IMPLEMENTASI Binary Search untuk FITUR SEARCH
// =================================================================================

void search_menu(Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Location locations[], int location_count) {
    int choice;
    print_header("MENU PENCARIAN");
    printf("[1] Cari Judul Film\n");
    printf("[2] Cari Nama Bioskop\n");
    printf("[0] Kembali\n");
    print_divider();
    printf("Pilihan Anda: ");

    if (scanf("%d", &choice) != 1) {
        choice = -1;
        clean_input_buffer();
    }
    
    char search_term[MAX_NAME_LEN];
    int result_index = -1;

    switch(choice) {
        case 1:
            printf("Masukkan judul film yang dicari (harus sama persis): ");
            clean_input_buffer();
            fgets(search_term, MAX_NAME_LEN, stdin);
            search_term[strcspn(search_term, "\n")] = 0;
            
            result_index = binary_search_movie(movies, movie_count, search_term);
            
            clear_screen();
            print_header("Hasil Pencarian Film");
            if (result_index != -1) {
                printf("Film Ditemukan!\n");
                print_divider();
                printf("Judul    : %s\n", movies[result_index].title);
                printf("Genre    : %s\n", movies[result_index].genre);
                printf("Durasi   : %d menit\n", movies[result_index].duration_minutes);
                printf("Rating   : %s\n", movies[result_index].age_rating);
            } else {
                printf("Film dengan judul '%s' tidak ditemukan.\n", search_term);
            }
            press_enter_to_continue();
            break;
        case 2:
            printf("Masukkan nama bioskop yang dicari (harus sama persis): ");
            clean_input_buffer();
            fgets(search_term, MAX_NAME_LEN, stdin);
            search_term[strcspn(search_term, "\n")] = 0;

            result_index = binary_search_cinema(cinemas, cinema_count, search_term);

            clear_screen();
            print_header("Hasil Pencarian Bioskop");
            if (result_index != -1) {
                printf("Bioskop Ditemukan!\n");
                print_divider();
                printf("Nama     : %s\n", cinemas[result_index].name);

                char* loc_name = "N/A";
                for(int i = 0; i < location_count; i++){
                    if(locations[i].location_id == cinemas[result_index].location_id){
                        loc_name = locations[i].name;
                        break;
                    }
                }
                printf("Lokasi   : %s\n", loc_name);
            } else {
                printf("Bioskop dengan nama '%s' tidak ditemukan.\n", search_term);
            }
            press_enter_to_continue();
            break;
        case 0:
            break;
        default:
            printf("Pilihan tidak valid.\n");
            press_enter_to_continue();
            break;
    }
}

int binary_search_movie(Movie movies[], int count, const char* target) {
    int low = 0;
    int high = count - 1;
    
    while(low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcasecmp(movies[mid].title, target);

        if (cmp == 0) {
            return mid; 
        }
        if (cmp < 0) {
            low = mid + 1; 
        } else {
            high = mid - 1; 
        }
    }
    return -1; 
}

int binary_search_cinema(Cinema cinemas[], int count, const char* target) {
    int low = 0;
    int high = count - 1;
    
    while(low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcasecmp(cinemas[mid].name, target);

        if (cmp == 0) {
            return mid;
        }
        if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// =================================================================================
// BAGIAN 4: IMPLEMENTASI FILE I/O UNTUK .TXT
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
        sscanf(line_buffer, "%d, %[^,], %[^,], %d, %[^,], %d",
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
    while(count < max_items && fscanf(fp, "%d, %d, %d, %[^,], %d", &showtimes[count].show_id, &showtimes[count].movie_id, &showtimes[count].cinema_id, showtimes[count].show_time, &showtimes[count].price) == 5) { count++; }

    fclose(fp);
    return count; 
}
 
// =================================================================================
// BAGIAN 5: IMPLEMENTASI LOGIKA APLIKASI
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
    if (k == 1) {
        printf("Tidak ada bioskop di lokasi ini yang menayangkan film tersebut.\n");
        press_enter_to_continue();
        return -1;
    }

    print_divider();
    printf("Pilih bioskop: ");
    scanf("%d", &choice);
    clean_input_buffer();

    if(choice > 0 && choice < k) return cinemas_id[choice];
        return -1;
}

void buy_ticket_flow(int selected_movie_id, int selected_cinema_id, Showtime showtimes[], int showtime_count, Booking bookings[], int *booking_count, Movie movies[], int movie_count, UserBookingsNode** bookings_root, MaxHeap *recent_bookings_heap, char username[], bool login) {
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
    printf("Butuh berapa tiket? (1-%d): ", MAX_SEATS_PER_BOOKING);
    scanf("%d", &jumlah_tiket);
    if (jumlah_tiket <= 0 || jumlah_tiket > MAX_SEATS_PER_BOOKING) {
        printf("Jumlah tiket tidak valid.\n");
        press_enter_to_continue();
        return;
    }


    // 4. proses pemilihan kursi & booking 
    Booking new_booking;
    new_booking.booking_id = *booking_count + 1;
    new_booking.show_id = selected_showtime->show_id;
    new_booking.num_selected_seats = jumlah_tiket;
    new_booking.total_price = jumlah_tiket * selected_showtime->price;

    if (login) {
        strcpy(new_booking.user, username); 
    } else {
        printf("Silahkan login terlebih dahulu!\n");
        press_enter_to_continue();
        clear_screen();
        return;
    }

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
    scanf(" %c", &confirmation);

    if (tolower(confirmation) == 'y') {

        bookings[*booking_count] = new_booking;
        (*booking_count)++; 
        *bookings_root = addUserBooking(*bookings_root, new_booking);
        insertIntoHeap(recent_bookings_heap, new_booking);
        printf("\nSelamat! Pemesanan berhasil dibuat.\n");

    } else {

        for(int i=0; i<new_booking.num_selected_seats; i++){
            int row = toupper(new_booking.selected_seats[i][0]) - 'A';
            int col = atoi(&new_booking.selected_seats[i][1]) - 1;
            if(row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLS){
                selected_showtime->seats[row][col] = 0;
            }
        }
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

void print_movies(Movie movies[], int movie_count, int upcoming, int current_film, int ascending, Movie sorted_movies[]) {

    print_header("Film");
    
    if (movie_count <= 0) return;

    int temp_count = 0;
    for (int i = 0; i < movie_count; i++) {
        if ((upcoming && movies[i].upcoming) || (!upcoming && !movies[i].upcoming))
            temp_count++;
    }

    if (temp_count == 0) {
        printf("Tidak ada film untuk ditampilkan.\n");
        return;
    }

    int idx = 0;
    for (int i = 0; i < movie_count; i++) {
        if ((upcoming && movies[i].upcoming) || (!upcoming && !movies[i].upcoming)) {
            sorted_movies[idx++] = movies[i];
        }
    }

    merge_sort_movies(sorted_movies, 0, temp_count - 1, ascending);

    if (upcoming)
        printf("Akan tayang\n");
    else
        printf("Lagi tayang\n");

    for (int i = 0; i < temp_count; i++) {
        if (!upcoming && current_film == i + 1) printf("%-3s [%2d] %-20s\n", "-->", i+1, movies[i].title);
        else printf("%-3s [%2d] %-20s\n", "   ", i+1, movies[i].title);

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
        int cmp = strcasecmp(L[i].title, R[j].title);
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

void merge_cinemas(Cinema arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    Cinema L[n1], R[n2];
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (strcasecmp(L[i].name, R[j].name) <= 0) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort_cinemas(Cinema arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort_cinemas(arr, l, m);
        merge_sort_cinemas(arr, m + 1, r);
        merge_cinemas(arr, l, m, r);
    }
}

// =================================================================================
// BAGIAN 6: Binary Search Tree untuk PESANAN SAYA 
// =================================================================================

UserBookingsNode* addUserBooking(UserBookingsNode* root, Booking new_booking) {
    if (root == NULL) {
        root = (UserBookingsNode*)malloc(sizeof(UserBookingsNode));
        strcpy(root->user, new_booking.user);
        root->left = root->right = NULL;
        root->bookings_head = NULL;
    }

    int cmp = strcmp(new_booking.user, root->user);

    if (cmp == 0) {
        BookingNode* new_booking_node = (BookingNode*)malloc(sizeof(BookingNode));
        new_booking_node->booking_data = new_booking;
        new_booking_node->next = root->bookings_head;
        root->bookings_head = new_booking_node;
    } else if (cmp < 0) {
        root->left = addUserBooking(root->left, new_booking);
    } else {
        root->right = addUserBooking(root->right, new_booking);
    }
    return root;
}

void deleteUserBooking(UserBookingsNode* userNode, int booking_index_to_delete, Showtime showtimes[], int showtime_count) {
    if (userNode == NULL || userNode->bookings_head == NULL) return;

    BookingNode* current = userNode->bookings_head;
    BookingNode* prev = NULL;
    int current_index = 1;

    while (current != NULL && current_index < booking_index_to_delete) {
        prev = current;
        current = current->next;
        current_index++;
    }

    if (current == NULL) {
        printf("Nomor pesanan tidak ditemukan.\n");
        return;
    }

    Booking deleted_booking = current->booking_data;

    if (prev == NULL) {
        userNode->bookings_head = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);

    for (int i = 0; i < showtime_count; i++) {
        if (showtimes[i].show_id == deleted_booking.show_id) {
            for (int j = 0; j < deleted_booking.num_selected_seats; j++) {
                int row = toupper(deleted_booking.selected_seats[j][0]) - 'A';
                int col = atoi(&deleted_booking.selected_seats[j][1]) - 1;
                if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLS) {
                    showtimes[i].seats[row][col] = 0;
                }
            }
            break;
        }
    }
    printf("Pesanan berhasil dibatalkan dan kursi telah tersedia kembali.\n");
}

UserBookingsNode* findMinNode(UserBookingsNode* node) {
    UserBookingsNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

UserBookingsNode* deleteUserNode(UserBookingsNode* root, const char* user_key) {
    if (root == NULL) return root;

    int cmp = strcmp(user_key, root->user);

    if (cmp < 0) {
        root->left = deleteUserNode(root->left, user_key);
    } else if (cmp > 0) {
        root->right = deleteUserNode(root->right, user_key);
    } else { 
        // if node punya <= 1 child 
        if (root->left == NULL) {
            UserBookingsNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            UserBookingsNode* temp = root->left;
            free(root);
            return temp;
        }

        // if node punya 2 child
        UserBookingsNode* temp = findMinNode(root->right); 
        strcpy(root->user, temp->user); 
        root->bookings_head = temp->bookings_head; 
        
        root->right = deleteUserNode(root->right, temp->user);
    }
    return root;
}


void view_my_bookings(UserBookingsNode** bookings_root_ptr, Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count) {
    print_header("PESANAN SAYA");
    if (*bookings_root_ptr == NULL) {
        printf("Belum ada pesanan yang dibuat dalam sesi ini.\n");
        press_enter_to_continue();
        return;
    }
    
    char search_user[MAX_NAME_LEN];
    printf("Masukkan nama Anda untuk mencari pesanan: ");
    clean_input_buffer();
    fgets(search_user, MAX_NAME_LEN, stdin);
    search_user[strcspn(search_user, "\n")] = 0;

    UserBookingsNode* userNode = *bookings_root_ptr;
    while (userNode != NULL) {
        int cmp = strcmp(search_user, userNode->user);
        if (cmp == 0) break;
        if (cmp < 0) userNode = userNode->left;
        else userNode = userNode->right;
    }
    
    bool still_in_menu;
    if (userNode == NULL || userNode->bookings_head == NULL) {
        printf("\nTidak ada pesanan yang ditemukan untuk pengguna '%s'.\n", search_user);
    } else {
        still_in_menu = true;
        while(still_in_menu) {
            clear_screen();
            printf("\nMenampilkan pesanan untuk: %s\n", userNode->user);
            BookingNode* booking_ptr = userNode->bookings_head;
            int booking_num = 1;

            if (booking_ptr == NULL) {
                printf("Anda tidak memiliki pesanan aktif.\n");
                break;
            }

            while (booking_ptr != NULL) {
                print_divider();
                Booking b = booking_ptr->booking_data;
                char* title = "N/A"; char* cinema_name = "N/A"; char* show_time = "N/A";
                for(int i=0; i<showtime_count; i++){
                    if(showtimes[i].show_id == b.show_id){
                        show_time = showtimes[i].show_time;
                        for(int j=0; j<movie_count; j++) if(movies[j].movie_id == showtimes[i].movie_id) title = movies[j].title;
                        for(int j=0; j<cinema_count; j++) if(cinemas[j].cinema_id == showtimes[i].cinema_id) cinema_name = cinemas[j].name;
                        break;
                    }
                }
                printf("[%d] Film    : %s\n", booking_num, title);
                printf("    Bioskop : %s\n", cinema_name);
                printf("    Jadwal  : %s\n", show_time);
                printf("    Kursi   : ");
                for(int i=0; i<b.num_selected_seats; i++) printf("%s ", b.selected_seats[i]);
                printf("\n    Total   : Rp%d\n", b.total_price);
                
                booking_ptr = booking_ptr->next;
                booking_num++;
            }
            
            print_divider();
            printf("\n[1] Batalkan Pesanan\n");
            printf("[0] Kembali\n");
            printf("Pilihan Anda: ");
            int choice;
            scanf("%d", &choice);
            if (choice == 1) {
                printf("Masukkan nomor pesanan yang ingin dibatalkan: ");
                int booking_to_delete;
                scanf("%d", &booking_to_delete);
                if(booking_to_delete > 0 && booking_to_delete < booking_num){
                    deleteUserBooking(userNode, booking_to_delete, showtimes, showtime_count);
                    if (userNode->bookings_head == NULL) {
                        *bookings_root_ptr = deleteUserNode(*bookings_root_ptr, userNode->user);
                        still_in_menu = false; 
                    }
                } else {
                    printf("Nomor pesanan tidak valid.\n");
                }
                press_enter_to_continue();
            } else {
                still_in_menu = false;
            }
        }
    }
    if(!still_in_menu) return; 
    press_enter_to_continue();
}

void freeUserBookingsTree(UserBookingsNode* root) {
    if (root == NULL) return;
    freeUserBookingsTree(root->left);
    freeUserBookingsTree(root->right);

    BookingNode* current = root->bookings_head;
    while(current != NULL){
        BookingNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(root);
}

// =================================================================================
// BAGIAN 7: HEAP untuk Riwayat Pesanan
// =================================================================================

MaxHeap* createHeap(int capacity) {
    MaxHeap* heap = (MaxHeap*)malloc(sizeof(MaxHeap));
    heap->array = (Booking*)malloc(capacity * sizeof(Booking));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(Booking* a, Booking* b) {
    Booking temp = *a;
    *a = *b;
    *b = temp;
}

void siftUp(MaxHeap* heap, int index) {
    int parent = (index - 1) / 2;
    if (parent >= 0 && heap->array[parent].booking_id < heap->array[index].booking_id) {
        swap(&heap->array[parent], &heap->array[index]);
        siftUp(heap, parent);
    }
}

void insertIntoHeap(MaxHeap* heap, Booking new_booking) {
    if (heap->size == heap->capacity) {
        printf("Heap penuh, pesanan tidak bisa ditambahkan ke riwayat terbaru.\n");
        return;
    }
    heap->size++;
    int index = heap->size - 1;
    heap->array[index] = new_booking;
    siftUp(heap, index);
}

void siftDown(Booking arr[], int size, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && arr[left].booking_id > arr[largest].booking_id)
        largest = left;

    if (right < size && arr[right].booking_id > arr[largest].booking_id)
        largest = right;

    if (largest != index) {
        swap(&arr[index], &arr[largest]);
        siftDown(arr, size, largest);
    }
}

void viewRecentBookings(MaxHeap* heap, Movie movies[], int movie_count, Cinema cinemas[], int cinema_count, Showtime showtimes[], int showtime_count) {

    int choice; 
    do {
        print_header("RIWAYAT PESANAN dari TERBARU");
        if (heap->size == 0) {
            printf("Belum ada pesanan yang dibuat dalam sesi ini.\n");
            press_enter_to_continue();
            return;
        }

        Booking temp_array[heap->size];
        memcpy(temp_array, heap->array, heap->size * sizeof(Booking));
        int temp_size = heap->size;

        for (int i = 0; i < heap->size; i++) {
            Booking b = temp_array[0];

            print_divider();
            char* title = "N/A"; char* cinema_name = "N/A"; char* show_time = "N/A";
            for(int k = 0; k < showtime_count; k++){
                if(showtimes[k].show_id == b.show_id){
                    show_time = showtimes[k].show_time;
                    for(int j = 0; j<movie_count; j++) 
                        if(movies[j].movie_id == showtimes[k].movie_id) title = movies[j].title;
                    for(int j = 0; j<cinema_count; j++) 
                        if(cinemas[j].cinema_id == showtimes[k].cinema_id) cinema_name = cinemas[j].name;
                    break;
                }
            }
            printf("Pengguna: %s (ID Pesanan: %d)\n", b.user, b.booking_id);
            printf("Film    : %s\n", title);
            printf("Bioskop : %s @ %s\n", cinema_name, show_time);
            
            if (i < temp_size - 1) {
                temp_array[0] = temp_array[temp_size - 1];
                temp_size--;
                siftDown(temp_array, temp_size, 0);
            }
        }
       
        print_divider();
        printf("\n[1] Hapus riwayat terbaru\n");
        printf("[0] Kembali\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Masukkan nomor pesanan yang ingin dibatalkan: ");
            int booking_to_delete;
            clean_input_buffer();
            scanf("%d", &booking_to_delete);
            clean_input_buffer();

            if(booking_to_delete > 0 && booking_to_delete <= heap->size){
                deleteFromHeap(heap, booking_to_delete);
                
            } else {
                printf("Nomor pesanan tidak valid.\n");
            }
            press_enter_to_continue();
        } 

    } while(choice != 0);
}

void deleteFromHeap(MaxHeap* heap, int booking_id_to_delete) {
    if (heap->size == 0) return;

    int index_to_delete = -1;
    for (int i = 0; i < heap->size; i++) {
        if (heap->array[i].booking_id == booking_id_to_delete) {
            index_to_delete = i;
            break;
        }
    }
    
    if (index_to_delete == -1) return; 

    heap->array[index_to_delete] = heap->array[heap->size - 1];
    heap->size--;

    int parent = (index_to_delete - 1) / 2;
    if (index_to_delete > 0 && heap->array[parent].booking_id < heap->array[index_to_delete].booking_id) {
        siftUp(heap, index_to_delete);
    } else {
        siftDown(heap->array, heap->size, index_to_delete);
    }
}

void freeHeap(MaxHeap* heap) {
    if (heap == NULL) return;
    free(heap->array);
    free(heap);
}

// =================================================================================
// BAGIAN 9: UTILITAS INTERNAL
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


