
#include <stdio.h>
#include <libavformat/avformat.h>

FILE *fp_open = NULL;
FILE *fp_write = NULL;

/*
 * @brief read callback func, Fill buf_size of the content into buf
 * @return bytes have read.
 */
int fill_iobuffer(void *opaque, uint8_t *buf, int buf_size) {
    int true_size = 0;
    if (!feof(fp_open)) {
        true_size = fread(buf, 1, buf_size, fp_open);
        return true_size;
    } else {
        return -1;
    }
}

/*
 * @brief write callback func.
 * * return  bytes have written.
 */
int write_buffer(void *opaque, uint8_t *buf, int buf_size) {
    int true_size = 0;
    if (!feof(fp_write)) {
        true_size = fwrite(buf, 1, buf_size, fp_write);
        return true_size;
    } else {
        return -1;
    }
}

int main(int argc, char *argv[]) {

    int err = -1;
    fp_open = fopen("input.h264", "rb+");
    fp_write = fopen("output.h264", "wb+");
    AVFormatContext *ic = NULL;
    AVFormatContext *oc = NULL;
    unsigned char *iobuffer = NULL;
    unsigned char *outbuffer = NULL;
    AVIOContext *avio = NULL;
    AVIOContext *avio_out = NULL;


    av_register_all();
    //avformat_network_init();

    ic = avformat_alloc_context();
    iobuffer = (unsigned char *)av_malloc(32768);
    avio = avio_alloc_context(iobuffer, 32768, 0, NULL, fill_iobuffer, NULL, NULL);
    ic->pb = avio;

    err = avformat_open_input(&ic, "nothint", NULL, NULL);

    // decoding

    avformat_alloc_output_context2(&oc, NULL, "h.264", NULL);
    outbuffer = (unsigned char *)av_malloc(32768);
    avio_out = avio_alloc_context(outbuffer, 32768, 0, NULL, NULL, write_buffer, NULL);
    oc->pb = avio_out;
    oc->flags = AVFMT_FLAG_CUSTOM_IO;

    return 0;
}







































