#include <stdint.h>
#include "../libDaisy/src/daisy_seed.h"
#include "../DaisySeedGFX2/cDisplay.h"
#include "Fonts/Vanilla_Extract_20p.h"
#include "Images/Penda.h"


using namespace daisy;
#define TOPI 6.283185307179

// Gestion du hardware de la carte
DaisySeed hw;

// Gestion de l'écran
DECLARE_DISPLAY(__Display);
DECLARE_LAYER(BackgroundLayer0, 128, 160)
DECLARE_LAYER(BackgroundLayer90, 160, 128)

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
    for (size_t i = 0; i < size; i++){
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main(void)
{
	// Configuration et Initialisation du Hardware
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
  
	// Configuration et initialisation de l'écran
    INIT_DISPLAY(__Display);

	// Start
	hw.StartAudio(AudioCallback);

    DadGFX::cLayer* pBackground90 = ADD_LAYER(BackgroundLayer90, 0, 0, 0);
    DadGFX::cLayer* pBackground0 = ADD_LAYER(BackgroundLayer0, 0, 0, 0);
    pBackground90->drawFillRect(0, 0, 160, 128, DadGFX::sColor(0, 200, 200, 255));
    pBackground0->drawFillRect(0, 0, 128, 160, DadGFX::sColor(200, 0, 200, 255));
    __Display.addLayer(Penda_map, 20, 10, 80, 80, 8); 

    while (1){
        __Display.setOrientation(Rotation::Degre_0);  
        pBackground0->changeZOrder(1);
        pBackground90->changeZOrder(0);
        __Display.flush();
        HAL_Delay(2000);

        __Display.setOrientation(Rotation::Degre_90);  
        pBackground0->changeZOrder(0);
        pBackground90->changeZOrder(1);
        __Display.flush();
        HAL_Delay(2000);
        
        __Display.setOrientation(Rotation::Degre_180);  
        pBackground0->changeZOrder(1);
        pBackground90->changeZOrder(0);
        __Display.flush();
        HAL_Delay(2000);

        __Display.setOrientation(Rotation::Degre_270);  
        pBackground0->changeZOrder(0);
        pBackground90->changeZOrder(1);
        __Display.flush();
        HAL_Delay(2000);

    }


   
}
