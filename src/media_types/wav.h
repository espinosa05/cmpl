#ifndef MEDIA_TYPES_WAV_H
#define MEDIA_TYPES_WAV_H

ssize_t load_wav_media(Media_Info **info);

ssize_t play_wav_media(Media_Info *info);
ssize_t stop_wav_media(Media_Info *info);
ssize_t delete_wav_media(Media_Info *info);

#endif /* MEDIA_TYPES_WAV_H */
