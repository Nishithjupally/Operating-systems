/*
 * Program: Musical chairs game with n players and m intervals.
 * Author:  changeme  changeme
 * Roll# :  changeme
 */

#include <stdlib.h>  /* for exit, atoi */
#include <iostream>  /* for fprintf */
#include <errno.h>   /* for error code eg. E2BIG */
#include <getopt.h>  /* for getopt */
#include <assert.h>  /* for assert */
#include <chrono>	/* for timers */
#include <thread>
#include <condition_variable>
#include <mutex>

using namespace std;

/*
 * Forward declarations
 */

void usage(int argc, char *argv[]);
unsigned long long musical_chairs(int nplayers);

mutex m,ps;
mutex check;

condition_variable cv,cv2;
int count = 0;
int n = 0;
int lap = 1,play = 0;
int playersleep[10000] = {0};

int sle=0;

int main(int argc, char *argv[])
{
    int c;
	int nplayers = 0;

    /* Loop through each option (and its's arguments) and populate variables */
    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"help",            no_argument,        0, 'h'},
            {"nplayers",         required_argument,    0, '1'},
            {0,        0,            0,  0 }
        };

        c = getopt_long(argc, argv, "h1:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            cerr << "option " << long_options[option_index].name;
            if (optarg)
                cerr << " with arg " << optarg << endl;
            break;

        case '1':
            nplayers = atoi(optarg);
            break;

        case 'h':
        case '?':
            usage(argc, argv);

        default:
            cerr << "?? getopt returned character code 0%o ??n" << c << endl;
            usage(argc, argv);
        }
    }

    if (optind != argc) {
        cerr << "Unexpected arguments.\n";
        usage(argc, argv);
    }


	if (nplayers == 0) {
		cerr << "Invalid nplayers argument." << endl;
		return EXIT_FAILURE;
	}

    unsigned long long game_time;
	game_time = musical_chairs(nplayers);

    cout << "Time taken for the game: " << game_time << " us" << endl;

    exit(EXIT_SUCCESS);
}

/*
 * Show usage of the program
 */
void usage(int argc, char *argv[])
{
    cerr << "Usage:\n";
    cerr << argv[0] << "--nplayers <n>" << endl;
    exit(EXIT_FAILURE);
}

void umpire_main(int nplayers)
{
    /* Add your code here */
	/* read stdin only from umpire */
    while(lap < nplayers)
    {
        string temp;
        if (cin>>temp)
        {
            /* code */
        
        

        if (temp == "lap_start")
        {
            //lapstart.notify_all();
          //  lap++;
            count = 0;
            //play = 0;
           // cv.notify_all();

        }
        if (temp == "music_start")
        {
            cv.notify_all();
            sle = 1;
            //musicstart.notify_all();
        }
        if (temp == "umpire_sleep")
        {
            int timeOfSleep;
            cin>>timeOfSleep;
            this_thread::sleep_for(chrono::microseconds(timeOfSleep));
        }
        if (temp == "player_sleep")
        {
            int id,timeOfSleep;
            cin>>id>>timeOfSleep;
         //   std::unique_lock<std::mutex> player_sleepl(ps);
            playersleep[id] = timeOfSleep;
          //  player_sleepl.unlock();


        }
        if (temp == "music_stop")
        {
            std::unique_lock<std::mutex> wait_lock1(m);
            play = 1;
            cv2.wait(wait_lock1);
        }
        if (temp == "lap_stop")
        {
             lap++;
             n--;
            if (lap == nplayers)
            {
                cv.notify_one();
            }
            play = 0;
            sle = 0;
          
        }
    }
    else
    {
        break;
    }


        
    }

	return;
}

void player_main(int plid)
{
    
    while(1)
    {

        if(sle)
        {
                
         if (playersleep[plid] != 0)
         {
             this_thread::sleep_for(chrono::microseconds(playersleep[plid]));
             playersleep[plid] = 0;
         }

        if (play)
        {
            std::unique_lock<std::mutex> wait_lock(m);
    
        if (count == n-1)
        {
            cout<<"======= lap# "<<lap<<" =======\n";
            cout<< plid<<" could not get chair\n";
            cout<<"**********************\n";
            //lap++;
           // counter_lock.unlock();
            wait_lock.unlock();
            cv2.notify_one();
            
            break;
        }
    
        count++;
        cv.wait(wait_lock);
       
        }
        }
        if (n == 1)
        {
            cout<<"Winner is "<<plid<<"\n";
            break;
        }
    }
	return;
}

unsigned long long musical_chairs(int nplayers)
{
	auto t1 = chrono::steady_clock::now();

    thread players[nplayers];
    n = nplayers;
    cout<<"Musical Chairs: "<<nplayers<<" player game with "<<nplayers-1<<" laps.\n";
    thread umpire(umpire_main,nplayers);

    for (int i = 0; i < nplayers; i++)
    {
        players[i] = thread(player_main,i);
    }
    
    for (int i = 0; i < nplayers; i++)
    {
        players[i].join();
    }
    umpire.join();
    
	// Spawn umpire thread.
    /* Add your code here */

	// Spawn n player threads.
    /* Add your code here */

	auto t2 = chrono::steady_clock::now();

	auto d1 = chrono::duration_cast<chrono::microseconds>(t2 - t1);

	return d1.count();
}
