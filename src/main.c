#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "cmpl.h"


int main(int argc, char **argv)
{
    if (argc < 2) {
        USAGE("%s [FILE]", *argv);
        return EXIT_FAILURE;
    }

    Media_Data data = new_media_data();

    char *media_path = argv[1];
    parse_media_file(data, media_path); /* extract the file information */

    Media_Player player = new_media_player(data); /* configure the media player for correct decoding */
    play_media_file(player);

    delete_media_data(data);
    delete_media_player(player);
    return EXIT_SUCCESS;
}
