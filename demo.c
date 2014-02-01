#include <stdio.h>

#include <mouse_comm.h>


int main(int argc, char * argv[]) {

  struct Cauterize c;
  uint8_t packet_buffer[MAX_ENCODED_LENGTH_mouse_message];

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
      packet_buffer,
      sizeof(packet_buffer));

  err = Pack_mouse_message(&c, &mouse_message);

  for(size_t i = 0; i < c.used; i++) {
    printf("%02X ", packet_buffer[i]);
  }
  printf("\n");

  return 0;
}
