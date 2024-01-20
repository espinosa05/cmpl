#ifndef MEDIA_TYPES_GENERIC_H
#define MEDIA_TYPES_GENERIC_H

#include <sys/types.h>

typedef void Media_Info;

typedef ssize_t (*Media_Info_Callback) (Media_Info *);
/* function pointers to the APIs */
extern Media_Info_Callback play_media;
extern Media_Info_Callback stop_media;
extern Media_Info_Callback delete_media;

#endif /* MEDIA_TYPES_GENERIC_H */
