//#include <allegro5/allegro.h>
//#include <allegro5/allegro_audio.h>
//#include <allegro5/allegro_acodec.h>
//int main() {
//	ALLEGRO_DISPLAY* display = NULL;
//	ALLEGRO_SAMPLE* sample = NULL;
//	ALLEGRO_SAMPLE_INSTANCE* sampleInstance = NULL;
//	al_init();
//	al_install_audio();
//	al_init_acodec_addon();
//	al_reserve_samples(1);
//	sample = al_load_sample("explode.wav");
//	// The commented out version below is much easier  	
//	//al_play_sample(sample, 1.0, 0, 1, ALLEGRO_PLAYMODE_LOOP,NULL);    
//	//However if you need access to additional control, such as currently playing position, you need  
//	//to attach it to a mixer, like the following example  	sampleInstance = al_create_sample_instance(sample);  
//	al_attach_sample_instance_to_mixer(sampleInstance, al_get_default_mixer());
//	al_play_sample_instance(sampleInstance);
//	// Loop until sound effect is done playing  	
//	while (al_get_sample_instance_playing(sampleInstance)) {}    	al_destroy_sample_instance(sampleInstance);
//	al_destroy_sample(sample);  	al_uninstall_audio();
//}