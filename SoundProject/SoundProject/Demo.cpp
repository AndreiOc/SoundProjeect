#include "fmod.hpp"
#include "common.h"

int FMOD_Main()
{
    FMOD::System* system;
    FMOD::Sound* sounds[6];
    FMOD::Sound* soundStream, * sound_to_play; //sound for the stream
    FMOD::Channel* channelStream1;
    FMOD::Channel* channelStream2;

    FMOD::Channel* channels[6];


    //Channel group for the channels
    FMOD::ChannelGroup* channelGroup; 
    const char* name = "GMO";
    FMOD_RESULT result;
    void* extradriverdata = 0;


    bool isLooped = false;
    bool isPaused;
    bool isPlayed = false;
   
    float volume = 1.0f;
    float pan = 0.0f;

    Common_Init(&extradriverdata);

    /*
        Create a System object and initialize
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);


    //Static sounds
    result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ERRCHECK(result);
    result = system->createSound(Common_MediaPath("drumloop.wav"), FMOD_LOOP_OFF, 0, &sounds[0]);
    ERRCHECK(result);
    result = system->createSound(Common_MediaPath("jaguar.wav"), FMOD_LOOP_OFF, 0, &sounds[1]);
    ERRCHECK(result);
    result = system->createSound(Common_MediaPath("swish.wav"), FMOD_LOOP_OFF, 0, &sounds[2]);
    ERRCHECK(result);
    result = system->createSound(Common_MediaPath("c.ogg"), FMOD_LOOP_OFF, 0, &sounds[3]);
    ERRCHECK(result);
    result = system->createSound(Common_MediaPath("d.ogg"), FMOD_LOOP_OFF, 0, &sounds[4]);
    ERRCHECK(result);
    result = system->createSound(Common_MediaPath("e.ogg"), FMOD_LOOP_OFF, 0, &sounds[5]);


    //Stream sound
    result = system->createStream(Common_MediaPath("singing.wav"), FMOD_LOOP_OFF , 0, &soundStream);
    ERRCHECK(result);



    result = system->createChannelGroup(name, &channelGroup);
    ERRCHECK(result);
    
    channelGroup->setVolume(volume);

    result = channelGroup->getPaused(&isPaused);
    ERRCHECK(result);

    /*
        Main loop
    */
    do
    {
        Common_Update();

        //Play first audio 
        if (Common_BtnPress(BTN_ACTION1))
        {

            result = system->playSound(sounds[0], 0, false, &channels[0]);
            ERRCHECK(result);
            result = channels[0]->setChannelGroup(channelGroup);
        }

        //Play second audio 
        if (Common_BtnPress(BTN_ACTION2))
        {
            result = system->playSound(sounds[1], 0, false, &channels[1]);
            ERRCHECK(result);
            result = channels[1]->setChannelGroup(channelGroup);

        }

        //Play third audio 
        if (Common_BtnPress(BTN_ACTION3))
        {
            result = system->playSound(sounds[2], 0, false, &channels[2]);
            ERRCHECK(result);
            result = channels[2]->setChannelGroup(channelGroup);

        }

        //Play fourth audio 
        if (Common_BtnPress(BTN_ACTION4))
        {
            result = system->playSound(sounds[3], 0, false, &channels[3]);
            ERRCHECK(result);
            result = channels[3]->setChannelGroup(channelGroup);

        }

        //Play fifth audio
        if (Common_BtnPress(BTN_ACTION5))
        {
            result = system->playSound(sounds[4], 0, false, &channels[4]);
            ERRCHECK(result);
            result = channels[4]->setChannelGroup(channelGroup);

        }

        //Play sixth audio 
        if (Common_BtnPress(BTN_ACTION6))
        {
            result = system->playSound(sounds[5], 0, false, &channels[5]);
            ERRCHECK(result);
            result = channels[5]->setChannelGroup(channelGroup);

        }

        //Play sixth audio (stream audio)
        if (Common_BtnPress(BTN_ACTION7))
        {
            result = system->playSound(soundStream, 0, false, &channelStream1);
            ERRCHECK(result);
        }

        //Press space bar to stop music
        if (Common_BtnPress(BTN_MORE))
        {
            result = channelGroup->getPaused(&isPaused);
            ERRCHECK(result);
            result = channelGroup->setPaused(!isPaused);
            ERRCHECK(result);

        }
        //Press up or down arrow key to high or low muisc 
        if (Common_BtnPress(BTN_UP))
        {
            channelGroup->getVolume(&volume);
            if (volume < 1.0f)
            {
                volume += 0.1f;
                result = channelGroup->setVolume(volume);
            }
            ERRCHECK(result);
        }
        if (Common_BtnPress(BTN_DOWN))
        {
            channelGroup->getVolume(&volume);
            if (volume > 0.0f)
            {
                volume -= 0.1f;
                result = channelGroup->setVolume(volume);
            }
            ERRCHECK(result);
        }

        //Press tab to loop music
        if (Common_BtnPress(BTN_TAB))
        {
            if (!isLooped)
            {
                for (int count = 0; count < 6; count++)
                {
                    result = sounds[count]->setMode(FMOD_LOOP_NORMAL);
                    ERRCHECK(result);
                }
                isLooped = true;
            }
            else
            {
                //I need to stop the sound and restart it after the playing
                result = channelGroup->stop();
                ERRCHECK(result);

                for (int count = 0; count < 6; count++)
                {
                    result = sounds[count]->setMode(FMOD_LOOP_OFF);
                    ERRCHECK(result);
                }
                isLooped = false;
            }
        }

        //Press left or right arrow key to pan sound
        if (Common_BtnDown(BTN_LEFT))
        {
            pan = (pan <= -0.9f) ? -1.0f : pan - 0.1f;

            result = channelGroup->setPan(pan);
            ERRCHECK(result);
        }
        if (Common_BtnDown(BTN_RIGHT))
        {
            pan = (pan >= 0.9f) ? 1.0f : pan + 0.1f;

            result = channelGroup->setPan(pan);
            ERRCHECK(result);
        }


        result = system->update();
        ERRCHECK(result);

        {
            unsigned int ms = 0;
            unsigned int lenms = 0;
            bool playing = 0;
            bool paused = 0;
            int channelsplaying = 0;

            const char* looping = (isLooped) ? looping = "yes" : "no";

            system->getChannelsPlaying(&channelsplaying, NULL);

            Common_Draw("==================================================");
            Common_Draw("Sound Project Example.");
            Common_Draw("GMO (c) Firelight Technologies 2024-2024.");
            Common_Draw("==================================================");
            Common_Draw("");
            Common_Draw("Press %s to play a mono sound (drumloop)", Common_BtnStr(BTN_ACTION1));
            Common_Draw("Press %s to play a mono sound (jaguar)", Common_BtnStr(BTN_ACTION2));
            Common_Draw("Press %s to play a stereo sound (swish)", Common_BtnStr(BTN_ACTION3));
            Common_Draw("Press %s to play a stereo sound (c)", Common_BtnStr(BTN_ACTION4));
            Common_Draw("Press %s to play a stereo sound (d)", Common_BtnStr(BTN_ACTION5));
            Common_Draw("Press %s to play a stereo sound (e)", Common_BtnStr(BTN_ACTION6));
            Common_Draw("Press %s to play a stream sound (wave_vorbis)", Common_BtnStr(BTN_ACTION7));

            Common_Draw("");
            Common_Draw("==================================================");
            Common_Draw("");

            Common_Draw("Press %s to pause music: %s", Common_BtnStr(BTN_MORE), (isPaused) ? ("Play") : ("Pause"));
            Common_Draw("Press %s to loop music : %s", Common_BtnStr(BTN_TAB), looping);
            Common_Draw("Press %s or %s to high or low volume : %0.2f ", Common_BtnStr(BTN_UP), Common_BtnStr(BTN_DOWN), (volume < 0) ? volume * (-1) : volume);
            Common_Draw("Press %s or %s to pan : %0.2f", Common_BtnStr(BTN_LEFT), Common_BtnStr(BTN_RIGHT), pan);
            Common_Draw("Channels Playing %d", channelsplaying);
            Common_Draw("");
            Common_Draw("==================================================");
            Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

        }

        Common_Sleep(50);
    } while (!Common_BtnPress(BTN_QUIT));

    /*
        Shut down
    */
    for (int count = 0; count < 6; count++)
    {
        result = sounds[count]->release();
        ERRCHECK(result);
    }
    ERRCHECK(result);
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);

    Common_Close();

    return 0;
}
