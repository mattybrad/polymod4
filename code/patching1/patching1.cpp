#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

// took these from Arduino source code, need to check this works okay!
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// include polymod classes
#include "Patchbay.h"
#include "SocketInput.h"
#include "SocketOutput.h"
#include "modules/Module.h"
#include "modules/VCO.h"
#include "modules/Output.h"

// define polymod things (todo: don't hardcode sizes/lengths of things)
Patchbay patchbay;
uint8_t patchingInputValues[32];
bool ledStates[5];
int numOutputRegisters = 4;
int numInputRegisters = 4;
ShiftRegister595 outputChain;
dsy_gpio_pin outputChainPins[3] = {D1,D2,D7};
AdcChannelConfig analogInputs[2];
float potValues[16];
//Module modules[16];
//VCO vco1;
//Output output;
SocketOutput socketOutputs[32];
SocketInput socketInputs[32];

DaisySeed hw;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = 0.0f;
		out[1][i] = 0.0f;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	outputChain.Init(outputChainPins, numOutputRegisters+1);
	analogInputs[0].InitSingle(A0);
	analogInputs[1].InitSingle(A1);
	hw.adc.Init(analogInputs, 2);

	// define sockets
	//socketOutputs[0] = vco1.squareOut;
	//socketInputs[0] = output.mainOutput;

	while(true) {
		//vco1.update();

		for(int i=0; i<8; i++) {
			// send i-th bit to all 595s
			for(int j=0; j<numOutputRegisters; j++) {
				for(int k=0; k<8; k++) {
					uint8_t channelNumber = j*8 + k;
					bool outputBit = bitRead(channelNumber, i);
					outputChain.Set(channelNumber, outputBit);
				}
			}

			uint8_t utilOutputBits = 0;

			// write the 4051 address to the util register
			for(int j=0; j<3; j++) {
				bitWrite(utilOutputBits, j, bitRead(i, j));
			}
			// write LED states to util register
			for(int j=0; j<5; j++) {
				bitWrite(utilOutputBits, j+3, ledStates[j]);
			}

			outputChain.Write();

			// read analog values from 2 x 4051s
			potValues[i] = hw.adc.GetFloat(0);
			potValues[i+8] = hw.adc.GetFloat(1);

			// read i-th bit from all 165s
			// (DUPLICATE/ADAPT 595 FUNCTION FOR 165?)

			// read button values from util 165

		}
		for(int i=0; i<32; i++) {
			//patchbay.updateInputChannel(i, patchingInputValues[i]);
		}
		//patchbay.update();
	}
	
}