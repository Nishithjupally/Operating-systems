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

mutex counter_lock,m,m1,m2,m3;

condition_variable musicend,musicstart,lapend,lapstart,cv,cv2;
int count = 0;
int n = 0;
int lap = 1,play = 0;




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
        cin>>temp;

        if (temp == "lap_start")
        {
            //lapstart.notify_all();
           // lap++;
            count = 0;
            play = 0;
            cv.notify_all();

        }
        if (temp == "music_start")
        {
            //musicstart.notify_all();
        }
        if (temp == "music_stop")
        {
            std::unique_lock<std::mutex> wait_lock1(m);
            play = 1;
           // musicend.notify_all();
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
            //lapend.notify_all();
        }

        
    }

	return;
}

void player_main(int plid)
{
    //cout<<"ff2\n";
    while(1)
    {
        if (play)
        {
            std::unique_lock<std::mutex> wait_lock(m);
      //  counter_lock.lock();
      //  cout<<"ff23\n";

        if (count == n-1)
        {
            cout<<"======= lap# "<<lap<<" =======\n";
            cout<< plid+1<<" could not get chair\n";
            cout<<"**********************\n";
            //lap++;
           // counter_lock.unlock();
            wait_lock.unlock();
            cv2.notify_one();
            
            break;
        }
        if (n == 1)
        {
            cout<<"Winner is "<<plid+1<<"\n";
            //counter_lock.unlock();
            break;
        }
        count++;
        
        cv.wait(wait_lock);
       // cout<<"ee\n";
        
       // counter_lock.unlock();
        
    }
    }
	return;
}

unsigned long long musical_chairs(int nplayers)
{
	auto t1 = chrono::steady_clock::now();
   // cout<<"ff1\n";
    thread players[nplayers];
    n = nplayers;

    thread umpire(umpire_main,nplayers);
    for (int i = 0; i < nplayers; i++)
    {
        players[i] = thread(player_main,i);
    }
    //umpire_main(nplayers);
  //  cout<<"ff2\n";
    
    for (int i = 0; i < nplayers; i++)
    {
        players[i].join();
    }
    
	// Spawn umpire thread.
    /* Add your code here */

	// Spawn n player threads.
    /* Add your code here */

	auto t2 = chrono::steady_clock::now();

	auto d1 = chrono::duration_cast<chrono::microseconds>(t2 - t1);

	return d1.count();
}
