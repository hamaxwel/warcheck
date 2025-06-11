#ifndef STB_VORBIS_H
#define STB_VORBIS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stb_vorbis stb_vorbis;

stb_vorbis* stb_vorbis_open_memory(const unsigned char* data, int len, int* error, const unsigned char* alloc_buffer);
void stb_vorbis_close(stb_vorbis* f);
int stb_vorbis_get_info(stb_vorbis* f, int* channels, int* sample_rate);
int stb_vorbis_get_frame_float(stb_vorbis* f, int* channels, float*** output);
int stb_vorbis_seek_frame(stb_vorbis* f, unsigned int sample_number);
int stb_vorbis_seek(stb_vorbis* f, unsigned int sample_number);
unsigned int stb_vorbis_stream_length_in_samples(stb_vorbis* f);

#ifdef __cplusplus
}
#endif

#endif // STB_VORBIS_H 