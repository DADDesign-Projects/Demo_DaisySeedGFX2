#include <stdint.h>
#include "../libDaisy/src/daisy_seed.h"
#include "../DaisySeedGFX2/cDisplay.h"
#include "Flasher.h"

using namespace daisy;

// Gestion du hardware de la carte
DaisySeed hw;

// Gestion de l'écran
DECLARE_DISPLAY(__Display);
DECLARE_LAYER(TextLayer, 250, 100)

// Flash Memory
Dad::cQSPI DSY_QSPI_BSS __QSPI;

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

    __Display.setOrientation(Rotation::Degre_270);
    uint8_t* pImg=nullptr;
    uint8_t NbFrames=1;
    uint16_t Witdh=0;
    uint16_t Height=0;

    if(false ==__QSPI.GetImgInformation("Nature.png", pImg, NbFrames, Witdh, Height)){
        while(1); // Error flashing image 
    }
    DadGFX::cImageLayer* pNature = __Display.addLayer(pImg, 0 , 0, Witdh, Height, 1, NbFrames);
    
    if(false == __QSPI.GetImgInformation("Butterfly.gif", pImg, NbFrames, Witdh, Height)){
        while(1); // Error flashing image 
    }
    DadGFX::cImageLayer* pButterfly = __Display.addLayer(pImg, 0 , 0, Witdh, Height, 10, NbFrames);

    __Display.flush();

    uint8_t ButterImage = 1;
    float AvanceX = 5;
    float AvanceY = 3;
    int16_t PosX = 0;
    int16_t PosY = 100;
    while (1){
        ButterImage++;
        if(ButterImage > NbFrames) ButterImage=1;
        pButterfly->setFrame(ButterImage);

        PosX += AvanceX;
        if(PosX < 0){
            PosX = 0;
            AvanceX = - AvanceX;
        }
        if(PosX >= TFT_HEIGHT-Height){
            PosX = TFT_HEIGHT-Height;
            AvanceX = - AvanceX;
        }

        PosY += AvanceY; 
        if(PosY < 0){
            PosY = 0;
            AvanceY = - AvanceY;
        }

        if(PosY >= TFT_WIDTH-Witdh){
            PosY = TFT_WIDTH-Witdh;
            AvanceY = - AvanceY;
        }

        pButterfly->moveLayer(PosX, PosY);
   
        __Display.flush();
        HAL_Delay(100);
    }


   
}
