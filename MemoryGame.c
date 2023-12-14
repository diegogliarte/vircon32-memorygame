#include "video.h"
#include "input.h"
#include "time.h"
#include "misc.h"
#include "input.h"
#include "math.h"
#include "audio.h"


#include "MainTexture.h"

#define CardSize 37
#define TotalCards 24
#define BoardColumns 6
#define BoardRows 4

int CardFlip = 0;
int SoundWin = 1;
int Correct = 2;
int Fail = 3;

int[TotalCards]
hidden_cards;
int[TotalCards]
visible_cards;
int position_x = 0;
int position_y = 0;
int previous_index = -1;
int second_previous_index = -1;
int wait_frames = 0;

void initialize_cards();

void shuffle_cards();

void draw_board(int margin_y, int spacing, int margin_x);

void draw_card(int margin_y, int spacing, int margin_x, int i, int j);

void draw_highlight(int margin_y, int spacing, int margin_x);

void handle_input();

void reveal_card();

void check_win();

void draw_score();

void main(void) {
    srand(get_time());
    select_gamepad(0);

    DefineRegions_MainTexture();

    initialize_cards();


    int margin_y = 64;
    int spacing = (screen_height - margin_y * 2 - CardSize * BoardRows) / (BoardRows - 1);
    int margin_x = (screen_width - CardSize * BoardColumns - spacing * (BoardColumns - 1)) / 2;

    while (true) {
        handle_input();
        draw_board(margin_y, spacing, margin_x);

        if (second_previous_index != -1 && wait_frames <= 0) {
            visible_cards[previous_index] = RegionCardBack;
            visible_cards[second_previous_index] = RegionCardBack;
            previous_index = -1;
            second_previous_index = -1;
        }

        check_win();

        wait_frames--;
        end_frame();
    }
}

void draw_board(int margin_y, int spacing, int margin_x) {
    clear_screen(color_black);

    for (int i = 0; i < BoardColumns; i++) {
        for (int j = 0; j < BoardRows; j++) {
            draw_card(margin_y, spacing, margin_x, i, j);
            if (position_x == i && position_y == j) {
                draw_highlight(margin_y, spacing, margin_x);
            }
        }
    }

    draw_score();
}

void draw_card(int margin_y, int spacing, int margin_x, int i, int j) {
    int card_index = visible_cards[i + j * BoardColumns];

    select_region(card_index);

    int pos_x = margin_x + (CardSize + spacing) * i;
    int pos_y = margin_y + (CardSize + spacing) * j;

    draw_region_at(pos_x, pos_y);
}

void draw_highlight(int margin_y, int spacing, int margin_x) {
    select_region(RegionHighlight);

    int pos_x = margin_x + (CardSize + spacing) * position_x - 1;
    int pos_y = margin_y + (CardSize + spacing) * position_y - 1;

    draw_region_at(pos_x, pos_y);
}

void draw_score() {
}

void initialize_cards() {
    for (int i = 0; i < TotalCards / 2; i++) {
        hidden_cards[i] = FirstRegionCards + i;
        hidden_cards[i + TotalCards / 2] = FirstRegionCards + i;
    }

    for (int i = 0; i < TotalCards; i++) {
        visible_cards[i] = RegionCardBack;
    }

    shuffle_cards();
}

// Fisher-Yates shuffle
void shuffle_cards() {
    for (int i = 0; i < TotalCards; i++) {
        int j = rand() % (i + 1);

        int temp = hidden_cards[j];
        hidden_cards[j] = hidden_cards[i];
        hidden_cards[i] = temp;
    }
}

void handle_input() {

    if (gamepad_up() == 1) {
        position_y = max(0, position_y - 1);
    }

    if (gamepad_down() == 1) {
        position_y = min(BoardRows - 1, position_y + 1);
    }

    if (gamepad_left() == 1) {
        position_x = max(0, position_x - 1);
    }

    if (gamepad_right() == 1) {
        position_x = min(BoardColumns - 1, position_x + 1);
    }

    if (gamepad_button_a() == 1) {
        if (second_previous_index == -1) {
            reveal_card();
        }
    }
}

void reveal_card() {
    int current_index = position_x + position_y * BoardColumns;
    if (current_index == previous_index) {
        return;
    }

    play_sound(CardFlip);

    int current_card = hidden_cards[current_index];
    visible_cards[current_index] = current_card;

    if (previous_index == -1) {
        previous_index = current_index;
    } else {
        if (hidden_cards[previous_index] == current_card) {
            visible_cards[current_index] = current_card;
            previous_index = -1;
            play_sound(Correct);

        } else {
            second_previous_index = current_index;
            wait_frames = 60;
            play_sound(Fail);

        }
    }
}

void check_win() {
    bool win = true;
    for (int i = 0; i < TotalCards; i++) {
        if (visible_cards[i] == RegionCardBack) {
            win = false;
            break;
        }
    }

    if (win) {
        play_sound(SoundWin);
        wait_frames = 200;
        initialize_cards();
    }
}
