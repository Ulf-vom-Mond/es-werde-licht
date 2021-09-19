#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void print_help(){
  printf(
    "Usage: artpoll [OPTION]...\n"
    "Send an ArtPoll packet according to the Art-Net protocol standard and listen for the ArtPollReply responses.\n\n"

    "OPTIONS:\n"
    "    -h, --host           ip address of receiver (2.255.255.255)\n"
    "    -p, --port           port of receiver (6454)\n"
    "\n"
  );

  exit(0);
}

int main(int argc, char const *argv[]) {
  char *host = "2.255.255.255";
  char *port = "6454";

  if (argc == 1) {
    print_help();
  }

  for (size_t i = 1; i < argc; i++) {
    if (strcmp("-h", argv[i]) == 0 || strcmp("--host", argv[i]) == 0) {
      host = argv[i + 1];
    }else if (strcmp("-p", argv[i]) == 0 || strcmp("--port", argv[i]) == 0) {
      port = argv[i + 1];
    }else if (strcmp("--help", argv[i]) == 0) {
      print_help();
    }
  }

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd == -1) {
    perror("socket creation failed");
    exit(1);
  }

  char data[] = {
    'A', 'r', 't', '-', 'N', 'e', 't', '\0', // ID
    0x00, 0x20,                              // OpCode
    0x00, 0x0E,                              // Protocol Version
    0x00,                                    // TalkToMe Bitmask
    0x00                                     // Priority
  };
  size_t data_length = sizeof(data);

  struct sockaddr_in dst_addr;
  dst_addr.sin_family = AF_INET;
  dst_addr.sin_port = htons(atoi(port));
  dst_addr.sin_addr.s_addr = inet_addr(host);

  if (dst_addr.sin_addr.s_addr == -1) {
    perror("error");
    exit(1);
  }

  //memset(dst_addr.sin_zero, '\0', sizeof dst_addr.sin_zero);
  socklen_t dst_addr_size = sizeof(dst_addr);

  struct sockaddr_in listen_addr;
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(atoi(port));
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("10.42.0.204");
  //memset(listen_addr.sin_zero, '\0', sizeof listen_addr.sin_zero);
  socklen_t listen_addr_size = sizeof(listen_addr);

  if (bind(sockfd, &listen_addr, listen_addr_size) == -1){
    perror("binding to socket failed");
    exit(1);
  }

  if (sendto(sockfd, data, data_length, 0, &dst_addr, dst_addr_size) != -1) {
    printf("packet successfully sent\n");
  }else{
    perror("packet sending failed");
    exit(1);
  }

  while (1) {
    char receive_buffer[2048] = "";
    if (recvfrom(sockfd, receive_buffer, 2048, 0, NULL, NULL) == -1){
      perror("packet receiving failed");
      exit(1);
    }
    printf("%s\n", receive_buffer);
  }

  return 0;
}
