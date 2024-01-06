#include <stdio.h>
#include <stdlib.h>

#include "cmpl.h"


int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s [FILE]\n", *argv);
        return EXIT_FAILURE;
    }

    Media_Data data = new_media_data();
    Media_Player player = new_media_player();

    char *media_path = argv[1];
    parse_media_file(data, media_path);
    play_media_file(player, data);

    return EXIT_SUCCESS;
}
