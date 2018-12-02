#include <Wire.h>
#include <MIDI.h>
#include "mpr121.h"

#define BREATH_PIN 3
#define BREATH_CONTROL 2
#define MIDI_CHNL 1
#define BREATH_ON_THRESHOLD 45
#define BREATH_OFF_THRESHOLD 40
#define BREATH_MAX 300
#define PLAYING 0
#define SILENT 1
#define MPR121_1_ADDR 0x5A
#define MPR121_2_ADDR 0x5B
//Number of samples for breath averaging.
#define BREATH_CONTROL_NUM_SAMPLES 64
#define DEBUG 9

// -1 should be bigger than the number of possible fingerings
uint32_t lastFingering = -1;
int lastNote = -1;
int lastUnmodifiedNote = -1;
char state = SILENT;
unsigned long lastSent = 0;
long summedBreaths = 0;
char sampleTaken = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
    #ifdef DEBUG
    Serial.begin(9600);
    #else
    MIDI.begin();
    // All Notes Off
    MIDI.sendControlChange(123, 0, MIDI_CHNL);
    #endif
    Wire.begin();
    mpr121_setup(MPR121_1_ADDR);
    mpr121_setup(MPR121_2_ADDR);
}

int midiNoteFromFingering(uint32_t fingering) {
    // Generated from fingering file.
    int note = 0;
    //fingerings are the first 13 bits
    uint16_t fingered_note = fingering&8491;
    //modifiers are bits 14 to 21
    uint16_t modifiers = (fingering>>13)&255;
    #ifdef DEBUG
    Serial.print("modifiers: ");
    Serial.println(modifiers);
    #endif

    switch(fingering) {
        case(7511):
            note = (-1);
            break;
        case(3415):
            note = 0;
            break;
        case(1367):
            note = 1;
            break;
        case(17):
        case(25):
        case(29):
        case(81):
        case(85):
        case(89):
        case(93):
        case(273):
        case(277):
        case(281):
        case(285):
        case(337):
        case(341):
        case(345):
        case(349):
        case(529):
        case(537):
        case(541):
        case(593):
        case(597):
        case(601):
        case(605):
        case(785):
        case(789):
        case(793):
        case(797):
        case(849):
        case(853):
        case(857):
        case(861):
        case(1361):
        case(1369):
        case(3409):
        case(3417):
        case(7505):
        case(7513):
            note = 10;
            break;
        case(1):
        case(9):
        case(65):
        case(73):
        case(257):
        case(265):
        case(321):
        case(329):
        case(513):
        case(521):
        case(577):
        case(585):
        case(769):
        case(777):
        case(833):
        case(841):
        case(1345):
        case(1353):
        case(3393):
        case(3401):
        case(7489):
        case(7497):
            note = 11;
            break;
            note = 12;
            break;
        case(0):
        case(2):
        case(4):
        case(8):
        case(10):
        case(12):
        case(14):
        case(16):
        case(18):
        case(20):
        case(24):
        case(26):
        case(28):
        case(30):
        case(64):
        case(66):
        case(68):
        case(72):
        case(74):
        case(76):
        case(78):
        case(80):
        case(82):
        case(84):
        case(88):
        case(90):
        case(92):
        case(94):
        case(256):
        case(258):
        case(260):
        case(264):
        case(266):
        case(268):
        case(270):
        case(272):
        case(274):
        case(276):
        case(280):
        case(282):
        case(284):
        case(286):
        case(320):
        case(322):
        case(324):
        case(328):
        case(330):
        case(332):
        case(334):
        case(336):
        case(338):
        case(340):
        case(344):
        case(346):
        case(348):
        case(350):
        case(512):
        case(514):
        case(516):
        case(520):
        case(522):
        case(524):
        case(528):
        case(530):
        case(532):
        case(536):
        case(538):
        case(540):
        case(542):
        case(576):
        case(578):
        case(580):
        case(584):
        case(586):
        case(588):
        case(590):
        case(592):
        case(594):
        case(596):
        case(600):
        case(602):
        case(604):
        case(606):
        case(768):
        case(770):
        case(772):
        case(776):
        case(778):
        case(780):
        case(782):
        case(784):
        case(786):
        case(788):
        case(792):
        case(794):
        case(796):
        case(798):
        case(832):
        case(834):
        case(836):
        case(840):
        case(842):
        case(844):
        case(846):
        case(848):
        case(850):
        case(852):
        case(856):
        case(858):
        case(860):
        case(1360):
        case(1362):
        case(1364):
        case(1368):
        case(1370):
        case(1372):
        case(1374):
        case(3408):
        case(3410):
        case(3412):
        case(3416):
        case(3418):
        case(3420):
        case(3422):
        case(7504):
        case(7506):
        case(7508):
        case(7512):
        case(7514):
        case(7516):
        case(7518):
            note = 13;
            break;
        case(6):
        case(22):
        case(70):
        case(86):
        case(262):
        case(278):
        case(326):
        case(342):
        case(518):
        case(534):
        case(582):
        case(598):
        case(774):
        case(790):
        case(838):
        case(854):
        case(1366):
        case(3414):
        case(7510):
            note = 14;
            break;
        case(526):
        case(646):
        case(678):
        case(862):
            note = 15;
            break;
        case(91):
        case(603):
            note = 16;
            break;
        case(21):
        case(533):
            note = 17;
            break;
        case(261):
        case(773):
            note = 18;
            break;
        case(343):
            note = 2;
            break;
        case(855):
            note = 3;
            break;
        case(87):
        case(599):
            note = 4;
            break;
        case(23):
        case(279):
        case(535):
        case(791):
        case(1303):
        case(1815):
        case(3351):
        case(3863):
        case(7447):
        case(7959):
            note = 5;
            break;
        case(71):
        case(263):
        case(327):
        case(583):
        case(775):
        case(839):
            note = 6;
            break;
        case(7):
        case(519):
            note = 7;
            break;
        case(15):
        case(31):
        case(79):
        case(95):
        case(271):
        case(287):
        case(335):
        case(351):
        case(527):
        case(543):
        case(591):
        case(607):
        case(783):
        case(799):
        case(847):
        case(863):
        case(1375):
        case(3423):
        case(7519):
            note = 8;
            break;
        case(3):
        case(11):
        case(19):
        case(27):
        case(67):
        case(75):
        case(259):
        case(267):
        case(275):
        case(283):
        case(323):
        case(331):
        case(515):
        case(523):
        case(531):
        case(539):
        case(579):
        case(587):
        case(771):
        case(779):
        case(787):
        case(795):
        case(835):
        case(843):
        case(1363):
        case(1371):
        case(3411):
        case(3419):
        case(7507):
        case(7515):
            note = 9;
            break;
        default:
            note = lastUnmodifiedNote;
    }
    lastUnmodifiedNote = note;
    switch(modifiers) {
        case(128):
            note += 108;
            break;
        case(1):
            note += 12;
            break;
        case(3):
            note += 24;
            break;
        case(6):
            note += 36;
            break;
        case(12):
            note += 48;
            break;
        case(24):
            note += 60;
            break;
        case(48):
            note += 72;
            break;
        case(96):
            note += 84;
            break;
        case(192):
            note += 96;
            break;
    }
    return note;
}

/*returns a 32 bits number that represents the status of the 24 electrodes
  in this configuration: XXXXXXXXBA9876543210BA9876543210 where x doesn't matter and the
  last 16 bits corresponds to the mpr121 with adress MPR121_1_ADDR.
*/
uint32_t readFingering(){
    Wire.requestFrom(MPR121_1_ADDR, 2);
    byte LSB_1 = Wire.read();
    byte MSB_1 = Wire.read();
    Wire.requestFrom(MPR121_2_ADDR, 2);
    byte LSB_2 = Wire.read();
    byte MSB_2 = Wire.read();
    uint32_t val = 0;
    val |= ((MSB_1 << 8) | LSB_1);
    val |= ((MSB_2 << 8) | LSB_2) << 12;
    return val;
}

int mapBreath(int breath) {
    int value = map(breath, BREATH_OFF_THRESHOLD, BREATH_MAX, 0, 127);
    if(value < 0) {
        value = 0;
    } else if (value > 127) {
        value = 127;
    }

    return value;
}

void loop() {
    int sensor_reading = analogRead(BREATH_PIN);
    /*#ifdef DEBUG
    Serial.println(sensor_reading);
    #endif*/
    int breath = mapBreath(sensor_reading);
    if(state == PLAYING) {
        if(sensor_reading < BREATH_OFF_THRESHOLD) {
            #ifndef DEBUG
            MIDI.sendNoteOff(lastNote, 0, MIDI_CHNL);
            MIDI.sendControlChange(BREATH_CONTROL, 0, MIDI_CHNL);
            #else
            Serial.print("Entering SILENT note off :");
            Serial.print(lastNote);
            Serial.print(" Sensor Reading: ");
            Serial.println(sensor_reading);
            #endif
            //lastNote = -1;
            lastFingering = -1;
            sampleTaken = 0;
            summedBreaths = 0;
            state = SILENT;
        } else {
            #ifdef DEBUG
            long response = millis();
            #endif
            uint32_t fingering = readFingering();
            #ifdef DEBUG
            response = millis() - response;
            #endif
            int note;
            if(fingering != lastFingering && lastNote != (note = midiNoteFromFingering(fingering))) {
                #ifndef DEBUG
                MIDI.sendNoteOff(lastNote, 0, MIDI_CHNL);
                MIDI.sendNoteOn(note, breath, MIDI_CHNL);
                #else
                Serial.print("Stopping note #");
                Serial.print(lastNote);
                Serial.print(" Playing note #");
                Serial.print(note);
                Serial.print(" with breath control ");
                Serial.println(breath);
                #endif
                lastNote = note;
                lastFingering = fingering;
            }
            //TODO: figure out a good rate to send out breath control

            unsigned long currentTime = millis();
            if(sampleTaken == BREATH_CONTROL_NUM_SAMPLES ) {
                #ifndef DEBUG
                MIDI.sendControlChange(BREATH_CONTROL, summedBreaths >> 6, MIDI_CHNL);
                #endif
                summedBreaths = 0;
                sampleTaken = 0;
            } else {
                summedBreaths += breath;
                sampleTaken++;
            }

        }
    } else {
        if(sensor_reading > BREATH_ON_THRESHOLD) {
            unsigned long breathAverage = 0;
            int i;
            for(i = 0; i<32; i++) {
                breathAverage += analogRead(BREATH_PIN);
            }
            //fast divide by 32
            breathAverage >>= 5;
            #ifdef DEBUG
            Serial.println(breathAverage);
            #endif
            breath = mapBreath(breathAverage);
            if(breath <= 0){
                return;
            }
            uint32_t fingering = readFingering();
            int note = midiNoteFromFingering(fingering);
            #ifndef DEBUG
            MIDI.sendNoteOn(note, breath, MIDI_CHNL);
            MIDI.sendControlChange(BREATH_CONTROL, breath, MIDI_CHNL);
            #else
            Serial.print("Entering PLAYING note #");
            Serial.print(note);
            Serial.print(" breath:");
            Serial.println(breath);
            #endif
            state = PLAYING;
            lastNote = note;
            lastFingering = fingering;
        }
    }
}


void mpr121_setup(int addr){

    set_register(addr, ELE_CFG, 0x00);

    // Section A - Controls filtering when data is > baseline.
    set_register(addr, MHD_R, 0x01);
    set_register(addr, NHD_R, 0x01);
    set_register(addr, NCL_R, 0x00);
    set_register(addr, FDL_R, 0x00);

    // Section B - Controls filtering when data is < baseline.
    set_register(addr, MHD_F, 0x01);
    set_register(addr, NHD_F, 0x01);
    set_register(addr, NCL_F, 0xFF);
    set_register(addr, FDL_F, 0x02);

    // Section C - Sets touch and release thresholds for each electrode
    set_register(addr, ELE0_T, TOU_THRESH);
    set_register(addr, ELE0_R, REL_THRESH);

    set_register(addr, ELE1_T, TOU_THRESH);
    set_register(addr, ELE1_R, REL_THRESH);

    set_register(addr, ELE2_T, TOU_THRESH);
    set_register(addr, ELE2_R, REL_THRESH);

    set_register(addr, ELE3_T, TOU_THRESH);
    set_register(addr, ELE3_R, REL_THRESH);

    set_register(addr, ELE4_T, TOU_THRESH);
    set_register(addr, ELE4_R, REL_THRESH);

    set_register(addr, ELE5_T, TOU_THRESH);
    set_register(addr, ELE5_R, REL_THRESH);

    set_register(addr, ELE6_T, TOU_THRESH);
    set_register(addr, ELE6_R, REL_THRESH);

    set_register(addr, ELE7_T, TOU_THRESH);
    set_register(addr, ELE7_R, REL_THRESH);

    set_register(addr, ELE8_T, TOU_THRESH);
    set_register(addr, ELE8_R, REL_THRESH);

    set_register(addr, ELE9_T, TOU_THRESH);
    set_register(addr, ELE9_R, REL_THRESH);

    set_register(addr, ELE10_T, TOU_THRESH);
    set_register(addr, ELE10_R, REL_THRESH);

    set_register(addr, ELE11_T, TOU_THRESH);
    set_register(addr, ELE11_R, REL_THRESH);

    // Section D
    // Set the Filter Configuration
    // Set ESI2

    // 4 samples at 16ms interval : 64 ms response time
    //set_register(addr, FIL_CFG, 0x04);

    // 4 samples at 4ms interval : 16ms response time
    // set_register(addr, FIL_CFG, 0x02);

    // 4 samples at 2ms interval : 8ms response time
    set_register(addr, FIL_CFG, 0x01);

    // Section E
    // Electrode Configuration
    // Set ELE_CFG to 0x00 to return to standby mode
    set_register(addr, ELE_CFG, 0x0C);  // Enables all 12 Electrodes

    set_register(addr, ELE_CFG, 0x0C);
}

void set_register(int address, unsigned char r, unsigned char v){
    Wire.beginTransmission(address);
    Wire.write(r);
    Wire.write(v);
    Wire.endTransmission();
}
