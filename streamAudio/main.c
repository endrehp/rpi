#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>


int main()
{

    char server_message[256] = "You have reached the server";

	// create server socket
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// define the server address
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9005);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind the socket to our specified IP and port
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	listen(server_socket, 1);

	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);


	///// audio stuff ///////
    long loops;
    int rc;
    int size;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;
    snd_pcm_uframes_t frames;
    char *buffer;

    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(&handle, "plughw",
                    SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(rc));
    exit(1);
    }

    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_alloca(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(handle, params);

    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(handle, params,
                        SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(handle, params,
                                SND_PCM_FORMAT_U8);

    /* One channel (mono) */
    snd_pcm_hw_params_set_channels(handle, params, 1);

    /* 44100 bits/second sampling rate (CD quality) */
    val = 8000;
    snd_pcm_hw_params_set_rate_near(handle, params,
                                    &val, &dir);

    /* Set period size to 32 frames. */
    frames = 1024;
    snd_pcm_hw_params_set_period_size_near(handle,
                                params, &frames, &dir);

    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(rc));
    exit(1);
    }

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params,
                                        &frames, &dir);
    size = frames; /* 2 bytes/sample, 2 channels */
    buffer = (char *) malloc(size);

    /* We want to loop for 5 seconds */
    snd_pcm_hw_params_get_period_time(params,
                                            &val, &dir);
    loops = 5000000 / val;


	////////////////////////
    printf("%d", sizeof(buffer));

	///////// The main stream /////////////////

	while (loops > 0) {
        loops--;
        rc = snd_pcm_readi(handle, buffer, frames);

        //printf("%d", sizeof(buffer));

        if (rc == -EPIPE) {
          /* EPIPE means overrun */
          fprintf(stderr, "overrun occurred\n");
          snd_pcm_prepare(handle);
        } else if (rc < 0) {
          fprintf(stderr,
                  "error from read: %s\n",
                  snd_strerror(rc));
        } else if (rc != (int)frames) {
          fprintf(stderr, "short read, read %d frames\n", rc);
        }

        // Send data to client
        send(client_socket, buffer, size, 0);


        //send(client_socket, server_message, sizeof(server_message), 0);
        /*
        rc = write(1, buffer, size);
        if (rc != size)
          fprintf(stderr,
                  "short write: wrote %d bytes\n", rc);
        */
    }


    ///////////////////////////////////
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    free(buffer);


    // send message
	//send(client_socket, server_message, sizeof(server_message), 0);

	// close the socket
	close(server_socket);

	return 0;
}
