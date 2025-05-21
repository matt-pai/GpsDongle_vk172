#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "nmea/parser.h"
#include "nmea/gmath.h"

int main(int argc, char *argv[])
{
	int fd;
	pid_t child_pid, sid;
	char device[32] = "/dev/ttyACM0";
	char *inBuf = malloc(1024);
    nmeaINFO info;
    nmeaPARSER parser;
    nmeaPOS dpos;

	if (argv[1]) {
		memset(device, 0, sizeof(device));
		strncpy(device, argv[1], sizeof(device)-1);
	}
		
	
	child_pid = fork();
	if (child_pid < 0) {
		perror("fork fail");
		_exit(0);
	} else if (child_pid > 0) {
		_exit(0);
	}

	sid = setsid();
	if (sid < 0) {
		perror("setsid fail");
		return 0;
	}

	fd  = open(device, O_RDWR | O_NOCTTY);
	if (fd < 0) {
		perror("open fail");
		return 0;
	}
	dup2(fd, fileno(stdin));

	nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
	
	while (1) {
		scanf("%s", inBuf);
		//printf("GPS: %s\n", inBuf);
		nmea_parse(&parser, inBuf, (int)strlen(inBuf), &info);
		nmea_info2pos(&info, &dpos);
        //printf("Lat: %f, Lon: %f, Sig: %d, Fix: %d\n", dpos.lat,
        //       dpos.lon, info.sig, info.fix);
	}
	nmea_parser_destroy(&parser);
	return 0;
}
