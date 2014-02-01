#include <stdio.h>

#include <mouse_comm.h>

uint8_t packet_buffer[MAX_ENCODED_LENGTH_mouse_packet];

int main(int argc, char * argv[]) {

  struct Cauterize c;

  struct mouse_packet mouse_packet = {
    .tag = GROUP_MOUSE_PACKET_TYPE_MOUSE_UPDATE,
    .data = {
      .mouse_update = {
        .cursor_type = IBEAM,
        .resolution = {
          .width = 1024,
          .height = 768,
        },
        .position = {
          .x = 500,
          .y = 500,
        },
      },
    },
  };

  CAUTERIZE_STATUS_T err = CA_OK;

  err = CauterizeInitAppend(
      &c,
      packet_buffer,
      sizeof(packet_buffer));

  err = Pack_mouse_packet(&c, &mouse_packet);

  for(size_t i = 0; i < c.used; i++) {
    printf("%02X ", packet_buffer[i]);
  }
  printf("\n");

  /* const char * fname = "./sample.dat"; */
  /* FILE* outfile = fopen(fname,"w"); */
  /* if (outfile != NULL) { */
  /*   fwrite(response_buffer, c.used, 1, outfile); */
  /*   if (!fclose(outfile)) { */
  /*     printf("Wtf fail fclose(%s)\n", fname); */
  /*   } */

  /*   printf("Wrote '%s'.\n",fname); */
  /* } else {  */
  /*   printf("Fail to open outfile '%s'.\n",fname); */
  /* } */

  return 0;
}
