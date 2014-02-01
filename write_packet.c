#include <stdio.h>

#include <mouse_comm.h>

uint8_t message_buffer[MAX_ENCODED_LENGTH_mouse_message];

int main(int argc, char * argv[]) {

  struct Cauterize c;

  struct mouse_message mouse_message = {
    .tag = GROUP_MOUSE_MESSAGE_TYPE_MOUSE_UPDATE,
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
      message_buffer,
      sizeof(message_buffer));

  err = Pack_mouse_message(&c, &mouse_message);

  const char * fname = "./mouse_message.dat";
  FILE* outfile = fopen(fname,"w");
  if (outfile != NULL) {
    fwrite(message_buffer, c.used, 1, outfile);
    fclose(outfile);
    printf("Wrote '%s'.\n",fname);
    return 0;
  } else { 
    printf("Failed to open '%s' for writing. Abort.\n",fname);
    return -1;
  }

}
