#ifndef CMPL_H
#define CMPL_H


typedef struct media_data* Media_Data;
typedef struct media_player* Media_Player;

Media_Data new_media_data();
Media_Player new_media_player();

void delete_media_data(Media_Data data);
void delete_media_player(Media_Player player);

void parse_media_file(Media_Data data, char *file_path);
void play_media_file(Media_Player player, Media_Data data);

#endif /* CMPL_H */
