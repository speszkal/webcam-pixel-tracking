#ifndef Timer_h
#define Timer_h

class Timer
{   
    public:
        Timer();
        ~Timer();

        void start();
        void stop();
        void pause();
        void unpause();

        Uint64 getTicks();

        bool getIfStarted() { return isStarted; };
        bool getIfPaused() { return isPaused && isStarted; };

    private:
        Uint64 startTick;
        Uint64 pauseTick;

        bool isStarted;
        bool isPaused;
        
};

#endif // Timer_h