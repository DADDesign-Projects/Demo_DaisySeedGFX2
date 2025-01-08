#include <stdint.h>
#include "../libDaisy/src/daisy_seed.h"
#include "../DaisySeedGFX2/cDisplay.h"
#include "Flasher.h"

using namespace daisy;

// Gestion du hardware de la carte
DaisySeed hw;

// Gestion de l'écran
DECLARE_DISPLAY(__Display);
DECLARE_LAYER(BallLayer, 16, 16)
DECLARE_LAYER(RacquetLayer, 6, 40)
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

    __Display.setOrientation(Rotation::Degre_90);

    DadGFX::cImageLayer* pAbstract1 = __Display.addLayer(__QSPI.GetFilePtr("Abstract1.png"), 0 ,0 , 320, 240, 1);
    DadGFX::cImageLayer* pAbstract2 = __Display.addLayer(__QSPI.GetFilePtr("Abstract2.png"), 0 ,0 , 320, 240, 0);
    DadGFX::cImageLayer* pPenda = __Display.addLayer(__QSPI.GetFilePtr("Penda.png"), 20 , 10, 80, 80, 8);
       
    DadGFX::cLayer* pRacquet = ADD_LAYER(RacquetLayer, 0, 0, 11);
    pRacquet->drawFillRect(0,0,6,40, DadGFX::sColor(255, 255, 255, 255));

    DadGFX::cLayer* pBall = ADD_LAYER(BallLayer, 0, 0, 10);
    pBall->drawFillCircle(7,7,7,DadGFX::sColor(255, 255, 255,255));

    DadGFX::cFont Vanilla((DadGFX::GFXBinFont *) __QSPI.GetFilePtr("Vanilla_Extract_20p.bin"));

    DadGFX::cLayer* pText = ADD_LAYER(TextLayer, 20, 160, 2);
    pText->setFont(&Vanilla);
    pText->setCursor(0,0);
    pText->setTextFrontColor(DadGFX::sColor(255,255,255,180));
    pText->drawText("Demo");
    pText->setCursor(0,Vanilla.getHeight());
    pText->drawText("DaisySeedGFX2");
    
    __Display.flush();


    float AvanceX = 7;
    float AvanceY = 5;
    int16_t PosX = 0;
    int16_t PosY = 100;
    int16_t PosRacquetX = 320-30;
    int16_t PosRacquetY = (240/2)-20;
    uint16_t Ct = 0;
    uint8_t  Back = 0;
    while (1){
        PosX += AvanceX;
        if(PosX < 0){
            PosX = 0;
            AvanceX = - AvanceX;
        }
        if(PosX >= TFT_HEIGHT-30){
            PosX = TFT_HEIGHT-30;
            AvanceX = - AvanceX;
        }

        PosY += AvanceY; 
        if(PosY < 0){
            PosY = 0;
            AvanceY = - AvanceY;
        }

        if(PosY >= TFT_WIDTH-7){
            PosY = TFT_WIDTH-7;
            AvanceY = - AvanceY;
        }

        int16_t DelatY = (PosRacquetY + 20) - PosY;
        
        PosRacquetY -= DelatY / (3+((TFT_HEIGHT-PosX)/25));

        if(PosRacquetY < 0){
            PosRacquetY = 0;
        }

        if((PosRacquetY+40) > TFT_WIDTH){
            PosRacquetY = TFT_WIDTH-40;
        }

        pBall->moveLayer(PosX, PosY);
        pRacquet->moveLayer(PosRacquetX, PosRacquetY);
        Ct++;
        if(Ct == 300){
            Ct = 0;
            if(Back == 0){
                Back =1;
                pAbstract1->changeZOrder(0);
                pAbstract2->changeZOrder(1);
            }else{
                pAbstract1->changeZOrder(1);
                pAbstract2->changeZOrder(0);
                Back = 0;
            }
        }
        __Display.flush();
        HAL_Delay(10);
    }


   
}
