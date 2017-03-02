#include <Windows.h>
#include "vlc/vlc.h"

int main(int argc, char **argv)
{
	libvlc_instance_t * inst;
	inst = libvlc_new (0, NULL);

	libvlc_media_player_t *mp1;
	libvlc_media_t *m1;

	m1 = libvlc_media_new_location(inst, "file:///D:\\test.sdp");

	const char *options1[] = {
		":no-audio",
		":sout=#transcode{vcodec=theo,vb=1024,fps=25,acodec=none}:http{mux=ogg,dst=:8080/xxzy}",
		":network-caching=10",
		":netsync-master"
	};

	for (int i = 0; i < sizeof(options1) / sizeof(options1[0]); i++)
	{
		libvlc_media_add_option (m1, options1[i]);
	}
	mp1 = libvlc_media_player_new_from_media (m1);
	libvlc_media_release (m1);
	libvlc_media_player_play (mp1);

	while(true)
	{
		;
	}
	return 0;
}